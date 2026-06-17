#include "rtos.h"

int system_ticks = 0;
int current_idx = 0;
task_control_board_t *tcbs[NUM_TASKS];

void yield(void) {
        system_ticks++;

        for (int i = 0; i < NUM_TASKS; i++) {
                if (tcbs[i]->state == TASK_BLOCKED && tcbs[i]->wake_time <= system_ticks) {
                        tcbs[i]->state = TASK_READY;
                }
        }

        for (int offset = 1; offset <= NUM_TASKS; offset++) {
                int i = (current_idx + offset) % NUM_TASKS;
                if (tcbs[i]->state == TASK_READY) {
                        int prev = current_idx;
                        current_idx = i;
                        swapcontext(&tcbs[prev]->exec_state, &tcbs[i]->exec_state);
                        return;
                }
        }
}

void task_delay(int ticks) {
        tcbs[current_idx]->state = TASK_BLOCKED;
        tcbs[current_idx]->wake_time = system_ticks + ticks;
        yield();
}

