#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

#define GRAPHICS_WIDTH 500
#define GRAPHICS_HEIGHT 2000

/** Configures and initializes the graphics system. Called at the start of the
 * program. */
void graphics_config(void);
#ifdef GRAPHICS_INTERRUPT_ID
/** Handles the graphics interrupts. Called by the interrupt handler. */
void graphics_isr(void);
#endif // GRAPHICS_INTERRUPT_ID
/** Draws the image that corresponds to the given id at the given screen
 * positions which has the origin at the top-left corner of the screen in
 * pixels. Called by the game logic. */
void graphics_play(uint8_t image, uint16_t xpos, uint16_t ypos);
/** Swaps the frame buffers, and clears the back buffer pixels. Called by the
 * game logic. */
void graphics_render(void);

#endif // GRAPHICS_H
