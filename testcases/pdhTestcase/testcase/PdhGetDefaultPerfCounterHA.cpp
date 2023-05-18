#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    PDH_HLOG hDataSource = nullptr;
    LPCSTR szMachineName = nullptr;
    LPCSTR szObjectName = "Processor";
    LPSTR szDefaultCounterName = nullptr;
    DWORD pcchBufferSize = 0;

    PDH_HQUERY query;

    // Open a query for performance data
    status = PdhOpenQueryH(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }

    // Get the default counter name for the specified object
    status = PdhGetDefaultPerfCounterHA(hDataSource, szMachineName, szObjectName, szDefaultCounterName, &pcchBufferSize);
    if (status == PDH_MORE_DATA) {
        szDefaultCounterName = new char[pcchBufferSize];
        status = PdhGetDefaultPerfCounterHA(hDataSource, szMachineName, szObjectName, szDefaultCounterName, &pcchBufferSize);
    }
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting default counter name: " << status << std::endl;
        PdhCloseLog(hDataSource, 0);
        return 1;
    }

    std::cout << "Default counter name for object " << szObjectName << ": " << szDefaultCounterName << std::endl;

    // Clean up
    delete[] szDefaultCounterName;
    PdhCloseLog(hDataSource, 0);

    return 0;
}
