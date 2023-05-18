#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;
    DWORD version;

    // Get the version of the PDH DLL
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhGetDllVersion(&version);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting PDH DLL version: " << status << std::endl;
        return 1;
    }

    std::cout << "PDH DLL version: " << HIWORD(version) << "." << LOWORD(version) << std::endl;

    return 0;
}
