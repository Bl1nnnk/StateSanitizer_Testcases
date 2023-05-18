#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;
    PDH_HLOG hDataSource = nullptr;
    LPCWSTR szMachineName = nullptr;
    LPCWSTR szObjectName = L"Processor";
    LPWSTR szDefaultCounterName = nullptr;
    DWORD cchBufferSize = 0;

    PDH_HQUERY query;

    // Open a query for performance data
    status = PdhOpenQueryH(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }

    // Get the default counter name for the specified object
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhGetDefaultPerfCounterHW(hDataSource, szMachineName, szObjectName, szDefaultCounterName, &cchBufferSize);
    if (status == PDH_MORE_DATA) {
        // Allocate a buffer for the counter name
        szDefaultCounterName = new WCHAR[cchBufferSize];

        // Get the default counter name again with the allocated buffer
        status = PdhGetDefaultPerfCounterHW(hDataSource, szMachineName, szObjectName, szDefaultCounterName, &cchBufferSize);
    }

    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting default counter name: " << status << std::endl;
        PdhCloseQuery(hDataSource);
        return 1;
    }

    std::wcout << L"Default counter name for object " << szObjectName << L": " << szDefaultCounterName << std::endl;

    // Clean up
    delete[] szDefaultCounterName;
    PdhCloseQuery(hDataSource);

    return 0;
}
