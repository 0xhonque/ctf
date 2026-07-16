flag = "compit{perasaan_assembly_ga_kaya_gini_deh_bentukannya}"
key = "pytecode"

enc = ""

for i in range(len(flag)):
  x = ord(flag[i]) ^ ord(key[i % len(key)])
  x = (x + i) % 256
  enc += chr(x)

print(enc.encode().hex())