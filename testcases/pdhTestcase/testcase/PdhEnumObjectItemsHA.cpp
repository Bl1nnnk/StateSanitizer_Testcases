cpp
#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    PDH_HLOG hDataSource = NULL;
    LPCSTR szMachineName = NULL;
    LPCSTR szObjectName = "Processor";
    PZZSTR mszCounterList = NULL;
    DWORD cchCounterListLength = 0;
    PZZSTR mszInstanceList = NULL;
    DWORD cchInstanceListLength = 0;
    DWORD dwDetailLevel = PERF_DETAIL_WIZARD;
    DWORD dwFlags = 0;

    // Open a log file for performance data
    status = PdhOpenLog("perflog.log", PDH_LOG_READ_ACCESS, &hDataSource);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening log file: " << status << std::endl;
        return 1;
    }

    // Enumerate the counters for the specified object
    status = PdhEnumObjectItemsHA(hDataSource, szMachineName, szObjectName, mszCounterList, &cchCounterListLength, mszInstanceList, &cchInstanceListLength, dwDetailLevel, dwFlags);
    if (status == PDH_MORE_DATA) {
        // Allocate memory for the counter list and instance list
        mszCounterList = (PZZSTR)malloc(cchCounterListLength * sizeof(TCHAR));
        mszInstanceList = (PZZSTR)malloc(cchInstanceListLength * sizeof(TCHAR));

        // Enumerate the counters again with the allocated memory
        status = PdhEnumObjectItemsHA(hDataSource, szMachineName, szObjectName, mszCounterList, &cchCounterListLength, mszInstanceList, &cchInstanceListLength, dwDetailLevel, dwFlags);
        if (status != ERROR_SUCCESS) {
            std::cerr << "Error enumerating counters: " << status << std::endl;
            free(mszCounterList);
            free(mszInstanceList);
            PdhCloseLog(hDataSource, 0);
            return 1;
        }

        // Output the counter and instance names
        std::cout << "Counters:" << std::endl;
        for (PZZSTR pCounter = mszCounterList; *pCounter != '\0'; pCounter += lstrlen(pCounter) + 1) {
            std::cout << "\t" << pCounter << std::endl;
        }

        std::cout << "Instances:" << std::endl;
        for (PZZSTR pInstance = mszInstanceList; *pInstance != '\0'; pInstance += lstrlen(pInstance) + 1) {
            std::cout << "\t" << pInstance << std::endl;
        }

        // Free the allocated memory
        free(mszCounterList);
        free(mszInstanceList);
    }
    else {
        std::cerr << "Error enumerating counters: " << status << std::endl;
        PdhCloseLog(hDataSource, 0);
        return 1;
    }

    // Close the log file when finished
    status = PdhCloseLog(hDataSource, 0);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing log file: " << status << std::endl;
        return 1;
    }

    return 0;
}
