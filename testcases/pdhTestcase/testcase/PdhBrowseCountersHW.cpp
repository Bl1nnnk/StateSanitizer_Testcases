#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

int main()
{
    PDH_STATUS status;
    PDH_BROWSE_DLG_CONFIG_HW browseDlgData = { 0 };
    browseDlgData.bIncludeInstanceIndex = TRUE;
    browseDlgData.bSingleCounterPerAdd = TRUE;
    browseDlgData.bSingleCounterPerDialog = TRUE;
    browseDlgData.bLocalCountersOnly = FALSE;
    browseDlgData.bWildCardInstances = TRUE;
    browseDlgData.bHideDetailBox = FALSE;
    browseDlgData.bInitializePath = TRUE;
    browseDlgData.bDisableMachineSelection = FALSE;
    browseDlgData.bIncludeCostlyObjects = FALSE;
    browseDlgData.bShowObjectBrowser = TRUE;
    browseDlgData.hWndOwner = NULL;
    browseDlgData.szReturnPathBuffer = NULL;
    browseDlgData.cchReturnPathLength = 0;
    browseDlgData.pCallBack = NULL;
    browseDlgData.dwCallBackArg = 0;
    browseDlgData.CallBackStatus = ERROR_SUCCESS;
    browseDlgData.dwDefaultDetailLevel = PERF_DETAIL_WIZARD;
    browseDlgData.szDialogBoxCaption = L"Select a counter";
    browseDlgData.szDataSource = NULL;
    browseDlgData.dwFlags = 0;
    browseDlgData.szHelpFileName = NULL;
    browseDlgData.dwHelpContext = 0;
    browseDlgData.szMachineName = NULL;
    browseDlgData.szObjectHelp = NULL;
    browseDlgData.szObjectList = NULL;
    browseDlgData.szObjectListParent = NULL;
    browseDlgData.szObjectName = NULL;
    browseDlgData.szReturnObjectName = NULL;
    browseDlgData.szReturnSelectedCounter = NULL;
    browseDlgData.szReturnSelectedInstance = NULL;
    browseDlgData.szReturnSelectedMachine = NULL;
    browseDlgData.szUserCaption = NULL;

    // Browse for a counter
    status = PdhBrowseCountersHW(&browseDlgData);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error browsing counters: " << status << std::endl;
        return 1;
    }

    // Use the selected counter...
    std::cout << "Selected counter: " << browseDlgData.szReturnSelectedCounter << std::endl;

    return 0;
}
