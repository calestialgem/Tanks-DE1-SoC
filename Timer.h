#ifndef TIMER_H
#define TIMER_H

/** Timer's interrupt id. */
#define TIMER_INTERRUPT_ID 72
/** Timer's intervals in milliseconds, which corresponds to 60 Hz. */
#define TIMER_MILLIS 16
/** Timer's intervals in seconds. */
#define TIMER_STEP (TIMER_MILLIS / 1000.0F)

/** Configures and initializes the timer. Called at the start of the
 * program. */
void timer_config(void);
#ifdef TIMER_INTERRUPT_ID
/** Handles the timer interrupts. Called by the interrupt handler. */
void timer_isr(void);
#endif // TIMER_INTERRUPT_ID

#endif // TIMER_H
