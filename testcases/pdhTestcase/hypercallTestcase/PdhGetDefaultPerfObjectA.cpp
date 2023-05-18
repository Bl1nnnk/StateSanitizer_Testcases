#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;
    LPSTR szDefaultObjectName = nullptr;
    DWORD dwBufferSize = 0;

    // Determine the required buffer size
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhGetDefaultPerfObjectA(nullptr, nullptr, szDefaultObjectName, &dwBufferSize);
    if (status != ERROR_SUCCESS && status != PDH_MORE_DATA) {
        std::cerr << "Error getting default object name buffer size: " << status << std::endl;
        return 1;
    }

    // Allocate the buffer
    szDefaultObjectName = new char[dwBufferSize];

    // Get the default object name
    status = PdhGetDefaultPerfObjectA(nullptr, nullptr, szDefaultObjectName, &dwBufferSize);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting default object name: " << status << std::endl;
        delete[] szDefaultObjectName;
        return 1;
    }

    // Use the default object name...

    // Free the buffer
    delete[] szDefaultObjectName;

    return 0;
}
