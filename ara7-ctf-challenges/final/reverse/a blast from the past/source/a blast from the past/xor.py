import sys

def xor_data(data, key):
    return bytes([b ^ key for b in data])

def format_db(data, per_line=16):
    lines = []
    for i in range(0, len(data), per_line):
        chunk = data[i:i+per_line]
        hex_bytes = ", ".join(f"0x{b:02x}" for b in chunk)
        lines.append(f"    db {hex_bytes}")
    return "\n".join(lines)

def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <file> <xor_key_hex>")
        print(f"Example: {sys.argv[0]} payload.bin 0x55")
        sys.exit(1)

    filename = sys.argv[1]
    key = int(sys.argv[2], 16)

    if not (0 <= key <= 0xFF):
        print("Key must be a single byte (0x00-0xFF)")
        sys.exit(1)

    with open(filename, "rb") as f:
        data = f.read()

    xored = xor_data(data, key)

    if 0x00 in xored:
        print("[!] Warning: XOR result contains NULL bytes!")

    print(f"\n; XOR key = 0x{key:02x}")
    print("payload:")
    print(format_db(xored))
    print(f"\npayload_len equ $ - payload")

if __name__ == "__main__":
    main()
