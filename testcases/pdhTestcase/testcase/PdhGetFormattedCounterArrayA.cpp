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
    PPDH_FMT_COUNTERVALUE_ITEM_A itemBuffer = nullptr;

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

    // Collect data for the counter
    status = PdhCollectQueryData(query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error collecting query data: " << status << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    for (int fmt = PDH_FMT_LONG; fmt <= PDH_FMT_LARGE; fmt *= 2) {
        for (int ret = PDH_FMT_1000; ret <= PDH_FMT_NOSCALE; ret *= 2) {
            int options = fmt | ret;
            // Get the formatted counter values
            status = PdhGetFormattedCounterArrayA(counter, options, &bufferSize, &itemCount, itemBuffer);
            if (status == PDH_MORE_DATA) {
                itemBuffer = (PPDH_FMT_COUNTERVALUE_ITEM_A)malloc(bufferSize);
                if (itemBuffer == nullptr) {
                    std::cerr << "Error allocating memory for item buffer" << std::endl;
                    PdhCloseQuery(query);
                    return 1;
                }
            }
            status = PdhGetFormattedCounterArrayA(counter, options, &bufferSize, &itemCount, itemBuffer);
            if (status != ERROR_SUCCESS) {
                std::cerr << "Error getting formatted counter array: " << status << std::endl;
                PdhCloseQuery(query);
                free(itemBuffer);
                return 1;
            }
        }
    }

    // Clean up
    PdhCloseQuery(query);
    free(itemBuffer);

    return 0;
}
