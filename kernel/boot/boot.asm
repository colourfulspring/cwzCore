org 0x7c00

BaseOfStack equ 0x7c00

Label_Start:
    MOV AX, CS
    MOV DS, AX
    MOV ES, AX
    MOV SS, AX
    MOV SP, BaseOfStack

; clear screen
    MOV AX, 0600H
    MOV BX, 0700H
    MOV CX, 0
    MOV DX, 184FH
    INT 10H

; set focus
    MOV AX, 0200H
    MOV BX, 0000H
    MOV DX, 0000H
    INT 10H

; display on screen: Start Booting......
    MOV AX, 1301H
    MOV BX, 000FH
    MOV CX, 19
    MOV DX, 0000H
    PUSH AX
    MOV AX, DS
    MOV ES, AX
    POP AX
    MOV BP, StartBootMessage
    INT 10H

; reset floppy
    XOR AH, AH
    XOR DL, DL
    INT 13H
    JMP $

StartBootMessage: db "Start Booting......"

    times 510 - ($ - $$) db 0
    dw 0xaa55


