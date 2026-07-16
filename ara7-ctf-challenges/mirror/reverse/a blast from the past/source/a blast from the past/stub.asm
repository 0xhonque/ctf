; nasm stub.asm -f bin -o stub.bin

BITS 16
ORG 0

; ==============================================================================
; MACROS
; ==============================================================================
; In EXE mode, the 64-byte header is stripped from the loaded segment.
; NASM offsets start at 0 (file beginning).
; Runtime Offset = Label - 64.
%define REF(x) (x - 64)

%macro DBG 1
    push ax
    push dx
    mov ah, 02h
    mov dl, %1
    int 21h
    pop dx
    pop ax
%endmacro

%macro TIME_CHECK 0
    ; Date Check (1981 - 1989)
    mov ah, 2Ah
    int 21h
    cmp cx, 1981
    jb not_allowed
    cmp cx, 1989
    ja not_allowed
%endmacro

; ==============================================================================
; POLYGLOT MZ HEADER
; ==============================================================================
mz_header:
    db 'M', 'Z'                 ; 0x00
    db 0x26, 0x00               ; 0x02: Bytes on last page (38)

    dw 4                        ; 0x04: Total pages (4 -> 1574 bytes)
    dw 0                        ; 0x06: Relocations
    dw 4                        ; 0x08: Header size (64 bytes)
    dw 0x100                    ; 0x0A: Min mem
    dw 0xFFFF                   ; 0x0C: Max mem
    dw 0                        ; 0x0E: SS
    dw 0x1000                   ; 0x10: SP
    dw 0                        ; 0x12: Checksum
    dw 0                        ; 0x14: IP (Start at 0 relative to segment)
    dw 0                        ; 0x16: CS
    dw 0x40                     ; 0x18: Reloc Table
    dw 0                        ; 0x1A: Overlay

    jmp short start             ; 0x1C

    times 0x3C - ($ - mz_header) db 0
    dd 0                        ; 0x3C: PE Header Pointer
    times 0x40 - ($ - mz_header) db 0

; ==============================================================================
; ENTRY POINT
; ==============================================================================
start:
    ; --------------------------------------------------------------------------
    ; EXTENSION CHECK
    ; --------------------------------------------------------------------------
    ; DOS loads us as EXE (CS=Code, DS=PSP).
    ; We must check the Environment Block for the filename.
    
    push ds
    pop es              ; ES = PSP
    mov ds, [es:0x2c]   ; DS = Environment Segment
    xor si, si          ; SI = 0

find_env_end:
    lodsb
    or al, al
    jnz find_env_end    ; Skip var chars
    lodsb
    or al, al
    jnz find_env_end    ; Skip separator, unless double null

    ; Found 00 00. End of Env Vars.
    add si, 2           ; Skip word count (2 bytes)
    
    ; DS:SI now points to the full path (e.g., C:\CHIMERA.COM)
    ; Find the end of the string.
find_path_end:
    lodsb
    or al, al
    jnz find_path_end

    ; SI points after NULL. Extension is at SI-4, SI-3, SI-2.
    ; Check for 'M' (from .COM) or 'm'
    mov al, [si-2]
    and al, 0xDF        ; To Upper
    cmp al, 'M'
    jne is_exe

    ; Check for 'O'
    mov al, [si-3]
    and al, 0xDF
    cmp al, 'O'
    jne is_exe

    ; Check for 'C'
    mov al, [si-4]
    and al, 0xDF
    cmp al, 'C'
    jne is_exe

is_com:
    ; Restore DS to CS for variable access
    push cs
    pop ds
    push cs
    pop es
    jmp com_mode

is_exe:
    ; Restore DS to CS for variable access
    push cs
    pop ds
    push cs
    pop es
    jmp exe_mode

; ==============================================================================
; SCENARIO B: DOS EXE (Date Check + Decrypt)
; ==============================================================================
exe_mode:
    ; DBG 'E'

    TIME_CHECK

allowed:
    ; DBG 'D'
    ; DBG 'X'

    ; Decrypt Payload
    mov si, REF(payload)
    mov di, REF(payload)
    mov cx, payload_len
    mov bl, 0ABh
    cld

decode:
    lodsb
    xor al, bl
    stosb
    loop decode

    ; DBG 'Y'
    ; DBG 'J'

    ; Jump to Payload
    mov ax, cs
    mov dx, REF(payload)
    mov cl, 4
    shr dx, cl
    add ax, dx
    
    push ax
    xor ax, ax
    push ax
    retf

not_allowed:
    mov dx, REF(stub_msg)
    mov ah, 09h
    int 21h
    mov ax, 4C00h
    int 21h

; ==============================================================================
; SCENARIO C: DOS COM (TEA Cipher)
; ==============================================================================
com_mode:
    ; DBG 'C'

    TIME_CHECK

    mov dx, REF(com_prompt)
    mov ah, 09h
    int 21h

    mov si, REF(input)
    mov cx, 8

    jmp read_loop

read_loop:
    mov ah, 01h
    int 21h
    mov [si], al
    inc si
    loop read_loop

    ; Load Input
    mov si, REF(input)
    mov ax, [si]
    mov [REF(y)], ax
    mov ax, [si+2]
    mov [REF(y)+2], ax
    mov ax, [si+4]
    mov [REF(z)], ax
    mov ax, [si+6]
    mov [REF(z)+2], ax

    ; Reset Sum
    mov word [REF(sum)], 079B9h
    mov word [REF(sum)+2], 09E37h

    mov cx, 32

tea_loop:
    push cx 

    ; TEA Round
    mov ax, [REF(z)]
    mov bx, [REF(z)+2]
    shl ax, 1
    rcl bx, 1
    shl ax, 1
    rcl bx, 1
    shl ax, 1
    rcl bx, 1
    shl ax, 1
    rcl bx, 1
    add ax, [REF(key0)]
    adc bx, [REF(key0)+2]
    mov dx, ax
    mov si, bx

    mov ax, [REF(z)]
    mov bx, [REF(z)+2]
    add ax, [REF(sum)]
    adc bx, [REF(sum)+2]
    xor dx, ax
    xor si, bx

    mov ax, [REF(z)]
    mov bx, [REF(z)+2]
    shr bx, 1
    rcr ax, 1
    shr bx, 1
    rcr ax, 1
    shr bx, 1
    rcr ax, 1
    shr bx, 1
    rcr ax, 1
    shr bx, 1
    rcr ax, 1
    add ax, [REF(key1)]
    adc bx, [REF(key1)+2]
    xor dx, ax
    xor si, bx

    mov ax, [REF(y)]
    mov bx, [REF(y)+2]
    add ax, dx
    adc bx, si
    mov [REF(y)], ax
    mov [REF(y)+2], bx

    mov ax, [REF(y)]
    mov bx, [REF(y)+2]
    shl ax, 1
    rcl bx, 1
    shl ax, 1
    rcl bx, 1
    shl ax, 1
    rcl bx, 1
    shl ax, 1
    rcl bx, 1
    add ax, [REF(key2)]
    adc bx, [REF(key2)+2]
    mov dx, ax
    mov si, bx

    mov ax, [REF(y)]
    mov bx, [REF(y)+2]
    add ax, [REF(sum)]
    adc bx, [REF(sum)+2]
    xor dx, ax
    xor si, bx

    mov ax, [REF(y)]
    mov bx, [REF(y)+2]
    shr bx, 1
    rcr ax, 1
    shr bx, 1
    rcr ax, 1
    shr bx, 1
    rcr ax, 1
    shr bx, 1
    rcr ax, 1
    shr bx, 1
    rcr ax, 1
    add ax, [REF(key3)]
    adc bx, [REF(key3)+2]
    xor dx, ax
    xor si, bx

    mov ax, [REF(z)]
    mov bx, [REF(z)+2]
    add ax, dx
    adc bx, si
    mov [REF(z)], ax
    mov [REF(z)+2], bx

    mov ax, [REF(sum)]
    add ax, [REF(delta)]
    mov [REF(sum)], ax
    mov ax, [REF(sum)+2]
    adc ax, [REF(delta)+2]
    mov [REF(sum)+2], ax

    pop cx
    dec cx
    jnz near tea_loop

    ; Compare
    mov ax, [REF(y)]
    cmp ax, [REF(ct_y)]
    jne com_fail
    mov ax, [REF(y)+2]
    cmp ax, [REF(ct_y)+2]
    jne com_fail
    mov ax, [REF(z)]
    cmp ax, [REF(ct_z)]
    jne com_fail
    mov ax, [REF(z)+2]
    cmp ax, [REF(ct_z)+2]
    jne com_fail

com_success:
    mov dx, REF(msg_ok)
    mov ah, 09h
    int 21h
    jmp exit

com_fail:
    mov dx, REF(msg_fail)
    mov ah, 09h
    int 21h

exit:
    mov ax, 4C00h
    int 21h

; ==============================================================================
; DATA SECTIONS
; ==============================================================================

stub_msg db "!This program cannot be run in DOS mode.$"

com_prompt db ">> $"
msg_ok     db 13,10,"OK$"
msg_fail   db 13,10,"NO$"

input times 8 db 0
y dw 0,0
z dw 0,0

key0 dd 0x37415241
key1 dd 0x41455458
key2 dd 0x3159454B
key3 dd 0x35343332

ct_y dd 0x9CD2B546
ct_z dd 0xE5E5719B

sum   dw 079B9h, 09E37h
delta dw 079B9h, 09E37h

; ==============================================================================
; EXE PAYLOAD (Encrypted)
; ==============================================================================
align 16

; XOR key = 0xab
payload:
    db 0x40, 0xab, 0x27, 0x63, 0x25, 0x73, 0x25, 0x6b, 0x11, 0x0d, 0xaa, 0x1f, 0xa2, 0x66, 0x8a, 0x15
    db 0xd1, 0xab, 0x12, 0xcb, 0xab, 0x1f, 0xaa, 0x66, 0x8a, 0x23, 0xaf, 0xed, 0x49, 0x5c, 0x15, 0xd1
    db 0xab, 0x14, 0x71, 0xab, 0x20, 0xb5, 0x31, 0xaa, 0x12, 0xcb, 0xab, 0x43, 0x99, 0xab, 0x23, 0x73
    db 0x99, 0xaf, 0x23, 0xae, 0xed, 0xec, 0x49, 0x58, 0x15, 0x71, 0xab, 0x14, 0x91, 0xaa, 0x12, 0xcb
    db 0xab, 0x21, 0xaf, 0x91, 0xae, 0xde, 0xa6, 0xed, 0xec, 0x49, 0x5d, 0x11, 0x37, 0xaa, 0x1f, 0xa2
    db 0x66, 0x8a, 0x40, 0xac, 0x11, 0x0a, 0xaa, 0x1f, 0xa2, 0x66, 0x8a, 0x13, 0xab, 0xe7, 0x66, 0x8a
    db 0x22, 0x73, 0x22, 0x69, 0x6a, 0x49, 0xac, 0x9a, 0x7b, 0x22, 0x69, 0x6a, 0x41, 0xa2, 0x9a, 0x7b
    db 0x22, 0x69, 0x6a, 0x49, 0xa3, 0x9a, 0x7b, 0x22, 0x68, 0x68, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab
    db 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab
    db 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab
    db 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab
    db 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab
    db 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab
    db 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab
    db 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab
    db 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab
    db 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab
    db 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab
    db 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab
    db 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xe0, 0x41, 0xcb, 0xf9, 0xb9, 0x6a
    db 0x13, 0x82, 0x11, 0xf4, 0x93, 0xd5, 0xa2, 0x15, 0x50, 0x30, 0x07, 0x01, 0x72, 0x86, 0x8f, 0xcf
    db 0xf9, 0x29, 0xfe, 0x71, 0xd3, 0x62, 0xf6, 0xdc, 0x4f, 0x12, 0xd7, 0x77, 0x25, 0x68, 0x60, 0x1b
    db 0xad, 0xb4, 0xf4, 0x01, 0x1e, 0x55, 0xa7, 0x21, 0xd0, 0x4b, 0x48, 0x25, 0xd4, 0xe5, 0xd2, 0x60
    db 0x2c, 0xda, 0x55, 0xcc, 0x1b, 0xd1, 0xc5, 0x3c, 0xaf, 0x6a, 0x47, 0xf7, 0xf2, 0x8f, 0xd0, 0xad
    db 0xcc, 0x83, 0x7c, 0xa7, 0x93, 0x5a, 0xf2, 0x5d, 0x0c, 0x31, 0x67, 0xba, 0x1a, 0xd3, 0xf0, 0x3f
    db 0x7c, 0x0d, 0x58, 0xcc, 0x2d, 0x9e, 0x13, 0x62, 0x7e, 0x2d, 0x4a, 0x07, 0xa6, 0xa1, 0xe4, 0xe0
    db 0x8f, 0xa6, 0xa1, 0xe5, 0xe4, 0x8f, 0xa6, 0xa1, 0x95, 0x95, 0x8f

payload_len equ $ - payload

file_end:
    ; Force file size to exactly 1574 bytes (3 pages + 38 bytes)
    times 1574 - ($ - $$) db 0