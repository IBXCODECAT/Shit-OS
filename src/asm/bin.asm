; This assembly is responsible for including binaries (like images) in our OS

%macro IncBin 2                 ; Create a macro
    SECTION .rodata             ; Declare as read only data
    GLOBAL %1                   ; Declare parameter 1 as global


%1:
    incbin %2                   ; Command?
    db 0                        ; Null terminate whatever we have loaded
    %1_size: dq %1_size - %1    ; Declare a label where we can extract the size of whatever we load

%endmacro                       ; Terminate our macro

IncBin binTest, "./binaries/bintest.txt"

