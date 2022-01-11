#include "Audio.h"
#define LEDs ((volatile unsigned int *) 0xFF200000)	
	
static void set_A9_IRQ_stack (void);
static void config_GIC (void);
static void config_interval_timer (void);
static void enable_A9_interrupts (void);

static volatile int tick = 0; // set to 1 in every 15 microseconds.
	
static int main(void) {
	
	set_A9_IRQ_stack (); // initialize the stack pointer for IRQ mode
	config_GIC (); // configure the general interrupt controller
	config_interval_timer (); // configure Altera interval timer to generate interrupt
	enable_A9_interrupts (); // enable interrupts in the A9 processor
	volatile int *lcd_ptr = (int *) 0xFF200020;
	
	int i=0;
	while (1){
		
			
	}

	return 0;
}

/* Initialize the banked stack pointer register for IRQ mode */
static void set_A9_IRQ_stack(void){
	int stack, mode;
	stack = 0xFFFFFFF8; // top of A9 on-chip memory, aligned to 8 bytes
	/* change processor to IRQ mode with interrupts disabled */
	mode = 0b11010010;
	asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
	/* set banked stack pointer */
	asm("mov sp, %[ps]" : : [ps] "r" (stack));
	/* go back to SVC mode before executing subroutine return! */
	mode = 0b11010011;
	asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
}


/* Turn on interrupts in the ARM processor */
static void enable_A9_interrupts(void){
	int status = 0b01010011;
	asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}


/* Configure the Generic Interrupt Controller (GIC) */
static void config_GIC(void){
	/* configure the FPGA interval timer */
	*((int *) 0xFFFED848) = 0x00000101;
	*((int *) 0xFFFED108) = 0x00000300;
	// Set Interrupt Priority Mask Register (ICCPMR). Enable interrupts of all priorities
	*((int *) 0xFFFEC104) = 0xFFFF;
	// Set CPU Interface Control Register (ICCICR). Enable signaling of interrupts
	*((int *) 0xFFFEC100) = 1; // enable = 1
	// Configure the Distributor Control Register (ICDDCR) to send pending interrupts to CPUs
	*((int *) 0xFFFED000) = 1; // enable = 1
}

/* setup the interval timer interrupts in the FPGA */
static void config_interval_timer(){
	volatile int * interval_timer_ptr = (int *) 0xFF202000; // interal timer base address
	/* set the interval timer period for scrolling the HEX displays */
	int counter = 1600000; // 1/(100 MHz)*(1500) = 15 usec
	*(interval_timer_ptr + 0x2) = (counter & 0xFFFF);
	*(interval_timer_ptr + 0x3) = (counter >> 16) & 0xFFFF;
	/* start interval timer, enable its interrupts */
	*(interval_timer_ptr + 1) = 0x7; // STOP = 0, START = 1, CONT = 1, ITO = 1
}


/* Define the IRQ exception handler */

static void __attribute__ ((interrupt)) __cs3_isr_irq (void){
	// Read the ICCIAR from the processor interface
	int int_ID = *((int *) 0xFFFEC10C);
	
	
	interval_timer_ISR ();
	
	
	// Write to the End of Interrupt Register (ICCEOIR)
	*((int *) 0xFFFEC110) = int_ID;
	return;
}

static void interval_timer_ISR (void);



// Define the remaining exception handlers */
static void __attribute__ ((interrupt)) __cs3_isr_undef (void){
	while (1);
}
static void __attribute__ ((interrupt)) __cs3_isr_swi (void){
	while (1);
}
static void __attribute__ ((interrupt)) __cs3_isr_pabort (void){
	while (1);
}
static void __attribute__ ((interrupt)) __cs3_isr_dabort (void){
	while (1);
}
static void __attribute__ ((interrupt)) __cs3_isr_fiq (void){
	while (1);
}


/******************************************************************************
* Interval timer interrupt service routine
* 
* Lights the LEDs and Plays the corresponding sound in a loop.
******************************************************************************/
static volatile int move_index = 0;
static volatile int turret_index = 0;
static volatile char byte1 = 0, byte2 = 0, byte3 = 0;

static void interval_timer_ISR( ){
	volatile int *interval_timer_ptr = (int *) 0xFF202000; // Altera timer address
	volatile int *ps2_ptr = (int *) 0xFF200100;
	volatile int *led_ptr = (int *) 0xFF200000;
	volatile int *lcd_ptr = (int *) 0xFF200020;
	volatile int *audio_ptr = (int *) 0xFF203040;
	
	int RVALID, PS_Data;
	int sound_counter = 0;
	*(interval_timer_ptr) = 0; // clear the interrupt

	PS_Data = *ps2_ptr;        // Read data info from PS2 register.
	RVALID = PS_Data & 0x8000; // extract the RVALID field
	
	
	if (RVALID){               // If there is data in keyboard FIFO buffer:
		byte1 = byte2;         
		byte2 = byte3;         // Store the most recent 3 data info
		byte3 = PS_Data & 0xFF;
		if (byte2 == 0xF0)     // if the button is released
			*led_ptr = 0;      
		else if(byte3 == 0x1c){   // if the button is not released and A is pressed
			*led_ptr = 2;
			
			while(1){
				int fifospace = *(audio_ptr + 1);
				int wsrc = (fifospace & 0x00FF0000) >> 16;
				if (wsrc > 96) {
					while (wsrc > 32){
						*(audio_ptr+2) = move_data[move_index];
						*(audio_ptr+3) = move_data[move_index];
						move_index += 3;
						if(move_index >= MOVE_BUF_SIZE)
							move_index = 0;
						++sound_counter;
						if (sound_counter == 240)
							break;
						fifospace = *(audio_ptr + 1);
						wsrc = (fifospace & 0x00FF0000) >> 16;
					}
				}
				if (sound_counter == 240)
							break;
			}
			
		}
		else if (byte3 == 0x23){  // if the button is not released and D is pressed
			*led_ptr = 1;
			
			while(1){
				int fifospace = *(audio_ptr + 1);
				int wsrc = (fifospace & 0x00FF0000) >> 16;
				if (wsrc > 96) {
					while (wsrc > 32){
						*(audio_ptr+2) = move_data[move_index];
						*(audio_ptr+3) = move_data[move_index];
						move_index += 3;
						if(move_index >= MOVE_BUF_SIZE)
							move_index = 0;
						++sound_counter;
						if (sound_counter == 240)
							break;
						fifospace = *(audio_ptr + 1);
						wsrc = (fifospace & 0x00FF0000) >> 16;
					}
				}
				if (sound_counter == 240)
							break;
			}
			
		}
		else if (byte3 == 0x1D){  // if the button is not released and W is pressed
			*led_ptr = 8;
			
			while(1){
				int fifospace = *(audio_ptr + 1);
				int wsrc = (fifospace & 0x00FF0000) >> 16;
				if (wsrc > 96) {
					while (wsrc > 32){
						*(audio_ptr+2) = turret_data[turret_index];
						*(audio_ptr+3) = turret_data[turret_index];
						turret_index += 3;
						if(turret_index >= TURRET_BUF_SIZE)
							turret_index = 0;
						++sound_counter;
						if (sound_counter == 240)
							break;
						fifospace = *(audio_ptr + 1);
						wsrc = (fifospace & 0x00FF0000) >> 16;
					}
				}
				if (sound_counter == 240)
							break;
			}
	}
		else if (byte3 == 0x1B){  // if the button is not released and S is pressed
			*led_ptr = 4;
			
			
			while(1){
				int fifospace = *(audio_ptr + 1);
				int wsrc = (fifospace & 0x00FF0000) >> 16;
				if (wsrc > 96) {
					while (wsrc > 32){
						*(audio_ptr+2) = turret_data[turret_index];
						*(audio_ptr+3) = turret_data[turret_index];
						turret_index += 3;
						if(turret_index >= TURRET_BUF_SIZE)
							turret_index = 0;
						++sound_counter;
						if (sound_counter == 240)
							break;
						fifospace = *(audio_ptr + 1);
						wsrc = (fifospace & 0x00FF0000) >> 16;
					}
				}
				if (sound_counter == 240)
							break;
			}
			
		}	
	}
return;
}
