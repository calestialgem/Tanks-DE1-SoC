#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

/** Initializes the interrupt mechanism. Called at the start of the program,
 * before all the subsystems enable their interrupts. */
void interrupt_init(void);
/** Enables the interrupts for the given device id. Should only be called at the
 * configuration of a system, at the start of the program. */
void interrupt_enable(uint32_t id);
/** Configures the interrupt mechanism. Called at the start of the program,
 * after all the subsystems enable their interrupts. */
void interrupt_config(void);

#endif // INTERRUPT_H
