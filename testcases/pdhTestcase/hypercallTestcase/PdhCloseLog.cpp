#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_HLOG hLog = NULL;
    PDH_STATUS pdhStatus = 0;
    PDH_HQUERY query;
    DWORD dwLogType = PDH_LOG_TYPE_CSV;

    pdhStatus = PdhOpenQueryW(nullptr, 0, &query);
    if (pdhStatus != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << pdhStatus << std::endl;
        return 1;
    }

    
    //pdhStatus = PdhOpenLogA("C:\\Users\\Public\\WDAC_Diagnostics.etl", PDH_LOG_WRITE_ACCESS | PDH_LOG_CREATE_ALWAYS, &dwLogType, query, 0, nullptr, &hLog);
    // Open a log file for writing
    pdhStatus = PdhOpenLogA("C:\\Users\\Public\\log.txt",
        PDH_LOG_WRITE_ACCESS | PDH_LOG_CREATE_ALWAYS,
        &dwLogType,
        query,
        0,
        nullptr,
        &hLog);
    if (pdhStatus != ERROR_SUCCESS) {
        std::cerr << "Error opening log file: " << pdhStatus << std::endl;
        PdhCloseQuery(query);
        return 1;
    }
    // ... do some logging ...

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    pdhStatus = PdhCloseLog(hLog, PDH_FLAGS_CLOSE_QUERY);
    if (pdhStatus != ERROR_SUCCESS)
    {
        std::cerr << "Error closing log: " << pdhStatus << std::endl;
        return 1;
    }
    return 0;
}