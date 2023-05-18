#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
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
    status = PdhAddCounterW(query, L"\\Processor(_Total)\\% Processor Time", 0, &counter);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error adding counter: " << status << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    // Enumerate the counters for the Processor object
    DWORD counterListLength = 0;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhEnumObjectItemsA(nullptr, nullptr, "Processor", nullptr, &counterListLength, nullptr, nullptr, PERF_DETAIL_WIZARD, 0);
    

    char* counterList = new char[counterListLength];
    while (status == PDH_MORE_DATA) {
        status = PdhEnumObjectItemsA(nullptr, nullptr, "Processor", counterList, &counterListLength, nullptr, nullptr, PERF_DETAIL_WIZARD, 0);
        if (status == ERROR_SUCCESS) {
            std::cout << "Counters for Processor object:" << std::endl;
            std::cout << counterList << std::endl;
        }
        else {
            std::cerr << "Error enumerating counters: " << status << std::endl;
            delete[] counterList;
            PdhCloseQuery(query);
            return 1;
        }
        delete[] counterList;
    }
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error enumerating counters: " << status << std::endl;
        delete[] counterList;
        PdhCloseQuery(query);
        return 1;
    }
    

    // Remove the counter from the query
    status = PdhRemoveCounter(counter);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error removing counter: " << status << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    // Close the query when finished
    status = PdhCloseQuery(query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing query: " << status << std::endl;
        return 1;
    }

    return 0;
}
