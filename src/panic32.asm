; This assembly is responsible for all panic conditions prior to entering 64 bit LongMode

FORMATTING_BYTE equ 0x1F							;Blue Background = 1, White foreground = F

PANIC_NO_CPUID:
	mov [0xb8000], byte "C"
	mov [0xb8001], byte FORMATTING_BYTE
	mov [0xb8002], byte "P"
	mov [0xb8003], byte FORMATTING_BYTE
	mov [0xb8004], byte "U"
	mov [0xb8005], byte FORMATTING_BYTE
	mov [0xb8006], byte "I"
	mov [0xb8007], byte FORMATTING_BYTE
	mov [0xb8008], byte "D"
	mov [0xb8009], byte FORMATTING_BYTE
	mov [0xb800A], byte "_"
	mov [0xb800B], byte FORMATTING_BYTE
	mov [0xb800C], byte "N"
	mov [0xb800D], byte FORMATTING_BYTE
	mov [0xb800E], byte "O"
	mov [0xb800F], byte FORMATTING_BYTE
	mov [0xb8010], byte "T"
	mov [0xb8011], byte FORMATTING_BYTE
	mov [0xb8012], byte "_"
	mov [0xb8013], byte FORMATTING_BYTE
	mov [0xb8014], byte "S"
	mov [0xb8015], byte FORMATTING_BYTE
	mov [0xb8016], byte "U"
	mov [0xb8017], byte FORMATTING_BYTE
	mov [0xb8018], byte "P"
	mov [0xb8019], byte FORMATTING_BYTE
	mov [0xb801A], byte "P"
	mov [0xb801B], byte FORMATTING_BYTE
	mov [0xb801C], byte "O"
	mov [0xb801D], byte FORMATTING_BYTE
	mov [0xb801E], byte "R"
	mov [0xb801F], byte FORMATTING_BYTE
	mov [0xb8020], byte "T"
	mov [0xb8021], byte FORMATTING_BYTE
	mov [0xb8022], byte "E"
	mov [0xb8023], byte FORMATTING_BYTE
	mov [0xb8024], byte "D"
	mov [0xb8025], byte FORMATTING_BYTE
	hlt										; Terminate OS | Halt

PANIC_NO_LONGMODE:
	mov [0xb8000], byte "x"
	mov [0xb8001], byte FORMATTING_BYTE
	mov [0xb8002], byte "8"
	mov [0xb8003], byte FORMATTING_BYTE
	mov [0xb8004], byte "6"
	mov [0xb8005], byte FORMATTING_BYTE
	mov [0xb8006], byte "6"
	mov [0xb8007], byte FORMATTING_BYTE
	mov [0xb8008], byte "4"
	mov [0xb8009], byte FORMATTING_BYTE
	mov [0xb800A], byte "_"
	mov [0xb800B], byte FORMATTING_BYTE
	mov [0xb800C], byte "N"
	mov [0xb800D], byte FORMATTING_BYTE
	mov [0xb800E], byte "O"
	mov [0xb800F], byte FORMATTING_BYTE
	mov [0xb8010], byte "T"
	mov [0xb8011], byte FORMATTING_BYTE
	mov [0xb8012], byte "_"
	mov [0xb8013], byte FORMATTING_BYTE
	mov [0xb8014], byte "S"
	mov [0xb8015], byte FORMATTING_BYTE
	mov [0xb8016], byte "U"
	mov [0xb8017], byte FORMATTING_BYTE
	mov [0xb8018], byte "P"
	mov [0xb8019], byte FORMATTING_BYTE
	mov [0xb801A], byte "P"
	mov [0xb801B], byte FORMATTING_BYTE
	mov [0xb801C], byte "O"
	mov [0xb801D], byte FORMATTING_BYTE
	mov [0xb801E], byte "R"
	mov [0xb801F], byte FORMATTING_BYTE
	mov [0xb8020], byte "T"
	mov [0xb8021], byte FORMATTING_BYTE
	mov [0xb8022], byte "E"
	mov [0xb8023], byte FORMATTING_BYTE
	mov [0xb8024], byte "D"
	mov [0xb8025], byte FORMATTING_BYTE
	hlt										; Terminate OS | Halt