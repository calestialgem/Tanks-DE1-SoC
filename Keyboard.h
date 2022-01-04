#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

// /** Keyboard's interrupt id. */
// #define KEYBOARD_INTERRUPT_ID 0

/** Configures and initializes the keyboard system. Called at the start of the
 * program. */
void keyboard_config(void);
#ifdef KEYBOARD_INTERRUPT_ID
/** Handles the keyboard interrupts. Called by the interrupt handler. */
void keyboard_isr(void);
#endif // KEYBOARD_INTERRUPT_ID
/** Returns wether the key that corresponds to the given id is currently
 * pressed down. Called by the game logic. */
bool keyboard_down(uint8_t key);

#endif // KEYBOARD_H
