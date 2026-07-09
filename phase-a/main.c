#include <stdint.h>

#define REG(addr) (*(volatile uint32_t *)(addr))

#define RCC_APB2ENR     REG(0x40021018)
#define RCC_IOPCEN      (1 << 4)
#define CTRL            REG(0xE000E010)
#define LOAD            REG(0xE000E014)
#define VAL             REG(0xE000E018)

#define GPIOC_CRH     REG(0x40011004)
#define GPIOC_ODR     REG(0x4001100C)

#define PC13          13

volatile uint32_t system_ticks = 0;

void systick_init() {
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

int main(void) {
        RCC_APB2ENR |= RCC_IOPCEN;
        GPIOC_CRH &= ~(0xF << 20);
        GPIOC_CRH |=  (0x2 << 20);
        systick_init();

        while (1) {
                GPIOC_ODR ^= (1 << PC13);
                delay_ms(500);
        }
}

