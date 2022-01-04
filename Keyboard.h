#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

/** Configures and initializes the keyboard system. Called at the start of the
 * program. */
void keyboard_config(void);
/** Handles the keyboard interrupts. Called by the interrupt handler. */
void keyboard_isr(void);
/** Returns wether the key that corresponds to the given id is currently
 * pressed down. Called by the game logic. */
bool keyboard_down(uint8_t key);

#endif // KEYBOARD_H
