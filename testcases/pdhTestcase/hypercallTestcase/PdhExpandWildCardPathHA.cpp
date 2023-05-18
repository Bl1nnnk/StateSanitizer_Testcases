#include "agent.h"
cpp
#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;
    PDH_HLOG hDataSource = nullptr;
    LPCSTR szWildCardPath = "\\Processor(*)\\% Processor Time";
    PZZSTR mszExpandedPathList = nullptr;
    DWORD pcchPathListLength = 0;
    DWORD dwFlags = 0;

    // Open a query for performance data
    status = PdhOpenLog(nullptr, 0, &hDataSource);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening log: " << status << std::endl;
        return 1;
    }

    // Expand the wildcard path
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhExpandWildCardPathHA(hDataSource, szWildCardPath, mszExpandedPathList, &pcchPathListLength, dwFlags);
    if (status == PDH_MORE_DATA) {
        mszExpandedPathList = new char[pcchPathListLength];
        status = PdhExpandWildCardPathHA(hDataSource, szWildCardPath, mszExpandedPathList, &pcchPathListLength, dwFlags);
    }
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error expanding wildcard path: " << status << std::endl;
        PdhCloseLog(hDataSource, 0);
        return 1;
    }

    // Use the expanded path list...

    // Close the log when finished
    status = PdhCloseLog(hDataSource, 0);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing log: " << status << std::endl;
        return 1;
    }

    delete[] mszExpandedPathList;

    return 0;
}
