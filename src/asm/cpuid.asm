
DetectCPUID:						; Check for CPUID (set a bit in flags register, copy it back, if still fliped CPUID is supported)
	pushfd							; Push CPU flags onto the stack
	pop eax							; Pop the CPU flags into our eax register
	
	mov ecx, eax					; Copy eax into ecx to compare values later on
	xor eax, 1 << 21				; Flip the bit in the eax register

	push eax						; Push the value of the eax register onto the stack
	popfd							; Pop flags
	
	pushfd							; Push flags onto the stack
	pop eax							; Pop into the eax register

	push ecx						; Restor CPU flags to old version
	popfd							; Pop flags

	xor eax, ecx					; Compare values
	jz PANIC_NO_CPUID				; Jump if a CPUID was not detected

	ret								; Return

DetectLongMode:						; Check if our CPU supports 64 bit long mode
	mov eax, 0x80000001				; Attempt to copy 64 bit value into eax 
	cpuid							
	test edx, 1 << 29				; Test for longmode
	jz PANIC_NO_LONGMODE			; Jump if there is long mode support

	ret								; Return