; This assembly is responsible for setting the GDT (Global Descriptor Table)
; In this file we define a gdt description structure which contains a size (size of the gdt entry) and an offset (memory adress of the gdt)
; A GDT entry has three parts: A null descriptor for memory spacing, a code segment, and a data segment (memory definitions for code and data)

GDT_Null_Descriptor:										; Create a Null Descriptor for our GDT (2 bytes of space)
	dd 0													; Decalare a double (4 bytes)
	dd 0													; Declare a double (4 bytes)
	
GDT_Code_Segment:											; Create the Code Segment for the GDT
	dw 0xFFFF												; Set the size limit for our code segment (0xFFFF covers the entire span of memory availible)
	dw 0x0000												; Set the first half of our base adress for our code segment (starting point in memory)
	db 0x00													; Set the second base for our code segment (starting point in memory)

	; Set the acess byte...
	; Present bit 1 = valid
	; Memory privledge 00 = krnl
	; Descriptor type = 1 = code_seg
	; Executable bit 1 = code_seg
	; Conforming bit 0 = ?
	; Read/Write bit 1 = readable
	; Access bit 0 = unacessed (cpu sets to 1 when acessed)
	db 10011010b

	; Memory Flags...
	; Granularity (0 = 1 byte blocks, 1 = 4kb blocks)
	; Size bit (0 = 16 bit, 1 = 32 bit)
	; 2 bits (0 for spacing)
	; Limit extension (4 bit binary number, 1111 max)
	db 11001111b

	db 0x00													; High portion of memory base (set to zero for max range in our case)

GDT_Data_Segment:											; Create the Code Segment for the GDT
	dw 0xFFFF												; Set the size limit for our code segment (0xFFFF covers the entire span of memory availible)
	dw 0x0000												; Set the first half of our base adress for our code segment (starting point in memory)
	db 0x00													; Set the second base for our code segment (starting point in memory)

															; Set the acess byte...
															; Present bit 1 = valid
															; Memory privledge 00 = krnl
															; Descriptor type = 1 = code_seg
															; Executable bit 0 = data_seg
															; Conforming bit 0 = ?
															; Read/Write bit 1 = readable
															; Access bit 0 = unacessed (cpu sets to 1 when acessed)
	db 10010010b

															; Memory Flags...
															; Granularity (0 = 1 byte blocks, 1 = 4kb blocks)
															; Size bit (0 = 16 bit, 1 = 32 bit)
															; 2 bits (0 for spacing)
															; Limit extension (4 bit binary number, 1111 max)
	db 11001111b

	db 0x00													; High portion of memory base (set to zero for max range in our case)

GDT_END:													; The end marker label for our GDT

GDT_Descriptor:												; Define the size and the offset of the GDT (will be passed to the CPU)
	gdt_size:												; Define the size of the GDT overall
		dw GDT_END - GDT_Null_Descriptor - 1				; Size of the GDT (Subtracted by 1 for some reason)
		dd GDT_Null_Descriptor								; The null descriptor for our GDT

CODE_SEGMENT equ GDT_Code_Segment - GDT_Null_Descriptor		; Define the code segment for our GDT
DATA_SEGMENT equ GDT_Data_Segment - GDT_Null_Descriptor		; Define the data segment for our GDT

[bits 32]													; We are in 32 bit Protected Mode!

GDT_Update_LongMode:										; Update our GDT for LongMode
	mov [GDT_Code_Segment + 6], byte 10101111b				; Edit the Access Byte for the code segment
	mov [GDT_Data_Segment + 6], byte 10101111b				; Edit the Access byte for the data segment
	ret

[bits 16]													; We are in 16 bit RealMode!