#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>

// /** Audio's interrupt id. */
// #define AUDIO_INTERRUPT_ID 0

/** Configures and initializes the audio system. Called at the start of the
 * program. */
void audio_config(void);
#ifdef AUDIO_INTERRUPT_ID
/** Handles the audio interrupts. Called by the interrupt handler. */
void audio_isr(void);
#endif // AUDIO_INTERRUPT_ID
/** Plays the sound that corresponds to the given id. Stops the currently
 * running sound if there is one. Called by the game logic. */
void audio_play(uint8_t sound);

#endif // AUDIO_H
