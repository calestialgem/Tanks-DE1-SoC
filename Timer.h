#ifndef TIMER_H
#define TIMER_H

/** Timer's interrupt id. */
#define TIMER_INTERRUPT_ID 72
/** Timer's frequency in Hz. */
#define TIMER_FREQUENCY 60
/** Timer's intervals in seconds. */
#define TIMER_STEP (1.0F / TIMER_FREQUENCY)

/** Configures and initializes the timer. Called at the start of the
 * program. */
void timer_config(void);
/** Disables interrupts. Used for stopping the update calls while reading the
 * game object at the start of the rendering. */
void timer_disable_interrupts(void);
/** Enables interrupts. Used for restarting the update calls when the game
 * object is read at the start of the rendeing rendering.*/
void timer_enable_interrupts(void);

#endif // TIMER_H
