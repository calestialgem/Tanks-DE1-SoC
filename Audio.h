#ifndef AUDIO_H
#define AUDIO_H

/* Audio Interrupt's ID */
#define AUDIO_INTERRUPT_ID 78

void audio_play_tank_death(void);
void audio_play_tank_movement(void);
void audio_play_shooting(void);
void audio_play_explosion(void);
void audio_play_barrel_movement(void);

/* Stops playing the sound by disabling the interrupt bit in the audio register.
Should be put into the "Is key released?" check. */
void audio_stop(void);

/* Takes which sound to be played from "audio_play" and plays it until the whole
sound is completed. Disables the interrupt bit in the audio register once the
whole sound is played. */
void audio_ISR(void);

#endif // AUDIO_H
