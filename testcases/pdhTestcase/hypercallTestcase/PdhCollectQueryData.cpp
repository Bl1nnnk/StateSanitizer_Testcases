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

    // Open a query for performance data
    status = PdhOpenQueryW(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }
    
    // Add a counter to the query
    LPCWSTR counterPath = L"\\Processor(_Total)\\% Processor Time";
    status = PdhAddCounterW(query, counterPath, 0, &counter);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error adding counter: " << status << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    // Collect data for the query
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhCollectQueryData(query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error collecting query data: " << status << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    // Do something with the collected data...

    // Close the query when finished
    status = PdhCloseQuery(query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing query: " << status << std::endl;
        return 1;
    }

    return 0;
}
