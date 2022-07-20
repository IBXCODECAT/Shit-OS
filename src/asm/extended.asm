;[org 0x7e00]								; Tell the assembler where in memory this code will be loaded (linker now does this)

[bits 16]									; We are in 16-bit Real Mode!

mov bx, DISK_READ_SUCCESS					; Move the contents of DISK_READ_SUCCESS into bx
call Print									; Call Print: from teletype.asm

jmp EnterProtectedMode						; Jump to instruction( i.e. forever ).

%include "./asm/teletype.asm"				; Include file in compilation (16-bit code)
%include "./asm/gdt.asm"					; Include file in compilation (16-bit code)

EnterProtectedMode:							; Enter 32-Bit Protected Mode (Disable interupts, Enable A20 Line, Load GDT, Set GDT)
	call EnableA20Line						; Enable the A20 Line
	cli										; Disable Interupts
	lgdt [GDT_Descriptor]					; Load the GDT into our GDT descriptor

										
	mov eax, cr0					
	or eax, 1						
	mov cr0, eax
	
	jmp CODE_SEGMENT:StartProtectedMode		; Do a far jump to flush the CPU Pipeline (A CPU multitasking whilst swapping modes could result in unexpected behaviour)

EnableA20Line:								; Enabling the A20 Line is required for backwards compatability with older CPUS (this test works on most devices)
	in al, 0x92								; Copy a value
	or al, 2								; Do some bitwise math
	out 0x92, al							; Shove the value back out
	ret										; Return

[bits 32]									; We are now in 32-bit Protected Mode!

%include "./asm/cpuid.asm"					; Include file in compilation (32-bit code)
%include "./asm/paging.asm"					; Include file in compilation (32-bit code)
%include "./asm/panic32.asm"				; Include file in compilation (32-bit code)

StartProtectedMode:							; Run a quck procedure before computing in 32-Bit protected mode

	mov ax, DATA_SEGMENT					; Update CPU Registers?
	
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	
	mov [0xb800a], byte " "					; Set byte in video memory

	mov ebp, 0x90000						; Increase the size of the CPU stack
	mov esp, ebp							; Move the stack pointer

	call DetectCPUID						; Check for CPUID Support
	call DetectLongMode						; Check for LongMode Support
	call SetupIdentityPaging				; Setup Identity Paging (Required)
	call GDT_Update_LongMode				; Update our GDT for LongMode (Required)
	jmp CODE_SEGMENT:StartLongMode			; Do a far jump to flush the CPU Pipeline (A CPU multitasking whilst swapping modes could result in unexpected behaviour)

[bits 64]									; We are now in 64 bit LongMode!!!
[extern _krnlTest]							; Reference this name from a c++ file
[extern _start]								; Reference this name from a c++ file

%include "./asm/idt.asm"					; Include the contents of IDT.asm here

StartLongMode:
	mov edi, 0xb8000
	mov rax, 0x1f201f201f201f20				; Quick test
	mov ecx, 500
	rep stosq

	;call _krnlTest							; Call _krnlTest from krnl.cpp (Should write PP to screen)
	
	call ActivateSSE						; Call ActivateSSE from this file
	call _start								; Call _start from krnl.cpp
	jmp $

ActivateSSE:								; Activate the SSE to print floats/doubles (even when casted)
	
	mov rax, cr0							; Set the bits of the cr0 register
	and ax, 0b11111101
	or ax, 0b00000001
	mov cr0, rax

	mov rax, cr4							; Set the bits of the cr4 register
	or ax, 0b1100000000
	mov cr4, rax
	ret

times 2048-($-$$) db 0						; Zerofill 4 sectors for testing purposes