#include "rtos.h"

#define REG(addr)   (*(volatile uint32_t *)(addr))

#define CTRL        REG(0xE000E010)
#define LOAD        REG(0xE000E014)
#define VAL         REG(0xE000E018)

volatile uint32_t system_ticks = 0;

void systick_init(void) {
        LOAD = 7999;
        VAL = 0;
        CTRL = (1<<0) | (1<<1) | (1<<2);
}

void SysTick_Handler(void) {
        system_ticks++;
}

static void delay_ms(uint32_t ms) {
        uint32_t start = system_ticks;
        while ((system_ticks - start) < ms) { }
}

