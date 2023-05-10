#include <cstdio>
#include <cstdlib>
#include <Windows.h>
#include <evntrace.h>
#include <wmistr.h>
#include <tdh.h>
#include"agent.h"

#pragma comment(lib, "advapi32.lib")


VOID WINAPI MyCallbackFunc(PEVENT_TRACE pEvent)
{
    // Your code here
}

void WINAPI EventCallback( PEVENT_TRACE  EventRecord)
{
    // Do something with the event record
    // This function will be called when an event is traced
}

void __cdecl ProcessEvent(PEVENT_RECORD pEvent)
{
    printf("Event received: ID=%d Opcode=%d Task=%d\n", pEvent->EventHeader.EventDescriptor.Id, pEvent->EventHeader.EventDescriptor.Opcode, pEvent->EventHeader.EventDescriptor.Task);
}

int main()
{
{
    ss_hypercall(SS_HC_SUBMIT_CR3, 0);
    ss_hypercall(SS_HC_BEGIN_TEST, 0);
    ULONG propertyCount = 1;
    PEVENT_TRACE_PROPERTIES traceProperties = (PEVENT_TRACE_PROPERTIES) malloc(sizeof(EVENT_TRACE_PROPERTIES));
    ZeroMemory(traceProperties, sizeof(EVENT_TRACE_PROPERTIES));
    traceProperties->BufferSize = 1024;
    traceProperties->MinimumBuffers = 2;
    traceProperties->MaximumBuffers = 10;
    traceProperties->LogFileMode = EVENT_TRACE_FILE_MODE_SEQUENTIAL;
    traceProperties->FlushTimer = 1;
    traceProperties->EnableFlags = EVENT_TRACE_FLAG_DISK_FILE_IO;
    traceProperties->AgeLimit = 0;
    traceProperties->NumberOfBuffers = 0;
    traceProperties->FreeBuffers = 0;
    traceProperties->EventsLost = 0;
    traceProperties->BuffersWritten = 0;
    traceProperties->LogBuffersLost = 0;
    traceProperties->RealTimeBuffersLost = 0;
    traceProperties->LoggerThreadId = 0;
    traceProperties->LogFileNameOffset = 0;
    traceProperties->LoggerNameOffset = 0;

    ULONG loggerCount = 0;
    ULONG result = QueryAllTracesW(&traceProperties, propertyCount, &loggerCount);
    if (result != ERROR_SUCCESS)
    {
        printf("QueryAllTracesW failed with error code %lu.\n", result);
    }

    printf("Number of loggers: %lu.\n", loggerCount);

    free(traceProperties);

    
}
{
    TRACEHANDLE traceHandle = 0;
    PEVENT_TRACE_PROPERTIES traceProperties = (PEVENT_TRACE_PROPERTIES) malloc(sizeof(EVENT_TRACE_PROPERTIES));
    ZeroMemory(traceProperties, sizeof(EVENT_TRACE_PROPERTIES));
    traceProperties->BufferSize = 1024;
    traceProperties->MinimumBuffers = 2;
    traceProperties->MaximumBuffers = 10;
    traceProperties->LogFileMode = EVENT_TRACE_FILE_MODE_SEQUENTIAL;
    traceProperties->FlushTimer = 1;
    traceProperties->EnableFlags = EVENT_TRACE_FLAG_DISK_FILE_IO;
    traceProperties->AgeLimit = 0;
    traceProperties->NumberOfBuffers = 0;
    traceProperties->FreeBuffers = 0;
    traceProperties->EventsLost = 0;
    traceProperties->BuffersWritten = 0;
    traceProperties->LogBuffersLost = 0;
    traceProperties->RealTimeBuffersLost = 0;
    traceProperties->LoggerThreadId = 0;
    traceProperties->LogFileNameOffset = 0;
    traceProperties->LoggerNameOffset = 0;

    LPCSTR instanceName = "MyTraceSession";

    WCHAR sessionName[] = L"DiagLog1";  // 事件跟踪器会话名称
    TRACEHANDLE hTrace = NULL;               // 事件跟踪器句柄
    // 创建事件跟踪器会话
    ULONG status = StartTraceW(&hTrace, sessionName, NULL);
    if (status != ERROR_SUCCESS) {
        printf("Failed");
    }

    auto result = ControlTraceA(traceHandle, instanceName, traceProperties, EVENT_TRACE_CONTROL_QUERY);
    if (result != ERROR_SUCCESS)
    {
        printf("ControlTraceA failed with error code %lu.\n", result);
    }

    printf("Trace handle: %lu.\n", traceHandle);

    result = QueryTraceA(traceHandle, instanceName, traceProperties);
    if (result != ERROR_SUCCESS)
    {
        printf("QueryTraceA failed with error code %lu.\n", result);
    }

    printf("Buffer size: %lu.\n", traceProperties->BufferSize);
    printf("Minimum buffers: %lu.\n", traceProperties->MinimumBuffers);
    printf("Maximum buffers: %lu.\n", traceProperties->MaximumBuffers);
    printf("Log file mode: %lu.\n", traceProperties->LogFileMode);
    printf("Flush timer: %lu.\n", traceProperties->FlushTimer);
    printf("Enable flags: %lu.\n", traceProperties->EnableFlags);
    printf("Age limit: %ld.\n", traceProperties->AgeLimit);

    result = ControlTraceA(traceHandle, instanceName, traceProperties, EVENT_TRACE_CONTROL_STOP);
    if (result != ERROR_SUCCESS)
    {
        printf("ControlTraceA failed with error code %lu.\n", result);
    }

    result = ControlTraceA(traceHandle, instanceName, traceProperties, EVENT_TRACE_CONTROL_FLUSH);
    if (result != ERROR_SUCCESS)
    {
        printf("ControlTraceA failed with error code %lu.\n", result);
    }

    free(traceProperties);

    
}
{
    EVENT_TRACE_PROPERTIES properties;
    TRACEHANDLE traceHandle;
    LPCWSTR instanceName = L"MyTraceSession";

    ZeroMemory(&properties, sizeof(EVENT_TRACE_PROPERTIES));
    properties.Wnode.BufferSize = sizeof(EVENT_TRACE_PROPERTIES);
    properties.Wnode.Flags = WNODE_FLAG_TRACED_GUID;
    properties.Wnode.ClientContext = 1;
    properties.LogFileMode = EVENT_TRACE_FILE_MODE_SEQUENTIAL;
    properties.MaximumFileSize = 100;
    properties.MaximumBuffers = 100;
    properties.MinimumBuffers = 10;
    properties.NumberOfBuffers = 20;
    properties.EnableFlags = EVENT_TRACE_FLAG_DISK_IO | EVENT_TRACE_FLAG_PROCESS;
    properties.AgeLimit = 60;

    // Start the trace session
    ULONG result = StartTraceW(&traceHandle, instanceName, &properties);
    if (result != ERROR_SUCCESS) {
        wprintf(L"Failed to start trace session (error %lu)\n", result);

    }

    // Query the trace session
    result = QueryTraceW(traceHandle, NULL, &properties);
    if (result != ERROR_SUCCESS) {
        wprintf(L"Failed to query trace session (error %lu)\n", result);
        StopTraceW(traceHandle, instanceName, &properties);
        
    }

    // Stop the trace session
    result = StopTraceW(traceHandle, instanceName, &properties);
    if (result != ERROR_SUCCESS) {
        wprintf(L"Failed to stop trace session (error %lu)\n", result);
        
    }

    wprintf(L"Trace session stopped successfully.\n");
    
}
{
    TRACE_GUID_REGISTRATION TraceGuidReg = {0};
    GUID ControlGuid = {0x12345678, 0x1234, 0x5678, {0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78}};
    TRACEHANDLE RegistrationHandle = 0;

    // Set up TraceGuidReg
    TraceGuidReg.Guid = &ControlGuid;
    TraceGuidReg.RegHandle = NULL;

    // Register the control GUID
    ULONG status = RegisterTraceGuidsA((WMIDPREQUEST)MyCallbackFunc, NULL, &ControlGuid, 1, &TraceGuidReg, NULL, NULL, &RegistrationHandle);

    // Check for errors
    if (status != ERROR_SUCCESS)
    {
        // Error handling code here
        
    }

    // Wait for events to arrive
    // Your code here

    // Unregister the control GUID
    status = UnregisterTraceGuids(RegistrationHandle);

    // Check for errors
    if (status != ERROR_SUCCESS)
    {
        // Error handling code here
        
    }

    
} {
    // Define the control GUID
    const GUID controlGuid = { /* your control GUID here */ };

    // Define the trace provider information
    TRACE_GUID_REGISTRATION traceProvider = {
        &controlGuid,
        NULL
    };

    // Register the trace provider
    TRACEHANDLE traceHandle = 0;
    ULONG status = RegisterTraceGuidsW(
        NULL,                    // Use default callback function
        NULL,                    // Use default callback function context
        &controlGuid,            // Control GUID
        1,                       // One trace provider
        &traceProvider,          // Trace provider information
        NULL,                    // No MOF file
        NULL,                    // No MOF resource name
        &traceHandle             // Trace registration handle
    );
    
    if (status == ERROR_SUCCESS) {
        // Successfully registered the trace provider
        // You can now start tracing events using this trace handle
        
        // Unregister the trace provider when done
        status = UnregisterTraceGuids(traceHandle);
        if (status != ERROR_SUCCESS) {
            // Failed to unregister the trace provider
        }
    }
    else {
        // Failed to register the trace provider
    }

    
}
{
    // Create a trace handle
    TRACEHANDLE traceHandle = 0;
    ULONG result = ERROR_SUCCESS;

    if (result != ERROR_SUCCESS)
    {
        printf("Error starting trace session. Error code: %lu\n", result);
        
    }
    if (result != ERROR_SUCCESS)
    {
        printf("Error enabling trace. Error code: %lu\n", result);
        
    }

    // Set the trace callback function
    GUID ControlGuid = {0x12345678, 0x1234, 0x5678, {0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78}};
    result = SetTraceCallback(&ControlGuid, EventCallback);

    if (result != ERROR_SUCCESS)
    {
        printf("Error setting trace callback. Error code: %lu\n", result);
        
    }

    // Wait for events to be traced
    Sleep(5000);


    if (result != ERROR_SUCCESS)
    {
        printf("Error stopping trace session. Error code: %lu\n", result);
        
    }

    
}
{
    TRACEHANDLE traceHandle;
    EVENT_TRACE_PROPERTIES traceProperties;
    ULONG status;

    ZeroMemory(&traceProperties, sizeof(traceProperties));
    traceProperties.Wnode.BufferSize = sizeof(traceProperties);
    traceProperties.Wnode.Flags = WNODE_FLAG_TRACED_GUID;
    traceProperties.Wnode.ClientContext = 1;
    traceProperties.Wnode.Guid = { 0x12345678, 0x1234, 0x5678, { 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78 } };
    traceProperties.LogFileMode = EVENT_TRACE_FILE_MODE_SEQUENTIAL;
    traceProperties.MaximumFileSize = 1024;
    traceProperties.MaximumBuffers = 1024;
    traceProperties.MinimumBuffers = 256;
    traceProperties.FlushTimer = 1;
    traceProperties.AgeLimit = 0;
    traceProperties.NumberOfBuffers = 256;

    status = StartTraceA(&traceHandle, "MyTrace", &traceProperties);
    if (status != ERROR_SUCCESS)
    {
        printf("Failed to start trace: %lu\n", status);
        
    }

    printf("Trace started successfully!\n");

    // Do some tracing...

    status = ControlTraceA(traceHandle, "MyTrace", &traceProperties, EVENT_TRACE_CONTROL_STOP);
    if (status != ERROR_SUCCESS)
    {
        printf("Failed to stop trace: %lu\n", status);
        
    }

    printf("Trace stopped successfully!\n");

    
}
{
    TRACEHANDLE traceHandle = 0;
    EVENT_TRACE_PROPERTIES traceProperties = { 0 };
    traceProperties.Wnode.BufferSize = sizeof(EVENT_TRACE_PROPERTIES);
    traceProperties.Wnode.Flags = WNODE_FLAG_TRACED_GUID;
    traceProperties.Wnode.ClientContext = 1; // QPC clock resolution
    traceProperties.LogFileMode = EVENT_TRACE_REAL_TIME_MODE;
    traceProperties.LoggerNameOffset = sizeof(EVENT_TRACE_PROPERTIES);
    traceProperties.LogFileNameOffset = 0;

    WCHAR loggerName[] = L"MyTraceLogger";
    ULONG status = StartTraceW(&traceHandle, loggerName, &traceProperties);
    if (status != ERROR_SUCCESS)
    {
        wprintf(L"Failed to start trace, status = %lu\n", status);
        return 0;
    }

    wprintf(L"Trace started successfully, handle = %lu\n", traceHandle);

    // Do something here...

    status = ControlTraceW(traceHandle, loggerName, &traceProperties, EVENT_TRACE_CONTROL_STOP);
    if (status != ERROR_SUCCESS)
    {
        wprintf(L"Failed to stop trace, status = %lu\n", status);
        return 0;
    }

    wprintf(L"Trace stopped successfully\n");

}
{
    // Set up the event trace properties
    EVENT_TRACE_PROPERTIES props;
    ZeroMemory(&props, sizeof(EVENT_TRACE_PROPERTIES));
    props.Wnode.BufferSize = sizeof(EVENT_TRACE_PROPERTIES);
    props.Wnode.Flags = WNODE_FLAG_TRACED_GUID;
    props.Wnode.ClientContext = 1;
    props.LoggerNameOffset = 0;
    props.LogFileNameOffset = 0;
    props.MaximumFileSize = 1024;
    props.LogFileMode = EVENT_TRACE_FILE_MODE_SEQUENTIAL;

    // Start the trace session
    TRACEHANDLE traceHandle;
    ULONG status = StartTraceA(&traceHandle, "MyTraceSession", &props);
    if (status != ERROR_SUCCESS)
    {
        std::printf("Failed to start trace session: %d\n", status);
        
    }

    // ... Do some tracing ...

    // Stop the trace session
    status = StopTraceA(traceHandle, "MyTraceSession", &props);
    if (status != ERROR_SUCCESS)
    {
        std::printf("Failed to stop trace session: %d\n", status);
        
    }

    
}
{
    TRACEHANDLE traceHandle;
    EVENT_TRACE_PROPERTIES properties = { 0 };

    properties.Wnode.BufferSize = sizeof(EVENT_TRACE_PROPERTIES);
    properties.Wnode.Flags = WNODE_FLAG_TRACED_GUID;
    properties.Wnode.ClientContext = 1;
    properties.LoggerNameOffset = sizeof(EVENT_TRACE_PROPERTIES);

    ULONG status = StartTraceW(&traceHandle, L"MyTraceSession", &properties);
    if (status != ERROR_SUCCESS) {
        // Handle error
        
    }

    // Do some tracing here...

    status = StopTraceW(traceHandle, L"MyTraceSession", &properties);
    if (status != ERROR_SUCCESS) {
        // Handle error
        
    }

    
}
{
    TRACEHANDLE traceHandle;
    EVENT_TRACE_LOGFILE traceLogfile;
    TRACE_LOGFILE_HEADER* pHeader = &traceLogfile.LogfileHeader;

    ZeroMemory(&traceLogfile, sizeof(EVENT_TRACE_LOGFILE));
    traceLogfile.LoggerName = KERNEL_LOGGER_NAME;
    traceLogfile.ProcessTraceMode = PROCESS_TRACE_MODE_REAL_TIME;
    traceLogfile.EventRecordCallback = (PEVENT_RECORD_CALLBACK) ProcessEvent;

    traceHandle = OpenTrace(&traceLogfile);
    if (traceHandle == INVALID_PROCESSTRACE_HANDLE)
    {
        printf("OpenTrace failed with %d\n", GetLastError());
        
    }

    printf("Waiting for events...\n");
    getchar();

    CloseTrace(traceHandle);

    
}
{
    // Create and configure the EVENT_INSTANCE_HEADER structure
    EVENT_INSTANCE_HEADER InstanceHeader = { 0 };
    InstanceHeader.Size = sizeof(EVENT_INSTANCE_HEADER);
    InstanceHeader.ThreadId = GetCurrentThreadId();
    InstanceHeader.ProcessId = GetCurrentProcessId();
    InstanceHeader.TimeStamp.QuadPart = 0;
    InstanceHeader.RegHandle = 0;
    InstanceHeader.InstanceId = 0;
    InstanceHeader.ParentInstanceId = 0;

    // Create the EVENT_INSTANCE_INFO structures for the event and its parent
    EVENT_INSTANCE_INFO EventInstanceInfo = { 0 };
    EventInstanceInfo.RegHandle = 0;
    EventInstanceInfo.InstanceId = 0;
    EVENT_INSTANCE_INFO ParentInstanceInfo = { 0 };
    ParentInstanceInfo.RegHandle = 0;
    ParentInstanceInfo.InstanceId = 0;

    // Start the trace session
    TRACEHANDLE TraceHandle;
    EVENT_TRACE_PROPERTIES* TraceProperties = nullptr;
    ULONG Result = StartTraceA(&TraceHandle, "MyTraceSession", TraceProperties);
    if (Result != ERROR_SUCCESS)
    {
        return Result;
    }

    // Create and trace the event instance
    Result = TraceEventInstance(TraceHandle, reinterpret_cast<PEVENT_INSTANCE_HEADER>(&InstanceHeader),
        &EventInstanceInfo, &ParentInstanceInfo);
    if (Result != ERROR_SUCCESS)
    {
        StopTrace(TraceHandle, nullptr, nullptr);
        return Result;
    }

    // Stop the trace session
    Result = StopTrace(TraceHandle, nullptr, nullptr);
    if (Result != ERROR_SUCCESS)
    {
        return Result;
    }

    
}
{
    TRACEHANDLE traceHandle = 0;
    EVENT_TRACE_PROPERTIES* pTraceProperties = nullptr;
    ULONG status = 0;

    // allocate memory for trace properties
    pTraceProperties = (EVENT_TRACE_PROPERTIES*)malloc(sizeof(EVENT_TRACE_PROPERTIES) + 1024);
    if (pTraceProperties == nullptr)
    {
        printf("Error: Failed to allocate memory for trace properties.\n");
        
    }
    ZeroMemory(pTraceProperties, sizeof(EVENT_TRACE_PROPERTIES) + 1024);
    pTraceProperties->Wnode.BufferSize = sizeof(EVENT_TRACE_PROPERTIES) + 1024;
    pTraceProperties->Wnode.Flags = WNODE_FLAG_TRACED_GUID;
    pTraceProperties->Wnode.ClientContext = 1; // QPC clock resolution
    pTraceProperties->LogFileMode = EVENT_TRACE_FILE_MODE_CIRCULAR;
    pTraceProperties->MaximumFileSize = 100; // 100 MB
    pTraceProperties->LoggerNameOffset = sizeof(EVENT_TRACE_PROPERTIES);
    pTraceProperties->LogFileNameOffset = 0;

    // start the trace session
    status = StartTraceA(&traceHandle, "MyTraceSession", pTraceProperties);
    if (status != ERROR_SUCCESS)
    {
        printf("Error: Failed to start trace session. Error code: %lu\n", status);
        free(pTraceProperties);
        
    }

    printf("Trace session started successfully.\n");

    // write a message to the trace session
    GUID ControlGuid = {0x12345678, 0x1234, 0x5678, {0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78}};
    TraceMessage(traceHandle, 0, &ControlGuid, 1, "Hello, TraceMessage!");

    // stop the trace session
    status = ControlTrace(traceHandle, nullptr, pTraceProperties, EVENT_TRACE_CONTROL_STOP);
    if (status != ERROR_SUCCESS)
    {
        printf("Error: Failed to stop trace session. Error code: %lu\n", status);
    }
    else
    {
        printf("Trace session stopped successfully.\n");
    }

    free(pTraceProperties);

    
}
{
    TRACEHANDLE sessionHandle = 0;
    EVENT_TRACE_PROPERTIES* sessionProperties = NULL;
    ULONG status = ERROR_SUCCESS;
    ULONG bufferSize = 0;

    // Define the session properties.
    sessionProperties = (EVENT_TRACE_PROPERTIES*)malloc(sizeof(EVENT_TRACE_PROPERTIES));
    ZeroMemory(sessionProperties, sizeof(EVENT_TRACE_PROPERTIES));
    sessionProperties->Wnode.BufferSize = sizeof(EVENT_TRACE_PROPERTIES);
    sessionProperties->Wnode.Flags = WNODE_FLAG_TRACED_GUID;
    sessionProperties->Wnode.ClientContext = 1; //QPC clock resolution
    sessionProperties->LoggerNameOffset = sizeof(EVENT_TRACE_PROPERTIES);
    sessionProperties->LogFileNameOffset = 0;
    sessionProperties->MaximumFileSize = 1024; // 1 MB
    sessionProperties->LogFileMode = EVENT_TRACE_FILE_MODE_SEQUENTIAL;
    sessionProperties->FlushTimer = 1; // Flush buffer every 1 second
    sessionProperties->EnableFlags = EVENT_TRACE_FLAG_PROCESS | EVENT_TRACE_FLAG_THREAD |
        EVENT_TRACE_FLAG_DISK_IO | EVENT_TRACE_FLAG_NETWORK_TCPIP;
    sessionProperties->AgeLimit = 0; // Stop the trace only on command

    // Start the trace session.
    status = StartTrace((PTRACEHANDLE)&sessionHandle, KERNEL_LOGGER_NAME, sessionProperties);
    if (status != ERROR_SUCCESS) {
        printf("StartTrace failed with %lu\n", status);
        goto cleanup;
    }

    printf("Trace started successfully.\n");

    // Log a message to the trace session.
    GUID ControlGuid = {0x12345678, 0x1234, 0x5678, {0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78}};
    status = TraceMessageVa(sessionHandle, TRACE_LEVEL_INFORMATION, &ControlGuid,0,0);
    if (status != ERROR_SUCCESS) {
        printf("TraceMessageVa failed with %lu\n", status);
        goto cleanup;
    }

    printf("Message logged to trace session.\n");

    // Stop the trace session.
    status = ControlTrace(sessionHandle, KERNEL_LOGGER_NAME, sessionProperties, EVENT_TRACE_CONTROL_STOP);
    if (status != ERROR_SUCCESS) {
        printf("ControlTrace failed with %lu\n", status);
        goto cleanup;
    }

    printf("Trace stopped successfully.\n");

cleanup:
    if (sessionProperties != NULL) {
        free(sessionProperties);
    }

}
{
    TRACEHANDLE sessionHandle = 0; // Set this to the session handle that you want to configure
    TRACE_INFO_CLASS infoClass = TraceStackTracingInfo; // Set this to the information class that you want to configure
    ULONG infoLength = 0; // Set this to the length of the information that you want to configure
    PVOID info = nullptr; // Set this to a pointer to the information that you want to configure

    // Call TraceSetInformation to set the information for the specified session handle
    ULONG result = TraceSetInformation(sessionHandle, infoClass, info, infoLength);
    if (result != ERROR_SUCCESS)
    {
        // Handle the error
        return result;
    }

    
} {
    // Replace with your own RegistrationHandle
    TRACEHANDLE RegistrationHandle = 0x12345678;

    ULONG result = UnregisterTraceGuids(RegistrationHandle);

    if (result != ERROR_SUCCESS) {
        printf("Failed to unregister trace guids with error code %lu\n", result);
        
    }

    printf("Successfully unregistered trace guids\n");

    
} {
    TRACEHANDLE traceHandle;
    EVENT_TRACE_PROPERTIES traceProperties;
    ULONG bufferSize = sizeof(EVENT_TRACE_PROPERTIES) + sizeof(ULONG);
    memset(&traceProperties, 0, bufferSize);
    traceProperties.Wnode.BufferSize = bufferSize;
    traceProperties.Wnode.Flags = WNODE_FLAG_TRACED_GUID;
    traceProperties.Wnode.ClientContext = 1;
    traceProperties.Wnode.Guid = { /* add your GUID here */ };
    traceProperties.LogFileNameOffset = sizeof(EVENT_TRACE_PROPERTIES);
    traceProperties.LoggerNameOffset = 0;
    traceProperties.BufferSize = 1024;
    traceProperties.MinimumBuffers = 2;
    traceProperties.MaximumBuffers = 4;
    traceProperties.MaximumFileSize = 1024;
    traceProperties.FlushTimer = 1;
    traceProperties.EnableFlags = EVENT_TRACE_FLAG_DISK_IO | EVENT_TRACE_FLAG_NETWORK_TCPIP;
    traceProperties.AgeLimit = 0;

    DWORD result = StartTraceA(&traceHandle, "MyTraceSession", &traceProperties);
    if (result != ERROR_SUCCESS) {
        printf("StartTraceA failed with error %lu\n", result);
        
    }

    printf("Trace started successfully!\n");

    result = ControlTraceA(traceHandle, NULL, &traceProperties, EVENT_TRACE_CONTROL_UPDATE);
    if (result != ERROR_SUCCESS) {
        printf("ControlTraceA failed with error %lu\n", result);
        StopTraceA(traceHandle, "MyTraceSession", &traceProperties);
        
    }

    printf("Trace updated successfully!\n");

    result = StopTraceA(traceHandle, "MyTraceSession", &traceProperties);
    if (result != ERROR_SUCCESS) {
        printf("StopTraceA failed with error %lu\n", result);
        
    }

    printf("Trace stopped successfully!\n");

    
} {
    TRACEHANDLE traceHandle;
    EVENT_TRACE_PROPERTIES* traceProperties = nullptr;
    ULONG status = ERROR_SUCCESS;
    const wchar_t* instanceName = L"MyTraceSession";

    // Allocate memory for the trace properties structure
    traceProperties = (EVENT_TRACE_PROPERTIES*)malloc(sizeof(EVENT_TRACE_PROPERTIES));
    if (traceProperties == nullptr) {
        printf("Failed to allocate memory for trace properties\n");
        
    }

    // Initialize the trace properties structure
    ZeroMemory(traceProperties, sizeof(EVENT_TRACE_PROPERTIES));
    traceProperties->Wnode.BufferSize = sizeof(EVENT_TRACE_PROPERTIES);
    traceProperties->Wnode.Flags = WNODE_FLAG_TRACED_GUID;
    traceProperties->Wnode.ClientContext = 1; // QPC clock resolution
    traceProperties->LoggerNameOffset = sizeof(EVENT_TRACE_PROPERTIES);
    traceProperties->LogFileNameOffset = 0;
    traceProperties->MinimumBuffers = 2;
    traceProperties->MaximumBuffers = 10;
    traceProperties->MaximumFileSize = 100; // 100 MB
    traceProperties->LogFileMode = EVENT_TRACE_FILE_MODE_SEQUENTIAL;
    traceProperties->FlushTimer = 1; // Flush every second
    traceProperties->EnableFlags = EVENT_TRACE_FLAG_PROCESS | EVENT_TRACE_FLAG_THREAD;
    traceProperties->AgeLimit = 60; // 1 minute

    // Start the trace session
    status = StartTraceW(&traceHandle, instanceName, traceProperties);
    if (status != ERROR_SUCCESS) {
        printf("Failed to start trace session with error: %d\n", status);
        free(traceProperties);
        
    }

    printf("Trace session started successfully\n");

    // Do some tracing here...

    // Stop the trace session
    status = StopTraceW(traceHandle, instanceName, traceProperties);
    if (status != ERROR_SUCCESS) {
        printf("Failed to stop trace session with error: %d\n", status);
        free(traceProperties);
        
    }

    printf("Trace session stopped successfully\n");

    // Free memory for trace properties structure
    free(traceProperties);

    
}


{
     WCHAR sessionName[] = L"DiagLog1";  // 事件跟踪器会话名称
    TRACEHANDLE hTrace = NULL;               // 事件跟踪器句柄
    // 创建事件跟踪器会话
    ULONG status = StartTraceW(&hTrace, sessionName, NULL);
    if (status != ERROR_SUCCESS) {
        printf("Failed");
        
    }
    EVENT_TRACE_PROPERTIES traceProps = { 0 };
    traceProps.Wnode.BufferSize = sizeof(EVENT_TRACE_PROPERTIES);
    traceProps.Wnode.Flags = WNODE_FLAG_TRACED_GUID;
    traceProps.LogFileMode |= EVENT_TRACE_FILE_MODE_CIRCULAR;
    status = UpdateTraceW(hTrace, sessionName, &traceProps);
    if (status != ERROR_SUCCESS)
    {
        printf("Failed to update trace session (error code %lu)!\n", status);
        
    }
    ULONG bufferSize = 0;
    ULONG traceCount = 0;

    ULONG flags = 0;
    PEVENT_TRACE_PROPERTIES* pLogFile = NULL;
    pLogFile = (PEVENT_TRACE_PROPERTIES*)malloc(sizeof(EVENT_TRACE_LOGFILEA));

    status = QueryAllTracesW(pLogFile, flags, NULL);
    if (status != ERROR_SUCCESS)
    {
        printf("Failed");
        
    }
    status = QueryTraceW(hTrace, NULL, &traceProps);
    if (status != ERROR_SUCCESS)
    {
        printf("Failed to query trace session (error code %lu)!\n", status);
        
    }
    status = FlushTraceW(hTrace, NULL, NULL);
    if (status != ERROR_SUCCESS)
    {
        printf("Failed to flush trace buffer (error code %lu)!\n", status);
        
    }
    status = ControlTraceW(hTrace, sessionName, NULL, EVENT_TRACE_CONTROL_STOP);
    if (status != ERROR_SUCCESS) {
        wprintf(L"Failed to stop trace session. Error code: %lu\n", status);
        
    }
    wprintf(L"Trace session stopped successfully.\n");
    status = StopTraceW(hTrace, NULL, NULL);
    if (status != ERROR_SUCCESS)
    {
        printf("StopTraceW failed with error %d\n", status);
        
    }


    CHAR sessionName1[] = "NetCore";  // 事件跟踪器会话名称
    TRACEHANDLE SessionHandle = 0;
    status = StartTraceA(&SessionHandle, sessionName1, NULL);
    if (status != ERROR_SUCCESS)
    {
        printf("StartTraceA failed with error %d\n", status);
        
    }
    status = UpdateTraceA(SessionHandle, sessionName1, &traceProps);
    if (status != ERROR_SUCCESS)
    {
        printf("Failed to update trace session (error code %lu)!\n", status);
        
    }

    ZeroMemory(pLogFile, sizeof(EVENT_TRACE_LOGFILEA));
    status = QueryAllTracesA(pLogFile, flags, NULL);
    if (status != ERROR_SUCCESS)
    {
        printf("Failed");
        
    }
    status = QueryTraceW(SessionHandle, NULL, &traceProps);
    if (status != ERROR_SUCCESS)
    {
        printf("Failed to query trace session (error code %lu)!\n", status);
        
    }
    status = FlushTraceA(SessionHandle, NULL, NULL);
    if (status != ERROR_SUCCESS)
    {
        printf("Failed to flush trace buffer (error code %lu)!\n", status);
        
    }
    status = ControlTraceA(SessionHandle, sessionName1, NULL, EVENT_TRACE_CONTROL_STOP);
    if (status != ERROR_SUCCESS) {
        wprintf(L"Failed to stop trace session. Error code: %lu\n", status);
        
    }
    wprintf(L"Trace session stopped successfully.\n");
    status = StopTraceA(SessionHandle, NULL, NULL);
    if (status != ERROR_SUCCESS)
    {
        printf("StopTraceA failed with error %d\n", status);
        
    }
    

    TRACE_GUID_INFO* traceGuids = nullptr;
    ULONG traceGuidsCount = 0;
    bufferSize = 0;
    status = EnumerateTraceGuidsEx(TRACE_QUERY_INFO_CLASS::TraceGuidQueryInfo, nullptr, 0, traceGuids, 0, &bufferSize);

    
    PEVENT_TRACE_LOGFILEA traceLogFile1 = { 0 };
    EVENT_TRACE_LOGFILEW traceLogFile2 = { 0 };

    TRACEHANDLE traceHandle = OpenTraceW(&traceLogFile2);
    if (traceHandle == INVALID_PROCESSTRACE_HANDLE)
    {
        printf("Failed to open trace.\n");
        
    }

    traceHandle = OpenTraceA(traceLogFile1);
    if (traceHandle == INVALID_PROCESSTRACE_HANDLE)
    {
        printf("Failed to open trace.\n");
        
    }

    int result = ProcessTrace(&traceHandle, 1, NULL, NULL);
    if (result != ERROR_SUCCESS && result != ERROR_CANCELLED) {
        printf("Failed to process trace. Error code: \n");
        return -1;
    }

    status = GetTraceEnableFlags(traceHandle);
    if (status != ERROR_SUCCESS)
    {
        printf("Failed to get trace enable flags.\n");
        CloseTrace(hTrace);
        
    }

    status = GetTraceEnableLevel(traceHandle);
    if (status != ERROR_SUCCESS)
    {
        printf("Failed to get trace enable flags.\n");
        CloseTrace(hTrace);
        
    }

    CloseTrace(traceHandle);

    LPCGUID uid = { 0 };
    PEVENT_INSTANCE_INFO info = 0;
    HANDLE bufa = 0;

    status =  CreateTraceInstanceId(&hTrace, info);
    if (status != ERROR_SUCCESS)
    {
        printf("Failed to create trace session.\n");
        
    }

    status = EnableTraceEx2(hTrace, uid, EVENT_CONTROL_CODE_DISABLE_PROVIDER, TRACE_LEVEL_INFORMATION, 0, 0, 0, NULL);
    if (status != ERROR_SUCCESS)
    {
        printf("Failed to enable trace.\n");
        
    }

    status = RemoveTraceCallback(uid);
    if (status != ERROR_SUCCESS)
    {
        printf("Failed to remove trace callback.\n");
        
    }

    PVOID Buffer = 0;
    TRACEHANDLE loggerHandle = GetTraceLoggerHandle(Buffer);
    if (loggerHandle == INVALID_PROCESSTRACE_HANDLE)
    {
        printf("Failed to get trace logger handle.\n");
        
    }
    
    UCHAR lev = 0;
    PEVENT_FILTER_DESCRIPTOR fil = 0;
    PENABLE_TRACE_PARAMETERS ablenum = 0;

    EnableTrace(true, 0, 0, uid, loggerHandle);
    if (hTrace == INVALID_PROCESSTRACE_HANDLE)
    {
        printf("Failed to enable trace.\n");
        
    }

    GUID guidProvider = { 0x01234567, 0x89ab, 0xcdef, {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef} };

    // 启用跟踪会话
    EVENT_TRACE_PROPERTIES props = { 0 };
    props.Wnode.BufferSize = sizeof(EVENT_TRACE_PROPERTIES);
    props.Wnode.Flags = WNODE_FLAG_TRACED_GUID;
    props.Wnode.ClientContext = 1;
    props.LogFileNameOffset = 0;
    props.LoggerNameOffset = sizeof(EVENT_TRACE_PROPERTIES);
    props.EnableFlags = EVENT_TRACE_FLAG_PROCESS | EVENT_TRACE_FLAG_THREAD;
    props.MaximumFileSize = 100; // 100MB
    props.MinimumBuffers = 20;
    props.MaximumBuffers = 50;
    props.FlushTimer = 1;
    props.FlushThreshold = 1;
    props.EnableFlags |= EVENT_TRACE_FLAG_DISK_IO;
    hTrace = EnableTraceEx(&guidProvider, NULL, 0, props.EnableFlags, props.MaximumFileSize, props.MinimumBuffers, props.MaximumBuffers, props.FlushTimer, fil);
    if (hTrace == INVALID_PROCESSTRACE_HANDLE)
    {
        printf("Failed to enable trace.\n");
        
    }


    WCHAR szFileName[MAX_PATH] = L"trace.etl";
    ETW_OPEN_TRACE_OPTIONS* Options = NULL;
    TRACE_LOGFILE_HEADER* LogFileHeader = 0;

    void* buffer = NULL;
    PCWSTR LogName = L"name1";

    ETW_BUFFER_HEADER* buf = NULL;

    ETW_PROCESS_HANDLE_INFO_TYPE InformationClass = {};
    PVOID InBuffer = NULL;
    ULONG InBufferSize = 0;
    PVOID OutBuffer = NULL;
    ULONG OutBufferSize = 0;
    PULONG ReturnLength = NULL;

}

 return 0;
 }