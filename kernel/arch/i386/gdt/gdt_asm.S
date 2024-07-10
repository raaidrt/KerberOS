.intel_syntax noprefix
.global load_segment_registers, load_gdt
.extern gdt_pointer

load_segment_registers:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:flush
flush:
	ret

load_gdt: 
	lgdt [gdt_pointer]
	ret
