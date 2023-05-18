#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_HQUERY query;
    PDH_STATUS status;
    PDH_HLOG hLog;
    int runId = 12345; // example run ID value
    DWORD dwLogType = PDH_LOG_TYPE_CSV;

    // Open a query for performance data
    status = PdhOpenQueryW(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }

    // Open a log file for writing

    status = PdhOpenLogW(L"C:\\Users\\Public\\log.txt", PDH_LOG_WRITE_ACCESS | PDH_LOG_CREATE_ALWAYS, &dwLogType, query, 0, nullptr, &hLog);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening log file: " << status << std::endl;
        return 1;
    }

    // Set the run ID for the log file
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhSetLogSetRunID(hLog, runId);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error setting log file run ID: " << status << std::endl;
        PdhCloseLog(hLog, 0);
        return 1;
    }
    std::cout << "run id: " << runId << std::endl;

    // Close the log file when finished
    status = PdhCloseLog(hLog, 0);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing log file: " << status << std::endl;
        return 1;
    }

    return 0;
}
