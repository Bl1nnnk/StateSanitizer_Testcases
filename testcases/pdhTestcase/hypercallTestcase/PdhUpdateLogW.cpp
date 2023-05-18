#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;
    PDH_HLOG hLog;
    PDH_HQUERY query;
    LPCWSTR szUserString = L"User-defined string";

    // Open a query for performance data
    status = PdhOpenQueryW(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }

    // Open a log file for writing
    status = PdhOpenLogA("C:\\log.txt", PDH_LOG_WRITE_ACCESS | PDH_LOG_CREATE_ALWAYS, NULL, query, 0, nullptr, &hLog);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening log file: " << status << std::endl;
        return 1;
    }

    // Update the log file with new data
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhUpdateLogW(hLog, szUserString);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error updating log file: " << status << std::endl;
        PdhCloseLog(hLog, 0);
        return 1;
    }

    // Close the log file when finished
    status = PdhCloseLog(hLog, 0);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing log file: " << status << std::endl;
        return 1;
    }

    return 0;
}
