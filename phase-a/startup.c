#include <stdint.h>

extern uint32_t _sidata;   // start of .data initial values in flash
extern uint32_t _sdata;    // start of .data in RAM
extern uint32_t _edata;    // end of .data in RAM
extern uint32_t _sbss;     // start of .bss
extern uint32_t _ebss;     // end of .bss
extern uint32_t _estack;   // top of stack

int main(void);
void Reset_Handler(void);
void Default_Handler(void);
void SysTick_Handler(void);

// Place this array in the .isr_vector section so the linker script puts it at 0x08000000
__attribute__((section(".isr_vector")))
const uint32_t vector_table[] = {
    (uint32_t)&_estack,           // 0: Initial stack pointer
    (uint32_t)Reset_Handler,      // 1: Reset
    (uint32_t)Default_Handler,    // 2: NMI
    (uint32_t)Default_Handler,    // 3: HardFault
    (uint32_t)Default_Handler,    // 4: MemManage
    (uint32_t)Default_Handler,    // 5: BusFault
    (uint32_t)Default_Handler,    // 6: UsageFault
    0, 0, 0, 0,                   // 7-10: Reserved
    (uint32_t)Default_Handler,    // 11: SVCall
    (uint32_t)Default_Handler,    // 12: DebugMon
    0,                            // 13: Reserved
    (uint32_t)Default_Handler,    // 14: PendSV
    (uint32_t)SysTick_Handler,    // 15: SysTick
    // (60 peripheral IRQs would follow — omitted for A0)
};

void Reset_Handler(void) {
        // Copy data from flash to RAM
        uint32_t *src = &_sidata;
        uint32_t *dst = &_sdata;
        while (dst < &_edata) {
                *dst++ = *src++;
        }

        // Zero .bss
        dst = &_sbss;
        while (dst < &_ebss) {
                *dst++ = 0;
        }

        // Call Main
        main();

        // If main ever returns, loop forever (shouldn't happen)
        while (1) { }
}

void Default_Handler(void) {
        while (1) { }
}

