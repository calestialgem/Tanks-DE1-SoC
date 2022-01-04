#include "Interrupt.h"

#include "Audio.h"
#include "Error.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Timer.h"

// Some portion of the fallowing code is taken from the manual.

static void disable_interrupts(void) {
	uint32_t const status = 0b11010011;
	asm("msr cpsr, %[ps]" : : [ps] "r"(status));
}
static void enable_interrupts(void) {
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
	uint32_t const enableBit = N & 0x1F;
	uint32_t const enableMask = 0x1 << enableBit;
	volatile uint32_t *const ICDISER = 0xFFFED100 + ICDISERn;
	*ICDISER |= enableMask;
	uint32_t const ICDIPTRn = (N & 0xFFFFFFFC);
	uint32_t const index = N & 0x3;
	uint8_t const targetCPU = 1;
	volatile uint8_t *const ICDIPTR = 0xFFFED800 + ICDIPTRn + index;
	*ICDIPTR = targetCPU;
}
static void config_gic(void) {
#ifdef AUDIO_INTERRUPT_ID
	config_interrupt(AUDIO_INTERRUPT_ID);
#endif // AUDIO_INTERRUPT_ID
#ifdef KEYBOARD_INTERRUPT_ID
	config_interrupt(KEYBOARD_INTERRUPT_ID);
#endif // KEYBOARD_INTERRUPT_ID
#ifdef GRAPHICS_INTERRUPT_ID
	config_interrupt(GRAPHICS_INTERRUPT_ID);
#endif // GRAPHICS_INTERRUPT_ID
#ifdef TIMER_INTERRUPT_ID
	config_interrupt(TIMER_INTERRUPT_ID);
#endif // TIMER_INTERRUPT_ID
	volatile uint32_t *const ICCPMR = 0xFFFEC104;
	uint16_t const priorityMask = 0xFFFF;
	*ICCPMR = priorityMask;
	volatile uint32_t *const ICCICR = 0xFFFEC100;
	*ICCICR = 1;
	volatile uint32_t *const ICDDCR = 0xFFFED000;
	*ICDDCR = 1;
}
void interrupt_config(void) {
	disable_interrupts();
	set_interrupt_stack();
	config_gic();
	enable_interrupts();
}
/** Handles normal interrupts. */
void __attribute__((interrupt)) __cs3_isr_irq(void) {
	// Read the ICCIAR from the CPU Interface in the GIC.
	volatile uint32_t *const ICCIAR = 0xFFFEC10C;
	uint32_t const id = *ICCIAR;
	switch (id) {
#ifdef AUDIO_INTERRUPT_ID
	case AUDIO_INTERRUPT_ID:
		audio_isr();
		break;
#endif // AUDIO_INTERRUPT_ID
#ifdef KEYBOARD_INTERRUPT_ID
	case KEYBOARD_INTERRUPT_ID:
		keyboard_isr();
		break;
#endif // KEYBOARD_INTERRUPT_ID
#ifdef GRAPHICS_INTERRUPT_ID
	case GRAPHICS_INTERRUPT_ID:
		graphics_isr();
		break;
#endif // GRAPHICS_INTERRUPT_ID
#ifdef TIMER_INTERRUPT_ID
	case TIMER_INTERRUPT_ID:
		timer_isr();
		break;
#endif // TIMER_INTERRUPT_ID
	default:
		error_show(ERROR_UNDEFINED_ISR);
	};
	// Write to the end of interrupt register (ICCEOIR).
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
