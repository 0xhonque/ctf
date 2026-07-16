The intended solution for this challenge is actually to blackbox (most other Movuscated challenges ive seen online are also intended to be solved via either side channel or blackbox like this one, see https://ctftime.org/writeup/12263) and after you understand the order of which the binary checks for the key, therefor the order of the keys, you can use the binary as the keygen.

If you wanna reverse the actual program and find out how the key is actually created, be my guest.

A little ammount of static analysis and bravery is needed here, as you need to know that the comparison is being done by `strncmp` and you need to identify from `strings` that the long string is for a custom base64. After that knowledge has been gained, getting a correct key for a username is as easy as debugging

One thing to note is that movfuscator uses a lot of signals for the trick which means that we need to make sure gdb doesnt stop when encountering these signals. These commands need to be used before running the binary:

> handle SIGSEGV nostop noprint pass

> handle SIGILL nostop noprint pass

After that we set a breakpoint on `strncmp`, and see what our input is being compared against. The checks are pretty scattered and non linear so the first check isnt for the first sections, and there are quite a few overlapping char checks, so be sure to use a string of different char to note which part is being compared against which string. Since the goal isnt to crackmes but rather to keygen, you can just use this method in gdb to act as your keygen and script. If you wanna do it manually, inputing 50 usernames and checking the stack everytime it hits a strncmp, be my guest.

> see [gdb](gdb.out) for an overview of debugging

> see [solver](solp.py)