import os
import time
from pwn import *

context.log_level = 'error'

charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_{}"
FLAG_LENGTH = 59
OUTPUT_FILE = "results.txt"

with open(OUTPUT_FILE, "w") as f:
  f.write("--- Flagle Brute-Force Results ---\n")

attempt_count = 0

if os.path.exists(os.path.expanduser("~/.sh")):
  os.remove(os.path.expanduser("~/.sh"))

p = process("./flagle")
p.recvuntil(b"Enter your guess (A-Z, a-z, 0-9, _{}): \n")

for char_to_test in charset:
  if attempt_count >= 3:
    p.close()
    if os.path.exists(os.path.expanduser("~/.sh")):
      os.remove(os.path.expanduser("~/.sh"))
    
    p = process("./flagle")
    p.recvuntil(b"Enter your guess (A-Z, a-z, 0-9, _{}): \n")
    attempt_count = 0

  payload = (char_to_test * FLAG_LENGTH).encode('ascii')
  print(f"[*] Testing: '{char_to_test}'")
  p.sendline(payload)

  response_line = p.recvline().decode().strip()
  
  if "skill issue" in response_line or not response_line:
    response_line = "ERROR"
  
  with open(OUTPUT_FILE, "a") as f:
    f.write(f"Input: {payload.decode('ascii')} | Result: {response_line}\n")
  
  attempt_count += 1
  
  if attempt_count < 3:
    p.recvuntil(b"Enter your guess (A-Z, a-z, 0-9, _{}): \n")

print("[+] Data gathering complete.")
p.close()