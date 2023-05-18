#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    PDH_HLOG hLog;
    DWORD dwBufferSize = 0;
    LPSTR lpBuffer = nullptr;

    // Open a log file
    status = PdhOpenLogA("C:\\log.txt", PDH_LOG_READ_ACCESS, &hLog, 0);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening log file: " << status << std::endl;
        return 1;
    }

    // Enumerate the log set names
    status = PdhEnumLogSetNamesA(nullptr, lpBuffer, &dwBufferSize, PDH_LOG_TYPE_UNDEFINED);
    if (status == PDH_MORE_DATA) {
        lpBuffer = new char[dwBufferSize];
        status = PdhEnumLogSetNamesA(nullptr, lpBuffer, &dwBufferSize, PDH_LOG_TYPE_UNDEFINED);
    }
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error enumerating log set names: " << status << std::endl;
        PdhCloseLog(hLog, 0);
        delete[] lpBuffer;
        return 1;
    }

    // Use the log set names...
    std::cout << "Log set names:" << std::endl;
    for (LPSTR lpName = lpBuffer; *lpName != '\0'; lpName += strlen(lpName) + 1) {
        std::cout << lpName << std::endl;
    }

    // Close the log file when finished
    status = PdhCloseLog(hLog, 0);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing log file: " << status << std::endl;
        delete[] lpBuffer;
        return 1;
    }

    delete[] lpBuffer;
    return 0;
}
