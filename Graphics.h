#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

#define GRAPHICS_WIDTH 100
#define GRAPHICS_HEIGHT 200

/** Configures and initializes the graphics system. Called at the start of the
 * program. */
void graphics_config(void);
#ifdef GRAPHICS_INTERRUPT_ID
/** Handles the graphics interrupts. Called by the interrupt handler. */
void graphics_isr(void);
#endif // GRAPHICS_INTERRUPT_ID
/** Swaps the frame buffers, and clears the back buffer pixels. Called by the
 * game logic. */
void graphics_render(void);

#endif // GRAPHICS_H
