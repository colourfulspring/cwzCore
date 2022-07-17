org 0x10000
    JMP Label_Start

; %include "fat12.inc"

[SECTION .s16]
[BITS 16]

Label_Start:
    MOV AX, CS
    MOV DS, AX
    MOV ES, AX
    MOV AX, 0x00
    MOV SS, AX
    MOV SP, 0x7C00

; display on screen: Start Loader
    MOV AX, 1301H
    MOV BX, 000FH
    MOV DX, 0200H
    MOV CX, 12
    PUSH AX
    MOV AX, DS
    MOV ES, AX
    POP AX
    MOV BP, StartLoaderMessage
    INT 10H

; open address A20 for 4GB real mode address
    PUSH AX
    IN AL, 92H
    OR AL, 00000010B
    OUT 92H, AL
    POP AX

    CLI
    
    

StartLoaderMessage: db "Start Loader"