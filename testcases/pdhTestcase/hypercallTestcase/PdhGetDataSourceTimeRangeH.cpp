#include "agent.h"
cpp
#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;
    PDH_HLOG hDataSource = NULL;
    DWORD dwNumEntries = 0;
    PDH_TIME_INFO pInfo = { 0 };
    DWORD dwBufferSize = sizeof(PDH_TIME_INFO);

    // Open a data source
    status = PdhOpenLogW(L"perflog", PDH_LOG_READ_ACCESS | PDH_LOG_OPEN_EXISTING, &hDataSource);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening data source: " << status << std::endl;
        return 1;
    }

    // Get the time range of the data source
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhGetDataSourceTimeRangeH(hDataSource, &dwNumEntries, &pInfo, &dwBufferSize);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting data source time range: " << status << std::endl;
        PdhCloseLog(hDataSource, 0);
        return 1;
    }

    // Use the time range data...

    // Close the data source when finished
    status = PdhCloseLog(hDataSource, 0);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing data source: " << status << std::endl;
        return 1;
    }

    return 0;
}
