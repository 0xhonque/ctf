def make_ciphertext(flag: str, key: str):
  # Extend key to match flag length
  extended_key = (key * ((len(flag) // len(key)) + 1))[:len(flag)]

  # Convert both to byte arrays
  flag_bytes = [ord(c) for c in flag]
  key_bytes = [ord(c) for c in extended_key]

  # Compute ciphertext = flag ^ key
  arr1 = [f ^ k for f, k in zip(flag_bytes, key_bytes)]

  return arr1, key_bytes


# Example usage
flag = "compit{google_search_javascript_deobfuscator}"
key = "key"   # short key that will be repeated

arr1, arr2 = make_ciphertext(flag, key)

print("var _0x21512c =", [hex(x) for x in arr1], ";")
print("const _0x200ef9 =", [hex(x) for x in arr2], ";")
