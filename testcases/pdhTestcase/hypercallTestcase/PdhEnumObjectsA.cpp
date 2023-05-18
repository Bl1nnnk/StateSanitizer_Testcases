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
    PDH_RAW_COUNTER rawCounter;
    PDH_STATISTICS stats;

    // Open a query for performance data
    status = PdhOpenQueryW(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }

    // Enumerate the available objects
    DWORD bufferSize = 0;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhEnumObjectsA(nullptr, nullptr, nullptr, &bufferSize, PERF_DETAIL_WIZARD, TRUE);
    if (status == PDH_MORE_DATA) {
        char* buffer = new char[bufferSize];
        status = PdhEnumObjectsA(nullptr, nullptr, buffer, &bufferSize, PERF_DETAIL_WIZARD, TRUE);
        if (status == ERROR_SUCCESS) {
            std::cout << "Available objects:" << std::endl;
            for (char* p = buffer; *p != '\0'; p += strlen(p) + 1) {
                std::cout << "- " << p << std::endl;
            }
        } else {
            std::cerr << "Error enumerating objects: " << status << std::endl;
            delete[] buffer;
            PdhCloseQuery(query);
            return 1;
        }
        delete[] buffer;
    } else {
        std::cerr << "Error enumerating objects: " << status << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    // Add a counter to the query
    status = PdhAddCounterW(query, L"\\Processor(_Total)\\% Processor Time", 0, &counter);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error adding counter: " << status << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    // Collect a raw counter value
    status = PdhCollectQueryData(query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error collecting query data: " << status << std::endl;
        PdhRemoveCounter(counter);
        PdhCloseQuery(query);
        return 1;
    }

    // Compute statistics for the raw counter value
    status = PdhGetRawCounterValue(counter, nullptr, &rawCounter);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting raw counter value: " << status << std::endl;
        PdhRemoveCounter(counter);
        PdhCloseQuery(query);
        return 1;
    }

    for (int fmt = PDH_FMT_LONG; fmt <= PDH_FMT_LARGE; fmt *= 2) {
        for (int ret = PDH_FMT_1000; ret <= PDH_FMT_NOSCALE; ret *= 2) {
            int options = fmt | ret;

            std::cout << "Options: " << options << std::endl;

            PdhCollectQueryData(NULL);
            status = PdhComputeCounterStatistics(counter, options, 0, 1, &rawCounter, &stats);
            if (status != ERROR_SUCCESS) {
                std::cerr << "Error computing counter statistics: " << status << std::endl;
                PdhRemoveCounter(counter);
                PdhCloseQuery(query);
                return 1;
            }
        }
    }

    // Use the statistics data...

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
