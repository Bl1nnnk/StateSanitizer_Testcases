#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    PDH_HLOG hLog;
    PDH_HQUERY query;
    LONGLONG llSize;
    DWORD logType = PDH_LOG_TYPE_CSV;

    // Open a query for performance data
    status = PdhOpenQueryW(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }
    LPCWSTR logfile = L"C:\\Users\\Public\\log.txt";
    //LPCWSTR logfile = L"counter.log";
    // Open the performance log file
    status = PdhOpenLogW(logfile, PDH_LOG_WRITE_ACCESS | PDH_LOG_CREATE_ALWAYS, &logType, query, 0, nullptr, &hLog);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening log file: " << status << std::endl;
        return 1;
    }

    // Get the size of the log file
    status = PdhGetLogFileSize(hLog, &llSize);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting log file size: " << status << std::endl;
        PdhCloseLog(hLog, 0);
        return 1;
    }

    std::cout << "Log file size: " << llSize << std::endl;

    // Close the log file when finished
    status = PdhCloseLog(hLog, 0);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing log file: " << status << std::endl;
        return 1;
    }

    return 0;
}
