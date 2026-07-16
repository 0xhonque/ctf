import struct

XTEA_KEY = b"ARA7XTEAKEY12345" 
XORSHIFT_SEED = 0xACE1

# =========================
# SPLIT FLAG
# =========================
p1 = b"FLARE-ON"
p2 = b"DOT COM ISNT JUST FOR WEBSITE BTW. MAYBE PIECE TOGETHER WHERE YOU ARE CURRENTLY AND THAT HINT???"
p3 = b"HMMM I WONDER WHERE PARTS 1 AND 2 IS..."

# =========================
# XORSHIFT16
# =========================
def xorshift16(x):
    x &= 0xFFFF
    x ^= ((x << 7) & 0xFFFF)
    x ^= (x >> 9)
    x ^= ((x << 8) & 0xFFFF)
    return x & 0xFFFF


def xorshift_keystream(seed, length):
    state = seed
    stream = []

    for _ in range(length):
        state = xorshift16(state)
        stream.append(state & 0xFF)

    return stream


def xorshift_encrypt(data: bytes, seed=0xACE1):
    ks = xorshift_keystream(seed, len(data))
    return bytes([b ^ k for b, k in zip(data, ks)])


# =========================
# CUSTOM TEA
# =========================
def custom_tea_encrypt_block(y, z, key, rounds=32):
    sum_ = 0x9E3779B9
    delta = 0x9E3779B9
    
    k0, k1, k2, k3 = key

    for _ in range(rounds):
        term1 = (((z << 4) & 0xFFFFFFFF) + k0) & 0xFFFFFFFF
        term2 = (z + sum_) & 0xFFFFFFFF
        term3 = (((z >> 5) & 0xFFFFFFFF) + k1) & 0xFFFFFFFF
        y = (y + (term1 ^ term2 ^ term3)) & 0xFFFFFFFF

        term1 = (((y << 4) & 0xFFFFFFFF) + k2) & 0xFFFFFFFF
        term2 = (y + sum_) & 0xFFFFFFFF
        term3 = (((y >> 5) & 0xFFFFFFFF) + k3) & 0xFFFFFFFF
        z = (z + (term1 ^ term2 ^ term3)) & 0xFFFFFFFF
        
        sum_ = (sum_ + delta) & 0xFFFFFFFF

    return y, z


def custom_tea_encrypt(data: bytes, key: bytes):
    key_words = struct.unpack("<4I", key)

    if len(data) % 8 != 0:
        data += b'\x00' * (8 - len(data) % 8)

    out = b''

    for i in range(0, len(data), 8):
        y, z = struct.unpack("<2I", data[i:i+8])
        y, z = custom_tea_encrypt_block(y, z, key_words)
        out += struct.pack("<2I", y, z)

    return out


# =========================
# ENCRYPT
# =========================
cipher1 = custom_tea_encrypt(p1, XTEA_KEY)
cipher2 = xorshift_encrypt(p2, XORSHIFT_SEED)


# =========================
# ASM FORMATTERS
# =========================
def to_db(data):
    return "db " + ",".join(f"0x{b:02X}" for b in data)

def to_dw(data):
    words = struct.unpack("<" + "H"*(len(data)//2), data)
    return "dw " + ",".join(f"0x{w:04X}" for w in words)

def to_dd(data):
    dwords = struct.unpack("<" + "I"*(len(data)//4), data)
    return "dd " + ",".join(f"0x{d:08X}" for d in dwords)

def to_dd_key(key):
    dd_list = []
    for i in range(0, len(key), 4):
        chunk = key[i:i+4]
        val = struct.unpack("<I", chunk)[0]
        dd_list.append(f"0x{val:08X}")
    return "\n".join(f"key{i} dd {v}" for i, v in enumerate(dd_list))


# =========================
# OUTPUT
# =========================

print("\n========== PART 1 (COM / CUSTOM TEA) ==========")
print("Plaintext:")
print(p1.decode())
print("\nCiphertext (paste into com.asm):")
ct_y = cipher1[:4]
ct_z = cipher1[4:]
print(f"ct_y {to_dd(ct_y)}")
print(f"ct_z {to_dd(ct_z)}")
print("\nKey (paste into com.asm):")
print(to_dd_key(XTEA_KEY))

print("\n========== PART 2 (DOS / XORSHIFT) ==========")
print("Plaintext:")
print(p2.decode())
print("\nCiphertext (paste into dos.asm):")
print(f"stored {to_db(cipher2)}")
print(f"\nSeed: dw 0x{XORSHIFT_SEED:04X}h")

print("\n========== PART 3 (EXE) ==========")
print("Plaintext (for your EXE checker):")
print(p3.decode())