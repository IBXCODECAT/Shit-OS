; Interupt Service Routines for our Interupt Descriptor Table... 

[extern _idt]					; Connect _idt with our linker
idtDescriptor:
	dw 4095						; The size of our IDT
	dq _idt						; Offset declared in our linker

	%macro PUSHALL 0
		push rax
		push rcx
		push rdx
		push r8
		push r9
		push r10
		push r11
	%endmacro

	%macro POPALL 0
		pop r11
		pop r10
		pop r9
		pop r8
		pop rdx
		pop rcx
		pop rax
	%endmacro

[extern isr1_handler]			; Connect isr1_handler with idt.h header
isr1:
	PUSHALL						; Push all of our registers
	call isr1_handler			; Call our isr1_handler
	POPALL						; Pop all of our registers
	iretq
	GLOBAL isr1

LoadIDT:
	lidt[idtDescriptor]			; Load the IDT
	sti							; Set interupt flag
	ret							; Return
	GLOBAL LoadIDT
