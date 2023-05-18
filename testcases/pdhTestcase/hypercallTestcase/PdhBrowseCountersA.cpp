#include "agent.h"
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

int main()
{
    tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);
    PDH_STATUS status;
    PDH_BROWSE_DLG_CONFIG browseConfig = { 0 };
    browseConfig.bIncludeInstanceIndex = TRUE;
    browseConfig.bSingleCounterPerAdd = TRUE;
    browseConfig.bSingleCounterPerDialog = TRUE;
    browseConfig.bLocalCountersOnly = FALSE;
    browseConfig.bWildCardInstances = TRUE;
    browseConfig.bHideDetailBox = FALSE;
    browseConfig.bInitializePath = TRUE;
    browseConfig.bDisableMachineSelection = FALSE;
    browseConfig.bIncludeCostlyObjects = FALSE;
    browseConfig.bShowObjectBrowser = TRUE;
    browseConfig.hWndOwner = NULL;
    browseConfig.szDialogBoxCaption = "Select a counter";
    browseConfig.szDataSource = NULL;
    browseConfig.szReturnPathBuffer = NULL;
    browseConfig.cchReturnPathLength = 0;
    browseConfig.pCallBack = NULL;
    browseConfig.dwCallBackArg = 0;
    browseConfig.CallBackStatus = ERROR_SUCCESS;
    
    // Browse for a counter
    tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);
    status = PdhBrowseCountersA(&browseConfig);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Error browsing counters: " << status << std::endl;
        return 1;
    }
    
    // Use the selected counter path...
    
    return 0;
}
