#include<windows.h>
#include<stdio.h>
#include"agent.h"

#pragma comment (lib, "Advapi32.lib")

DWORD WINAPI threadFunction(LPVOID lpThreadParameter)
{
    while (1)
        Sleep(1000);
    return 0;
}

DWORD WINAPI threadFunction2(LPVOID lpThreadParameter)
{
    ExitThread(0);
    return 0;
}

VOID CALLBACK MyAPC(ULONG_PTR param) 
{
    
    return;
}

int main()
{
    ss_hypercall(SS_HC_SUBMIT_CR3, 0);
    ss_hypercall(SS_HC_BEGIN_TEST, 0);

    
    const wchar_t* executablePath = L"C:\\Windows\\notepad.exe";

    // Initialize the necessary structures
    STARTUPINFOW startupInfo = { sizeof(STARTUPINFOW) };

    PROCESS_INFORMATION processInfo;

    HANDLE hThread = CreateThread(nullptr, 0, threadFunction2, nullptr, 0, nullptr);
    CloseHandle(hThread);

    // Create the process
    if (!CreateProcessW(executablePath,NULL, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startupInfo, &processInfo))
    {
        return 1;
    }

    SuspendThread(processInfo.hThread);
    CONTEXT threadContext;
    threadContext.ContextFlags = CONTEXT_CONTROL;
    GetThreadContext(processInfo.hThread, &threadContext);

    threadContext.ContextFlags = CONTEXT_CONTROL;
    SetThreadContext(processInfo.hThread, &threadContext);
    ResumeThread(processInfo.hThread);

    HANDLE hRemoteThread = CreateRemoteThread(processInfo.hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"),"Sleep"), nullptr, 0, nullptr);
    TerminateThread(hRemoteThread, 0);
    DWORD exitCode;
    GetExitCodeThread(hRemoteThread, &exitCode);
    CloseHandle(hRemoteThread);

    SIZE_T attributeListSize = 0;
    InitializeProcThreadAttributeList(nullptr, 1, 0, &attributeListSize);
    if (attributeListSize == 0)
    {
        return 1;
    }

    LPPROC_THREAD_ATTRIBUTE_LIST pAttributeList = reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(malloc(attributeListSize));
    if (pAttributeList == nullptr)
    {
        return 1;
    }

    if (!InitializeProcThreadAttributeList(pAttributeList, 1, 0, &attributeListSize))
    {
        free(pAttributeList);
        return 1;
    }
    DeleteProcThreadAttributeList(pAttributeList);
    free(pAttributeList);

    FlushInstructionCache(GetCurrentProcess(), NULL, 0);
    FlushProcessWriteBuffers();
    GetCurrentProcessId();
    GetCurrentProcessorNumber();

    PROCESSOR_NUMBER procNumber;
    GetCurrentProcessorNumberEx(&procNumber);
    GetCurrentThread();
    GetCurrentThreadEffectiveToken();
    GetCurrentThreadId();

    ULONG_PTR lowLimit, highLimit;
    GetCurrentThreadStackLimits(&lowLimit, &highLimit);

    SetThreadToken(NULL, GetCurrentThreadToken());

    SetPriorityClass(GetCurrentProcess(), GetPriorityClass(GetCurrentProcess()));

    DWORD cpuSetCount;
    ULONG RequiredIdCount = 1;
    GetProcessDefaultCpuSets(GetCurrentProcess(), NULL, 0, &RequiredIdCount);
    SetProcessDefaultCpuSets(GetCurrentProcess(), NULL, 0);


    DWORD handleCount;
    BOOL result = GetProcessHandleCount(GetCurrentProcess(), &handleCount);
    GetCurrentProcessId();
    GetProcessIdOfThread(GetCurrentThread());

    {
        PROCESS_MITIGATION_DEP_POLICY policyInfo;
        GetProcessMitigationPolicy(GetCurrentProcess(), ProcessDEPPolicy, &policyInfo, sizeof(policyInfo));
        SetProcessMitigationPolicy(ProcessDEPPolicy, &policyInfo, sizeof(policyInfo));
    }
    {
        PROCESS_MITIGATION_ASLR_POLICY policyInfo;
        GetProcessMitigationPolicy(GetCurrentProcess(), ProcessASLRPolicy, &policyInfo, sizeof(policyInfo));
        SetProcessMitigationPolicy(ProcessASLRPolicy, &policyInfo, sizeof(policyInfo));
    }
    {
        PROCESS_MITIGATION_DYNAMIC_CODE_POLICY policyInfo;
        GetProcessMitigationPolicy(GetCurrentProcess(), ProcessDynamicCodePolicy, &policyInfo, sizeof(policyInfo));
        SetProcessMitigationPolicy(ProcessDynamicCodePolicy, &policyInfo, sizeof(policyInfo));
    }
    {
        PROCESS_MITIGATION_STRICT_HANDLE_CHECK_POLICY  policyInfo;
        GetProcessMitigationPolicy(GetCurrentProcess(), ProcessStrictHandleCheckPolicy, &policyInfo, sizeof(policyInfo));
        SetProcessMitigationPolicy(ProcessStrictHandleCheckPolicy, &policyInfo, sizeof(policyInfo));
    }
    {
        PROCESS_MITIGATION_SYSTEM_CALL_DISABLE_POLICY  policyInfo;
        GetProcessMitigationPolicy(GetCurrentProcess(), ProcessSystemCallDisablePolicy, &policyInfo, sizeof(policyInfo));
        SetProcessMitigationPolicy(ProcessSystemCallDisablePolicy, &policyInfo, sizeof(policyInfo));
    }
    {
        PROCESS_MITIGATION_EXTENSION_POINT_DISABLE_POLICY  policyInfo;
        GetProcessMitigationPolicy(GetCurrentProcess(), ProcessMitigationOptionsMask, &policyInfo, sizeof(policyInfo));
        SetProcessMitigationPolicy(ProcessMitigationOptionsMask, &policyInfo, sizeof(policyInfo));
    }
    {
        PROCESS_MITIGATION_CONTROL_FLOW_GUARD_POLICY  policyInfo;
        GetProcessMitigationPolicy(GetCurrentProcess(), ProcessControlFlowGuardPolicy, &policyInfo, sizeof(policyInfo));
        SetProcessMitigationPolicy(ProcessControlFlowGuardPolicy, &policyInfo, sizeof(policyInfo));

    }
    {
        PROCESS_MITIGATION_BINARY_SIGNATURE_POLICY  policyInfo;
        GetProcessMitigationPolicy(GetCurrentProcess(), ProcessSignaturePolicy, &policyInfo, sizeof(policyInfo));
        SetProcessMitigationPolicy(ProcessSignaturePolicy, &policyInfo, sizeof(policyInfo));

    }
    {
        PROCESS_MITIGATION_FONT_DISABLE_POLICY  policyInfo;
        GetProcessMitigationPolicy(GetCurrentProcess(), ProcessFontDisablePolicy, &policyInfo, sizeof(policyInfo));
        SetProcessMitigationPolicy(ProcessFontDisablePolicy, &policyInfo, sizeof(policyInfo));

    }
    {
        PROCESS_MITIGATION_IMAGE_LOAD_POLICY  policyInfo;
        GetProcessMitigationPolicy(GetCurrentProcess(), ProcessImageLoadPolicy, &policyInfo, sizeof(policyInfo));
        SetProcessMitigationPolicy(ProcessImageLoadPolicy, &policyInfo, sizeof(policyInfo));

    }
    {
        PROCESS_MITIGATION_REDIRECTION_TRUST_POLICY policyInfo;
        GetProcessMitigationPolicy(GetCurrentProcess(), ProcessRedirectionTrustPolicy, &policyInfo, sizeof(policyInfo));
        SetProcessMitigationPolicy(ProcessRedirectionTrustPolicy, &policyInfo, sizeof(policyInfo));

    }
    {
        PROCESS_MITIGATION_SIDE_CHANNEL_ISOLATION_POLICY policyInfo;
        GetProcessMitigationPolicy(GetCurrentProcess(), ProcessSideChannelIsolationPolicy, &policyInfo, sizeof(policyInfo));
        SetProcessMitigationPolicy(ProcessSideChannelIsolationPolicy, &policyInfo, sizeof(policyInfo));

    }
    {
        PROCESS_MITIGATION_USER_SHADOW_STACK_POLICY  policyInfo;
        GetProcessMitigationPolicy(GetCurrentProcess(), ProcessUserShadowStackPolicy, &policyInfo, sizeof(policyInfo));
        SetProcessMitigationPolicy(ProcessUserShadowStackPolicy, &policyInfo, sizeof(policyInfo));

    }

    {
        PROCESS_MEMORY_EXHAUSTION_INFO ProcessInformation;
        GetProcessInformation(GetCurrentProcess(), ProcessMemoryPriority, &ProcessInformation, sizeof(ProcessInformation));
        SetProcessInformation(GetCurrentProcess(), ProcessMemoryPriority, &ProcessInformation, sizeof(ProcessInformation));
    }
    {
        APP_MEMORY_INFORMATION  ProcessInformation;
        GetProcessInformation(GetCurrentProcess(), ProcessAppMemoryInfo, &ProcessInformation, sizeof(ProcessInformation));
        SetProcessInformation(GetCurrentProcess(), ProcessAppMemoryInfo, &ProcessInformation, sizeof(ProcessInformation));
    }
    {
        PROCESS_POWER_THROTTLING_STATE ProcessInformation;
        GetProcessInformation(GetCurrentProcess(), ProcessPowerThrottling, &ProcessInformation, sizeof(ProcessInformation));
        SetProcessInformation(GetCurrentProcess(), ProcessPowerThrottling, &ProcessInformation, sizeof(ProcessInformation));
    }
    {
        PROCESS_PROTECTION_LEVEL_INFORMATION ProcessInformation;
        GetProcessInformation(GetCurrentProcess(), ProcessProtectionLevelInfo, &ProcessInformation, sizeof(ProcessInformation));
        SetProcessInformation(GetCurrentProcess(), ProcessProtectionLevelInfo, &ProcessInformation, sizeof(ProcessInformation));
    }
    {
        PROCESS_LEAP_SECOND_INFO ProcessInformation;
        GetProcessInformation(GetCurrentProcess(), ProcessLeapSecondInfo, &ProcessInformation, sizeof(ProcessInformation));
        SetProcessInformation(GetCurrentProcess(), ProcessLeapSecondInfo, &ProcessInformation, sizeof(ProcessInformation));
    }
    {
        PROCESS_MACHINE_INFORMATION ProcessInformation;
        GetProcessInformation(GetCurrentProcess(), ProcessMachineTypeInfo, &ProcessInformation, sizeof(ProcessInformation));
        SetProcessInformation(GetCurrentProcess(), ProcessMachineTypeInfo, &ProcessInformation, sizeof(ProcessInformation));
    }

    BOOL priorityBoost;
    GetProcessPriorityBoost(GetCurrentProcess(), &priorityBoost);
    SetProcessPriorityBoost(GetCurrentProcess(), priorityBoost);


    DWORD shutdownLevel;
    DWORD shutdownFlags;
    GetProcessShutdownParameters(&shutdownLevel, &shutdownFlags);
    SetProcessShutdownParameters(shutdownLevel, shutdownFlags);

    FILETIME creationTime, exitTime, kernelTime, userTime;
    GetProcessTimes(GetCurrentProcess(), &creationTime, &exitTime, &kernelTime, &userTime);

    GetProcessVersion(GetCurrentProcessId());

    {
        STARTUPINFOW startupInfo;
        GetStartupInfoW(&startupInfo);
    }
    
    {
        SYSTEM_CPU_SET_INFORMATION buffer[64];
        DWORD bufferSize = sizeof(buffer);
        DWORD returnSize;
        GetSystemCpuSetInformation(buffer, bufferSize, &returnSize, nullptr, 0);
        FILETIME idleTime, kernelTime, userTime;
        GetSystemTimes(&idleTime, &kernelTime, &userTime);
    }
   

    PWSTR threadDescription = nullptr;
    GetThreadDescription(GetCurrentThread(), &threadDescription);

    PROCESSOR_NUMBER idealProcessor;
    GetThreadIdealProcessorEx(GetCurrentThread(), &idealProcessor);
    SetThreadIdealProcessorEx(GetCurrentThread(), &idealProcessor, nullptr);

    {
        MEMORY_PRIORITY_INFORMATION ThreadInformation;
        GetThreadInformation(GetCurrentThread(), ThreadMemoryPriority, &ThreadInformation, sizeof(ThreadInformation));
        SetThreadInformation(GetCurrentThread(), ThreadMemoryPriority, &ThreadInformation, sizeof(ThreadInformation));
    }
    {
        LONG ThreadInformation;
        GetThreadInformation(GetCurrentThread(), ThreadAbsoluteCpuPriority, &ThreadInformation, sizeof(ThreadInformation));
        SetThreadInformation(GetCurrentThread(), ThreadAbsoluteCpuPriority, &ThreadInformation, sizeof(ThreadInformation));
    }
    {
        DWORD ThreadInformation;
        GetThreadInformation(GetCurrentThread(), ThreadDynamicCodePolicy, &ThreadInformation, sizeof(ThreadInformation));
        SetThreadInformation(GetCurrentThread(), ThreadDynamicCodePolicy, &ThreadInformation, sizeof(ThreadInformation));
    }
    BOOL ioPendingFlag;
    GetThreadIOPendingFlag(GetCurrentThread(), &ioPendingFlag);
    SetThreadPriority(GetCurrentThread(), GetThreadPriority(GetCurrentThread()));

    SetThreadPriorityBoost(GetCurrentThread(), GetThreadPriorityBoost(GetCurrentThread(), &priorityBoost));

    ULONG bufferSize = 0;
    GetThreadSelectedCpuSets(hThread, nullptr, 0, &bufferSize);

    {
        FILETIME creationTime, exitTime, kernelTime, userTime;
        GetThreadTimes(hThread, &creationTime, &exitTime, &kernelTime, &userTime);
    }
    
    {
        LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList = nullptr;
        SIZE_T attributeListSize = 0;
        InitializeProcThreadAttributeList(lpAttributeList, 1, 0, &attributeListSize);

    }
    
    BOOL isCritical = FALSE;
    IsProcessCritical(GetCurrentProcess(), &isCritical);

    IsProcessorFeaturePresent(PF_ARM_VFP_32_REGISTERS_AVAILABLE);

    DWORD processId = GetCurrentProcessId();
    DWORD sessionId;
    ProcessIdToSessionId(processId, &sessionId);

    QueryProcessAffinityUpdateMode(GetCurrentProcess(), 0);

    QueueUserAPC(MyAPC, GetCurrentThread(), NULL);

    ULONG byte = 1024 * 8;
    SetThreadStackGuarantee(&byte);
    SwitchToThread();

    TerminateProcess(processInfo.hProcess, 0);

    auto dwTlsIndex = TlsAlloc();
    DWORD value = 1;
    TlsSetValue(dwTlsIndex, &value);
    TlsGetValue(dwTlsIndex);
    TlsFree(dwTlsIndex);


    GetExitCodeProcess(processInfo.hProcess, &exitCode);
    CloseHandle(processInfo.hThread);
    CloseHandle(processInfo.hProcess);

    printf("success\n");

    ExitProcess(0);
    return 0;
}
