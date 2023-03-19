#ifndef __AGENT__
# define __AGENT__

#if defined(__MINGW64__)

#ifndef uint64_t
#define uint64_t UINT64
#endif
#ifndef int32_t
#define int32_t INT32
#endif
#ifndef uint8_t
#define uint8_t UINT8
#endif

#else

#include <stdint.h>

#endif

#if defined(__x86_64__) || defined(_WIN64)
#ifndef TARGET_LONG_SIZE
#define target_ulong uint64_t
#endif

#else

#ifndef TARGET_LONG_SIZE
#define target_ulong uint32_t
#endif

#endif


/* hypercall identifier in eax, when the guest executes "cpuid" */
#define TP_HC_IDENTIFIER 0x86E4CA11

/* function identifier in ebx */
/* the following identifier will be used in loader */
#define TP_FUNC_NOTHING          0x0
#define TP_FUNC_BEGINE_RECORD    0x1
#define TP_FUNC_END_RECORD       0x2
#define TP_FUNC_SUBMIT_OSI       0x3
#define TP_FUNC_GET_PROGRAM      0x4
/* the following identifier will be used in agent */
#define TP_FUNC_SUBMIT_CR3       0x5
#define TP_FUNC_GET_PAYLOAD      0x6
#define TP_FUNC_NEXT_PAYLOAD     0x7
#define TP_FUNC_TAINT_MEM        0x8
#define TP_FUNC_SAVE_VM          0x9
#define TP_FUNC_BEGIN_FUZZ       0xa
#define TP_FUNC_PRINT            0x10
#define TP_FUNC_CRASH            0x11

/* for compatibility with kAFL agent */
#define HYPERCALL_KAFL_SUBMIT_CR3 TP_FUNC_SUBMIT_CR3
#define HYPERCALL_KAFL_GET_PROGRAM TP_FUNC_GET_PROGRAM
#define HYPERCALL_KAFL_GET_PAYLOAD TP_FUNC_GET_PAYLOAD
#define HYPERCALL_KAFL_NEXT_PAYLOAD TP_FUNC_NEXT_PAYLOAD
#define HYPERCALL_KAFL_ACQUIRE TP_FUNC_BEGINE_RECORD
#define HYPERCALL_KAFL_RELEASE TP_FUNC_END_RECORD

#define PAYLOAD_SIZE						(128 << 18)				/* up to 32MB payloads */
#define PROGRAM_SIZE						(128 << 20)				/* kAFL supports 128MB programm data */
#define INFO_SIZE					(128 << 10)		/* 128KB info string */
#define TARGET_FILE					"/tmp/fuzzing_engine"	/* default target for the userspace component */
#define TARGET_FILE_WIN					"fuzzing_engine.exe"	

#define RR_NAME_LEN					32

/* some hypercall need return status of job implementation */
#define HYPER_STATUS_SUCCESS        0x01
#define HYPER_STATUS_FAILED         0x02
#define HYPER_STATUS_PL_TAIL        0x03


/* arguments of begin record (TP_FUNC_BEGINE_RECORD) */
#define MAX_TASK_NAME_LEGN 255
#define DEFAULT_RR_NAME "test"
/* _s_tname indicate rr name to begin record, which be located in ecx/rcx. */
typedef struct _s_tname {
	uint32_t  name_len;
	target_ulong prr_name;
} s_rrname, * ps_rrname;

/* arguments of submit osi information */
typedef struct _osi_info {
	target_ulong osi64_off; /* Offset between PsActiveProcessHead and PsLoadedModuleList */
	target_ulong off_allpool; /* The offset of ExAllocatePoolWithTag*/
	target_ulong off_freepool; /* The offset of ExFreePoolWithTag*/
	target_ulong off_allpool_ret; /* The offset of return*/
} osi_info, * posi_info;

/* arguments of get program */
typedef struct _get_program {
	target_ulong program_buffer;
	target_ulong buffer_len;
} get_program, * pget_program;

/* argument of get payload */
/* arg=payload buffer address */

/* arguments of next payload */
typedef struct _tp_payload {
	int32_t size;
	uint8_t data[PAYLOAD_SIZE - 4];
} tp_payload, * ptp_payload;
#define kAFL_payload tp_payload

/* TP_FUNC_TAINT_MEM is used to specify a memory region to taint, which means that
 * converts the corresponding memory region in tp-libtriton to variables, then symbolically
 * execute related instructions. */
 /* arguments of taint mem */
typedef struct _taint_mem {
	target_ulong addr;
	uint32_t len;
	bool taint_by_payload;
} taint_mem, * ptaint_mem;

/* TP_FUNC_SAVE_VM is used to take a snapshot, trigerred by loader.
 * */
typedef struct _save_vm {
	uint32_t name_len;
	target_ulong sp_name;
} save_vm, psave_vm;

/* TP_FUNC_PRINT is used to send message to the host to print.
 * */
typedef struct _tp_print {
	uint32_t message_len;
	target_ulong message_addr;
} tp_print, ptp_print;

#if defined(__MINGW64__)
static inline void tp_hypercall(target_ulong rbx, target_ulong rcx)
{
	target_ulong rax = TP_HC_IDENTIFIER;
	asm("movq %0, %%rcx;" : : "r"(rcx));
	asm("movq %0, %%rbx;" : : "r"(rbx));
	asm("movq %0, %%rax;" : : "r"(rax));
	asm("cpuid");
}
#else
#ifdef __cplusplus
extern "C" {
#endif
	target_ulong tp_hypercall(target_ulong rcx, target_ulong rdx);
#ifdef __cplusplus
}
#endif
#endif

#ifndef kAFL_hypercall
#define kAFL_hypercall tp_hypercall
#endif

#endif