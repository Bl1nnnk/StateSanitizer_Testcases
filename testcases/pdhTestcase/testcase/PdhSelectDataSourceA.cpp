#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    PDH_HQUERY query;
    DWORD bufferSize = MAX_PATH;
    char dataSource[MAX_PATH];

    // Open a query for performance data
    status = PdhOpenQueryW(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }

    // Select a data source
    status = PdhSelectDataSourceA(nullptr, 0, dataSource, &bufferSize);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error selecting data source: " << status << std::endl;
        return 1;
    }
    status = PdhSelectDataSourceA(nullptr, PDH_FLAGS_FILE_BROWSER_ONLY, dataSource, &bufferSize);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error selecting data source: " << status << std::endl;
        return 1;
    }

    return 0;
}
