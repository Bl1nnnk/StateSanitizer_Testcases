#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;
    PDH_HQUERY query;
    PDH_HCOUNTER counter;
    LONG scale = 2; // set the scale factor to 2

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

    // Set the scale factor for the counter
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhSetCounterScaleFactor(counter, scale);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error setting scale factor: " << status << std::endl;
        PdhRemoveCounter(counter);
        PdhCloseQuery(query);
        return 1;
    }

    // Collect data for the counter
    status = PdhCollectQueryData(query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error collecting data: " << status << std::endl;
        PdhRemoveCounter(counter);
        PdhCloseQuery(query);
        return 1;
    }
    for (int fmt = PDH_FMT_LONG; fmt <= PDH_FMT_LARGE; fmt *= 2) {
        for (int ret = PDH_FMT_1000; ret <= PDH_FMT_NOSCALE; ret *= 2) {
            int options = fmt | ret;
            // Get the value of the counter
            PDH_FMT_COUNTERVALUE value;
            status = PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, nullptr, &value);
            if (status != ERROR_SUCCESS) {
                std::cerr << "Error getting counter value: " << status << std::endl;
                PdhRemoveCounter(counter);
                PdhCloseQuery(query);
                return 1;
            }

            // Print the value of the counter
            std::cout << "Counter value: " << value.doubleValue << std::endl;

            // Remove the counter from the query
            status = PdhRemoveCounter(counter);
            if (status != ERROR_SUCCESS) {
                std::cerr << "Error removing counter: " << status << std::endl;
                PdhCloseQuery(query);
                return 1;
            }
        }
    }

    // Close the query when finished
    status = PdhCloseQuery(query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error closing query: " << status << std::endl;
        return 1;
    }

    return 0;
}
