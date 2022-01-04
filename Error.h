#ifndef ERROR_H
#define ERROR_H

#include <stdint.h>

// Individual LEDs and their meaning.

/** No LED is on for no error. */
#define ERROR_NO_ERROR ((uint32_t)(0))
/** First LED for errors in game logic. */
#define ERROR_LOGIC ((uint32_t)(1 << 0))
/** Second LED for errors in audio system. */
#define ERROR_AUDIO ((uint32_t)(1 << 1))
/** Third LED for errors in keyboard system. */
#define ERROR_KEYBOARD ((uint32_t)(1 << 2))
/** Fourth LED for errors in graphics system. */
#define ERROR_GRAPHICS ((uint32_t)(1 << 3))
/** Fifth LED for errors about an invalid id. */
#define ERROR_INVALID_ID ((uint32_t)(1 << 4))
/** Sixth LED for errors in an interrupt service routine. */
#define ERROR_INTERRUPT ((uint32_t)(1 << 5))
/** Seventh LED for undefined errors. */
#define ERROR_UNDEFINED ((uint32_t)(1 << 6))

// Combination of LEDs for an invalid id.

/** Error from an invalid sound id. */
#define ERROR_INVALID_SOUND (ERROR_AUDIO | ERROR_INVALID_ID)
/** Error from an invalid key id. */
#define ERROR_INVALID_KEY (ERROR_KEYBOARD | ERROR_INVALID_ID)
/** Error from an invalid image id. */
#define ERROR_INVALID_IMAGE (ERROR_GRAPHICS | ERROR_INVALID_ID)
/** Error from an invalid interrupt id. */
#define ERROR_INVALID_INTERRUPT (ERROR_INTERRUPT | ERROR_INVALID_ID)

// Combination of LEDs for an interrupt service routine.

/** Error from the logic interrupt service routine. */
#define ERROR_LOGIC_ISR (ERROR_LOGIC | ERROR_INTERRUPT)
/** Error from the audio interrupt service routine. */
#define ERROR_AUDIO_ISR (ERROR_AUDIO | ERROR_INTERRUPT)
/** Error from the keyboard interrupt service routine. */
#define ERROR_KEYBOARD_ISR (ERROR_KEYBOARD | ERROR_INTERRUPT)
/** Error from the graphics interrupt service routine. */
#define ERROR_GRAPHICS_ISR (ERROR_GRAPHICS | ERROR_INTERRUPT)
/** Error from an undefined interrupt service routine. */
#define ERROR_UNDEFINED_ISR (ERROR_UNDEFINED | ERROR_INTERRUPT)

/** Initializes the error system. Called at the start of the program. */
void error_init(void);
/** Shows the given error code in the LEDs and stalls the program. Called when
 * an error occurs. */
void error_show(uint32_t error);

#endif // ERROR_H
