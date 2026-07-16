# encrypted = "compit{REDACTED}"
data = "1106170a1d11180c1b19251e1d153c0c1b19251e1d153c0c1b1907"
key = "" # write the key here # rizztech

encrypted = bytes.fromhex(data)

flag = []
for i in range(len(encrypted)):
  flag.append(chr(encrypted[i] ^ ord(key[i % len(key)])))
  # flag.append(chr(ord(encrypted[i]) ^ ord(key[i % len(key)])))
# final = ''.join(flag).encode().hex()
final = ''.join(flag)

print(final)

# commented code is the generator