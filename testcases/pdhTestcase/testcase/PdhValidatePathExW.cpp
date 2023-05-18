#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    PDH_HLOG hDataSource = nullptr;
    LPCWSTR szFullPathBuffer = L"\\Processor(_Total)\\% Processor Time";

    // Validate the performance counter path
    status = PdhValidatePathExW(hDataSource, szFullPathBuffer);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error validating performance counter path: " << status << std::endl;
        return 1;
    }

    // Use the validated performance counter path...

    return 0;
}
