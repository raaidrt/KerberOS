.intel_syntax noprefix
.global load_idt
.extern idt_pointer

load_idt: 
	lidt [idt_pointer]
	ret
