#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    PDH_HLOG hDataSource = NULL;
    LPCSTR szMachineName = NULL;
    PZZSTR mszObjectList = NULL;
    DWORD dwDetailLevel = PERF_DETAIL_WIZARD;
    BOOL bRefresh = FALSE;
    DWORD dwBufferSize = 0;
    DWORD dwObjectCount = 0;

    // Open a query for performance data
    status = PdhEnumObjectsHA(hDataSource, szMachineName, mszObjectList, &dwBufferSize, dwDetailLevel, bRefresh);
    if (status == PDH_MORE_DATA) {
        mszObjectList = (PZZSTR)malloc(dwBufferSize * sizeof(TCHAR));
        if (mszObjectList == NULL) {
            std::cerr << "Error allocating memory for object list" << std::endl;
            return 1;
        }
        status = PdhEnumObjectsHA(hDataSource, szMachineName, mszObjectList, &dwBufferSize, dwDetailLevel, bRefresh);
    }
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error enumerating objects: " << status << std::endl;
        free(mszObjectList);
        return 1;
    }

    // Count the number of objects returned
    PZZSTR pObject = mszObjectList;
    while (*pObject != '\0') {
        dwObjectCount++;
        pObject += lstrlen(pObject) + 1;
    }

    // Output the object names
    std::cout << "Object count: " << dwObjectCount << std::endl;
    pObject = mszObjectList;
    while (*pObject != '\0') {
        std::cout << "Object name: " << pObject << std::endl;
        pObject += lstrlen(pObject) + 1;
    }

    // Free the memory allocated for the object list
    free(mszObjectList);

    return 0;
}
