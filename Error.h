#ifndef ERROR_H
#define ERROR_H

#include <stdint.h>

/* Starting with 0: No Error. */

/** No error. */
#define ERROR_NO_ERROR 0U

/* Starting with 1: Interrupt. */

/** A device with unkown id caused an interrupt. */
#define ERROR_INTERRUPT_DEVICE_ID 100000U

/* Starting with 11: Unsupported Interrupt. */

/** A reset interrupt happened. */
#define ERROR_INTERRUPT_RESET 110000U
/** An undefined instruction interrupt happened. */
#define ERROR_INTERRUPT_UNDEF 110001U
/** A supervisor call instruction interrupt happened. */
#define ERROR_INTERRUPT_SWI 110002U
/** A prefetch abort interrupt happened. */
#define ERROR_INTERRUPT_PABORT 110003U
/** A data abort interrupt happened. */
#define ERROR_INTERRUPT_DABORT 110004U
/** A fast interrupt happened. */
#define ERROR_INTERRUPT_FIQ 110005U

/* Starting with 2: Game Logic. */

/* Staring with 20: Game.c */

/** Restarting while the game is not finished. */
#define ERROR_LOGIC_STILL_PLAYING 20000U
/** Shooting while maximum amount of bullets is reached. */
#define ERROR_LOGIC_REACHED_BULLET_CAPACITY 20001U

/* Staring with 21: Tank.c */

/** Adding while maximum amount of tanks is reached. */
#define ERROR_LOGIC_REACHED_TANK_CAPACITY 21000U
/** Removing while there is not any tank. */
#define ERROR_LOGIC_NO_TANK_TO_REMOVE 21001U

/** Configures and initializes the error system. Called at the start of the
 * program. */
void error_config(void);
/** Shows the given error code in the seven segment displays, and stalls the
 * program. Called when an error occurs. */
void error_show(uint32_t const error);

#endif // ERROR_H
