.syntax unified
.cpu cortex-m3
.thumb

/* ----- */

.global PendSV_Handler
.type PendSV_Handler, %function
.thumb_func

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

/* ----- */

.global scheduler_start
.type scheduler_start, %function
.thumb_func

scheduler_start:
    // Get the first task's saved sp
    LDR r0, =current_tcb
    LDR r0, [r0]
    LDR r0, [r0]

    // Pop r4-r11
    LDMIA r0!, {r4-r11}

    // Load the entry PC from the frame
    LDR r1, [r0, #24]

    // Advance r0 past the frame
    ADD r0, r0, #32

    // Set PSP
    MSR PSP, r0

    // Switch thread mode to PSP
    MOV r0, #2
    MSR CONTROL, r0
    ISB

    BX r1

