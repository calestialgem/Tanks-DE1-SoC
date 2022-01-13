#include "Interrupt.h"

#include "Audio.h"
#include "Error.h"
#include "Timer.h"

#include <stdint.h>

static inline void set_stack(void) {
	int stack, mode;
	stack = 0xFFFFFFFF - 7; // top of A9 onchip memory, aligned to 8 bytes
	/* change processor to IRQ mode with interrupts disabled */
	mode = 0b11010010;
	asm("msr cpsr, %[ps]" : : [ps] "r"(mode));
	/* set banked stack pointer */
	asm("mov sp, %[ps]" : : [ps] "r"(stack));
	/* go back to SVC mode before executing subroutine return! */
	mode = 0b11010011;
	asm("msr cpsr, %[ps]" : : [ps] "r"(mode));
}
void interrupt_config(void) {
	interrupt_disable();
	set_stack();
	// configure the FPGA interval timer and Audio
	*((int *)0xFFFED848) = 0x00000001;
	*((int *)0xFFFED84C) = 0x00010000;
	*((int *)0xFFFED108) = 0x00004100;
	// configure the FPGA timer and Audio Timer Priorities
	*((int *)0xFFFED448) = 0x00000001;
	*((int *)0xFFFED44C) = 0x00000000;
	// Set Interrupt Priority Mask Register (ICCPMR). Enable interrupts of
	// all priorities
	*((int *)0xFFFEC104) = 0xFFFF;
	// Set CPU Interface Control Register (ICCICR). Enable signaling of
	// interrupts
	*((int *)0xFFFEC100) = 1; // enable = 1
	// Configure the Distributor Control Register (ICDDCR) to send pending
	// interrupts to CPUs
	*((int *)0xFFFED000) = 1; // enable = 1
}
void interrupt_disable(void) {
	int status = 0b11010011;
	asm("msr cpsr, %[ps]" : : [ps] "r"(status));
}
void interrupt_enable(void) {
	int status = 0b01010011;
	asm("msr cpsr, %[ps]" : : [ps] "r"(status));
}
/** Handles normal interrupts. */
void __attribute__((interrupt)) __cs3_isr_irq(void) {
	// Read the ICCIAR from the CPU Interface in the GIC
	int interrupt_ID = *((int *)0xFFFEC10C);
	switch (interrupt_ID) {
	case TIMER_INTERRUPT_ID:
		timer_interrupt();
		break;
	case AUDIO_INTERRUPT_ID:
		audio_ISR();
		break;
	default:
		error_show(ERROR_INTERRUPT_DEVICE_ID);
	};
	// Write to the End of Interrupt Register (ICCEOIR)
	*((int *)0xFFFEC110) = interrupt_ID;
}
/** Handles resets. */
void __attribute__((interrupt)) __cs3_reset(void) {
	error_show(ERROR_INTERRUPT_RESET);
}
/** Handles undefined instructions. */
void __attribute__((interrupt)) __cs3_isr_undef(void) {
	error_show(ERROR_INTERRUPT_UNDEF);
}
/** Handles supervisor call instructions. */
void __attribute__((interrupt)) __cs3_isr_swi(void) {
	error_show(ERROR_INTERRUPT_SWI);
}
/** Handles prefetch aborts. */
void __attribute__((interrupt)) __cs3_isr_pabort(void) {
	error_show(ERROR_INTERRUPT_PABORT);
}
/** Handles data aborts. */
void __attribute__((interrupt)) __cs3_isr_dabort(void) {
	error_show(ERROR_INTERRUPT_DABORT);
}
/** Handles fast interrupts. */
void __attribute__((interrupt)) __cs3_isr_fiq(void) {
	error_show(ERROR_INTERRUPT_FIQ);
}
