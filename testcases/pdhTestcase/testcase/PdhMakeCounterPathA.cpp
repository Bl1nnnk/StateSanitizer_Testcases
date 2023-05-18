#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    PDH_COUNTER_PATH_ELEMENTS_A counterPathElements = { 0 };
    counterPathElements.szMachineName = ".";
    counterPathElements.szObjectName = "Processor";
    counterPathElements.szInstanceName = "_Total";
    counterPathElements.szCounterName = "% Processor Time";

    // Determine the required buffer size
    DWORD bufferSize = 0;
    status = PdhMakeCounterPathA(&counterPathElements, nullptr, &bufferSize, 0);
    if (status != PDH_MORE_DATA) {
        std::cerr << "Error determining buffer size: " << status << std::endl;
        return 1;
    }

    // Allocate the buffer
    LPSTR buffer = new CHAR[bufferSize];

    // Create the counter path
    status = PdhMakeCounterPathA(&counterPathElements, buffer, &bufferSize, 0);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error creating counter path: " << status << std::endl;
        delete[] buffer;
        return 1;
    }

    // Use the counter path...
    std::cout << "Counter path: " << buffer << std::endl;

    // Free the buffer
    delete[] buffer;

    return 0;
}
