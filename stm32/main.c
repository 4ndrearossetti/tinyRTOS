#include "rtos.h"

#define REG(addr)       (*(volatile uint32_t *)(addr))

#define RCC_APB2ENR     REG(0x40021018)
#define RCC_IOPCEN      (1 << 4)

#define GPIOC_CRH       REG(0x40011004)
#define GPIOC_ODR       REG(0x4001100C)

#define PC13            13

#define SHPR3   (*(volatile uint32_t *)0xE000ED20)

static void task_led(void) {
    GPIOC_ODR |= (1 << PC13);
    while (1) {
        GPIOC_ODR ^= (1 << PC13);  // led toggle
        task_delay(500);
    }
}

static void task_dummy(void) {
    while (1) {
        task_delay(1000);
    }
}

static void task_idle(void) {
    while (1) {
        // always runnable
    }
}

static tcb_t tcb_led;
static tcb_t tcb_dummy;
static tcb_t tcb_idle;

int main(void) {
        RCC_APB2ENR |= RCC_IOPCEN;
        GPIOC_CRH &= ~(0xF << 20);
        GPIOC_CRH |=  (0x2 << 20);

        tcbs[0] = &tcb_led;
        tcbs[1] = &tcb_dummy;
        tcbs[2] = &tcb_idle;

        task_stack_init(&tcb_led,   task_led);
        task_stack_init(&tcb_dummy, task_dummy);
        task_stack_init(&tcb_idle,  task_idle);

        tcb_led.state   = TASK_READY;
        tcb_dummy.state = TASK_READY;
        tcb_idle.state  = TASK_READY;

        current_tcb = &tcb_led;
        next_tcb = current_tcb;

        SHPR3 |= (0xFFu << 16);  // PendSV priority = lowest

        systick_init();

        scheduler_start();

        while (1) {
        }
}

