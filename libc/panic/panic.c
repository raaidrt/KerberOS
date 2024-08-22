#include <panic.h>
#include <stdio.h>

__attribute__((noreturn)) void panic(const char *reason) {
    printf(reason);
    // TODO: Implement printing the backtrace 
    for (;;) __asm__ __volatile__("hlt");
}
