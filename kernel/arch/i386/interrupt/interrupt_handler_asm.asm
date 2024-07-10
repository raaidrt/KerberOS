extern interrupt_handler
%macro common_interrupt_handler 1
common_interrupt_handler_%1:               ; the common parts of the generic interrupt handler
	; save the registers
	push    eax
	push    ecx
	push	edx
	push	ebx
	push	esp
	push    ebp
	push	esi 
	push	edi

	; call the C function
	call    interrupt_handler

	; restore the registers
	pop	edi
	pop	esi
	pop	ebp
	pop	esp
	pop	ebx
	pop	edx
	pop	ecx 
	pop	eax
	
	; restore the esp
	add     esp, 8

	; return to the code that got interrupted
	iret
%endmacro

%macro no_error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
	push    dword 0                     ; push 0 as error code
	push    dword %1                    ; push the interrupt number
	jmp     common_interrupt_handler_%1    ; jump to the common handler
%endmacro

%macro error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
	push    dword %1                    ; push the interrupt number
	jmp     common_interrupt_handler_%1    ; jump to the common handler
%endmacro


%assign i 0
%rep 256
%if i == 8 || (10 <= i && i <= 14) || i == 17
error_code_interrupt_handler	i
%else
no_error_code_interrupt_handler	i
%endif
common_interrupt_handler	i
%assign i i+1
%endrep
