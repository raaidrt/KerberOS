#include <stdint.h>
#include <stdlib.h>
#include <stack_protection.h>
 
__attribute__((noreturn))
void __stack_chk_fail(void)
{
#if __STDC_HOSTED__
	abort();
#elif __is_myos_kernel
	panic("Stack smashing detected");
#endif
}