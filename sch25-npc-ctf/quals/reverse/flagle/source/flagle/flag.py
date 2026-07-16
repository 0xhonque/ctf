import base64
import hashlib

flag = "SCH25{since_when_did_wordle_became_this_annoying__6675636b}"
key = "SchematicsCTF2025"

encrypted = bytearray(
  (b + ord(key[i % len(key)])) & 0xFF
  for i, b in enumerate(flag.encode('utf-8'))
)

def generate_position_table(position, key):
  hash_input = f"{key}_pos_{position}_v2".encode()
  hash_digest = hashlib.sha256(hash_input).digest()
  table = list(range(256))
  for i in range(255, 0, -1):
    j = (hash_digest[i % len(hash_digest)] + 
        (hash_digest[(i * 3) % len(hash_digest)] << 8)) % (i + 1)
    table[i], table[j] = table[j], table[i]
  return table

def position_encrypt(char_val, position, key):
  sub_table = generate_position_table(position, key)
  return sub_table[char_val]

encrypted2 = bytearray(
  position_encrypt(b, i, key)
  for i, b in enumerate(flag.encode('utf-8'))
)

custom_alphabet = "abcdefghijklnmopqrstuwvxyzABCDEFGHIJKLNMOPQRSTUWVXYZ0123456789+/"
standard_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
custom_hex = "0123456789abcdef"
standard_hex = "0123456789abcdef"

b64table = str.maketrans(standard_alphabet, custom_alphabet)
hextable = str.maketrans(standard_hex, custom_hex)

base64_encoded = base64.b64encode(encrypted2).decode('utf-8')
step1 = base64_encoded.translate(b64table).encode('utf-8')
step2 = step1.hex()
final = step2.translate(hextable).encode('utf-8')

def xor_string(s, key):
  encoded = [ord(c) ^ key for c in s]
  c_array = "{ " + ", ".join(f"0x{b:02x}" for b in encoded) + ", 0x00 };"
  return c_array

XOR_KEY = 0xAB

print(f"// XOR Key: 0x{XOR_KEY:02x}")
print("char flag_str_xored[] = " + xor_string(final.decode('utf-8'), XOR_KEY))
print("char key_str_xored[] = " + xor_string(key, XOR_KEY))
print("char table_str_xored[] = " + xor_string(custom_alphabet, XOR_KEY))
print("char hex_str_xored[] = " + xor_string(custom_hex, XOR_KEY))