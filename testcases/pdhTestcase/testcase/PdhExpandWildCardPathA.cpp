#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    PDH_HQUERY query;
    PDH_HCOUNTER counter;

    // Open a query for performance data
    status = PdhOpenQueryW(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }

    // Expand a wildcard path to a list of matching paths
    const char* wildcardPath = "\\Processor(*)\\% Processor Time";
    DWORD pathListLength = 0;
    char* expandedPathList = new char[pathListLength];
    status = PdhExpandWildCardPathA(nullptr, wildcardPath, expandedPathList, &pathListLength, 0);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error expanding wildcard path: " << status << std::endl;
        delete[] expandedPathList;
        PdhCloseQuery(query);
        return 1;
    }

    

    // Remove the counters from the query
    path = expandedPathList;
    while (*path != '\0') {
        status = PdhRemoveCounter(counter);
        if (status != ERROR_SUCCESS) {
            std::cerr << "Error removing counter: " << status << std::endl;
            delete[] expandedPathList;
            PdhCloseQuery(query);
            return 1;
        }
        path += strlen(path) + 1;
    }

    // Close the query when finished
    status = PdhCloseQuery(query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing query: " << status << std::endl;
        return 1;
    }

    delete[] expandedPathList;
    return 0;
}

