#ifndef ERROR_H
#define ERROR_H

#include <stdint.h>

/* Starting with 0: No Error. */

/** No error. */
#define ERROR_NO_ERROR 0U

/* Starting with 1: Interrupt. */

/** A device with unkown id caused an interrupt. */
#define ERROR_INTERRUPT_DEVICE_ID 10U
/** A reset interrupt happened. */
#define ERROR_INTERRUPT_RESET 11U
/** An undefined instruction interrupt happened. */
#define ERROR_INTERRUPT_UNDEF 12U
/** A supervisor call instruction interrupt happened. */
#define ERROR_INTERRUPT_SWI 13U
/** A prefetch abort interrupt happened. */
#define ERROR_INTERRUPT_PABORT 14U
/** A data abort interrupt happened. */
#define ERROR_INTERRUPT_DABORT 15U
/** A fast interrupt happened. */
#define ERROR_INTERRUPT_FIQ 16U

/* Starting with 2: Game Logic. */
/** Restarting while the game is not finished. */
#define ERROR_LOGIC_STILL_PLAYING 20U
/** Shooting while maximum amount of bullets is reached. */
#define ERROR_LOGIC_REACHED_BULLET_CAPACITY 21U
/** Adding while maximum amount of tanks is reached. */
#define ERROR_LOGIC_REACHED_TANK_CAPACITY 22U
/** Removing while there is not any tank. */
#define ERROR_LOGIC_NO_TANK_TO_REMOVE 23U

#define DEBUG_START 0U

/** Configures and initializes the error system. Called at the start of the
 * program. */
void error_config(void);
/** Shows the given error code in the seven segment displays, and stalls the
 * program. Called when an error occurs. */
void error_show(uint8_t const error);
/** Shows the given debug code in the seven segment displays. Used to show the
 * stage of the program. */
void error_stage(uint8_t const debug);

#endif // ERROR_H
