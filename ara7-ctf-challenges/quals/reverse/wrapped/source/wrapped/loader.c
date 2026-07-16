// gcc -O2 -c -fPIC -fno-stack-protector -fno-builtin -fvisibility=hidden -fno-asynchronous-unwind-tables -ffunction-sections chall.c -o chall.o && ld -o chall.elf -T <(echo 'SECTIONS { . = 0; .text : { *(.text.entry) *(.text*) } .rodata : { *(.rodata*) } .data : { *(.data*) } .bss : { *(.bss*) } /DISCARD/ : { *(.eh_frame) *(.note*) *(.comment*) } }') chall.o && objcopy -O binary chall.elf chall.bin && python compress.py && xxd -i chall.z > chall.h && cat temp >> chall.h && rm temp && gcc -o loader loader.c -lz

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <zlib.h>
#include "chall.h"

typedef int (*shellcode)(const char *input);

int decompress_payload(void *dst, size_t dst_size, const unsigned char *src, size_t src_size) 
{
    z_stream strm;
    memset(&strm, 0, sizeof(strm));

    strm.next_in   = (Bytef *)src;
    strm.avail_in  = src_size;
    strm.next_out  = (Bytef *)dst;
    strm.avail_out = dst_size;

    if (inflateInit(&strm) != Z_OK) return -1;

    int ret = inflate(&strm, Z_FINISH);
    inflateEnd(&strm);

    return (ret == Z_STREAM_END) ? 0 : -1;
}

int main(int argc, char **argv)
{
    if (argc != 2) return 1;

    void *mem = mmap(NULL, CHALL_DECOMPRESSED_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (mem == MAP_FAILED) return 1;

    if (decompress_payload(mem, CHALL_DECOMPRESSED_SIZE, chall_z, chall_z_len) != 0) return 1;

    shellcode entry = (shellcode)mem;

    int result = entry(argv[1]);

    puts(result == 0 ? "jolly good show m8" : "get chinned m8");

    munmap(mem, CHALL_DECOMPRESSED_SIZE);
    return 0;
}
