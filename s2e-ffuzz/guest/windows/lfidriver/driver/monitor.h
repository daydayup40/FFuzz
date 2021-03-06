#ifndef _LFIDRIVER_MONITOR_H_
#define _LFIDRIVER_MONITOR_H_

#include <WindowsMonitor2.h>

typedef VOID REGISTER_KERNEL_STRUCTS(UINT_PTR KernelLoadBase, UINT_PTR KernelNativeBase);
typedef struct REGISTER_KERNEL_STRUCTS_HANDLERS {
    UINT32 CheckSum;
    REGISTER_KERNEL_STRUCTS *Handler;
} REGISTER_KERNEL_STRUCTS_HANDLERS;

VOID MonitorInitCommon(S2E_WINMON2_COMMAND *Command);

extern S2E_WINMON2_KERNEL_STRUCTS g_KernelStructs;
extern LFIDRIVER_KERNEL_STRUCTS g_LfiKernelStructs;
extern REGISTER_KERNEL_STRUCTS_HANDLERS g_KernelStructHandlers[];

#define IA32_FS_BASE 0xc0000100
#define IA32_GS_BASE 0xc0000101


#endif