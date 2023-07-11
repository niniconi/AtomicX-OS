#include "lib.h"
#include "print.h"
#include "gate.h"
#include "trap.h"

void do_div_error(regsf_t * regs){
    error("div(0)  RIP=%#018x\n",regs->rip);
    while(1);
}
void do_debug_exception(regsf_t * regs){
    debug("debug(1)\n");
    while(1);
}
void do_nmi(regsf_t * regs){
    interrupt("nmi(2)\n");
    while(1);
}
void do_breakpoint(regsf_t * regs){
    debug("breakpoint(3)\n");
    while(1);
}
void do_overflow(regsf_t * regs){
    error("overflow(4)\n");
    while(1);
}
void do_out_bounds(regsf_t * regs){
    error("out bound(5)\n");
    while(1);
}
void do_undefine(regsf_t * regs){
    error("undefine_mechine_opread(6)\n");
    while(1);
}
void do_device_exception(regsf_t * regs){
    error("device exception(7)\n");
    while(1);
}
void do_double_error(regsf_t * regs){
    error("double error abort(8)");
    while(1);
}
void do_coprocessor_error(regsf_t * regs){
    error("coprocessor error(9)\n");
    while(1);
}
void do_invaild_tss(regsf_t * regs){
    error("invaild tss(10)\n");
    while(1);
}
void do_undefine_segment(regsf_t * regs){
    error("undefine segment(11)\n");
    while(1);
}
void do_SS_segment_error(regsf_t * regs){
    error("ss segment error(12)\n");
    while(1);
}
void do_general_protect(regsf_t * regs){
    error("general protect(13)  ERRCODE=%#018x RIP=%#018x\n",regs->errorcode,regs->rip);
    while(1);
}
void do_page_fault(regsf_t * regs){
    error("page fault(14)  ERRCODE=%#018x RIP=%#018x ",regs->errorcode,regs->rip);
    if(!(regs->errorcode & 0x01))
        color_print(COL_RED, COL_BLACK, "page not exist,");

    if(regs->errorcode & 0x02)
        color_print(COL_RED, COL_BLACK, "write fault,");
    else
        color_print(COL_RED, COL_BLACK, "read fault,");

    if(regs->errorcode & 0x04)
        color_print(COL_RED, COL_BLACK, "user access,");
    else
        color_print(COL_RED, COL_BLACK, "system access,");

    if(regs->errorcode & 0x08)
        color_print(COL_RED, COL_BLACK, "set reserved bit fault,");

    if(regs->errorcode & 0x10)
        color_print(COL_RED, COL_BLACK, "while fetching instruction,");

    color_print(COL_RED, COL_BLACK, "\b.\n");
    while(1);
}
void do_x87_FPU(regsf_t * regs){
    error("x87 FPU error(16)\n");
    while(1);
}
void do_alignment_check(regsf_t * regs){
    error("alignment check(17)\n");
    while(1);
}
void do_mechine_check(regsf_t * regs){
    error("mechine check(18)\n");
    while(1);
}
void do_SIMD_float_exception(regsf_t * regs){
    error("SIMD float exception(19)\n");
    while(1);
}
void do_VM_exception(regsf_t * regs){
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
    info("init trap\n");
}
