org 100h

start:
    mov si, message
    mov cx, msg_len
    mov al, 67h

decrypt_loop:
    xor byte [si], al
    inc si
    loop decrypt_loop

    mov dx, message
    mov cx, msg_len
    mov bx, 1
    mov ah, 40h
    int 21h

    mov ax, 4C00h
    int 21h

message:
    db 26h,35h,26h,50h,1Ch,21h,35h,53h,53h,4Ch,51h,55h,47h,34h
    db 12h,0Ah,05h,02h,15h,4Bh,47h,34h,12h,15h,06h,0Ch,06h,15h
    db 13h,06h,47h,24h,0Eh,13h,1Eh,4Bh,47h,24h,02h,09h,13h,15h
    db 06h,0Bh,47h,2Dh,06h,11h,06h,1Ah

msg_len equ $ - message
