#ifndef TIMER_H
#define TIMER_H

/** Timer's interrupt id. */
#define TIMER_INTERRUPT_ID 72
/** Timer's frequency in Hz. */
#define TIMER_FREQUENCY 60.0F
/** Timer's intervals in seconds. */
#define TIMER_STEP (1.0F / TIMER_FREQUENCY)

/** Configures and initializes the timer. Called at the start of the
 * program. */
void timer_config(void);
/** Called when a timer interrupt happens. */
void timer_interrupt(void);

#endif // TIMER_H
