The challenge is a shellcode that is obfuscated with a technique known as assembly wrapping that tricks linear sweep disassembly, but when i tried it on a recursive decompiler like ghidra, it wasnt really that effective, which is why i used shellcode.

There are a few different types of assembly wrapping, a little reading is required but you can just check the [deobfuscator](deobfuscator.py)

The shellcode itself is not a complex one, its a flag checker that uses ChaCha20 with custom matrix initialization matrix constant

Just check my [solver](solp.py)