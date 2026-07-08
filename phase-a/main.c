#include <stdint.h>

#define REG(addr) (*(volatile uint32_t *)(addr))

/* RCC */
#define RCC_APB2ENR   REG(0x40021018)
#define RCC_IOPCEN    (1 << 4)          /* GPIOC clock enable */

/* GPIOC */
#define GPIOC_CRH     REG(0x40011004)   /* config for pins 8..15 */
#define GPIOC_ODR     REG(0x4001100C)   /* output data register */

#define PC13          13

static void delay(volatile uint32_t n) {
        while (n--) {
                __asm__ volatile ("nop");
        }
}

int main(void) {
        /* enable GPIOC clock */
        RCC_APB2ENR |= RCC_IOPCEN;

        /* PC13: 4 config bits at (13-8)*4 = 20.
         * 0b0010 = output push-pull, max 2 MHz. */
        GPIOC_CRH &= ~(0xF << 20);
        GPIOC_CRH |=  (0x2 << 20);

        while (1) {
                GPIOC_ODR ^= (1 << PC13);
                delay(200000);
        }
}

