#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
        PDH_STATUS status;
        PDH_HQUERY query;
        PDH_HLOG logHandle;

        DWORD dwLogType = PDH_LOG_TYPE_CSV;
        // Open a query for performance data
        
        status = PdhOpenQueryA(NULL, 0, &query);
        if (status != ERROR_SUCCESS) {
            std::cerr << "Error opening query: " << status << std::endl;
            return 1;
        }

        // Open a log file for writing
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
        status = PdhOpenLogA("C:\\Users\\Public\\log.txt",
            PDH_LOG_WRITE_ACCESS | PDH_LOG_CREATE_ALWAYS,
            &dwLogType,
            query,
            0,
            nullptr,
            &logHandle);

        if (status != ERROR_SUCCESS) {
            std::cerr << "Error opening log file: " << status << std::endl;
            PdhCloseQuery(query);
            return 1;
        }

        // Close the log file
        status = PdhCloseLog(logHandle, 0);
        if (status != ERROR_SUCCESS) {
            std::cerr << "Error closing log file: " << status << std::endl;
            PdhCloseQuery(query);
            return 1;
        }

        // Close the query when finished
        status = PdhCloseQuery(query);
        if (status != ERROR_SUCCESS) {
            std::cerr << "Error closing query: " << status << std::endl;
            return 1;
        }

        return 0;
}
