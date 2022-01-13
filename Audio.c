#include "Audio.h"

#include "AudioData.h"

#include <stdbool.h>
#include <stdio.h>

/* Variables used inside the audio_play() func. */
volatile int AUDIO_BUF_SIZE;
volatile int *audio_data;
volatile int buffer_index = 0;

void audio_clear(void) {
	volatile int *audio_ptr = (int *)0xFF203040;
	*audio_ptr |= 0b1000;
	*audio_ptr &= ~0b1111;
}

static inline void audio_play(int input_buf_size, int *input_array) {
	volatile int *audio_ptr = (int *)0xFF203040;
	if (input_buf_size != AUDIO_BUF_SIZE)
		buffer_index = 0;
	AUDIO_BUF_SIZE = input_buf_size;
	audio_data = input_array;
	*audio_ptr = *audio_ptr | 0b10;
}

void audio_play_tank_death(void) {
	audio_play(EXPLOSION_BUF_SIZE, explosion_data);
}
void audio_play_tank_movement(void) {
	audio_play(MOVE_BUF_SIZE, move_data);
}
void audio_play_shooting(void) {
	audio_play(SHOT_BUF_SIZE, shot_data);
}
void audio_play_explosion(void) {
	audio_play(SHOTLAND_BUF_SIZE, shotland_data);
}
void audio_play_barrel_movement(void) {
	audio_play(TURRET_BUF_SIZE, turret_data);
}

void audio_stop(void) {
	volatile int *audio_ptr = (int *)0xFF203040;
	*audio_ptr = *audio_ptr & ~0b10;
	buffer_index = 0;
}

void audio_ISR(void) {
	volatile int *audio_ptr = (int *)0xFF203040;

	int fifospace = *(audio_ptr + 1);
	int wsrc = (fifospace & 0x00FF0000) >> 16;
	while ((wsrc >= 32) && (buffer_index < AUDIO_BUF_SIZE)) {
		*(audio_ptr + 2) = audio_data[buffer_index];
		*(audio_ptr + 3) = audio_data[buffer_index];
		buffer_index += 3;
		fifospace = *(audio_ptr + 1);
		wsrc = (fifospace & 0x00FF0000) >> 16;
	}
	if (buffer_index >= AUDIO_BUF_SIZE) {
		*audio_ptr = *audio_ptr & ~0b10;
		buffer_index = 0;
	}
}
