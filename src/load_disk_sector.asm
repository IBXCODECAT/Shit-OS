EXTENDED_SPACE equ 0x8000				; Extended space starts 512+offseted bytes into memory

ReadDisk:
	mov ah, 0x02						; Setup BIOS function for reading disk
	mov bx, EXTENDED_SPACE				; Tell BIOS where in memory we want to load our extended program
	mov al, 32							; Read 32 disk sectors (will increase with more program data)
	mov dl, [BOOT_DRIVE]				; Read from this drive
	mov ch, 0x00						; Read from cylender 0 (Hard Drive)
	mov dh, 0x00						; Read from head 0 (Hard Drive)
	mov cl, 0x02						; Read from sector 2 (After Bootloader)
	
	int 0x13							; Call interupt to read data from disk with settings defined above

	jc DiskReadFailed					; Jump to DiskReadFailed if cary flag is set (Failed disk read)

	;call DataTest						; Test the data we are loading from EXTENDED_SPACE
	ret									; Return

DataTest:								; Print the first data value found in EXTENDED_SPACE
	mov ah, 0x0e						; Setup teletype routine
	mov al, [EXTENDED_SPACE]			; Move first data value in Extended space to lower a register
	int 0x10							; BIOS teletype routine interupt
	ret									; Return

DiskReadFailed:
	mov bx, DISK_READ_ERROR_STRING		; Move the contents of DISK_READ_ERROR_STRING into bx
	call Print							; Call Print: from teletype.asm
	hlt									; Terminate OS | Halt

BOOT_DRIVE: db 0						; Drive ID for Boot Drive

