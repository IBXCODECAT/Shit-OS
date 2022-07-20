; This assembly allows us to detect any memory we have availible using the BIOS

MemoryRegionCount:
    db 0
    GLOBAL MemoryRegionCount                            ; GLOBAL allows acess in C/CPP code

; To detect memory we need to:
; 1. set the ESDI to destination of where we want the BIOS memory table to be stored

DetectMemory:
    mov ax, 0
    mov es, ax
    mov di, 0x5000                                      ; We will store the memory map at 0x5000
    mov edx, 0x534d4150                                 ; MAGIC NUMBER (SMAP)
    xor ebx, ebx                                        ; Set the ebx register to zero

    .repeat:                                            ; Loop through every listing in the table and call BIOS interupt to save at destination...
        mov eax, 0xe820                                 ; 
        mov ecx, 24                                     ; Define the size of each memory table listing (24 bytes)
        int 0x15                                        ; Call the interupt to save a memory listing

        cmp ebx, 0                                      ; EBX will be zero when the interupt has finished interating throught the listings...
        je .finished                                    ; Jump to .finished if finished
        
        add di, 24                                      ; If not finished add 24 (table listing size) to our destination index
        inc byte [MemoryRegionCount]                    ; Incrment our MemoryRegionCount by 1
        jmp .repeat                                     ; Do the loop again

    .finished:
        
    ret

