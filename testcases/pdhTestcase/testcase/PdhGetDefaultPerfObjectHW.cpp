#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    PDH_HLOG hDataSource = nullptr;
    LPCWSTR szMachineName = nullptr;
    LPWSTR szDefaultObjectName = nullptr;
    DWORD cchBufferSize = 0;

    PDH_HQUERY query;

    // Open a query for performance data
    status = PdhOpenQueryH(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }

    // Get the default performance object name
    status = PdhGetDefaultPerfObjectHW(hDataSource, szMachineName, szDefaultObjectName, &cchBufferSize);
    if (status == PDH_MORE_DATA) {
        // Allocate a buffer for the object name
        szDefaultObjectName = new WCHAR[cchBufferSize];
        if (szDefaultObjectName == nullptr) {
            std::cerr << "Error allocating buffer for object name" << std::endl;
            PdhCloseQuery(hDataSource);
            return 1;
        }

        // Get the default performance object name again with the allocated buffer
        status = PdhGetDefaultPerfObjectHW(hDataSource, szMachineName, szDefaultObjectName, &cchBufferSize);
    }

    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting default performance object name: " << status << std::endl;
        PdhCloseQuery(hDataSource);
        delete[] szDefaultObjectName;
        return 1;
    }

    // Use the default performance object name...

    // Close the query when finished
    status = PdhCloseQuery(query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing query: " << status << std::endl;
        return 1;
    }

    delete[] szDefaultObjectName;

    return 0;
}
