key = "reverse_engineering"
flag = "compit{kirain_virtual_machine_kaya_vmware_atau_virtual_box}"

for i in range(len(flag)):
  if i % 2 == 0:
    R0 = hex(ord(flag[i]) ^ ord(key[i % len(key)]))
    R1 = hex(ord(key[i % len(key)]))

    print("\n; " + flag[i])
    print("INPUT R0")
    print("MOV R1, " + R1)
    print("XOR R0, R1")
    print("CMP R0, " + R0)
    print("JNE fail")

  else:
    R0 = hex(ord(flag[i]) + ord(key[i % len(key)]))
    R1 = hex(ord(key[i % len(key)]))

    print("\n; " + flag[i])
    print("INPUT R0")
    print("MOV R1, " + R1)
    print("ADD R0, R1")
    print("CMP R0, " + R0)
    print("JNE fail")