PAGE_TABLE_ENTRY equ 0x1000			; 0x1000 because there is code in the first section of memory we should not erase

SetupIdentityPaging:				; Setup identity paging (Required for LongMode)
	mov edi, PAGE_TABLE_ENTRY		; Copy PAGE_TABLE_ENTRY into the destination index
	mov cr3, edi					; Copy edi into control register 3 (tells our memory management unit that we start paging at PAGE_TABLE_ENTRY

	mov dword [edi], 0x2003			; Points to the address of our second table and sets a few flag bits
	add edi, 0x1000
	mov dword [edi], 0x3003
	add edi, 0x1000
	mov dword [edi], 0x4003
	add edi, 0x1000

	mov ebx, 0x00000003				; Set paging flag bits
	mov ecx, 512					; Let's the internal loop know we want to loop for 512 entries

	.SetEntry:
		mov dword [edi], ebx
		add ebx, 0x1000
		add edi, 8
		loop .SetEntry

									; Activate the Physical Adress Extension Paging (PAEP) by setting the PAE bit in the 4th control register
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

									; Set the LongMode bit in the EFER model specific register
	mov ecx, 0xC0000080
	rdmsr							; Read from model specific register
	or eax, 1 << 8					; Setting LongMode bit in model specific register
	wrmsr							; Write to model specific regsiter
	
									; Enable Paging
	mov eax, cr0					; Copy control register 0 to eax
	or eax, 1 << 31					; Set a bit in eax
	mov cr0, eax					; Copy eax into control register 0

	ret								; Return