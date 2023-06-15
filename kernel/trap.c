#include "lib.h"
#include "print.h"
#include "gate.h"
#include "trap.h"

void do_div_error(regs * regs,unsigned long error_code){
    error("div(0)  ERRCODE=%#018x\n",error_code);
    while(1);
}
void do_debug_exception(regs * regs){
    debug("debug(1)\n");
    while(1);
}
void do_nmi(regs * regs,unsigned long error_code){
    interrupt("nmi(2) ERRCODE=%#018x\n",error_code);
    while(1);
}
void do_breakpoint(regs * regs){
    debug("breakpoint(3)\n");
    while(1);
}
void do_overflow(regs * regs){
    error("overflow(4)\n");
    while(1);
}
void do_out_bounds(regs * regs){
    error("out bound(5)\n");
    while(1);
}
void do_undefine(regs * regs){
    error("undefine_mechine_opread(6)\n");
    while(1);
}
void do_device_exception(regs * regs){
    error("device exception(7)\n");
    while(1);
}
void do_double_error(regs * regs){
    error("double error abort(8)");
    while(1);
}
void do_coprocessor_error(regs * regs){
    error("coprocessor error(9)\n");
    while(1);
}
void do_invaild_tss(regs * regs){
    error("invaild tss(10)\n");
    while(1);
}
void do_undefine_segment(regs * regs){
    error("undefine segment(11)\n");
    while(1);
}
void do_SS_segment_error(regs * regs){
    error("ss segment error(12)\n");
    while(1);
}
void do_general_protect(regs * regs,unsigned long error_code){
    error("general protect(13)  ERRCODE=%#018x\n",error_code);
    while(1);
}
void do_page_fault(regs * regs,unsigned long error_code,unsigned long rip){
    error("page fault(14)  ERRCODE=%#018x RIP=%#018x ",error_code,rip);
    if(!(error_code & 0x01))
        color_print(COL_RED, COL_BLACK, "page not exist,");

    if(error_code & 0x02)
        color_print(COL_RED, COL_BLACK, "write fault,");
    else
        color_print(COL_RED, COL_BLACK, "read fault,");

    if(error_code & 0x04)
        color_print(COL_RED, COL_BLACK, "user access,");
    else
        color_print(COL_RED, COL_BLACK, "system access,");

    if(error_code & 0x08)
        color_print(COL_RED, COL_BLACK, "set reserved bit fault,");

    if(error_code & 0x10)
        color_print(COL_RED, COL_BLACK, "while fetching instruction,");

    color_print(COL_RED, COL_BLACK, "\b.\n");
    while(1);
}
void do_x87_FPU(regs * regs){
    error("x87 FPU error(16)\n");
    while(1);
}
void do_alignment_check(regs * regs){
    error("alignment check(17)\n");
    while(1);
}
void do_mechine_check(regs * regs){
    error("mechine check(18)\n");
    while(1);
}
void do_SIMD_float_exception(regs * regs){
    error("SIMD float exception(19)\n");
    while(1);
}
void do_VM_exception(regs * regs){
    error("VM exception(20)\n");
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
    set_trap0_gate(10,page_fault);
    set_trap0_gate(11,undefine_segment);
    set_trap0_gate(12,SS_segment_error);
    set_trap0_gate(13,general_protect);
    set_trap0_gate(14,page_fault);
    set_trap0_gate(16,x87_FPU);
    set_trap0_gate(17,alignment_check);
    set_trap0_gate(18,mechine_check);
    set_trap0_gate(19,SIMD_float_exception);
    set_trap0_gate(20,VM_exception);
}
