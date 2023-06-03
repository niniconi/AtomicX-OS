#include "lib.h"
#include "print.h"
#include "gate.h"
#include "trap.h"

void do_div_error(regs * regs,unsigned long error_code){
    error("DIV 0 error code : %#016x\n",error_code);
    while(1);
}
void do_debug_exception(regs * regs){
    debug("debug\n");
    while(1);
}
void do_nmi(regs * regs){
    interrupt("nmi\n");
    while(1);
}
void do_breakpoint(regs * regs){
    debug("breakpoint\n");
    while(1);
}
void do_overflow(regs * regs){
    error("overflow\n");
    while(1);
}
void do_out_bounds(regs * regs){
    error("out bound\n");
    while(1);
}
void do_undefine(regs * regs){
    error("undefine mechine opread\n");
    while(1);
}
void do_device_exception(regs * regs){
    error("device exception\n");
    while(1);
}
void do_double_error(regs * regs){
    error("double error abort");
    while(1);
}
void do_coprocessor_error(regs * regs){
    error("coprocessor error\n");
    while(1);
}
void do_invaild_tss(regs * regs){
    error("invaild tss\n");
    while(1);
}
void do_undefine_segment(regs * regs){
    error("undefine segment\n");
    while(1);
}
void do_SS_segment_error(regs * regs){
    error("ss segment error\n");
    while(1);
}
void do_general_protect(regs * regs){
    error("general protect\n");
    while(1);
}
void do_invaild_page(regs * regs){
    error("invaild page\n");
    while(1);
}
void do_x87_FPU(regs * regs){
    error("x87 FPU error\n");
    while(1);
}
void do_alignment_check(regs * regs){
    error("alignment check\n");
    while(1);
}
void do_mechine_check(regs * regs){
    error("mechine check\n");
    while(1);
}
void do_SIMD_float_exception(regs * regs){
    error("SIMD float exception\n");
    while(1);
}
void do_VM_exception(regs * regs){
    error("VM exception\n");
    while(1);
}

void init_trap(){
    set_trap0_gate(0,div_error);
    set_trap0_gate(1,debug_exception);
    set_intr_gate(2,nmi);
    set_trap3_gate(3,breakpoint);
    set_trap3_gate(4,overflow);
    set_trap3_gate(5,out_bounds);
    set_trap0_gate(6,undefine);
    set_trap0_gate(7,device_exception);
    set_trap0_gate(8,double_error);
    set_trap0_gate(9,coprocessor_error);
    set_trap0_gate(10,invaild_tss);
    set_trap0_gate(11,undefine_segment);
    set_trap0_gate(12,SS_segment_error);
    set_trap0_gate(13,general_protect);
    set_trap0_gate(14,invaild_page);
    set_trap0_gate(16,x87_FPU);
    set_trap0_gate(17,alignment_check);
    set_trap0_gate(18,mechine_check);
    set_trap0_gate(19,SIMD_float_exception);
    set_trap0_gate(20,VM_exception);
}
