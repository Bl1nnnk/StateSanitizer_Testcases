#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;
    PDH_FMT_COUNTERVALUE counterValue;
    DWORD bufferSize = 0;
    PDH_HCOUNTER counter;
    char* buffer = nullptr;
    DWORD index = 0;

    // Open a query for performance data
    status = PdhOpenQuery(nullptr, 0, nullptr);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }

    // Get the buffer size required for the counter name
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhLookupPerfNameByIndexA(nullptr, index, nullptr, &bufferSize);
    if (status != PDH_MORE_DATA) {
        std::cerr << "Error getting buffer size: " << status << std::endl;
        PdhCloseQuery(nullptr);
        return 1;
    }

    // Allocate the buffer for the counter name
    buffer = new char[bufferSize];

    // Get the counter name
    status = PdhLookupPerfNameByIndexA(nullptr, index, buffer, &bufferSize);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting counter name: " << status << std::endl;
        delete[] buffer;
        PdhCloseQuery(nullptr);
        return 1;
    }

    // Add a counter to the query
    status = PdhAddCounterA(nullptr, buffer, 0, &counter);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error adding counter: " << status << std::endl;
        delete[] buffer;
        PdhCloseQuery(nullptr);
        return 1;
    }

    // Collect the counter value
    status = PdhCollectQueryData(nullptr);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error collecting query data: " << status << std::endl;
        PdhRemoveCounter(counter);
        delete[] buffer;
        PdhCloseQuery(nullptr);
        return 1;
    }

    // Get the counter value
    status = PdhGetFormattedCounterValue(counter, PDH_FMT_LONG, nullptr, &counterValue);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting counter value: " << status << std::endl;
        PdhRemoveCounter(counter);
        delete[] buffer;
        PdhCloseQuery(nullptr);
        return 1;
    }

    std::cout << "Counter value: " << counterValue.longValue << std::endl;

    // Remove the counter from the query
    status = PdhRemoveCounter(counter);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error removing counter: " << status << std::endl;
        delete[] buffer;
        PdhCloseQuery(nullptr);
        return 1;
    }

    // Close the query when finished
    status = PdhCloseQuery(nullptr);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing query: " << status << std::endl;
        return 1;
    }

    delete[] buffer;

    return 0;
}
