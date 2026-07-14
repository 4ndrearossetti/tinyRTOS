.syntax unified
.cpu cortex-m3
.thumb

.global PendSV_Handler
.type PendSV_Handler, %function

PendSV_Handler:
    // Save outgoing task
    MRS r0, PSP
    STMDB r0!, {r4-r11}
    LDR r1, =current_tcb
    LDR r1, [r1]
    STR r0, [r1]

    // Load incoming task
    LDR r2, =next_tcb
    LDR r2, [r2]
    LDR r2, [r2]
    LDMIA r2!, {r4-r11}
    MSR PSP, r2

    // current_tcb = next_tcb
    LDR r0, =next_tcb
    LDR r0, [r0]
    LDR r1, =current_tcb
    STR r0, [r1]

    BX LR

