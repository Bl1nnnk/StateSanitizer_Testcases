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
    FILETIME ftRecord;
    DWORD dwBufferLength = 0;

    // Open a query for performance data
    status = PdhOpenQueryW(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }

    // Open a log file for writing
    status = PdhOpenLogW(L"C:\\log.txt", PDH_LOG_WRITE_ACCESS | PDH_LOG_CREATE_ALWAYS, NULL, query, 0, nullptr, &hLog);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening log file: " << status << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    // Get the first record in the log file
    status = PdhGetLogFileSize(hLog, (LONGLONG*)&dwBufferLength);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting log file size: " << status << std::endl;
        PdhCloseLog(hLog, 0);
        return 1;
    }

    PDH_RAW_LOG_RECORD* pRawLogRecord = NULL;
    DWORD dwBufferSize = 0;
    DWORD dwRecordCount = 0;

    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    // Call PdhReadRawLogRecord() the first time to get the buffer size required
    status = PdhReadRawLogRecord(
        hLog,
        NULL,
        &dwBufferSize,
        &dwRecordCount
    );

    if (status != PDH_MORE_DATA) {
        std::cerr << "Failed to get the buffer size required. Error code: " << status << std::endl;
        return 1;
    }

    pRawLogRecord = (PDH_RAW_LOG_RECORD*)malloc(dwBufferSize);
    if (!pRawLogRecord) {
        std::cerr << "Failed to allocate memory." << std::endl;
        return 1;
    }

    // Call PdhReadRawLogRecord() the second time to get the data
    status = PdhReadRawLogRecord(
        hLog,
        NULL,
        &dwBufferSize,
        &dwRecordCount
    );

    if (status != ERROR_SUCCESS) {
        std::cerr << "Failed to read log record. Error code: " << status << std::endl;
        free(pRawLogRecord);
        PdhCloseLog(hLog);
        return 1;
    }

    // Free the memory allocated for the log record
    free(pRawLogRecord);

    // Close the log file when finished
    status = PdhCloseLog(hLog, 0);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing log file: " << status << std::endl;
        return 1;
    }

    return 0;
}
