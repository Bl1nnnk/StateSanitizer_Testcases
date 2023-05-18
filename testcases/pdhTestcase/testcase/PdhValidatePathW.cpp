#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    LPCWSTR path = L"\\Processor(_Total)\\% Processor Time";

    // Validate the performance counter path
    status = PdhValidatePathW(path);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error validating path: " << status << std::endl;
        return 1;
    }

    // Use the validated path...

    return 0;
}
