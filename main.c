#include <ucontext.h>
#include <stdio.h>

#define NUM_TASKS 2

typedef struct {
        ucontext_t exec_state;
        char stack[16384];
} task_control_board_t;

task_control_board_t tcb_a;
task_control_board_t tcb_b;

int current_idx = 0;
task_control_board_t *tcbs[] = {&tcb_a, &tcb_b};

void init_tcb(task_control_board_t *tcb, void (*entry)(void)) {
        getcontext(&tcb->exec_state);
        tcb->exec_state.uc_stack.ss_sp = tcb->stack;
        tcb->exec_state.uc_stack.ss_size = sizeof(tcb->stack);
        tcb->exec_state.uc_link = NULL;
        makecontext(&tcb->exec_state, entry, 0);
}

void yield() {
        int prev = current_idx;
        current_idx = (current_idx + 1) % NUM_TASKS;
        swapcontext(&tcbs[prev]->exec_state, &tcbs[current_idx]->exec_state);
}

void task_a() {
        while (1) {
                printf("task_a\n");
                yield();
        }
}

void task_b() {
        while (1) {
                printf("task_b\n");
                yield();
        }
}

int main() {
        init_tcb(&tcb_a, task_a);
        init_tcb(&tcb_b, task_b);

        setcontext(&tcb_a.exec_state);

        return 0;
}

