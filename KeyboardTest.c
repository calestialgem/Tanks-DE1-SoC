/******************************************************************************
* Interval timer interrupt service routine
* Interrupt'a her girişinde tuşları kontrol ediyor.
* Basılı olan tuşa göre oyunda istenen hareket yapılıyor
******************************************************************************/
static volatile int move_index = 0;
static volatile int turret_index = 0;
static volatile char byte1 = 0, byte2 = 0, byte3 = 0;

static void interval_timer_ISR( ){
	volatile int *interval_timer_ptr = (int *) 0xFF202000; // Altera timer address
	volatile int *ps2_ptr = (int *) 0xFF200100;	
	*(interval_timer_ptr) = 0; // clear the interrupt
	
	int RVALID, PS_Data;
	PS_Data = *ps2_ptr;        // Read data info from PS2 register.
	RVALID = PS_Data & 0x8000; // extract the RVALID field
	
	if (RVALID){               // If there is data in keyboard FIFO buffer:
		byte1 = byte2;         
		byte2 = byte3;         // Store the most recent 3 data info
		byte3 = PS_Data & 0xFF;
		if (byte2 == 0xF0){    // Button release

		}           

		else if (byte3 == 0x1D){  // pressed W
		// increase turret angle
		}

		else if (byte3 == 0x1B){  // pressed S
		// decrease turret angle
		}	

		else if(byte3 == 0x1c){   // pressed A
		// move leftwards
		}

		else if (byte3 == 0x23){  // pressed D
		// move rightwards	
		}

		else if (byte3 == 0x15){  // pressed Q
		// increase power
		}

		else if (byte3 == 0x24){  // pressed E
		// decrease power
		}

		else if (byte3 == 0x29){  // pressed SPACE
		// shoot
		}
		
	}
return;
}
