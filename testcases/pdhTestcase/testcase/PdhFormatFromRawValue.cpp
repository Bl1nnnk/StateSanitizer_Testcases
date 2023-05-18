#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    PDH_HQUERY query;
    PDH_HCOUNTER counter;
    PDH_RAW_COUNTER rawCounter;
    PDH_FMT_COUNTERVALUE fmtValue;

    // Open a query for performance data
    status = PdhOpenQueryW(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }

    // Add a counter to the query
    status = PdhAddCounterW(query, L"\\Processor(_Total)\\% Processor Time", 0, &counter);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error adding counter: " << status << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    // Collect a raw counter value
    status = PdhCollectQueryData(query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error collecting query data: " << status << std::endl;
        PdhRemoveCounter(counter);
        PdhCloseQuery(query);
        return 1;
    }

    // Get the raw counter value
    status = PdhGetRawCounterValue(counter, nullptr, &rawCounter);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting raw counter value: " << status << std::endl;
        PdhRemoveCounter(counter);
        PdhCloseQuery(query);
        return 1;
    }

    // Format the raw counter value
    status = PdhFormatFromRawValue(PDH_FMT_DOUBLE, PDH_FMT_NOSCALE, nullptr, &rawCounter, nullptr, &fmtValue);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error formatting raw counter value: " << status << std::endl;
        PdhRemoveCounter(counter);
        PdhCloseQuery(query);
        return 1;
    }

    // Use the formatted value...
    std::cout << "Formatted value: " << fmtValue.doubleValue << std::endl;

    // Remove the counter from the query
    status = PdhRemoveCounter(counter);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error removing counter: " << status << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    // Close the query when finished
    status = PdhCloseQuery(query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing query: " << status << std::endl;
        return 1;
    }

    return 0;
}
