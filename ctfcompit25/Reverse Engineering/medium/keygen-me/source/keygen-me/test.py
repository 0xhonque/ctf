def pad_inputs(username: str, password: str):
  u = (username[:12]).ljust(12, "_")
  p = (password[:8]).ljust(8, "0")
  return (u + p).encode("utf-8")

def rotl8(x, n):
  return ((x << n) & 0xFF) | (x >> (8 - n))

def make_key(username: str, password: str) -> str:
  data = pad_inputs(username, password)
  mask = [0x13, 0x37, 0x42, 0x99, 0xAB, 0xCD]
  transformed = bytes([
    rotl8(b ^ mask[i % len(mask)], 1)
    for i, b in enumerate(data)
  ])
  hexed = transformed.hex().upper()
  return "-".join([hexed[i:i+4] for i in range(0, len(hexed), 4)])

if __name__ == "__main__":
  import sys
  if len(sys.argv) != 3:
    print("Usage: python test.py <username> <password>")
    exit(1)
  print(make_key(sys.argv[1], sys.argv[2]))

# jangan_pake_
# di_steam