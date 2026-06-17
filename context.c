#include <stddef.h>
#include "rtos.h"

void init_tcb(task_control_board_t *tcb, void (*entry)(void)) {
        getcontext(&tcb->exec_state);
        tcb->exec_state.uc_stack.ss_sp = tcb->stack;
        tcb->exec_state.uc_stack.ss_size = sizeof(tcb->stack);
        tcb->exec_state.uc_link = NULL;
        tcb->state = TASK_READY;
        tcb->wake_time = 0;
        makecontext(&tcb->exec_state, entry, 0);
}

