/// Copyright (C) 2003  Fabrice Bellard
/// Copyright (C) 2010  Dependable Systems Laboratory, EPFL
/// Copyright (C) 2016  Cyberhaven, Inc
/// Copyrights of all contributions belong to their respective owners.
///
/// This library is free software; you can redistribute it and/or
/// modify it under the terms of the GNU Library General Public
/// License as published by the Free Software Foundation; either
/// version 2 of the License, or (at your option) any later version.
///
/// This library is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
/// Library General Public License for more details.
///
/// You should have received a copy of the GNU Library General Public
/// License along with this library; if not, see <http://www.gnu.org/licenses/>.

#ifndef __LIBCPU_EXEC_H__

#define __LIBCPU_EXEC_H__

#include <cpu/config.h>
#include <cpu/i386/cpu.h>
#include <cpu/tb.h>
#include <cpu/interrupt.h>
#include <cpu/softmmu_defs.h>
#include <qemu-compiler.h>
#include <qemu-log.h>


/* The return address may point to the start of the next instruction.
   Subtracting one gets us the call instruction itself.  */
#if defined(CONFIG_TCG_INTERPRETER)
/* Alpha and SH4 user mode emulations and Softmmu call GETPC().
   For all others, GETPC remains undefined (which makes TCI a little faster. */
# if defined(CONFIG_SOFTMMU) || defined(TARGET_ALPHA) || defined(TARGET_SH4)
extern void *tci_tb_ptr;
#  define GETPC() tci_tb_ptr
# endif
#elif defined(__s390__) && !defined(__s390x__)
# define GETPC() \
    ((void *)(((uintptr_t)__builtin_return_address(0) & 0x7fffffffUL) - 1))
#elif defined(__arm__)
/* Thumb return addresses have the low bit set, so we need to subtract two.
   This is still safe in ARM mode because instructions are 4 bytes.  */
# define GETPC() ((void *)((uintptr_t)__builtin_return_address(0) - 2))
#else
# define GETPC() ((void *)((uintptr_t)__builtin_return_address(0) - 1))
#endif

int cpu_restore_state(struct TranslationBlock *tb,
                      CPUArchState *env, uintptr_t searched_pc);

void cpu_gen_init(void);
void cpu_exit(CPUArchState *s);
void cpu_exec_init_all(void);
void tcg_exec_init(unsigned long tb_size);

void tlb_flush(CPUArchState *env, int flush_global);
void tlb_flush_page(CPUArchState *env, target_ulong addr);
void tlb_fill(CPUArchState *env1, target_ulong addr, target_ulong page_addr,
              int is_write, int mmu_idx,
              void *retaddr);


void tb_flush(CPUArchState *env);
TranslationBlock *tb_find_pc(uintptr_t pc_ptr);

/* page related stuff */

#define TARGET_PAGE_SIZE (1 << TARGET_PAGE_BITS)
#define TARGET_PAGE_MASK ~(TARGET_PAGE_SIZE - 1)
#define TARGET_PAGE_ALIGN(addr) (((addr) + TARGET_PAGE_SIZE - 1) & TARGET_PAGE_MASK)

void *get_ram_list_phys_dirty(void);
ram_addr_t last_ram_offset(void);

#define QEMU_FILE_TYPE_BIOS   0
#define QEMU_FILE_TYPE_KEYMAP 1
char *qemu_find_file(int type, const char *name);

void cpu_dump_state(CPUArchState *env, FILE *f, fprintf_function cpu_fprintf,
                    int flags);
void cpu_dump_statistics(CPUArchState *env, FILE *f, fprintf_function cpu_fprintf,
                         int flags);

/* Return the physical page corresponding to a virtual one. Use it
   only for debugging because no protection checks are done. Return -1
   if no page found. */
target_phys_addr_t cpu_get_phys_page_debug(CPUArchState *env, target_ulong addr);

void QEMU_NORETURN cpu_abort(CPUArchState *env, const char *fmt, ...)
    GCC_FMT_ATTR(2, 3);
extern CPUArchState *first_cpu, *cpu_single_env;

typedef void (*CPUInterruptHandler)(CPUArchState *, int);
extern CPUInterruptHandler cpu_interrupt_handler;

void QEMU_NORETURN cpu_loop_exit(CPUArchState *env1);

#endif
