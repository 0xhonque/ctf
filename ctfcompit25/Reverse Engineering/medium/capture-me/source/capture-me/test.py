# helper.py
import sys

# Custom base64 alphabet (same as in C code)
b64_table = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+/"

def custom_b64_encode(data: bytes) -> str:
  res = []
  i = 0
  while i < len(data):
    arr3 = data[i:i+3]
    i += 3
    while len(arr3) < 3:
      arr3 += b'\x00'

    arr4 = [0]*4
    arr4[0] = (arr3[0] & 0xfc) >> 2
    arr4[1] = ((arr3[0] & 0x03) << 4) + ((arr3[1] & 0xf0) >> 4)
    arr4[2] = ((arr3[1] & 0x0f) << 2) + ((arr3[2] & 0xc0) >> 6)
    arr4[3] = arr3[2] & 0x3f

    for k in range(4):
      if i - (3 - k) > len(data):
        res.append('=')
      else:
        res.append(b64_table[arr4[k]])
  return ''.join(res)

def rc4(key: bytes, data: bytes) -> bytes:
  S = list(range(256))
  j = 0
  out = []

  # KSA
  for i in range(256):
    j = (j + S[i] + key[i % len(key)]) % 256
    S[i], S[j] = S[j], S[i]

  # PRGA
  i = j = 0
  for byte in data:
    i = (i + 1) % 256
    j = (j + S[i]) % 256
    S[i], S[j] = S[j], S[i]
    K = S[(S[i] + S[j]) % 256]
    out.append(byte ^ K)

  return bytes(out)

if __name__ == "__main__":
  if len(sys.argv) != 3:
    print("Usage: python helper.py <flag> <key>")
    sys.exit(1)

  flag = sys.argv[1].encode()
  key = sys.argv[2].encode()

  # Step 1: custom base64 encode the flag
  b64_flag = custom_b64_encode(flag).encode()

  # Step 2: RC4 encrypt the base64(flag)
  encrypted = rc4(key, b64_flag)

  # Step 3: custom base64 encode the key for obfuscation
  enc_key = custom_b64_encode(key)

  # Output as C string literals
  c_flag = ''.join(f"\\x{b:02x}" for b in encrypted)

  print(f'#define FLAG "{c_flag}"')
  print(f'#define ENC_KEY "{enc_key}"')
