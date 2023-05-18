#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    PDH_HLOG hLog;
    PDH_HQUERY query;
    HCOUNTER counter;
    DWORD dwCount;
    DWORD dwLogType = PDH_LOG_TYPE_CSV;
    CONST ULONG SAMPLE_INTERVAL_MS = 1000;

    // Open a query for performance data
    status = PdhOpenQueryW(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }
    // Add one counter that will provide the data.
    status = PdhAddCounterA(query,
        "\\Processor(0)\\% Processor Time",
        0,
        &counter); 
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error adding counter: " << status << std::endl;
        return 1;
    }

    // Open a log file for writing
    status = PdhOpenLogA("C:\\Users\\Public\\log.txt", PDH_LOG_WRITE_ACCESS | PDH_LOG_CREATE_ALWAYS, &dwLogType, query, 0, nullptr, &hLog);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening log file: " << status << std::endl;
        return 1;
    }
    // Write 10 records to the log file.
    for (dwCount = 0; dwCount < 10; dwCount++)
    {
        wprintf(L"Writing record %d\n", dwCount);

        status = PdhUpdateLog(hLog, NULL);
        if (ERROR_SUCCESS != status)
        {
            wprintf(L"PdhUpdateLog failed with 0x%x\n", status);
            return 1;
        }

        // Wait one second between samples for a counter update.
        Sleep(SAMPLE_INTERVAL_MS);
    }

    // Close the log file when finished
    status = PdhCloseLog(hLog, 0);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing log file: " << status << std::endl;
        return 1;
    }

    return 0;
}
