import struct

def rotl32(x, n):
    return ((x << n) & 0xffffffff) | (x >> (32 - n))

def pack4(b):
    return struct.unpack('<I', bytes(b))[0]

def unpack4(v):
    return struct.pack('<I', v)

# XOR Obfuscation function from C code
def str_obf(s):
    return bytes([c ^ 41 for c in s])

# 1. Derive Keys and Constants
key_src = b"A Renewal Agent CaptureTheFlag 7" # 32 bytes
nonce_src = b"Proper Meltt"                 # 12 bytes
constant_src = b"$91 /%awvl#85$a*"            # 16 bytes

key = str_obf(key_src)
nonce = str_obf(nonce_src)
constant = str_obf(constant_src)

# 2. Initialize ChaCha20 State
state = [0] * 16
# Constant (Custom, not the standard "expand 32-byte k")
state[0] = pack4(constant[0:4])
state[1] = pack4(constant[4:8])
state[2] = pack4(constant[8:12])
state[3] = pack4(constant[12:16])
# Key
state[4] = pack4(key[0:4])
state[5] = pack4(key[4:8])
state[6] = pack4(key[8:12])
state[7] = pack4(key[12:16])
state[8] = pack4(key[16:20])
state[9] = pack4(key[20:24])
state[10] = pack4(key[24:28])
state[11] = pack4(key[28:32])
# Counter (0)
state[12] = 0
# Nonce (12 bytes)
state[13] = pack4(nonce[0:4])
state[14] = pack4(nonce[4:8])
state[15] = pack4(nonce[8:12])

# 3. Generate Keystream (1 Block)
x = list(state)

def qr(a, b, c, d):
    x[a] = (x[a] + x[b]) & 0xffffffff
    x[d] = rotl32(x[d] ^ x[a], 16)
    x[c] = (x[c] + x[d]) & 0xffffffff
    x[b] = rotl32(x[b] ^ x[c], 12)
    x[a] = (x[a] + x[b]) & 0xffffffff
    x[d] = rotl32(x[d] ^ x[a], 8)
    x[c] = (x[c] + x[d]) & 0xffffffff
    x[b] = rotl32(x[b] ^ x[c], 7)

for i in range(10):
    qr(0, 4, 8, 12); qr(1, 5, 9, 13); qr(2, 6, 10, 14); qr(3, 7, 11, 15)
    qr(0, 5, 10, 15); qr(1, 6, 11, 12); qr(2, 7, 8, 13); qr(3, 4, 9, 14)

for i in range(16):
    x[i] = (x[i] + state[i]) & 0xffffffff

keystream = b''.join([unpack4(w) for w in x])

# 4. Decrypt Data
encrypted_data = bytes([
    0xf0, 0x90, 0x61, 0x21, 0x00, 0x09, 0x44, 0x81, 0x81, 0x13, 0xf7, 0x65, 0x8d, 0xc7, 0xdd, 0x83,
    0x4a, 0x5e, 0xca, 0xa6, 0x56, 0x7d, 0x7d, 0xce, 0xff, 0x87, 0x95, 0x2a, 0x8d, 0xd9, 0x1d, 0x30,
    0xab, 0x3f, 0x57, 0xe7, 0xeb, 0x44, 0x23, 0xac, 0x49, 0x34, 0x6d, 0x01, 0xc1, 0xe4, 0x87, 0x69,
    0x0c, 0x3f
])

flag = bytes([d ^ k for d, k in zip(encrypted_data, keystream)])
print(flag.decode('utf-8'))
