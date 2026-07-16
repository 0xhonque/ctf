# Proof of Concept

## TL;DR

extract shellcode, decompile dengan ndisasm -b 32, pahami kalau shellcode melakukan xor dengan value 0xed, lalu xor semua shellcode dengan 0xed untuk dapat flagnya. Atau buat ulang logika shellcodenya

### Flag:

> compit{kode_cangkang_simple_aja_yagesya}