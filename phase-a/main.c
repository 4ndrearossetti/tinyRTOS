#include "rtos.h"

#define REG(addr)     (*(volatile uint32_t *)(addr))

#define RCC_APB2ENR   REG(0x40021018)
#define RCC_IOPCEN    (1 << 4)

#define GPIOC_CRH     REG(0x40011004)
#define GPIOC_ODR     REG(0x4001100C)

int main(void) {
        RCC_APB2ENR |= RCC_IOPCEN;
        GPIOC_CRH &= ~(0xF << 20);
        GPIOC_CRH |=  (0x2 << 20);
        systick_init();

        while (1) {
        }
}

