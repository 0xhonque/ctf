# Proof of Concept

---

TL;DR - Obfuscated, Dotnet/C#, Decrypt

# Writeup

---

Lot's of ways to extract the executable without actually posing any real threats on your device. You can see that the binary is so huge which is probably the first indicator that this is a dotnet published with Publish Single File.

After that you can either use [sfextract](https://www.nuget.org/packages/sfextract/) (my intended way) or just use ILspy (shout out to @grb for telling me this) to see the dotnet IL and see that it's obfuscated

Universal dotnet deobfuscator is the goated [de4dot](https://github.com/kant2002/de4dot). This should give you enough idea on how to make a decryptor. Which is infact super easy because everything you need to decrypt the file is in the encrypted file itself

See [Decryptor](Program.cs)