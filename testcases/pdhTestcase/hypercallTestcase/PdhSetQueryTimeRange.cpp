#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;
    PDH_HQUERY query;

    // Open a query for performance data
    status = PdhOpenQueryW(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }

    // Set the time range for the query
    PDH_TIME_INFO timeInfo;
    timeInfo.StartTime = 0;
    timeInfo.EndTime = 0;
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhSetQueryTimeRange(query, &timeInfo);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error setting query time range: " << status << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    // Use the query with the specified time range...

    // Close the query when finished
    status = PdhCloseQuery(query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing query: " << status << std::endl;
        return 1;
    }

    return 0;
}
