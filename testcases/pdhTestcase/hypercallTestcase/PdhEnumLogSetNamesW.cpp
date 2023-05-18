#include "agent.h"
cpp
#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;
    PDH_HLOG hLog;
    DWORD dwBufferSize = 0;
    LPWSTR lpBuffer = nullptr;

    // Open a log file
    status = PdhOpenLogW(L"mylog.etl", PDH_LOG_READ_ACCESS, &hLog, NULL, 0, NULL, 0);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening log file: " << status << std::endl;
        return 1;
    }

    // Enumerate the log set names
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhEnumLogSetNamesW(NULL, lpBuffer, &dwBufferSize, PDH_LOG_TYPE_UNDEFINED);
    if (status == PDH_MORE_DATA) {
        lpBuffer = new WCHAR[dwBufferSize];
        status = PdhEnumLogSetNamesW(NULL, lpBuffer, &dwBufferSize, PDH_LOG_TYPE_UNDEFINED);
    }
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error enumerating log set names: " << status << std::endl;
        PdhCloseLog(hLog, 0);
        delete[] lpBuffer;
        return 1;
    }

    // Use the log set names...
    std::wcout << "Log set names:" << std::endl;
    LPWSTR lpLogSetName = lpBuffer;
    while (*lpLogSetName != L'\0') {
        std::wcout << lpLogSetName << std::endl;
        lpLogSetName += wcslen(lpLogSetName) + 1;
    }

    // Free the buffer and close the log file
    delete[] lpBuffer;
    status = PdhCloseLog(hLog, 0);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing log file: " << status << std::endl;
        return 1;
    }

    return 0;
}
