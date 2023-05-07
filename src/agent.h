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
#define SS_HC_IDENTIFIER 0x86E4CA11

/* hypercall identifier in ebx */
/* the following identifier will be used in loader */
typedef enum {
	SS_HC_NOTHING = 0,
	SS_HC_BEGINE_RECORD,
	SS_HC_END_RECORD,
	SS_HC_SUBMIT_OSI,
	SS_HC_GET_PROGRAM,

	/* the following identifier will be used in agent */
	SS_HC_SUBMIT_CR3,
	SS_HC_GET_PAYLOAD,
	SS_HC_NEXT_PAYLOAD,
	SS_HC_TAINT_MEM,
	SS_HC_SAVE_VM,
	SS_HC_BEGIN_TEST,
	SS_HC_PRINT,
	SS_HC_CRASH,
	SS_HC_SOURCE,
	SS_HC_SINK,
	SS_HC_MEM_OP,
	SS_HC_HELPER
} ss_hc_class;

#define PAYLOAD_SIZE             (128 << 18)				/* up to 32MB payloads */
#define PROGRAM_SIZE             (128 << 20)				/* supports 128MB programm data */
#define INFO_SIZE                (128 << 10)		/* 128KB info string */
#define TARGET_FILE              "/tmp/loader"	/* default target for the userspace component */
#define TARGET_FILE_WIN          "loader.exe"	

#define RR_NAME_LEN            32

/* some hypercall need return status of job implementation */
#define HYPER_STATUS_SUCCESS              0x01
#define HYPER_STATUS_FAILED               0x02
#define HYPER_STATUS_PL_TAIL              0x03
#define HYPER_STATUS_NOT_IN_RECORD        0x04

typedef struct _ss_env {
        target_ulong pid;
        target_ulong tid;
}ss_env, *pss_env;

/* arguments of begin record (SS_HC_BEGINE_RECORD) */
#define MAX_TASK_NAME_LEGN 255
#define DEFAULT_RR_NAME "test"
/* _ss_rrname indicate rr name to begin record, which be located in ecx/rcx. */
typedef struct _ss_rrname {
	uint32_t  name_len;
	target_ulong prr_name;
} ss_rrname, *pss_rrname;

/* arguments of submit osi information */
typedef struct _osi_info {
	target_ulong osi64_off; /* Offset between PsActiveProcessHead and PsLoadedModuleList */
	target_ulong off_allpool; /* The offset of ExAllocatePoolWithTag*/
	target_ulong off_freepool; /* The offset of ExFreePoolWithTag*/
	target_ulong off_allpool_ret; /* The offset of return*/
} osi_info, *posi_info;

/* arguments of get program */
typedef struct _get_program {
	target_ulong program_buffer;
	target_ulong buffer_len;
} get_program, *pget_program;

/* argument of get payload */
/* arg=payload buffer address */

/* arguments of next payload */
typedef struct _ss_payload {
	int32_t size;
	uint8_t data[PAYLOAD_SIZE-4];
} ss_payload, *pss_payload;

/* SS_HC_TAINT_MEM is used to specify a memory region to taint, which means that
 * converts the corresponding memory region in tp-libtriton to variables, then symbolically 
 * execute related instructions. */
/* arguments of taint mem */
typedef struct _taint_mem {
	target_ulong addr;
	uint32_t len;
	int taint_by_payload;
} taint_mem, *ptaint_mem;

/* SS_HC_SAVE_VM is used to take a snapshot, trigerred by loader.
 * */
typedef struct _save_vm {
	uint32_t name_len;
	target_ulong sp_name;
} save_vm, psave_vm;

/* SS_HC_PRINT is used to send message to the host to print.
 * */
typedef struct _ss_print {
	uint32_t message_len;
	target_ulong message_addr;
} ss_print, pss_print;

/* _op_reg.reg_type */
#ifndef SS_REG_TYPE
#define SS_REG_TYPE 1
typedef enum {
	/*XED_*/HREG_INVALID,
	/*XED_*/HREG_EFLAGS,
	/*XED_*/HREG_FS,
	/*XED_*/HREG_GS,
	/*XED_*/HREG_EAX,
	/*XED_*/HREG_ECX,
	/*XED_*/HREG_EDX,
	/*XED_*/HREG_EBX,
	/*XED_*/HREG_ESP,
	/*XED_*/HREG_EBP,
	/*XED_*/HREG_ESI,
	/*XED_*/HREG_EDI,
	/*XED_*/HREG_EIP,
	///*XED_*/HREG_RAX,
	///*XED_*/HREG_RCX,
	///*XED_*/HREG_RDX,
	///*XED_*/HREG_RBX,
	///*XED_*/HREG_RSP,
	///*XED_*/HREG_RBP,
	///*XED_*/HREG_RSI,
	///*XED_*/HREG_RDI,
	/*XED_*/HREG_R8,
	/*XED_*/HREG_R9,
	/*XED_*/HREG_R10,
	/*XED_*/HREG_R11,
	/*XED_*/HREG_R12,
	/*XED_*/HREG_R13,
	/*XED_*/HREG_R14,
	/*XED_*/HREG_R15,
	/* ext reg */
	/*XED_*/HREG_ST0,
	/*XED_*/HREG_ST1,
	/*XED_*/HREG_ST2,
	/*XED_*/HREG_ST3,
	/*XED_*/HREG_ST4,
	/*XED_*/HREG_ST5,
	/*XED_*/HREG_ST6,
	/*XED_*/HREG_ST7,
	/*XED_*/HREG_X87CONTROL,
	/*XED_*/HREG_X87STATUS,
	/*XED_*/HREG_X87TAG,
	/* MMX */
	/*XED_*/HREG_MMX0,
	/*XED_*/HREG_MMX1,
	/*XED_*/HREG_MMX2,
	/*XED_*/HREG_MMX3,
	/*XED_*/HREG_MMX4,
	/*XED_*/HREG_MMX5,
	/*XED_*/HREG_MMX6,
	/*XED_*/HREG_MMX7,
	/* SSE */
	/*XED_*/HREG_XMM0,
	/*XED_*/HREG_XMM1,
	/*XED_*/HREG_XMM2,
	/*XED_*/HREG_XMM3,
	/*XED_*/HREG_XMM4,
	/*XED_*/HREG_XMM5,
	/*XED_*/HREG_XMM6,
	/*XED_*/HREG_XMM7,
	/*XED_*/HREG_XMM8,
	/*XED_*/HREG_XMM9,
	/*XED_*/HREG_XMM10,
	/*XED_*/HREG_XMM11,
	/*XED_*/HREG_XMM12,
	/*XED_*/HREG_XMM13,
	/*XED_*/HREG_XMM14,
	/*XED_*/HREG_XMM15,
	/*XED_*/HREG_MXCSR,
} agent_x86_reg_enum_t;
#endif

typedef enum {
	SS_TYPE_REG = 1,
	SS_TYPE_MEM,
} ss_ss_type_t;

/* SS_HC_SINK and SS_HC_SOURCE is used to specified the sink/source point.
 * @type: Register or Memory
 * @reg: Which register to analyze. @reg is valid only if the @type is Register.
 * @addr: The address in the guest to analyze, @addr is valid only if the @type is Memory
 */
typedef struct _ss_source {
	ss_env t_env;
	ss_ss_type_t type;
	agent_x86_reg_enum_t reg;
	target_ulong addr;
	uint32_t addr_len;
} ss_source, *pss_source;
typedef struct _ss_sink {
	ss_env t_env;
	ss_ss_type_t type;
	agent_x86_reg_enum_t reg;
	target_ulong addr;
	uint32_t addr_len;
} ss_sink, *pss_sink;

typedef enum {
	SS_ACTION_OBJ_ALLOC = 1,
	SS_ACTION_OBJ_RELEASE,
} ss_action_obj_t;
/* SS_HC_MEM_OP
 * @action: Allocation or release or others.
 * @type: The pointer is in register or memory.
 * @reg: Which register that contains the obj pointer. It's valid if @type==SS_TYPE_REG && @action==ALLOC.
 * @addr: The guest address that the pointer located in. It's valid if @type==SS_TYPE_MEM && @action==ALLOC.
 * @oi: The base address and the length(optional) of obj.
 */
struct obj_info {
	target_ulong obj_addr_val;
	uint32_t obj_len;
};

typedef struct _ss_mem_op {
	ss_env t_env;
	ss_action_obj_t action;
	uint32_t type;
	agent_x86_reg_enum_t reg;
	target_ulong addr;
	struct obj_info oi;
} ss_mem_op, *pss_mem_op;

typedef enum _ss_hf_class {
	SS_HELPER_OTHER = 0,
	SS_HELPER_MEMMOVE,
	SS_HELPER_MEMCPY,
	SS_HELPER_MEMSET,
	SS_HELPER_STRLEN,
	SS_HELPER_ALLOC,
	SS_HELPER_FREE,
	SS_HELPER_BUGCHECK,
	SS_HELPER_CALLOC,
	SS_HELPER_REALLOC,
	SS_HELPER_EXECVE,
	SS_HELPER_FEXECVE,
	SS_HELPER_EXECV,
	SS_HELPER_EXECLE,
	SS_HELPER_EXECL,
	SS_HELPER_EXECVP,
	SS_HELPER_EXECLP
} ss_hf_class, *pss_hf_class;

typedef enum _ss_hf_action {
	SS_HELPER_ACTION_NONE = 0,
	SS_HELPER_ACTION_IN,
	SS_HELPER_ACTION_OUT,
} ss_hf_action;

/* SS_HC_HELPER
 * Helper functions.
 * @args: The arguments of that function. The specific structure of @args is depended on 
 * @func_id.
 */
typedef struct _ss_helper {
	ss_env t_env;
	uint32_t func_action;
	ss_hf_class func_class;
	target_ulong args;
} ss_helper, *pss_helper;


#ifdef __cplusplus
extern "C" { 
#endif

/* The ss_hypercall implemented by cupid instruction is used by agent/target to transmit commands/data to plugin in PANDA,
 * @rbx is hypecall identifier, like SS_HC_BEGINE_RECORD
 * @rcx is argument whose actual structure is dependent on the class of thus hypercall.
 * The return value is in rdi return value when cpuid returns.
 * */
#if defined(WIN32) || defined(_WIN32)
target_ulong ss_hypercall(target_ulong rcx, target_ulong rdx);
#else /* gcc-like compiler */
static inline target_ulong ss_hypercall(target_ulong rbx, target_ulong rcx)
{
	target_ulong rax = SS_HC_IDENTIFIER;
	__asm__ volatile (
		"movq %1, %%rcx;"
	        "movq %2, %%rbx;"
	        "movq %0, %%rax;"
		"cpuid;"
		"movq %%rdi, %0;"
		: "+r"(rax)
		: "g"(rcx), "g"(rbx)
		: "rax", "rcx", "rbx", "rdi"
	);
	return rax;
}
#endif /* __WIN32 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __AGENT__ */