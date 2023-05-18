#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    DWORD version;

    // Get the version of the PDH DLL
    status = PdhGetDllVersion(&version);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting PDH DLL version: " << status << std::endl;
        return 1;
    }

    std::cout << "PDH DLL version: " << HIWORD(version) << "." << LOWORD(version) << std::endl;

    return 0;
}
