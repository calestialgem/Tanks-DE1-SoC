#include "Error.h"

#include "MemoryMap.h"

void error_config(void) {
	// Clear the current error.
	error_show(ERROR_NO_ERROR);
}
/** Returns the signal to hex to show the given digit. */
static inline uint8_t hex_signal(uint8_t const digit) {
	switch (digit) {
	case 0:
		return 0b0111111;
	case 1:
		return 0b0000110;
	case 2:
		return 0b1011011;
	case 3:
		return 0b1001111;
	case 4:
		return 0b1100110;
	case 5:
		return 0b1101101;
	case 6:
		return 0b1111101;
	case 7:
		return 0b0000111;
	case 8:
		return 0b1111111;
	case 9:
		return 0b1101111;
	default:
		return 0b1001001;
	}
}
static inline void hexes_write(
	uint8_t const code, Register8 hex0, Register8 hex1) {
	*hex0 = hex_signal((code / (uint8_t)1e0) % 10);
	*hex1 = hex_signal((code / (uint8_t)1e1) % 10);
}
void error_show(uint8_t const error) {
	Register8 sevenSegment0 = (Register8)0xFF200020;
	Register8 sevenSegment1 = (Register8)0xFF200021;
	// Show the error in the seven segment displays.
	hexes_write(error, sevenSegment0, sevenSegment1);
	// Stall the execution when a problem exists.
	while (error)
		;
}
void error_stage(uint8_t const debug) {
	Register8 sevenSegment2 = (Register8)0xFF200022;
	Register8 sevenSegment3 = (Register8)0xFF200023;
	// Show the stage in the seven segment displays.
	hexes_write(debug, sevenSegment2, sevenSegment3);
}
