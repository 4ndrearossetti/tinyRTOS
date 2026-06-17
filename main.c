#include <stdio.h>
#include "rtos.h"

static task_control_board_t tcb_a;
static task_control_board_t tcb_b;
static task_control_board_t tcb_idle;

static void task_a(void) {
        while (1) {
                printf("task_a\n");
                task_delay(3);
        }
}

static void task_b(void) {
        while (1) {
                printf("task_b\n");
                task_delay(5);
        }
}

static void task_idle(void) {
        while (1) {
                yield();
        }
}

int main(void) {
        tcbs[0] = &tcb_a;
        tcbs[1] = &tcb_b;
        tcbs[2] = &tcb_idle;

        init_tcb(&tcb_a, task_a);
        init_tcb(&tcb_b, task_b);
        init_tcb(&tcb_idle, task_idle);

        setcontext(&tcb_a.exec_state);

        return 0;
}

