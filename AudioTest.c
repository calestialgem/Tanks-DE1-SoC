static volatile int AUDIO_BUF_SIZE;
static volatile int* audio_data;
static volatile int buffer_index = 0;
	
static void set_A9_IRQ_stack (void);
static void config_GIC (void);
static void config_interval_timer (void);
static void audio_clear (void);
static void enable_A9_interrupts (void);
static void play_audio(int input_buf_size, int* input_array);

static int main(void) {
	
	set_A9_IRQ_stack (); // initialize the stack pointer for IRQ mode
	config_GIC (); // configure the general interrupt controller
	config_interval_timer (); // configure Altera interval timer to generate interrupt
    audio_clear();
	enable_A9_interrupts (); // enable interrupts in the A9 processor
	volatile int *lcd_ptr = (int *) 0xFF200020;
	volatile int *switch_ptr = (int *) 0xff200040;
	
	int i=0;
	while (1){      // Initiate audio interrupt only when one of the switches (1&2) is turned on
		if (*switch_ptr == 1)
			play_audio(MOVE_BUF_SIZE, move_data);
		else if (*switch_ptr == 2)
			play_audio(TURRET_BUF_SIZE, turret_data);
	}

	return 0;
}

static void play_audio(int input_buf_size, int* input_array){
    volatile int *audio_ptr = (int *) 0xFF203040;
    AUDIO_BUF_SIZE = input_buf_size;
    audio_data = input_array;
    *audio_ptr = *audio_ptr | 0b10;
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
	/* configure the FPGA interval timer and Audio */
	*((int *) 0xFFFED848) = 0x00000001;
	*((int *) 0xFFFED84C) = 0x00010000;
	*((int *) 0xFFFED108) = 0x00004100;
	/* configure the FPGA timer and Audio Timer Priorities */
	*((int *) 0xFFFED448) = 0x00000001;
	*((int *) 0xFFFED44C) = 0x00000000;
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
	int counter = 1600000; // 1/(100 MHz)*(1600000) = 16 msec
	*(interval_timer_ptr + 0x2) = (counter & 0xFFFF);
	*(interval_timer_ptr + 0x3) = (counter >> 16) & 0xFFFF;
	/* start interval timer, enable its interrupts */
	*(interval_timer_ptr + 1) = 0x7; // STOP = 0, START = 1, CONT = 1, ITO = 1
}

static void audio_clear(){
    volatile int *audio_ptr = (int *) 0xFF203040;

    *audio_ptr |= 0b1000;
    *audio_ptr &= ~0b1111;
}

static void interval_timer_ISR (void);
static void audio_ISR (void);

/* Define the IRQ exception handler */

static void __attribute__ ((interrupt)) __cs3_isr_irq (void){
	// Read the ICCIAR from the processor interface
	int int_ID = *((int *) 0xFFFEC10C);
	
	if(int_ID == 72)
		interval_timer_ISR ();
	else if(int_ID == 78)
		audio_ISR();
	
	// Write to the End of Interrupt Register (ICCEOIR)
	*((int *) 0xFFFEC110) = int_ID;
	return;
}





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

static volatile char byte1 = 0, byte2 = 0, byte3 = 0;
static void interval_timer_ISR( ){
	volatile int *interval_timer_ptr = (int *) 0xFF202000; // Altera timer address
	volatile int *ps2_ptr = (int *) 0xFF200100;
	volatile int *led_ptr = (int *) 0xFF200000;
	volatile int *lcd_ptr = (int *) 0xFF200020;

	
	int RVALID, PS_Data;

	*(interval_timer_ptr) = 0; // clear the interrupt

	PS_Data = *ps2_ptr;        // Read data info from PS2 register.
	RVALID = PS_Data & 0x8000; // extract the RVALID field
	
	
	if (RVALID){               // If there is data in keyboard FIFO buffer:
		byte1 = byte2;         
		byte2 = byte3;         // Store the most recent 3 data info
		byte3 = PS_Data & 0xFF;
		if (byte2 == 0xF0)     // if the button is released
			*led_ptr = 0;      
		else if (byte3 == 0x23){  // if the button is not released and D is pressed
			*led_ptr = 1;
			play_audio(MOVE_BUF_SIZE, move_data);
		}
		else if (byte3 == 0x1C){  // if the button is not released and A is pressed
			*led_ptr = 2;
			play_audio(MOVE_BUF_SIZE, move_data);
		}
		else if (byte3 == 0x1D){  // if the button is not released and W is pressed
			*led_ptr = 8;
			play_audio(TURRET_BUF_SIZE, turret_data);
		}
		else if (byte3 == 0x1b){  // if the button is not released and S is pressed
			*led_ptr = 4;
			play_audio(TURRET_BUF_SIZE, turret_data);
		}	
			
	}
return;
}

static void audio_ISR (void){                        
    volatile int *audio_ptr = (int *) 0xFF203040;

    int fifospace = *(audio_ptr + 1);
	int wsrc = (fifospace & 0x00FF0000) >> 16;
		while ((wsrc >= 32) && (buffer_index < AUDIO_BUF_SIZE)){
			*(audio_ptr+2) = audio_data[buffer_index];
			*(audio_ptr+3) = audio_data[buffer_index];
			buffer_index += 3;
			fifospace = *(audio_ptr + 1);
			wsrc = (fifospace & 0x00FF0000) >> 16;
		}
    if(buffer_index >= AUDIO_BUF_SIZE){
        *audio_ptr = *audio_ptr & ~0b10;
		buffer_index = 0;
	}
}

	