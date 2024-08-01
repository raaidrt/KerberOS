MBALIGN		equ	1 << 0			; align loaded modules on page boundaries
MEMINFO		equ	1 << 1			; provide memory map
FLAGS		equ	MBALIGN | MEMINFO	; this is the Multiboot 'flag' field
MAGIC		equ	0x1BADB002		; 'magic number' lets bootloader find the header
CHECKSUM	equ	-(MAGIC + FLAGS)	; checksum of above, to prove we are multiboot
HHALF		equ	0xC0000000 ;		; location of the Higher Half kernel
HHALF_PDE	equ	HHALF >> 22		; Page Directory Index of Kernel

; Declare a header as in the Multiboot Standard.
section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

; The kernel entry point.
section .multiboot.text exec alloc
global _loader
_loader:
	lea eax, [INITIAL_PD - HHALF]
	mov cr3, eax

	mov eax, cr4
	or eax, 0x00000010
	mov cr4, eax

	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax

	lea eax, [higher_half]

	jmp eax

; Declare initial Page Directory and Page Tables
section .data alloc

%define PTE_RW_BIT	(1 << 1)
%define PTE_P_BIT	1

global IDENTITY_PT
align 4096
IDENTITY_PT:
%assign i 0
%rep 1024
%assign PTE	(i * 4096)
	dd (PTE + PTE_P_BIT + PTE_RW_BIT)
%assign i i+1
%endrep

%define PDE_PS_BIT	(1 << 7)
%define PDE_RW_BIT	(1 << 1)
%define PDE_P_BIT	1

%define IDENTITY_PT_FIRST_4MiB	0x0 + PDE_PS_BIT + PDE_P_BIT + PDE_RW_BIT
%define KERNEL_PT		IDENTITY_PT + PDE_P_BIT + PDE_RW_BIT

global INITIAL_PD
align 4096
INITIAL_PD:
	dd IDENTITY_PT_FIRST_4MiB	; Setup functions will reside here
	times(HHALF_PDE - 1) dd 0
	dd (KERNEL_PT - 0xC0000000)	; Kernel will reside here
	times(1024 - HHALF_PDE - 1) dd 0

; Reserve a stack for the initial thread.
section .bss alloc
align 16
stack_bottom:
resb 16384
stack_top:

section .text exec
extern _init
extern kernel_main
higher_half:
	mov dword [INITIAL_PD], 0 ; Can remove the first 4MiB Identity Mapping
	invlpg [0]

	; Start kernel setup
	mov esp, stack_top
	; Call the global constructors.
	call _init
	; Transfer control to the main kernel.
	cli
	push ebx
	call kernel_main

	; Hang if kernel_main unexpectedly returns.
	cli
.hang:
	hlt
	jmp .hang

