#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;
    LPCWSTR path = L"\\Processor(_Total)\\% Processor Time";

    // Validate the performance counter path
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhValidatePathW(path);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error validating path: " << status << std::endl;
        return 1;
    }

    // Use the validated path...

    return 0;
}
