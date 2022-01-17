#include "Keyboard.h"

#include "MemoryMap.h"

#define KEYBOARD_KEY_TANK_LEFT 0x1C    // A
#define KEYBOARD_KEY_TANK_RIGHT 0x23   // D
#define KEYBOARD_KEY_BARREL_LEFT 0x15  // Q
#define KEYBOARD_KEY_BARREL_RIGHT 0x24 // E
#define KEYBOARD_KEY_POWER_UP 0x1D     // W
#define KEYBOARD_KEY_POWER_DOWN 0x1B   // S
#define KEYBOARD_KEY_SHOOT 0x29	       // SPACE
#define KEYBOARD_KEY_PARACHUTE 0x4D    // P
#define KEYBOARD_KEY_REPAIR 0x2D       // R
#define KEYBOARD_KEY_TELEPORT 0x2C     // T
#define KEYBOARD_KEY_SHIELD 0x2B       // F
#define KEYBOARD_KEY_RESET 0x76        // ESC
#define KEYBOARD_BREAK_CODE 0xF0

/** Holds the key data. */
volatile static struct {
	bool tankLeft;
	bool tankRight;
	bool barrelLeft;
	bool barrelRight;
	bool powerUp;
	bool powerDown;
	bool shootCommand;
	bool shoot;
	bool resetCommand;
	bool reset;
} keyboard;
/** The previous byte in the buffer. Says if it is a press or a release. */
static volatile uint8_t previous_data;

void keyboard_config(void) {
	/* Start with a zero value. */
	previous_data = 0;
}
void keyboard_update(void) {
	Register32 PS2 = (Register32)0xFF200100;
	uint32_t data = *PS2;
	uint32_t const RVALID = data & 0x8000;
	if (RVALID) {
		bool keyState = previous_data != KEYBOARD_BREAK_CODE;
		previous_data = data & 0xFF;
		switch (previous_data) {
		case KEYBOARD_KEY_TANK_LEFT:
			keyboard.tankLeft = keyState;
			break;
		case KEYBOARD_KEY_TANK_RIGHT:
			keyboard.tankRight = keyState;
			break;
		case KEYBOARD_KEY_BARREL_LEFT:
			keyboard.barrelLeft = keyState;
			break;
		case KEYBOARD_KEY_BARREL_RIGHT:
			keyboard.barrelRight = keyState;
			break;
		case KEYBOARD_KEY_POWER_UP:
			keyboard.powerUp = keyState;
			break;
		case KEYBOARD_KEY_POWER_DOWN:
			keyboard.powerDown = keyState;
			break;
		case KEYBOARD_KEY_SHOOT:
			keyboard.shoot = !keyboard.shootCommand && keyState;
			keyboard.shootCommand = keyState;
			break;
		case KEYBOARD_KEY_RESET:
			keyboard.reset = !keyboard.resetCommand && keyState;
			keyboard.resetCommand = keyState;
			break;
		default:
			break;
		}
	}
}
bool keyboard_tank_left(void) {
	return keyboard.tankLeft;
}
bool keyboard_tank_right(void) {
	return keyboard.tankRight;
}
bool keyboard_barrel_left(void) {
	return keyboard.barrelLeft;
}
bool keyboard_barrel_right(void) {
	return keyboard.barrelRight;
}
bool keyboard_power_up(void) {
	return keyboard.powerUp;
}
bool keyboard_power_down(void) {
	return keyboard.powerDown;
}
bool keyboard_shoot(void) {
	return keyboard.shoot;
}
bool keyboard_reset(void) {
	return keyboard.reset;
}