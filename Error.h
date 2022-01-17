#ifndef ERROR_H
#define ERROR_H

#include <stdint.h>

/* Starting with 0: No Error. */

/** No error. */
#define ERROR_NO_ERROR 0U

/* Starting with 1: Interrupt. */

/** A device with unkown id caused an interrupt. */
#define ERROR_INTERRUPT_DEVICE_ID 100000U
/** A reset interrupt happened. */
#define ERROR_INTERRUPT_RESET 100001U
/** An undefined instruction interrupt happened. */
#define ERROR_INTERRUPT_UNDEF 100002U
/** A supervisor call instruction interrupt happened. */
#define ERROR_INTERRUPT_SWI 100003U
/** A prefetch abort interrupt happened. */
#define ERROR_INTERRUPT_PABORT 100004U
/** A data abort interrupt happened. */
#define ERROR_INTERRUPT_DABORT 100005U
/** A fast interrupt happened. */
#define ERROR_INTERRUPT_FIQ 100006U

/* Starting with 2: Game Logic. */

/** Restarting while the game is not finished. */
#define ERROR_LOGIC_STILL_PLAYING 200000U
/** Shooting while maximum amount of bullets is reached. */
#define ERROR_LOGIC_REACHED_BULLET_CAPACITY 200001U
/** Adding while maximum amount of tanks is reached. */
#define ERROR_LOGIC_REACHED_TANK_CAPACITY 200002U
/** Removing while there is not any tank. */
#define ERROR_LOGIC_NO_TANK_TO_REMOVE 200003U

/* Starting with 3: Graphics. */

/** Switching to a tilt that is out of bounds. */
#define ERROR_GRAPHICS_TANK_TILT_OUT_OF_BOUNDS 300000U

/** Configures and initializes the error system. Called at the start of the
 * program. */
void error_config(void);
/** Shows the given error code in the seven segment displays, and stalls the
 * program. Called when an error occurs. */
void error_show(uint32_t const error);

#endif // ERROR_H
