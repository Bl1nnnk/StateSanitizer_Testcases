#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;

    // Set the default real-time data source
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhSetDefaultRealTimeDataSource(DATA_SOURCE_WBEM);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error setting default real-time data source: " << status << std::endl;
        return 1;
    }

    return 0;
}
