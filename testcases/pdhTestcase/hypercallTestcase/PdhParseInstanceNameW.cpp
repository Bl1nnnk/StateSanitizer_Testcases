#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;
    PDH_HCOUNTER counter;
    PDH_RAW_COUNTER rawCounter;
    PDH_STATISTICS stats;

    // Open a query for performance data
    PDH_HQUERY query;
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

    // Collect a raw counter value
    status = PdhCollectQueryData(query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error collecting query data: " << status << std::endl;
        PdhRemoveCounter(counter);
        PdhCloseQuery(query);
        return 1;
    }

    // Parse the instance name of the counter
    LPWSTR instanceName = nullptr;
    DWORD instanceNameLength = 0;
    LPWSTR parentName = nullptr;
    DWORD parentNameLength = 0;
    DWORD index = 0;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhParseInstanceNameW(L"\\Processor(_Total)\\% Processor Time", instanceName, &instanceNameLength, parentName, &parentNameLength, &index);
    if (status == PDH_MORE_DATA) {
        instanceName = new WCHAR[instanceNameLength];
        parentName = new WCHAR[parentNameLength];
        status = PdhParseInstanceNameW(L"\\Processor(_Total)\\% Processor Time", instanceName, &instanceNameLength, parentName, &parentNameLength, &index);
    }
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error parsing instance name: " << status << std::endl;
        PdhRemoveCounter(counter);
        PdhCloseQuery(query);
        delete[] instanceName;
        delete[] parentName;
        return 1;
    }

    std::wcout << "Instance name: " << instanceName << std::endl;
    std::wcout << "Parent name: " << parentName << std::endl;
    std::wcout << "Index: " << index << std::endl;

    delete[] instanceName;
    delete[] parentName;

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
