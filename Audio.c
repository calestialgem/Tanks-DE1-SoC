#include "Audio.h"
#include <stdio.h>
#include <stdbool.h>


void audio_config(void){
    volatile int *audio_ptr = (int *) 0xFF203040;
    *audio_ptr |= 0b1000;
    *audio_ptr &= ~0b1111;
}

void audio_play(int input_buf_size, int* input_array){
    volatile int *audio_ptr = (int *) 0xFF203040;
    if(input_buf_size != AUDIO_BUF_SIZE)
        buffer_index = 0;
    AUDIO_BUF_SIZE = input_buf_size;
    audio_data = input_array;
    *audio_ptr = *audio_ptr | 0b10;
}

void audio_stop(void){
    volatile int *audio_ptr = (int *) 0xFF203040;
    *audio_ptr = *audio_ptr & ~0b10;
    buffer_index = 0;
}

void audio_ISR (void){                        
    volatile int *audio_ptr = (int *) 0xFF203040;

    int fifospace = *(audio_ptr + 1);
	int wsrc = (fifospace & 0x00FF0000) >> 16;
		while ((wsrc >= 32) && (buffer_index < AUDIO_BUF_SIZE)){
			*(audio_ptr+2) = audio_data[buffer_index];
			*(audio_ptr+3) = audio_data[buffer_index];
			buffer_index += 3;
			fifospace = *(audio_ptr + 1);
			wsrc = (fifospace & 0x00FF0000) >> 16;
		}
    if(buffer_index >= AUDIO_BUF_SIZE){
        *audio_ptr = *audio_ptr & ~0b10;
		buffer_index = 0;
	}
}
