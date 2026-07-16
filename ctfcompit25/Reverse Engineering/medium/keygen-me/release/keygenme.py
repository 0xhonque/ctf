SECRET_KEY = "F2AC-58FD-9547-988E-46E5-9D25-EEBC-3AD5-BF51-E4B4"

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

def main():
  print("Welcome to CompitKey v1.0")
  guess_user = input("username: ").strip()
  guess_pass = input("password: ").strip()
  if make_key(guess_user, guess_pass) == SECRET_KEY:
    print("Correct! Flag: compit{" + guess_user + guess_pass + "}")
  else:
    print("Nope")

if __name__ == "__main__":
  main()
