#include "rtos.h"

void task_stack_init(tcb_t *tcb, void (*entry)(void)) {
        tcb->sp = (uint32_t *)(((uint32_t)&tcb->stack[256]) & ~0x7u);

        *(--(tcb->sp)) = 0x01000000;            // xPSR
        *(--(tcb->sp)) = (uint32_t)entry;       // PC
        *(--(tcb->sp)) = 0;                     // LR
        *(--(tcb->sp)) = 0;                     // r12
        *(--(tcb->sp)) = 0;                     // r3
        *(--(tcb->sp)) = 0;                     // r2
        *(--(tcb->sp)) = 0;                     // r1
        *(--(tcb->sp)) = 0;                     // r0
        *(--(tcb->sp)) = 0;                     // r11
        *(--(tcb->sp)) = 0;                     // r10
        *(--(tcb->sp)) = 0;                     // r9
        *(--(tcb->sp)) = 0;                     // r8
        *(--(tcb->sp)) = 0;                     // r7
        *(--(tcb->sp)) = 0;                     // r6
        *(--(tcb->sp)) = 0;                     // r5
        *(--(tcb->sp)) = 0;                     // r4
}

