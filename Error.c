#include "Error.h"

#include <stdbool.h>

void error_config(void) {
	// Clear the current error.
	error_show(ERROR_NO_ERROR);
}
void error_show(uint32_t const error) {
	// Show the error in the LEDs.
	volatile uint32_t *const LEDs = 0xFF200000;
	*LEDs = error;
	// Stall the execution when a problem exists.
	while (error)
		;
}
