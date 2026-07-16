from pwn import *
import os
import re
from base64 import b64decode

BINARY = './mov'
HOST = 'localhost'
PORT = 6767

DUMMY_KEY = "abcdefghijklmnopqrstuvwxyzGHIJKLMNOPQSUVWXYZ!@#$%^&*()[]{}"
KEY_LEN = 50

# --- custom base64 alphabet ---
CUSTOM_B64_ALPHABET = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+/"
STD_B64_ALPHABET    = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
B64_TRANS = str.maketrans(CUSTOM_B64_ALPHABET, STD_B64_ALPHABET)

def custom_b64_decode(s):
    try:
        s2 = s.translate(B64_TRANS)
        return b64decode(s2).decode(errors="ignore")
    except Exception:
        return None

def get_key(username):
    with open("gdb_inp", "w") as f:
        f.write(f"{username}\n{DUMMY_KEY}\n")

    io = process(['gdb', '-q', '-nx', BINARY], level='error')

    commands = [
        b'break strncmp',
        b'handle SIGSEGV nostop noprint pass',
        b'handle SIGILL nostop noprint pass',
        b'run < gdb_inp'
    ]
    for cmd in commands:
        io.sendline(cmd)

    key_chars = list("ARACTF_" + "?" * 39 + "_7.0")

    while True:
        try:
            output = io.recvuntil(b'(gdb) ', timeout=2).decode('utf-8', 'ignore')
        except EOFError:
            break

        if "exited" in output:
            break

        if "Breakpoint 1" in output:
            io.sendline(b'x/s *(char **)($esp+4)')
            out1 = io.recvuntil(b'(gdb) ').decode('utf-8', 'ignore')

            io.sendline(b'x/s *(char **)($esp+8)')
            out2 = io.recvuntil(b'(gdb) ').decode('utf-8', 'ignore')

            s1 = parse_string(out1)
            s2 = parse_string(out2)

            if s1 and s2:
                d1 = custom_b64_decode(s1)
                d2 = custom_b64_decode(s2)

                v1 = d1 if d1 else s1
                v2 = d2 if d2 else s2

                if v1 in DUMMY_KEY:
                    idx = DUMMY_KEY.find(v1)
                    patch_key(key_chars, idx, v2)
                elif v2 in DUMMY_KEY:
                    idx = DUMMY_KEY.find(v2)
                    patch_key(key_chars, idx, v1)

            io.sendline(b'finish')
            io.recvuntil(b'(gdb) ')
            io.sendline(b'set $eax = 0')
            io.recvuntil(b'(gdb) ')
            io.sendline(b'c')

    io.close()
    return "".join(key_chars)

def parse_string(output):
    start = output.find('"')
    if start == -1:
        return None
    end = output.find('"', start + 1)
    if end == -1:
        return None
    return output[start + 1:end]

def patch_key(chars, idx, s):
    for i, c in enumerate(s):
        if idx + i < len(chars):
            chars[idx + i] = c

def main():
    if not os.path.exists(BINARY):
        print(f"[-] Binary {BINARY} not found.")
        return

    r = remote(HOST, PORT)

    while True:
        try:
            buffer = r.recvuntil(b'enter key: ').decode()
            print(buffer, end='')

            round_match = re.search(r'#(\d+)', buffer)
            is_last_round = False
            if round_match:
                round_num = int(round_match.group(1))
                if round_num == 50:
                    is_last_round = True

            lines = [l.strip() for l in buffer.split('\n') if l.strip()]
            lines = [l for l in lines if "enter key:" not in l]

            if lines:
                username = lines[-1]

                key = get_key(username)
                print(key)

                r.sendline(key.encode())

                if is_last_round:
                    final_output = r.recvall(timeout=5).decode('utf-8', 'ignore')
                    print(final_output)
                    break

        except EOFError:
            print("\n[-] Connection closed unexpectedly.")
            break
        except KeyboardInterrupt:
            print("\n[-] Interrupted.")
            break

if __name__ == "__main__":
    main()
