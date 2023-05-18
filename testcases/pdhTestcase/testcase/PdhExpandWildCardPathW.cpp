#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

int main()
{
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

    // Expand a wildcard path to a list of matching paths
    DWORD pathListLength = 0;
    status = PdhExpandWildCardPathW(nullptr, L"\\Processor(*)\\% Processor Time", nullptr, &pathListLength, 0);
    if (status != PDH_MORE_DATA) {
        std::cerr << "Error getting path list length: " << status << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    PZZWSTR pathList = new WCHAR[pathListLength];
    status = PdhExpandWildCardPathW(nullptr, L"\\Processor(*)\\% Processor Time", pathList, &pathListLength, 0);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error expanding wildcard path: " << status << std::endl;
        delete[] pathList;
        PdhCloseQuery(query);
        return 1;
    }

    // Add all matching counters to the query
    for (PZZWSTR path = pathList; *path != L'\0'; path += wcslen(path) + 1) {
        status = PdhAddCounterW(query, path, 0, &counter);
        if (status != ERROR_SUCCESS) {
            std::cerr << "Error adding counter: " << status << std::endl;
            delete[] pathList;
            PdhCloseQuery(query);
            return 1;
        }
    }

    delete[] pathList;

    // Collect a raw counter value
    status = PdhCollectQueryData(query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error collecting query data: " << status << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    // Compute statistics for the raw counter value
    status = PdhGetRawCounterValue(counter, nullptr, &rawCounter);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting raw counter value: " << status << std::endl;
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
