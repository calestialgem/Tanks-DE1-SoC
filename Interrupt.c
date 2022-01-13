#include "Interrupt.h"

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
void config_interrupt(int N) {
	int reg_offset, index, value, address;
	/* Configure the Interrupt Set-Enable Registers (ICDISERn).
	 * reg_offset = (integer_div(N / 32) * 4
	 * value = 1 << (N mod 32) */
	reg_offset = (N >> 3) & 0xFFFFFFFC;
	index = N & 0x1F;
	value = 0x1 << index;
	address = 0xFFFED100 + reg_offset;
	/* Now that we know the register address and value, set the appropriate
	 * bit */
	*(int *)address |= value;
	/* Configure the Interrupt Processor Targets Register (ICDIPTRn)
	 * reg_offset = integer_div(N / 4) * 4
	 * index = N mod 4 */
	reg_offset = (N & 0xFFFFFFFC);
	index = N & 0x3;
	address = 0xFFFED800 + reg_offset + index;
#define CPU_target ((char)1)
	/* Now that we know the register address and value, write to (only) the
	 * appropriate byte */
	*(char *)address = CPU_target;
}
void interrupt_config(void) {
	interrupt_disable();
	set_stack();
	config_interrupt(TIMER_INTERRUPT_ID);
	// Set Interrupt Priority Mask Register (ICCPMR). Enable interrupts of
	// all priorities
	*((int *)0xFFFEC104) = 0xFFFF;
	// Set CPU Interface Control Register (ICCICR). Enable signaling of
	// interrupts
	*((int *)0xFFFEC100) = 1;
	// Configure the Distributor Control Register (ICDDCR) to send pending
	// interrupts to CPUs
	*((int *)0xFFFED000) = 1;
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
