#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#include <stdint.h>

/** Register that has 8 bits. */
typedef volatile uint8_t *const Register8;
/** Register that has 16 bits. */
typedef volatile uint16_t *const Register16;
/** Register that has 32 bits. */
typedef volatile uint32_t *const Register32;

/** Readonly register that has 8 bits. */
typedef volatile uint8_t const *const ReadRegister8;
/** Readonly register that has 16 bits. */
typedef volatile uint16_t const *const ReadRegister16;
/** Readonly register that has 32 bits. */
typedef volatile uint32_t const *const ReadRegister32;

#endif // MEMORY_MAP_H
