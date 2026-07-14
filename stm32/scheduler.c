#include "rtos.h"

#define SCB_ICSR        (*(volatile uint32_t *)0xE000ED04)
#define PENDSVSET       (1u << 28)

static inline void irq_disable(void) { __asm__ volatile ("cpsid i" ::: "memory"); }
static inline void irq_enable(void)  { __asm__ volatile ("cpsie i" ::: "memory"); }

volatile uint32_t system_ticks = 0;

volatile uint32_t current_idx = 0;
tcb_t *tcbs[NUM_TASKS];
tcb_t * volatile current_tcb;
tcb_t * volatile next_tcb;

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
        irq_disable();
        current_tcb->wake_time = system_ticks + ticks;
        current_tcb->state = TASK_BLOCKED;
        scheduler_pick_next();
        SCB_ICSR = PENDSVSET;
        irq_enable();
}

void SysTick_Handler(void) {
        system_ticks++;
        scheduler_pick_next();
        if (next_tcb != current_tcb) {
                SCB_ICSR = PENDSVSET;
        }
}

