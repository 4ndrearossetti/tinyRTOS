#ifndef RTOS_H
#define RTOS_H

#include <stdint.h>

#define NUM_TASKS 3

typedef enum {
        TASK_READY,
        TASK_BLOCKED
} task_state_t;

typedef struct {
        uint32_t *sp;           // saved stack pointer
        uint32_t stack[256];    // task's private stack (1KB)
        task_state_t state;
        uint32_t wake_time;
} tcb_t;

extern volatile uint32_t system_ticks;
extern tcb_t *tcbs[NUM_TASKS];
extern tcb_t *current_tcb;
extern tcb_t *next_tcb;

void systick_init(void);

void scheduler_start(void);
void task_stack_init(tcb_t * tcb, void (*entry)(void));

void scheduler_pick_next(void);
void task_delay(uint32_t ticks);

#endif

