cpp
#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    PDH_HLOG hDataSource = nullptr;
    PZZWSTR mszMachineList = nullptr;
    DWORD dwBufferSize = 0;

    // Open a query for performance data
    status = PdhOpenLog(nullptr, PDH_LOG_READ_ACCESS | PDH_LOG_OPEN_EXISTING, nullptr, 0, nullptr, &hDataSource);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening log: " << status << std::endl;
        return 1;
    }

    // Enumerate the machines in the log
    status = PdhEnumMachinesHW(hDataSource, mszMachineList, &dwBufferSize);
    if (status == PDH_MORE_DATA) {
        mszMachineList = static_cast<PZZWSTR>(malloc(dwBufferSize * sizeof(WCHAR)));
        if (mszMachineList == nullptr) {
            std::cerr << "Error allocating memory for machine list" << std::endl;
            PdhCloseLog(hDataSource, 0);
            return 1;
        }
        status = PdhEnumMachinesHW(hDataSource, mszMachineList, &dwBufferSize);
    }
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error enumerating machines: " << status << std::endl;
        PdhCloseLog(hDataSource, 0);
        free(mszMachineList);
        return 1;
    }

    // Use the machine list...

    // Close the log when finished
    status = PdhCloseLog(hDataSource, 0);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing log: " << status << std::endl;
        return 1;
    }

    free(mszMachineList);

    return 0;
}
