import zlib

with open("chall.bin", "rb") as f:
    data = f.read()

compressed = zlib.compress(data, 9)

with open("chall.z", "wb") as f:
    f.write(compressed)

print("original:", len(data))
print("compressed:", len(compressed))

with open("temp", "wb") as f:
    f.write(f"\n#define CHALL_DECOMPRESSED_SIZE {len(data)}\n".encode())

