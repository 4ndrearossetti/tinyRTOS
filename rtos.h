#ifndef RTOS_H
#define RTOS_H

#include <ucontext.h>

#define NUM_TASKS 3

typedef enum {
        TASK_READY,
        TASK_BLOCKED
} task_state_t;

typedef struct {
        ucontext_t exec_state;
        char stack[16384];
        task_state_t state;
        int wake_time;
} task_control_board_t;

extern int system_ticks;
extern task_control_board_t *tcbs[NUM_TASKS];
extern int current_idx;

void init_tcb(task_control_board_t *tcb, void (*entry)(void));
void yield(void);
void task_delay(int ticks);

void timer_init();

#endif

