.intel_syntax noprefix
.global load_segment_registers

load_segment_registers:
	xor rax, rax
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov fs, ax
	mov ss, ax
	jmp 0x08:flush
flush:
	ret

