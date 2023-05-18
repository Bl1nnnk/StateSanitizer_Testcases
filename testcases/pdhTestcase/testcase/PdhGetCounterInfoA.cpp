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
    PPDH_COUNTER_INFO_A counterInfo = nullptr;

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

    // Get the required buffer size for the counter info
    status = PdhGetCounterInfoA(counter, TRUE, &bufferSize, nullptr);
    if (status != PDH_MORE_DATA) {
        std::cerr << "Error getting counter info buffer size: " << status << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    // Allocate memory for the counter info
    counterInfo = (PPDH_COUNTER_INFO_A)malloc(bufferSize);
    if (counterInfo == nullptr) {
        std::cerr << "Error allocating memory for counter info" << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    // Get the counter info
    status = PdhGetCounterInfoA(counter, TRUE, &bufferSize, counterInfo);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error getting counter info: " << status << std::endl;
        free(counterInfo);
        PdhCloseQuery(query);
        return 1;
    }

    // Use the counter info...

    // Free the counter info memory
    free(counterInfo);

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
