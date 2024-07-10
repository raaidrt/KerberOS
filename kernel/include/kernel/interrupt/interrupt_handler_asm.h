#ifndef _INTERRUPT_HANDLER_ASM_H
#define _INTERRUPT_HANDLER_ASM_H

#include <kernel/interrupt/interrupt_handler.h>

#define REPEAT_2(m, n) m(n) m(n+1)
#define REPEAT_4(m, n) REPEAT_2(m, n) REPEAT_2(m, n+2)
#define REPEAT_8(m, n) REPEAT_4(m, n) REPEAT_4(m, n+4)
#define REPEAT_16(m, n) REPEAT_8(m, n) REPEAT_8(m, n+8)
#define REPEAT_32(m, n) REPEAT_16(m, n) REPEAT_16(m, n+16)
#define REPEAT_64(m, n) REPEAT_32(m, n) REPEAT_32(m, n+32)
#define REPEAT_128(m, n) REPEAT_64(m, n) REPEAT_64(m, n+64)
#define REPEAT_256(m) REPEAT_128(m, 0) REPEAT_128(m, 128)

#define INTERRUPT_HANDLER(N) \
extern void CONCAT(interrupt_handler_, #N);

REPEAT_256(INTERRUPT_HANDLER)

#endif
