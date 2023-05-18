#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;
    PDH_HQUERY query;
    PDH_HCOUNTER counter;
    LPCWSTR machineName = L"localhost";

    // Connect to the specified machine
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhConnectMachineW(machineName);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error connecting to machine: " << status << std::endl;
        return 1;
    }

    // Open a query for performance data
    status = PdhOpenQueryW(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }

    // Add a counter to the query
    status = PdhAddCounterW(query, L"\\Processor(_Total)\\% Processor Time", 0, &counter);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error adding counter: " << status << std::endl;
        return 1;
    }

    // Collect data for the counter
    status = PdhCollectQueryData(query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error collecting query data: " << status << std::endl;
        return 1;
    }

    // Get the value of the counter
    PDH_FMT_COUNTERVALUE value;
    status = PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, nullptr, &value);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting counter value: " << status << std::endl;
        return 1;
    }

    std::cout << "Processor usage: " << value.doubleValue << "%" << std::endl;

    // Close the query and disconnect from the machine
    status = PdhCloseQuery(query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing query: " << status << std::endl;
        return 1;
    }

    return 0;
}
