#ifndef __TRAP_H
#define __TRAP_H
#include "lib.h"

extern void div_error();
extern void debug_exception();
extern void nmi();
extern void breakpoint();
extern void overflow();
extern void out_bounds();
extern void undefine();
extern void device_exception();
extern void double_error();
extern void coprocessor_error();
extern void invaild_tss();
extern void undefine_segment();
extern void SS_segment_error();
extern void general_protect();
extern void invaild_page();
extern void x87_FPU();
extern void alignment_check();
extern void mechine_check();
extern void SIMD_float_exception();
extern void VM_exception();

void do_div_error(unsigned long error_code,regs * regs);
void do_debug_exception(regs * regs);
void do_nmi(regs * regs);
void do_breakpoint(regs * regs);
void do_overflow(regs * regs);
void do_out_bounds(regs * regs);
void do_undefine(regs * regs);
void do_device_exception(regs * regs);
void do_double_error(regs * regs);
void do_coprocessor_error(regs * regs);
void do_invaild_tss(regs * regs);
void do_undefine_segment(regs * regs);
void do_SS_segment_error(regs * regs);
void do_general_protect(regs * regs);
void do_invaild_page(regs * regs);
void do_x87_FPU(regs * regs);
void do_alignment_check(regs * regs);
void do_mechine_check(regs * regs);
void do_SIMD_float_exception(regs * regs);
void do_VM_exception(regs * regs);

void init_trap();

#endif
