#include "Timer.h"

#include "Game.h"
#include "Keyboard.h"
#include "MemoryMap.h"

#include <stdlib.h>

/** Divides the 32-bit count to 16-bit halves. */
typedef union {
	uint32_t total;
	struct {
		uint16_t low;
		uint16_t high;
	};
} Counter;

void timer_config(void) {
	Register16 control = (Register16)0xFF202004;
	*control = 0;
	Counter const counter = {.total = 100.0e6F / TIMER_FREQUENCY};
	Register16 startLow = (Register16)0xFF202008;
	*startLow = counter.low;
	Register16 startHigh = (Register16)0xFF20200C;
	*startHigh = counter.high;
	*control = 0b0111;
}
void timer_interrupt(void) {
	keyboard_update();
	game_update();
	Register16 status = (Register16)0xFF202000;
	*status = 0;
}
