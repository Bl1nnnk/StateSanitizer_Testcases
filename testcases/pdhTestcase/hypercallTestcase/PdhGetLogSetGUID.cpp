#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;
    PDH_HLOG hLog;
    GUID guid;
    int runId;
    PDH_HQUERY query;
    DWORD dwLogType = PDH_LOG_TYPE_CSV;

    // Open a query for performance data
    status = PdhOpenQueryH(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }

    // Open a log file
    status = PdhOpenLogW(L"C:\\Users\\Public\\log.txt", PDH_LOG_WRITE_ACCESS | PDH_LOG_CREATE_ALWAYS, &dwLogType, query, 0, nullptr, &hLog);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening log file: " << status << std::endl;
        return 1;
    }

    // Get the GUID and run ID of the log file
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhGetLogSetGUID(hLog, &guid, &runId);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting log set GUID: " << status << std::endl;
        PdhCloseLog(hLog, 0);
        return 1;
    }

    // Use the GUID and run ID...

    // Close the log file when finished
    status = PdhCloseLog(hLog, 0);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing log file: " << status << std::endl;
        return 1;
    }

    return 0;
}
