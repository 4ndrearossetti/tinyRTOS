#include "rtos.h"

#define SCB_ICSR        (*(volatile uint32_t *)0xE000ED04)
#define PENDSVSET       (1u << 28)

volatile uint32_t system_ticks = 0;

volatile uint32_t current_idx = 0;
tcb_t *tcbs[NUM_TASKS];
tcb_t *current_tcb;
tcb_t *next_tcb;

void scheduler_pick_next(void) {
        for (uint32_t i = 0; i < NUM_TASKS; i++) {
                if ((tcbs[i]->state == TASK_BLOCKED) && (tcbs[i]->wake_time <= system_ticks)) {
                        tcbs[i]->state = TASK_READY;
                }
        }

        for (uint32_t off = 1; off <= NUM_TASKS; off++) {
                uint32_t i = (current_idx + off) % NUM_TASKS;
                if (tcbs[i]->state == TASK_READY) {
                        current_idx = i;
                        next_tcb = tcbs[i];
                        return;
                }
        }
}

void task_delay(uint32_t ticks) {
        current_tcb->state = TASK_BLOCKED;
        current_tcb->wake_time = system_ticks + ticks;
        scheduler_pick_next();
        SCB_ICSR = PENDSVSET;
}

