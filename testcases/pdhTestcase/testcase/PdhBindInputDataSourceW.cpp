#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    PDH_HLOG hDataSource;

    // Bind to a log file data source
    status = PdhBindInputDataSourceW(&hDataSource, L"C:\\Users\\Public\\DESKTOP-O20858E_20230425-000001\\WDAC_Diagnostics.etl");

    if (status != ERROR_SUCCESS) {
        std::cerr << "Error binding to data source: " << status << std::endl;
        return 1;
    }

    // Unbind from the data source
    status = PdhCloseLog(hDataSource, 0);

    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing data source: " << status << std::endl;
        return 1;
    }

    return 0;
}
