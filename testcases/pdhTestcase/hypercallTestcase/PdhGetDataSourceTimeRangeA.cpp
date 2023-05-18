#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;
    PDH_TIME_INFO timeInfo;
    DWORD numEntries = 0;
    DWORD bufferSize = sizeof(PDH_TIME_INFO);


    LPCSTR datasource = "C:\\Users\\Public\\log.txt";
    // Get the time range for the default data source
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhGetDataSourceTimeRangeA(datasource, &numEntries, &timeInfo, &bufferSize);
    if (status != ERROR_SUCCESS) {
        if (status == PDH_INVALID_ARGUMENT) {
             std::cerr << "Error getting data source time range: PDH_INVALID_ARGUMENT" << GetLastError() << std::endl;
             return 1;
        }
        else if (status == PDH_INVALID_HANDLE) {
            std::cerr << "Error getting data source time range: PDH_INVALID_HANDLE" << GetLastError() << std::endl;
            return 1;
        } else if (status == PDH_DATA_SOURCE_IS_REAL_TIME) {
            std::cerr << "Error getting data source time range: PDH_DATA_SOURCE_IS_REAL_TIME" << GetLastError() << std::endl;
            return 1;
        }
        else {
            std::cerr << "Error getting data source time range:" << GetLastError() << std::endl;
            return 1;
        }
    }
    std::cout << "Min time: " << timeInfo.StartTime << std::endl;
    std::cout << "Max time: " << timeInfo.EndTime << std::endl;

    
    // Use the time range data...

    return 0;
}
