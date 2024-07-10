.intel_syntax noprefix
.global clear_interrupt_flag, set_interrupt_flag

clear_interrupt_flag:
	cli
	ret

set_interrupt_flag:
	sti
	ret
