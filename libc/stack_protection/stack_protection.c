#include <stdint.h>
#include <stdlib.h>
#include <stack_protection.h>
#include <panic.h>
 
__attribute__((noreturn))
void __stack_chk_fail(void)
{
	panic("Stack smashing detected");
}