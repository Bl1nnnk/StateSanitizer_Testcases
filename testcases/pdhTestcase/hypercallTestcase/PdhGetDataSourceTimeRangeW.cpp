#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;
    PDH_TIME_INFO timeInfo;
    DWORD numEntries = 0;
    DWORD bufferSize = sizeof(PDH_TIME_INFO);

    LPCWSTR datasource = L"C:\\Users\\Public\\DESKTOP-O20858E_20230425-000001\\WDAC_Diagnostics.etl";
    // Get the time range for the default data source
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhGetDataSourceTimeRangeW(datasource, &numEntries, &timeInfo, &bufferSize);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting data source time range: " << status << std::endl;
        return 1;
    }

    // Use the time range data...

    return 0;
}
