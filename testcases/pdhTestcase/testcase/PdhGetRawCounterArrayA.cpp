#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    PDH_HQUERY query;
    PDH_HCOUNTER counter;
    DWORD bufferSize = 0;
    DWORD itemCount = 0;
    PPDH_RAW_COUNTER_ITEM_A itemBuffer = nullptr;

    // Open a query for performance data
    status = PdhOpenQueryW(nullptr, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error opening query: " << status << std::endl;
        return 1;
    }

    // Add a counter to the query
    status = PdhAddCounterA(query, "\\Processor(_Total)\\% Processor Time", 0, &counter);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error adding counter: " << status << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    // Collect raw counter data
    status = PdhCollectQueryData(query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error collecting query data: " << status << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    // Get the required buffer size for the raw counter data
    status = PdhGetRawCounterArrayA(counter, &bufferSize, &itemCount, nullptr);
    if (status != PDH_MORE_DATA) {
        std::cerr << "Error getting buffer size: " << status << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    // Allocate memory for the raw counter data buffer
    itemBuffer = (PPDH_RAW_COUNTER_ITEM_A)malloc(bufferSize);
    if (itemBuffer == nullptr) {
        std::cerr << "Error allocating buffer: " << GetLastError() << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    // Get the raw counter data
    status = PdhGetRawCounterArrayA(counter, &bufferSize, &itemCount, itemBuffer);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting raw counter data: " << status << std::endl;
        free(itemBuffer);
        PdhCloseQuery(query);
        return 1;
    }

    // Use the raw counter data...

    // Free the raw counter data buffer
    free(itemBuffer);

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