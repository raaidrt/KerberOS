#include <panic.h>

__attribute__((noreturn)) void panic(const char *reason) {
    printf(reason);
    // TODO: Implement printing the backtrace 
    __asm__ __volatile__("hlt");
}
