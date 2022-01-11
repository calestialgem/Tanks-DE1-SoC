#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>

/** Configures and initializes the audio system. Called at the start of the
 * program. */
void audio_config(void);
/** Plays the sound that corresponds to the given id. Stops the currently
 * running sound if there is one. Called by the game logic. */
void audio_play(uint8_t sound);

#endif // AUDIO_H
