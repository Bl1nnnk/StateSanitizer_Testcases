#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    PDH_HQUERY query;
    LONGLONG timestamp;

    // Open a query for performance data
    status = PdhOpenQueryW(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }

    // Add a counter to the query
    PDH_HCOUNTER counter;
    const char* counterPath = "\\Processor(_Total)\\% Processor Time";
    status = PdhAddCounterA(query, counterPath, 0, &counter);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error adding counter: " << status << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    // Collect data with timestamp
    status = PdhCollectQueryDataWithTime(query, &timestamp);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error collecting data: " << status << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    // Use the collected data...
    std::cout << "timestamp is " << timestamp << std::endl;
    // Close the query when finished
    status = PdhCloseQuery(query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing query: " << status << std::endl;
        return 1;
    }

    return 0;
}
