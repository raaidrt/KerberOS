.intel_syntax noprefix
.global load_idt
.extern idt_pointer

load_idt: 
	lgdt [idt_pointer]
	ret
