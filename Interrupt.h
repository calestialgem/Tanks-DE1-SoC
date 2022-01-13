#ifndef INTERRUPT_H
#define INTERRUPT_H

/** Configure interrupt controller. */
void interrupt_config(void);
/** Disables all the interrupts. */
void interrupt_disable(void);
/** Enables all the interrupts. */
void interrupt_enable(void);

#endif // INTERRUPT_H
