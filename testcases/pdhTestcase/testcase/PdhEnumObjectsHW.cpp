#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    PDH_HLOG hDataSource = NULL;
    LPCWSTR szMachineName = NULL;
    PZZWSTR mszObjectList = NULL;
    DWORD dwDetailLevel = PERF_DETAIL_WIZARD;
    BOOL bRefresh = FALSE;
    DWORD dwBufferSize = 0;
    DWORD dwItemCount = 0;

    // Open a query for performance data
    status = PdhEnumObjectsHW(hDataSource, szMachineName, mszObjectList, &dwBufferSize, dwDetailLevel, bRefresh);
    if (status == PDH_MORE_DATA) {
        mszObjectList = (PZZWSTR)malloc(dwBufferSize * sizeof(WCHAR));
        if (mszObjectList == NULL) {
            std::cerr << "Error allocating memory for object list" << std::endl;
            return 1;
        }
        status = PdhEnumObjectsHW(hDataSource, szMachineName, mszObjectList, &dwBufferSize, dwDetailLevel, bRefresh);
    }
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error enumerating objects: " << status << std::endl;
        return 1;
    }

    // Print the list of objects
    PZZWSTR pObject = mszObjectList;
    while (*pObject != L'\0') {
        std::wcout << pObject << std::endl;
        pObject += wcslen(pObject) + 1;
        dwItemCount++;
    }

    // Free the memory allocated for the object list
    free(mszObjectList);

    return 0;
}
