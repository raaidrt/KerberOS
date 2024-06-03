#ifndef _PANIC_H
#define _PANIC_H 1

#include <stdio.h>

 __attribute__((noreturn)) void panic(const char *reason);

#endif