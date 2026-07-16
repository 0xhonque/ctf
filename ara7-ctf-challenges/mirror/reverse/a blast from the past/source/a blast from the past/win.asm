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
    db 26h,35h,26h,50h,1ch,0eh,09h,14h,17h,0eh,15h,02h,03h,38h
    db 01h,15h,08h,0ah,38h,04h,0fh,0eh,0ah,02h,15h,06h,27h,01h
    db 0bh,06h,15h,02h,4ah,08h,09h,49h,04h,08h,0ah,1ah

msg_len equ $ - message
