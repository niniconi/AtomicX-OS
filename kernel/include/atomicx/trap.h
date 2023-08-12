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
extern void page_fault();
extern void x87_FPU();
extern void alignment_check();
extern void mechine_check();
extern void SIMD_float_exception();
extern void VM_exception();

void do_div_error(regsf_t * regs);
void do_debug_exception(regsf_t * regs);
void do_nmi(regsf_t * regs);
void do_breakpoint(regsf_t * regs);
void do_overflow(regsf_t * regs);
void do_out_bounds(regsf_t * regs);
void do_undefine(regsf_t * regs);
void do_device_exception(regsf_t * regs);
void do_double_error(regsf_t * regs);
void do_coprocessor_error(regsf_t * regs);
void do_invaild_tss(regsf_t * regs);
void do_undefine_segment(regsf_t * regs);
void do_SS_segment_error(regsf_t * regs);
void do_general_protect(regsf_t * regs);
void do_page_fault(regsf_t * regs);
void do_x87_FPU(regsf_t * regs);
void do_alignment_check(regsf_t * regs);
void do_mechine_check(regsf_t * regs);
void do_SIMD_float_exception(regsf_t * regs);
void do_VM_exception(regsf_t * regs);

void init_trap();

#endif
