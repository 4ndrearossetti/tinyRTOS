#include "rtos.h"

#define REG(addr)   (*(volatile uint32_t *)(addr))

#define CTRL        REG(0xE000E010)
#define LOAD        REG(0xE000E014)
#define VAL         REG(0xE000E018)

void systick_init(void) {
        LOAD = 7999;
        VAL = 0;
        CTRL = (1<<0) | (1<<1) | (1<<2);
}

