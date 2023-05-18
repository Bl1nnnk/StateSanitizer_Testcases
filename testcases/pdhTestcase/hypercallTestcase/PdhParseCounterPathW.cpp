#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;
    PDH_COUNTER_PATH_ELEMENTS_W counterPathElements;
    DWORD bufferSize = PDH_MAX_COUNTER_PATH;
    LPWSTR fullPathBuffer = L"\\Processor(_Total)\\% Processor Time";

    // Parse the counter path
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhParseCounterPathW(fullPathBuffer, &counterPathElements, &bufferSize, 0);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error parsing counter path: " << status << std::endl;
        return 1;
    }

    // Use the parsed counter path elements...
    std::wcout << "Machine name: " << counterPathElements.szMachineName << std::endl;
    std::wcout << "Object name: " << counterPathElements.szObjectName << std::endl;
    std::wcout << "Instance name: " << counterPathElements.szInstanceName << std::endl;
    std::wcout << "Parent instance: " << counterPathElements.szParentInstance << std::endl;
    std::wcout << "Counter name: " << counterPathElements.szCounterName << std::endl;

    return 0;
}
