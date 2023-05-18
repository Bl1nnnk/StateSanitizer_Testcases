#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;
    PDH_HQUERY query;
    DWORD bufferSize = MAX_PATH;
    LPWSTR dataSource = new WCHAR[bufferSize];

    // Open a query for performance data
    status = PdhOpenQueryW(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }

    // Select a data source
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhSelectDataSourceW(nullptr, PDH_FLAGS_FILE_BROWSER_ONLY, dataSource, &bufferSize);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error selecting data source: " << status << std::endl;
        delete[] dataSource;
        return 1;
    }
    // Select a data source
    status = PdhSelectDataSourceW(nullptr, 0, dataSource, &bufferSize);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error selecting data source: " << status << std::endl;
        delete[] dataSource;
        return 1;
    }

    delete[] dataSource;
    return 0;
}
