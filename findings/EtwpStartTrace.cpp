#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <strsafe.h>
#include <wmistr.h>
#include <evntrace.h>
#include <evntcons.h>

#define LOGFILE_PATH L"C:\\Users\\Public\\Desktop\\etw.log"
#define LOGSESSION_NAME L"My Event Trace Session"

// GUID that identifies your trace session.
// Remember to create your own session GUID.

// {AE44CB98-BD11-4069-8093-770EC9258A12}
// {123c80d2-937f-4cfe-80f4-c40d596e48b7}
static GUID SessionGuid =
{ 0x123c80d2, 0x937f, 0x4cfe, { 0x80, 0xf4, 0xc4, 0x0d, 0x59, 0x6e, 0x48, 0xb7 } };

// GUID that identifies the provider that you want
// to enable to your session.

// {D8909C24-5BE9-4502-98CA-AB7BDC24899D}
static GUID ProviderGuid =
{ 0xd8909c24, 0x5be9, 0x4502, {0x98, 0xca, 0xab, 0x7b, 0xdc, 0x24, 0x89, 0x9d } };

void wmain(void)
{
    ULONG status = ERROR_SUCCESS;
    TRACEHANDLE SessionHandle = 0;
    EVENT_TRACE_PROPERTIES* pSessionProperties = NULL;
    ULONG BufferSize = 0;
    BOOL TraceOn = TRUE;
    SID  Sid;
    SID_IDENTIFIER_AUTHORITY sa = SECURITY_NULL_SID_AUTHORITY;

    /*
    InitializeSid(&Sid, &sa, 1);
    status = EventAccessControl(&SessionGuid, EventSecuritySetDACL, &Sid, TRACELOG_LOG_EVENT, TRUE);
    if (ERROR_SUCCESS != status)
    {
        wprintf(L"EventAccessControl() failed with %lu\n", status);
        goto cleanup;
    }
    */


    // Allocate memory for the session properties. The memory must
    // be large enough to include the log file name and session name,
    // which get appended to the end of the session properties structure.

    BufferSize = sizeof(EVENT_TRACE_PROPERTIES) + sizeof(LOGFILE_PATH) + sizeof(LOGSESSION_NAME);
    pSessionProperties = (EVENT_TRACE_PROPERTIES*)malloc(BufferSize);
    if (NULL == pSessionProperties)
    {
        wprintf(L"Unable to allocate %d bytes for properties structure.\n", BufferSize);
        goto cleanup;
    }

    // Set the session properties. You only append the log file name
    // to the properties structure; the StartTrace function appends
    // the session name for you.

    ZeroMemory(pSessionProperties, BufferSize);
    pSessionProperties->Wnode.BufferSize = BufferSize;
    pSessionProperties->Wnode.Flags = WNODE_FLAG_TRACED_GUID;
    pSessionProperties->Wnode.ClientContext = 1; //QPC clock resolution
    //pSessionProperties->Wnode.Guid = SessionGuid;
    pSessionProperties->LogFileMode = EVENT_TRACE_FILE_MODE_SEQUENTIAL;
    pSessionProperties->MaximumFileSize = 1;  // 1 MB
    pSessionProperties->LoggerNameOffset = sizeof(EVENT_TRACE_PROPERTIES);
    pSessionProperties->LogFileNameOffset = sizeof(EVENT_TRACE_PROPERTIES) + sizeof(LOGSESSION_NAME);
    StringCbCopy((LPWSTR)((char*)pSessionProperties + pSessionProperties->LogFileNameOffset), sizeof(LOGFILE_PATH), LOGFILE_PATH);

    // Create the trace session.

    DebugBreak();
    status = StartTrace((PTRACEHANDLE)&SessionHandle, LOGSESSION_NAME, pSessionProperties);
    if (ERROR_SUCCESS != status)
    {
        wprintf(L"StartTrace() failed with %lu\n", status);
        goto cleanup;
    }

    // Enable the providers that you want to log events to your session.

    status = EnableTraceEx2(
        SessionHandle,
        (LPCGUID)&ProviderGuid,
        EVENT_CONTROL_CODE_ENABLE_PROVIDER,
        TRACE_LEVEL_INFORMATION,
        0,
        0,
        0,
        NULL
    );

    if (ERROR_SUCCESS != status)
    {
        wprintf(L"EnableTrace() failed with %lu\n", status);
        TraceOn = FALSE;
        goto cleanup;
    }

    wprintf(L"Run the provider application. Then hit any key to stop the session.\n");
    _getch();

cleanup:

    if (SessionHandle)
    {
        if (TraceOn)
        {
            status = EnableTraceEx2(
                SessionHandle,
                (LPCGUID)&ProviderGuid,
                EVENT_CONTROL_CODE_DISABLE_PROVIDER,
                TRACE_LEVEL_INFORMATION,
                0,
                0,
                0,
                NULL
            );
        }

        status = ControlTrace(SessionHandle, LOGSESSION_NAME, pSessionProperties, EVENT_TRACE_CONTROL_STOP);

        if (ERROR_SUCCESS != status)
        {
            wprintf(L"ControlTrace(stop) failed with %lu\n", status);
        }
    }

    if (pSessionProperties)
    {
        free(pSessionProperties);
        pSessionProperties = NULL;
    }
}