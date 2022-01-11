#include "Timer.h"

#include "Error.h"
#include "Game.h"

/* Some portion of the fallowing code is taken from the manual. */

void timer_disable_interrupts(void) {
	uint32_t const status = 0b11010011;
	asm("msr cpsr, %[ps]" : : [ps] "r"(status));
}
void timer_enable_interrupts(void) {
	uint32_t const status = 0b01010011;
	asm("msr cpsr, %[ps]" : : [ps] "r"(status));
}
static void set_interrupt_stack(void) {
	uint32_t const irqNoInterrupt = 0b11010010;
	asm("msr cpsr, %[ps]" : : [ps] "r"(irqNoInterrupt));
	uint32_t const onchipMemoryTop = 0xFFFFFFFF - 7;
	asm("mov sp, %[ps]" : : [ps] "r"(onchipMemoryTop));
	// Go back to SVC mode before executing subroutine return!
	uint32_t const svc = 0b11010011;
	asm("msr cpsr, %[ps]" : : [ps] "r"(svc));
}
static void config_interrupt(uint32_t const N) {
	uint32_t const ICDISERn = (N >> 3) & 0xFFFFFFFC;
	volatile uint32_t *const ICDISER = 0xFFFED100 + ICDISERn;
	uint32_t const enableBit = N & 0x1F;
	uint32_t const enableMask = 0x1 << enableBit;
	*ICDISER |= enableMask;
	uint32_t const ICDIPTRn = (N & 0xFFFFFFFC);
	uint32_t const index = N & 0x3;
	volatile uint8_t *const ICDIPTR = 0xFFFED800 + ICDIPTRn + index;
	uint8_t const targetCPU = 1;
	*ICDIPTR = targetCPU;
}
static void config_gic(void) {
	config_interrupt(TIMER_INTERRUPT_ID);
	volatile uint32_t *const ICCPMR = 0xFFFEC104;
	uint16_t const priorityMask = 0xFFFF;
	*ICCPMR = priorityMask;
	volatile uint32_t *const ICCICR = 0xFFFEC100;
	*ICCICR = 1;
	volatile uint32_t *const ICDDCR = 0xFFFED000;
	*ICDDCR = 1;
}
static void config_fpga_timer(void) {
	volatile uint16_t *const control = 0xFF202004;
	*control = 0;
	uint32_t const frequency = 100e6;
	uint32_t const counter = frequency / TIMER_FREQUENCY;
	volatile uint16_t *const startLow = 0xFF202008;
	*startLow = counter & 0xFFFF;
	volatile uint16_t *const startHigh = 0xFF20200C;
	*startHigh = (counter >> 8) & 0xFFFF;
	*control = 0b0111;
}
void timer_config(void) {
	timer_disable_interrupts();
	set_interrupt_stack();
	config_gic();
	config_fpga_timer();
	timer_enable_interrupts();
}
/** Handles normal interrupts. */
void __attribute__((interrupt)) __cs3_isr_irq(void) {
	volatile uint32_t const *const ICCIAR = 0xFFFEC10C;
	uint32_t const id = *ICCIAR;
	switch (id) {
	case TIMER_INTERRUPT_ID:
		game_update();
		break;
	default:
		error_show(ERROR_UNDEFINED_ISR);
	};
	volatile uint32_t *const ICCEOIR = 0xFFEC110;
	*ICCEOIR = id;
}
/** Handles resets. */
void __attribute__((interrupt)) __cs3_reset(void) {
	error_show(ERROR_UNDEFINED_ISR);
}
/** Handles undefined instructions. */
void __attribute__((interrupt)) __cs3_isr_undef(void) {
	error_show(ERROR_UNDEFINED_ISR);
}
/** Handles supervisor call instructions. */
void __attribute__((interrupt)) __cs3_isr_swi(void) {
	error_show(ERROR_UNDEFINED_ISR);
}
/** Handles prefetch aborts. */
void __attribute__((interrupt)) __cs3_isr_pabort(void) {
	error_show(ERROR_UNDEFINED_ISR);
}
/** Handles data aborts. */
void __attribute__((interrupt)) __cs3_isr_dabort(void) {
	error_show(ERROR_UNDEFINED_ISR);
}
/** Handles fast interrupts. */
void __attribute__((interrupt)) __cs3_isr_fiq(void) {
	error_show(ERROR_UNDEFINED_ISR);
}
