flag = "13171b180e20251c1d141f210e1b1849211b191322221e3347372f55242b3b234f3f3f2e2e552627404f402b39483f3d41484c4f3647"
key = "pytecode"

user_input = ""
enc = ""

user_input = input("enter flag: ")

for i in range(len(user_input)):
  x = ord(user_input[i]) ^ ord(key[i % len(key)])
  x = (x + i) % 256
  enc += chr(x)

if enc.encode().hex() == flag:
    print("👍")
else:
    print("jangan dikasih tau 🤫")