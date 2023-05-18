#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    PDH_COUNTER_PATH_ELEMENTS_A counterPathElements;
    DWORD bufferSize = PDH_MAX_COUNTER_PATH;
    char fullPathBuffer[PDH_MAX_COUNTER_PATH];

    // Set the counter path to parse
    const char* counterPath = "\\Processor(_Total)\\% Processor Time";

    // Parse the counter path
    status = PdhParseCounterPathA(counterPath, &counterPathElements, &bufferSize, 0);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error parsing counter path: " << status << std::endl;
        return 1;
    }

    // Print the parsed counter path elements
    std::cout << "Machine name: " << counterPathElements.szMachineName << std::endl;
    std::cout << "Object name: " << counterPathElements.szObjectName << std::endl;
    std::cout << "Instance name: " << counterPathElements.szInstanceName << std::endl;
    std::cout << "Parent instance: " << counterPathElements.szParentInstance << std::endl;
    std::cout << "Counter name: " << counterPathElements.szCounterName << std::endl;

    return 0;
}
