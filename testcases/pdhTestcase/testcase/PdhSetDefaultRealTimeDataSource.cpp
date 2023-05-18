#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

int main()
{
    PDH_STATUS status;

    // Set the default real-time data source
    status = PdhSetDefaultRealTimeDataSource(DATA_SOURCE_WBEM);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error setting default real-time data source: " << status << std::endl;
        return 1;
    }

    return 0;
}
