#!/usr/bin/env python3
import argparse
import mmap
import sys
import os

# The NOP instruction for x86-64
NOP = 0x90

class RawRegion:
    """A simple helper class to mimic the section object for our functions."""
    def __init__(self, name, offset, size):
        self.name = name
        # We mimic the structure the deobfuscation functions expect
        self.header = {'sh_offset': offset, 'sh_size': size}

def deob_true(data, section):
    """
    Deobfuscates the "Always True" forward jump.
    Signature: PUSHFQ; CMP RSP, RSP; JZ rel8; <junk>; POPFQ
    Bytes:     9C 48 39 E4 74 ?? <junk bytes> 9D
    Action:    Replaces the entire sequence with NOPs.
    """
    count = 0
    pattern = b'\x9C\x48\x39\xE4\x74'
    offset = section.header['sh_offset']
    size = section.header['sh_size']
    
    i = 0
    while i < size - 7: # Minimum size of this pattern is 8 bytes
        idx = data.find(pattern, offset + i, offset + size)
        if idx == -1:
            break
        
        junk_len = data[idx + 5]
        total_len = len(pattern) + 1 + junk_len + 1 # pattern + rel8 + junk + POPFQ

        # Sanity check: ensure the pattern is within bounds and ends with POPFQ
        if idx + total_len -1 < offset + size and data[idx + total_len - 1] == 0x9D:
            print(f"[*] Found 'gen_true' obfuscation at offset 0x{idx:x} (len={total_len})")
            for j in range(total_len):
                data[idx + j] = NOP
            count += 1
            i = (idx - offset) + total_len
        else:
            i = (idx - offset) + 1 # Move past the false positive
            
    return count

def deob_false(data, section):
    """
    Deobfuscates the "Always False" forward jump.
    Signature: PUSHFQ; CMP RSP, RSP; JNZ rel8; <nops>; POPFQ
    Bytes:     9C 48 39 E4 75 ?? <nop bytes> 9D
    Action:    Replaces the entire sequence with NOPs.
    """
    count = 0
    pattern = b'\x9C\x48\x39\xE4\x75'
    offset = section.header['sh_offset']
    size = section.header['sh_size']

    i = 0
    while i < size - 7:
        idx = data.find(pattern, offset + i, offset + size)
        if idx == -1:
            break

        junk_len = data[idx + 5]
        total_len = len(pattern) + 1 + junk_len + 1 # pattern + rel8 + junk + POPFQ
        
        if idx + total_len - 1 < offset + size and data[idx + total_len - 1] == 0x9D:
            print(f"[*] Found 'gen_false' obfuscation at offset 0x{idx:x} (len={total_len})")
            for j in range(total_len):
                data[idx + j] = NOP
            count += 1
            i = (idx - offset) + total_len
        else:
            i = (idx - offset) + 1
            
    return count

def deob_plus(data, section):
    """
    Deobfuscates the jump over one byte.
    Signature: JMP +1; <junk_byte>
    Bytes:     EB 01 ??
    Action:    Replaces the 3-byte sequence with NOPs.
    """
    count = 0
    pattern = b'\xEB\x01'
    offset = section.header['sh_offset']
    size = section.header['sh_size']

    i = 0
    while i < size - 2:
        idx = data.find(pattern, offset + i, offset + size)
        if idx == -1:
            break
        
        print(f"[*] Found 'gen_plus' obfuscation at offset 0x{idx:x}")
        data[idx] = NOP
        data[idx+1] = NOP
        data[idx+2] = NOP # NOP out the junk byte as well
        count += 1
        i = (idx - offset) + 3
        
    return count

def deob_misalign(data, section):
    """
    Deobfuscates the backward jump mis-alignment trick.
    Signature: JMP -1; PUSH RAX (encoded as FF F0); POP RAX
    Bytes:     EB FF F0 58
    Action:    Replaces the 4-byte sequence with NOPs.
    """
    count = 0
    pattern = b'\xEB\xFF\xF0\x58'
    offset = section.header['sh_offset']
    size = section.header['sh_size']

    i = 0
    while i < size - 3:
        idx = data.find(pattern, offset + i, offset + size)
        if idx == -1:
            break
            
        print(f"[*] Found 'gen_misalign' obfuscation at offset 0x{idx:x}")
        data[idx:idx+4] = bytes([NOP] * 4)
        count += 1
        i = (idx - offset) + 4
        
    return count

def deob_non(data, section):
    """
    Deobfuscates the assembly wrapping trick.
    Signature: PUSH RAX; <MOV-based core block>; POP RAX
    Action:    Identifies the full structure and replaces with NOPs.
    """
    count = 0
    valid_lengths = [4, 5, 6, 7, 8, 9]
    offset = section.header['sh_offset']
    size = section.header['sh_size']

    for i in range(offset, offset + size):
        # Check for PUSH RAX
        if data[i] != 0x50:
            continue
        
        for length in valid_lengths:
            total_len = length + 2 # PUSH + core_block + POP
            if i + total_len > offset + size:
                continue

            if data[i + total_len - 1] != 0x58: # POP RAX
                continue
            
            if data[i + total_len - 3] != 0xEB: # Trigger jump
                continue
            
            jump_operand = data[i + total_len - 2]
            expected_operand = (0x100 - length) & 0xFF
            
            if jump_operand == expected_operand:
                print(f"[*] Found 'gen_non' obfuscation at offset 0x{i:x} (len={total_len})")
                data[i : i + total_len] = bytes([NOP] * total_len)
                count += 1
                i += total_len - 1
                break
    return count

def main():
    parser = argparse.ArgumentParser(description="A deobfuscator for specific x86-64 obfuscation patterns in raw binary files.")
    parser.add_argument("file", help="The path to the raw binary file (e.g., shellcode) to deobfuscate.")
    parser.add_argument("-w", "--write", action="store_true", help="Write changes to the file. Without this flag, runs in dry-run mode.")
    args = parser.parse_args()

    try:
        file_size = os.path.getsize(args.file)
        if file_size == 0:
            print("[!] Input file is empty.")
            return

        with open(args.file, "r+b") as f:
            # Memory-map the file for efficient reading and writing
            access_mode = mmap.ACCESS_WRITE if args.write else mmap.ACCESS_READ
            with mmap.mmap(f.fileno(), 0, access=access_mode) as mm:
                total_found = 0
                
                # Create a "pseudo-section" that represents the entire file
                full_file_region = RawRegion("Entire File", 0, file_size)

                # List of deobfuscation functions to run
                deob_funcs = [
                    deob_true, deob_false, deob_plus, deob_misalign, deob_non,
                ]

                print(f"--- Starting Scan on {args.file} ({file_size} bytes) ---")
                for func in deob_funcs:
                    found = func(mm, full_file_region)
                    if found > 0:
                        print(f"    - Patched {found} instance(s) of '{func.__name__}'")
                        total_found += found
                
                print("\n--- Scan Complete ---")
                if total_found == 0:
                    print("No known obfuscation patterns were found.")
                else:
                    if args.write:
                        print(f"Total of {total_found} patterns found and NOP'd. File has been modified.")
                    else:
                        print(f"Total of {total_found} patterns found. Run with -w to write changes to the file.")

    except FileNotFoundError:
        print(f"Error: File not found at '{args.file}'", file=sys.stderr)
    except Exception as e:
        print(f"An unexpected error occurred: {e}", file=sys.stderr)

if __name__ == "__main__":
    main()
