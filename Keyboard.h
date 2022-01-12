#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>

/** Configures and initializes the keyboard system. Called at the start of the
 * program. */
void keyboard_config(void);
/** Updates the keyboard, called every frame. */
void keyboard_update(void);
/** Returns true if the key is pressured to move the tank left. */
bool keyboard_tank_left(void);
/** Returns true if the key is pressured to move the tank right. */
bool keyboard_tank_right(void);
/** Returns true if the key is pressured to turn the barrel left. */
bool keyboard_barrel_left(void);
/** Returns true if the key is pressured to turn the barrel right. */
bool keyboard_barrel_right(void);
/** Returns true if the key is pressured to turn the power up. */
bool keyboard_power_up(void);
/** Returns true if the key is pressured to turn the power down. */
bool keyboard_power_down(void);
/** Returns true if the key is pressured to shoot the bullet. */
bool keyboard_shoot(void);

#endif // KEYBOARD_H
