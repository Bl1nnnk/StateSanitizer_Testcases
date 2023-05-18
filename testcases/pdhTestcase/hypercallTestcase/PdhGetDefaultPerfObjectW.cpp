#include "agent.h"
#include <windows.h>
#include <pdhmsg.h>
#include <pdh.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;
    LPWSTR szDefaultObjectName = nullptr;
    DWORD pcchBufferSize = 0;

    // Get the default performance object name
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhGetDefaultPerfObjectW(nullptr, nullptr, szDefaultObjectName, &pcchBufferSize);
    if (status == PDH_MORE_DATA) {
        szDefaultObjectName = new WCHAR[pcchBufferSize];
        status = PdhGetDefaultPerfObjectW(nullptr, nullptr, szDefaultObjectName, &pcchBufferSize);
    }
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting default performance object name: " << status << std::endl;
        return 1;
    }

    // Use the default performance object name...
    std::wcout << L"Default performance object name: " << szDefaultObjectName << std::endl;

    // Free the memory allocated for the performance object name
    delete[] szDefaultObjectName;

    return 0;
}
