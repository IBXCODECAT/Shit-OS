[org 0x7c00]                        ; Tell the assembler where in memory this code will be loaded
[bits 16]							; We are in 16 bit real mode!

mov [BOOT_DRIVE], dl				; BIOS only stores the disk number used to boot

mov bp, 0x7c00                      ; Here we set our stack safely out of the way at 0x7c00
mov sp, bp							; Here we set the stack pointer to the begining of the stack

mov bx, SYS_MSG						; Move the contents of END_SYS_MSG into bx
call Print							; Call Print: from teletype.asm

mov bx, BOOT_STRING					; Move the contents of BOOT_STRING into bx
call Print							; Call Print: from teletype.asm

mov bx, 0							; Move null data into bx to prevent overwrite in function call
call ReadDisk						; Call ReadDisk: from load_disk_sector.asm

jmp EXTENDED_SPACE                  ; Jump to instructions in extended.asm

%include "./asm/teletype.asm"	        ; Include file in compilation
%include "./asm/load_disk_sector.asm"	; Include file in compilation

times 510-($-$$) db 0				; Fill up to 510 bytes
dw 0xaa55                           ; Boot Sector signature