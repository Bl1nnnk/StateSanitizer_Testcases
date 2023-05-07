/*

Copyright (C) 2017 Robert Gawlik

This file is part of kAFL state sanitizer (kAFL).

QEMU-PT is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

QEMU-PT is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with QEMU-PT.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <dbghelp.h>
#include "agent.h"

char target_file[MAX_PATH] = { 0 };

/* get KeBugCheck */
/* -------------- */

#include <psapi.h>
#define ARRAY_SIZE 1024

PCSTR ntoskrnl = "C:\\Windows\\System32\\ntoskrnl.exe";
PCSTR kernel_func1 = "KeBugCheck";
PCSTR kernel_func2 = "KeBugCheckEx";

#pragma comment(lib,"dbghelp.lib")

#define l_log(...)	 { \
	printf(__VA_ARGS__); \
	msg_len = sprintf_s(message, 1024, "loader log: "); \
	msg_len = sprintf_s(message + msg_len, 1024, __VA_ARGS__); \
	tp.message_addr = (target_ulong)message; \
	tp.message_len = msg_len; \
	ss_hypercall(SS_HC_PRINT, (target_ulong)&tp); \
}

#define l_elog(...) { \
	printf(__VA_ARGS__); \
	msg_len = sprintf_s(message, 1024, "loader error: "); \
	msg_len = sprintf_s(message + msg_len, 1024, __VA_ARGS__); \
	tp.message_addr = (target_ulong)message; \
	tp.message_len = msg_len; \
	ss_hypercall(SS_HC_PRINT, (target_ulong)&tp); \
}

#define TP_MSG

char message[1024];
int32_t msg_len;
ss_print tp;

void my_exit(int r)
{
	exit(r);
}

/* force termination on AVs */
void WINAPI nuke(){
	TerminateProcess((HANDLE)-1, 0x41);
}


LONG CALLBACK catch_all(struct _EXCEPTION_POINTERS *ExceptionInfo) {
	ExceptionInfo->ContextRecord->Rip = (DWORD64)nuke;
	return EXCEPTION_CONTINUE_EXECUTION; // return -1;
}


/* Resolve the return address of ExAllocatePoolWithTagRet*/
//target_ulong resolve_ExAllocatePoolWithTagRet(HMODULE kern_base, target_ulong EnterOff)
//{

//}

//target_ulong resolve_SymbolAddress(HMODULE kern_base, target_ulong EnterOff)
//{
//}

BOOL CALLBACK EnumSymProc( 
	PSYMBOL_INFO pSymInfo,   
	ULONG SymbolSize,	  
	PVOID UserContext)
{
	UNREFERENCED_PARAMETER(UserContext);
	
	l_log("%08llX %4u %s\n", 
		   pSymInfo->Address, SymbolSize, pSymInfo->Name);
	return TRUE;
}

void enum_nt_symbol()
{
	HANDLE hProcess = GetCurrentProcess();
	BOOL status;
	char *Mask = "*";
	target_ulong BaseOfDll;

	status = SymInitialize(hProcess, NULL, FALSE);
	if (status == FALSE)
	{
		return;
	}

	BaseOfDll = SymLoadModuleEx(hProcess,
								NULL,
								ntoskrnl,
								NULL,
								0,
								0,
								NULL,
								0);
								
	if (BaseOfDll == 0)
	{
		SymCleanup(hProcess);
		return;
	}								
		
	if (SymEnumSymbols(hProcess,	 // Process handle from SymInitialize.
						BaseOfDll,   // Base address of module.
						Mask,		// Name of symbols to match.
						EnumSymProc, // Symbol handler procedure.
						NULL))	   // User context.
	{
		// SymEnumSymbols succeeded
	}
	else
	{
		// SymEnumSymbols failed
		l_elog("SymEnumSymbols failed: %d\n", GetLastError());
	}
	
	SymCleanup(hProcess);
}

/* -------------- */

static inline void run_program(void* target){
	PROCESS_INFORMATION p1;
	STARTUPINFOA s1;

	ZeroMemory(&p1, sizeof(p1));
	ZeroMemory(&s1, sizeof(s1));
	s1.cb = sizeof(s1);

	l_log("Starting fuzzing target\n");
	BOOL success = CreateProcessA(NULL, (LPSTR)target, NULL, NULL, FALSE,
		0, NULL, NULL, &s1, &p1);
	if (!success){
		l_log("Cannot start fuzzing target, error code: %d\n", GetLastError());
		getchar();
		ExitProcess(0);
	}
	TerminateProcess((HANDLE)-1,0x41);
}

static inline void load_program(void* buf){
	HANDLE payload_file_handle = NULL;
	DWORD dwWritten;

	memset(target_file, 0x00, MAX_PATH);   
	DWORD tmp_path_len = GetTempPathA(MAX_PATH, target_file);
	memcpy(target_file + tmp_path_len, "\x5c", 1);
	memcpy(target_file + tmp_path_len + 1, TARGET_FILE_WIN, strlen(TARGET_FILE_WIN));

	payload_file_handle = CreateFileA((LPCSTR)target_file,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	BOOL result = WriteFile(
		payload_file_handle,
		buf,
		PROGRAM_SIZE,
		&dwWritten,
		NULL
	);
	if (result == 0){
		l_elog("Cannot write usermode state sanitizer (%ld)\n", GetLastError());
		/* blocks */
		getchar();
	}

	l_log("Executing target: %s\n", target_file);
	CloseHandle(payload_file_handle);
	run_program(target_file);
}

static inline UINT64 hex_to_bin(char* str){
	return (UINT64)_strtoui64(str, NULL, 16);
}


int main(int argc, char** argv){
	void* program_buffer;
	save_vm sv = {0};
	get_program gp = {0};
	DWORD ret_status;

	if (AddVectoredExceptionHandler(1, catch_all) == 0){
		l_elog("Cannot add veh handler %u\n", (UINT32)GetLastError());
		ExitProcess(0);
	}


	/* allocate 4MB contiguous virtual memory to hold state sanitizer program; data is provided by the state sanitizer */
	program_buffer = (void*)VirtualAlloc(0, PROGRAM_SIZE, MEM_COMMIT, PAGE_READWRITE);
	/* ensure that the virtual memory is *really* present in physical memory... */
	memset(program_buffer, 0xff, PROGRAM_SIZE);


	//enum_nt_symbol();
	/* this hypercall will generate a VM snapshot for the state sanitizer and subsequently terminate QEMU */

	sv.sp_name = (target_ulong)"loader";
	sv.name_len = sizeof("loader");
	ret_status = ss_hypercall(SS_HC_SAVE_VM, (target_ulong)&sv);
	if (ret_status == HYPER_STATUS_SUCCESS)
	{
		l_log("savevm success, ret_status %08x\n", ret_status);
	} else {
		l_log("savevm fail, ret_status %08x\n", ret_status);
	}

	/***** state sanitizer Entrypoint *****/

	gp.program_buffer = (target_ulong)program_buffer;
	gp.buffer_len = PROGRAM_SIZE;
	ret_status = ss_hypercall(SS_HC_GET_PROGRAM, (UINT64)&gp);
	if (ret_status == HYPER_STATUS_SUCCESS) {
		l_log("Successfully get program from host. ret_staus %08x\n", ret_status);
		
		ss_hypercall(SS_HC_BEGINE_RECORD, 0);
		
		/* execute state sanitizer program */
		load_program(program_buffer);
		
		ss_hypercall(SS_HC_BEGINE_RECORD, 0);
	}
	l_log("get program fail, ret_status %08x\n", ret_status);
	/* bye */ 
	return 0;
}
