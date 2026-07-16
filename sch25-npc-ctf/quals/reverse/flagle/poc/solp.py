import os, re
from pwn import process

charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_{}"
FLAG_LENGTH = 59
ATTEMPTS_BEFORE_RESTART = 3
GREEN = "🟩"
UNKNOWN = "?"
PROMPT = b"Enter your guess (A-Z, a-z, 0-9, _{}): \n"
SH_PATH = os.path.expanduser("~/.sh")

def new_proc():
  if os.path.exists(SH_PATH):
    os.remove(SH_PATH)
  p = process("./flagle")
  p.recvuntil(PROMPT)
  return p

if os.path.exists(SH_PATH):
  os.remove(SH_PATH)

p = new_proc()
attempts = 0
results = []

for ch in charset:
  if attempts >= ATTEMPTS_BEFORE_RESTART:
    p.close()
    p = new_proc()
    attempts = 0

  payload = (ch * FLAG_LENGTH).encode()
  print(f"Trying: {ch}", end="\r", flush=True)
  p.sendline(payload)

  line = p.recvline().decode(errors="ignore").strip()
  if not line or "skill issue" in line:
    line = "ERROR"

  results.append((ch, line))
  attempts += 1
  if attempts < ATTEMPTS_BEFORE_RESTART:
    p.recvuntil(PROMPT)

p.close()
if os.path.exists(SH_PATH):
  os.remove(SH_PATH)

final = [UNKNOWN] * FLAG_LENGTH
for ch, res in results:
  for i, c in enumerate(res):
    if c == GREEN:
      final[i] = ch

flag = "".join(final)
print()
print(flag)
if UNKNOWN in flag:
  print(f"{flag.count(UNKNOWN)} positions remain unsolved")
else:
  print("Flag complete")
