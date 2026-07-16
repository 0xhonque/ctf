; assemble: nasm -f elf32 shellcodeme.asm -o shellcodeme.o
; link (for quick test): ld -m elf_i386 shellcodeme.o -o shellcode
; to extract bytes: objcopy -O binary -j .text shellcode shellcodeme && xxd -i shellcodeme

section .text
global _start

_start:
	jmp short get_data	   ; jump to data, decoder is next

decoder:
	pop esi				  ; esi -> pointer to data (PC-relative)
	mov ebx, esi			 ; preserve start in ebx for later (we'll use ebx as pointer)
	mov cl, flag_len		 ; counter = length (use cl so imm8 only)
	mov edi, esi
	dec edi				  ; edi = start - 1 (setup for inc edi/stosb trick)

	; ---------- DECODE LOOP ----------
decode_loop:
	lodsb					; AL = [ESI] ; ESI++
	xor al, 0xED			 ; decode key
	inc edi				  ; point edi to destination
	stosb					; [EDI] = AL ; EDI++
	dec cl
	jnz decode_loop

	; re-prepare for re-encode: reset counter and pointers
	mov cl, flag_len		 ; reload counter (imm8)
	mov esi, ebx			 ; esi = start again
	mov edi, esi
	dec edi				  ; edi = start - 1

	; ---------- RE-ENCODE LOOP ----------
reenc_loop:
	lodsb					; AL = [ESI] ; ESI++
	xor al, 0x55			 ; re-encode key
	inc edi
	stosb
	dec cl
	jnz reenc_loop

	; ---------- write(1, ebx, flag_len) ----------
	xor eax, eax
	inc eax
	inc eax
	inc eax
	inc eax				   ; eax = 4  (sys_write)
	xor edx, edx
	mov dl, flag_len		 ; edx = length (imm8)
	xor ecx, ecx
	mov ecx, ebx			 ; ecx = pointer to start (preserved in ebx)
	xor ebx, ebx
	inc bl				   ; ebx = 1 (stdout fd)
	int 0x80

	; exit(0)
	xor eax, eax
	inc eax				  ; eax = 1 (sys_exit)
	xor ebx, ebx
	int 0x80

get_data:
	call decoder

; ---------------- data blob ----------------
; plaintext: "compit{kode_cangkang_simple_aja_yagesya}"
; encrypted with 0xED (this produces no 0x00 bytes)
flag:
	db 0x8e,0x82,0x80,0x9d,0x84,0x99,0x96,0x86,0x82,0x89,0x88,0xb2,0x8e,0x8c,0x83,0x8a,0x86,0x8c,0x83,0x8a,0xb2,0x9e,0x84,0x80,0x9d,0x81,0x88,0xb2,0x8c,0x87,0x8c,0xb2,0x94,0x8c,0x8a,0x88,0x9e,0x94,0x8c,0x90
flag_len equ $ - flag
