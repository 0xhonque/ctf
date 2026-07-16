# Proof of Concept

## TL;DR

debug dengan memberikan breakpoint sebelum loop kedua dan melihat isi decoded

```
┌──(ctf㉿LAPTOP-09HKEQB6)-[~/…/easy/debug-me/source/debug-me]
└─$ pwndbg ./a.out
Reading symbols from ./a.out...
(No debugging symbols found in ./a.out)
pwndbg: loaded 190 pwndbg commands. Type pwndbg [filter] for a list.
pwndbg: created 13 GDB functions (can be used with print/break). Type help function to see them.
------- tip of the day (disable with set show-tips off) -------
Disable Pwndbg context information display with set context-sections ''
pwndbg> disas printflag
Dump of assembler code for function printflag:
   0x0000000000001149 <+0>:     push   rbp
   0x000000000000114a <+1>:     mov    rbp,rsp
   0x000000000000114d <+4>:     sub    rsp,0xa0
   0x0000000000001154 <+11>:    mov    QWORD PTR [rbp-0x98],rdi
   0x000000000000115b <+18>:    mov    DWORD PTR [rbp-0xc],0x26
   0x0000000000001162 <+25>:    mov    rax,QWORD PTR [rbp-0x98]
   0x0000000000001169 <+32>:    mov    rdi,rax
   0x000000000000116c <+35>:    call   0x1030 <strlen@plt>
   0x0000000000001171 <+40>:    mov    DWORD PTR [rbp-0x10],eax
   0x0000000000001174 <+43>:    mov    DWORD PTR [rbp-0x4],0x0
   0x000000000000117b <+50>:    jmp    0x11b7 <printflag+110>
   0x000000000000117d <+52>:    mov    eax,DWORD PTR [rbp-0x4]
   0x0000000000001180 <+55>:    cdqe
   0x0000000000001182 <+57>:    lea    rdx,[rip+0xe8f]        # 0x2018
   0x0000000000001189 <+64>:    movzx  ecx,BYTE PTR [rax+rdx*1]
   0x000000000000118d <+68>:    mov    eax,DWORD PTR [rbp-0x4]
   0x0000000000001190 <+71>:    cdq
   0x0000000000001191 <+72>:    idiv   DWORD PTR [rbp-0x10]
   0x0000000000001194 <+75>:    mov    eax,edx
   0x0000000000001196 <+77>:    movsxd rdx,eax
   0x0000000000001199 <+80>:    mov    rax,QWORD PTR [rbp-0x98]
   0x00000000000011a0 <+87>:    add    rax,rdx
   0x00000000000011a3 <+90>:    movzx  eax,BYTE PTR [rax]
   0x00000000000011a6 <+93>:    xor    ecx,eax
   0x00000000000011a8 <+95>:    mov    edx,ecx
   0x00000000000011aa <+97>:    mov    eax,DWORD PTR [rbp-0x4]
   0x00000000000011ad <+100>:   cdqe
   0x00000000000011af <+102>:   mov    BYTE PTR [rbp+rax*1-0x50],dl
   0x00000000000011b3 <+106>:   add    DWORD PTR [rbp-0x4],0x1
   0x00000000000011b7 <+110>:   mov    eax,DWORD PTR [rbp-0x4]
   0x00000000000011ba <+113>:   cmp    eax,DWORD PTR [rbp-0xc]
   0x00000000000011bd <+116>:   jl     0x117d <printflag+52>
   0x00000000000011bf <+118>:   mov    eax,DWORD PTR [rbp-0xc]
   0x00000000000011c2 <+121>:   cdqe
   0x00000000000011c4 <+123>:   mov    BYTE PTR [rbp+rax*1-0x50],0x0
   0x00000000000011c9 <+128>:   mov    DWORD PTR [rbp-0x8],0x0
   0x00000000000011d0 <+135>:   jmp    0x11e3 <printflag+154>
   0x00000000000011d2 <+137>:   mov    eax,DWORD PTR [rbp-0x8]
   0x00000000000011d5 <+140>:   cdqe
   0x00000000000011d7 <+142>:   mov    BYTE PTR [rbp+rax*1-0x90],0x0
   0x00000000000011df <+150>:   add    DWORD PTR [rbp-0x8],0x1
   0x00000000000011e3 <+154>:   mov    eax,DWORD PTR [rbp-0x8]
   0x00000000000011e6 <+157>:   cmp    eax,DWORD PTR [rbp-0xc]
   0x00000000000011e9 <+160>:   jl     0x11d2 <printflag+137>
   0x00000000000011eb <+162>:   mov    eax,DWORD PTR [rbp-0xc]
   0x00000000000011ee <+165>:   cdqe
   0x00000000000011f0 <+167>:   mov    BYTE PTR [rbp+rax*1-0x90],0x0
   0x00000000000011f8 <+175>:   lea    rax,[rbp-0x90]
   0x00000000000011ff <+182>:   mov    rsi,rax
   0x0000000000001202 <+185>:   lea    rax,[rip+0xdff]        # 0x2008
   0x0000000000001209 <+192>:   mov    rdi,rax
   0x000000000000120c <+195>:   mov    eax,0x0
   0x0000000000001211 <+200>:   call   0x1040 <printf@plt>
   0x0000000000001216 <+205>:   nop
   0x0000000000001217 <+206>:   leave
   0x0000000000001218 <+207>:   ret
End of assembler dump.
pwndbg> break printflag
Breakpoint 1 at 0x114d
pwndbg> run
Starting program: /home/ctf/probset/ctfcompit25/rev101/easy/debug-me/source/debug-me/a.out
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".

Breakpoint 1, 0x000055555555514d in printflag ()
LEGEND: STACK | HEAP | CODE | DATA | WX | RODATA
─────────────────────────────────[ REGISTERS / show-flags off / show-compact-regs off ]─────────────────────────────────
 RAX  0x55555555603f ◂— 'debugger'
 RBX  0x7fffffffdd58 —▸ 0x7fffffffe036 ◂— '/home/ctf/probset/ctfcompit25/rev101/easy/debug-me/source/debug-me/a.out'
 RCX  0x555555557dd8 (__do_global_dtors_aux_fini_array_entry) —▸ 0x555555555100 (__do_global_dtors_aux) ◂— endbr64
 RDX  0x7fffffffdd68 —▸ 0x7fffffffe07f ◂— 0x5245545f5353454c ('LESS_TER')
 RDI  0x55555555603f ◂— 'debugger'
 RSI  0x7fffffffdd58 —▸ 0x7fffffffe036 ◂— '/home/ctf/probset/ctfcompit25/rev101/easy/debug-me/source/debug-me/a.out'
 R8   0
 R9   0x7ffff7fcbc20 ◂— push rbp
 R10  0x7fffffffd980 ◂— 0x800000
 R11  0x206
 R12  0
 R13  0x7fffffffdd68 —▸ 0x7fffffffe07f ◂— 0x5245545f5353454c ('LESS_TER')
 R14  0x7ffff7ffd000 (_rtld_global) —▸ 0x7ffff7ffe310 —▸ 0x555555554000 ◂— 0x10102464c457f
 R15  0x555555557dd8 (__do_global_dtors_aux_fini_array_entry) —▸ 0x555555555100 (__do_global_dtors_aux) ◂— endbr64
 RBP  0x7fffffffdc30 —▸ 0x7fffffffdc40 ◂— 1
 RSP  0x7fffffffdc30 —▸ 0x7fffffffdc40 ◂— 1
 RIP  0x55555555514d (printflag+4) ◂— sub rsp, 0xa0
──────────────────────────────────────────[ DISASM / x86-64 / set emulate on ]──────────────────────────────────────────
 ► 0x55555555514d <printflag+4>      sub    rsp, 0xa0                       RSP => 0x7fffffffdb90 (0x7fffffffdc30 - 0xa0)
   0x555555555154 <printflag+11>     mov    qword ptr [rbp - 0x98], rdi     [0x7fffffffdb98] <= 0x55555555603f ◂— 'debugger'
   0x55555555515b <printflag+18>     mov    dword ptr [rbp - 0xc], 0x26     [0x7fffffffdc24] <= 0x26
   0x555555555162 <printflag+25>     mov    rax, qword ptr [rbp - 0x98]     RAX, [0x7fffffffdb98] => 0x55555555603f ◂— 'debugger'
   0x555555555169 <printflag+32>     mov    rdi, rax                        RDI => 0x55555555603f ◂— 'debugger'
   0x55555555516c <printflag+35>     call   strlen@plt                  <strlen@plt>

   0x555555555171 <printflag+40>     mov    dword ptr [rbp - 0x10], eax
   0x555555555174 <printflag+43>     mov    dword ptr [rbp - 4], 0
   0x55555555517b <printflag+50>     jmp    printflag+110               <printflag+110>
    ↓
   0x5555555551b7 <printflag+110>    mov    eax, dword ptr [rbp - 4]
   0x5555555551ba <printflag+113>    cmp    eax, dword ptr [rbp - 0xc]
───────────────────────────────────────────────────────[ STACK ]────────────────────────────────────────────────────────
00:0000│ rbp rsp 0x7fffffffdc30 —▸ 0x7fffffffdc40 ◂— 1
01:0008│+008     0x7fffffffdc38 —▸ 0x55555555522c (main+19) ◂— mov eax, 0
02:0010│+010     0x7fffffffdc40 ◂— 1
03:0018│+018     0x7fffffffdc48 —▸ 0x7ffff7ddaca8 ◂— mov edi, eax
04:0020│+020     0x7fffffffdc50 —▸ 0x7fffffffdd40 —▸ 0x7fffffffdd48 ◂— 0x38 /* '8' */
05:0028│+028     0x7fffffffdc58 —▸ 0x555555555219 (main) ◂— push rbp
06:0030│+030     0x7fffffffdc60 ◂— 0x155554040
07:0038│+038     0x7fffffffdc68 —▸ 0x7fffffffdd58 —▸ 0x7fffffffe036 ◂— '/home/ctf/probset/ctfcompit25/rev101/easy/debug-me/source/debug-me/a.out'
─────────────────────────────────────────────────────[ BACKTRACE ]──────────────────────────────────────────────────────
 ► 0   0x55555555514d printflag+4
   1   0x55555555522c main+19
   2   0x7ffff7ddaca8 None
   3   0x7ffff7ddad65 __libc_start_main+133
   4   0x555555555081 _start+33
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
pwndbg> break *printflag+123
Breakpoint 2 at 0x5555555551c4
pwndbg> continue
Continuing.

Breakpoint 2, 0x00005555555551c4 in printflag ()
LEGEND: STACK | HEAP | CODE | DATA | WX | RODATA
─────────────────────────────────[ REGISTERS / show-flags off / show-compact-regs off ]─────────────────────────────────
*RAX  0x26
 RBX  0x7fffffffdd58 —▸ 0x7fffffffe036 ◂— '/home/ctf/probset/ctfcompit25/rev101/easy/debug-me/source/debug-me/a.out'
*RCX  0x7d
*RDX  0x7d
 RDI  0x55555555603f ◂— 'debugger'
 RSI  0x7fffffffdd58 —▸ 0x7fffffffe036 ◂— '/home/ctf/probset/ctfcompit25/rev101/easy/debug-me/source/debug-me/a.out'
 R8   0
 R9   0x7ffff7fcbc20 ◂— push rbp
*R10  3
*R11  0x7ffff7f18100 ◂— mov eax, edi
 R12  0
 R13  0x7fffffffdd68 —▸ 0x7fffffffe07f ◂— 0x5245545f5353454c ('LESS_TER')
 R14  0x7ffff7ffd000 (_rtld_global) —▸ 0x7ffff7ffe310 —▸ 0x555555554000 ◂— 0x10102464c457f
 R15  0x555555557dd8 (__do_global_dtors_aux_fini_array_entry) —▸ 0x555555555100 (__do_global_dtors_aux) ◂— endbr64
 RBP  0x7fffffffdc30 —▸ 0x7fffffffdc40 ◂— 1
*RSP  0x7fffffffdb90 ◂— 0x100
*RIP  0x5555555551c4 (printflag+123) ◂— mov byte ptr [rbp + rax - 0x50], 0
──────────────────────────────────────────[ DISASM / x86-64 / set emulate on ]──────────────────────────────────────────
 ► 0x5555555551c4 <printflag+123>    mov    byte ptr [rbp + rax - 0x50], 0     [0x7fffffffdc06] <= 0
   0x5555555551c9 <printflag+128>    mov    dword ptr [rbp - 8], 0             [0x7fffffffdc28] <= 0
   0x5555555551d0 <printflag+135>    jmp    printflag+154               <printflag+154>
    ↓
   0x5555555551e3 <printflag+154>    mov    eax, dword ptr [rbp - 8]           EAX, [0x7fffffffdc28] => 0
   0x5555555551e6 <printflag+157>    cmp    eax, dword ptr [rbp - 0xc]         0x0 - 0x26     EFLAGS => 0x293 [ CF pf AF zf SF IF df of ac ]
   0x5555555551e9 <printflag+160>  ✔ jl     printflag+137               <printflag+137>
    ↓
   0x5555555551d2 <printflag+137>    mov    eax, dword ptr [rbp - 8]           EAX, [0x7fffffffdc28] => 0
   0x5555555551d5 <printflag+140>    cdqe
   0x5555555551d7 <printflag+142>    mov    byte ptr [rbp + rax - 0x90], 0     [0x7fffffffdba0] <= 0
   0x5555555551df <printflag+150>    add    dword ptr [rbp - 8], 1             [0x7fffffffdc28] <= 1 (0 + 1)
   0x5555555551e3 <printflag+154>    mov    eax, dword ptr [rbp - 8]           EAX, [0x7fffffffdc28] => 1
───────────────────────────────────────────────────────[ STACK ]────────────────────────────────────────────────────────
00:0000│ rsp 0x7fffffffdb90 ◂— 0x100
01:0008│-098 0x7fffffffdb98 —▸ 0x55555555603f ◂— 'debugger'
02:0010│-090 0x7fffffffdba0 ◂— 0x40 /* '@' */
03:0018│-088 0x7fffffffdba8 ◂— 0xc /* '\x0c' */
04:0020│-080 0x7fffffffdbb0 ◂— 0x140000
05:0028│-078 0x7fffffffdbb8 ◂— 0xffffffffffffffff
06:0030│-070 0x7fffffffdbc0 ◂— 0x40 /* '@' */
07:0038│-068 0x7fffffffdbc8 ◂— 0xa /* '\n' */
─────────────────────────────────────────────────────[ BACKTRACE ]──────────────────────────────────────────────────────
 ► 0   0x5555555551c4 printflag+123
   1   0x55555555522c main+19
   2   0x7ffff7ddaca8 None
   3   0x7ffff7ddad65 __libc_start_main+133
   4   0x555555555081 _start+33
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
pwndbg> x/s $rbp-0x50
0x7fffffffdbe0: "compit{runtime_debugging_shenaniggans}"
pwndbg>
```

### Flag:

> compit{runtime_debugging_shenaniggans}