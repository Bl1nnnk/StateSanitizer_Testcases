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
    LPCSTR szMachineName = nullptr;
    LPSTR szDefaultObjectName = nullptr;
    DWORD cchBufferSize = 0;

    PDH_HQUERY query;

    // Open a query for performance data
    status = PdhOpenQueryH(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }

    // Get the default performance object name
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhGetDefaultPerfObjectHA(hDataSource, szMachineName, szDefaultObjectName, &cchBufferSize);
    if (status == PDH_MORE_DATA) {
        // Allocate a buffer for the object name
        szDefaultObjectName = new char[cchBufferSize];
        if (szDefaultObjectName == nullptr) {
            std::cerr << "Error allocating buffer for object name" << std::endl;
            PdhCloseQuery(hDataSource);
            return 1;
        }

        // Get the default performance object name again with the allocated buffer
        status = PdhGetDefaultPerfObjectHA(hDataSource, szMachineName, szDefaultObjectName, &cchBufferSize);
    }

    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting default performance object name: " << status << std::endl;
        PdhCloseQuery(hDataSource);
        delete[] szDefaultObjectName;
        return 1;
    }

    std::cout << "Default performance object name: " << szDefaultObjectName << std::endl;

    // Close the query when finished
    status = PdhCloseQueryH(hDataSource);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing query: " << status << std::endl;
        return 1;
    }

    delete[] szDefaultObjectName;

    return 0;
}
