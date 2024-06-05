#ifndef _LOGGER_H
#define _LOGGER_H 1

#define _DEBUG_LOG 1

#ifdef _DEBUG_LOG
#include <serial.h>
__attribute__((constructor)) void logger_init();
#endif

void dbg_logf(const char* restrict format, ...);

#endif