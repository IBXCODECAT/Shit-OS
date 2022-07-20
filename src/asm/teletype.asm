; This assembly is responible for the ability to print strings using the BIOS teletype routine

Print:
	push bx							; Push the data of our string onto the stack

	mov ah, 0x0e					; Setup the BIOS teletype routine
	.Loop:
	cmp [bx], byte 0				; Have we reached the null termination of the string?
	je .Exit						; If we have jump to .Exit
		mov al, [bx]				; Otherwise move the contents of bx into the low a register
		int 0x10					; Interupt for the BIOS teletype routine
		inc bx						; Incrment bx (our pointer)
		jmp .Loop					; Jump to .Loop and do it again
	.Exit:
					
	pop bx							; Pop the data of our string off the stack
	ret								; Return

; String definitions can go down here to be set into the bx register before calling Print:
SYS_MSG: db "Copyright(C) 2022 IBXCODECAT, Dom73 | ", 0
BOOT_STRING: db "Boot OK | ", 0
DISK_READ_SUCCESS: db "Disk Read OK | ", 0
DISK_READ_ERROR_STRING: db "ERR: Disk Read Failed | ", 0