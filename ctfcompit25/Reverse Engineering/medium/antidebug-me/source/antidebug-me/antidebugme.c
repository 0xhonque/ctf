// i686-w64-mingw32-gcc antidebugme.c -o antidebugme.exe
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <winternl.h>

unsigned char flag[] = {
  0x02, 0x01, 0x19, 0x19, 0x0d, 0x16, 0x1c, 0x0a, 0x07, 0x06, 
  0x08, 0x0d, 0x0c, 0x38, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x06, 
  0x02, 0x03, 0x1b, 0x13, 0x36, 0x0a, 0x1b, 0x06, 0x3e, 0x1b, 
  0x10, 0x08, 0x0c, 0x3d, 0x04, 0x14, 0x05, 0x01, 0x19, 0x3b, 
  0x09, 0x12, 0x00, 0x1a, 0x2b, 0x0c, 0x0c, 0x0a, 0x38, 0x15, 
  0x0b, 0x06, 0x07, 0x1d, 0x03, 0x13, 0x00, 0x31, 0x18, 0x08, 
  0x13, 0x03, 0x09, 0x0f, 0x17, 0x15, 0x36, 0x00, 0x07, 0x10, 
  0x00, 0x31, 0x06, 0x0c, 0x12, 0x07, 0x15, 0x12, 0x0b, 0x2b, 
  0x04, 0x05, 0x03, 0x17, 0x3e, 0x1e, 0x01, 0x01, 0x0c, 0x1f
};

#define key "YW50aWRiZw=="

BOOL IsDebuggedPEB() {
  unsigned int peb = 0;
  unsigned char being_debugged = 0;
  __asm__ __volatile__ ("movl %%fs:0x30, %0" : "=r" (peb));
  if (peb) being_debugged = *((unsigned char*)(peb + 2));
  return being_debugged != 0;
}

BOOL ExtraAPIChecks() {
  BOOL present = FALSE;
  if (IsDebuggerPresent()) return TRUE;
  CheckRemoteDebuggerPresent(GetCurrentProcess(), &present);
  if (present) return TRUE;
  SetLastError(0);
  OutputDebugStringA("probe");
  if (GetLastError() != 0) return TRUE;
  return FALSE;
}

BOOL NtQueryChecks() {
  HMODULE ntdll = GetModuleHandleA("ntdll.dll");
  if (!ntdll) return FALSE;
  typedef NTSTATUS (NTAPI *NtQIP_t)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);
  NtQIP_t NtQIP = (NtQIP_t)GetProcAddress(ntdll, "NtQueryInformationProcess");
  if (!NtQIP) return FALSE;

  ULONG_PTR debugPort = 0;
  if (NtQIP(GetCurrentProcess(), ProcessDebugPort, &debugPort, sizeof(debugPort), NULL) == 0 && debugPort != 0) return TRUE;

  HANDLE debugObj = NULL;
  if (NtQIP(GetCurrentProcess(), ProcessDebugObjectHandle, &debugObj, sizeof(debugObj), NULL) == 0 && debugObj != NULL) return TRUE;

  return FALSE;
}

BOOL TimingCheck() {
  LARGE_INTEGER a, b, freq;
  if (!QueryPerformanceFrequency(&freq)) return FALSE;
  QueryPerformanceCounter(&a);
  volatile int sink = 0;
  for (int i = 0; i < 200000; i++) sink += i;
  QueryPerformanceCounter(&b);
  double ms = (double)(b.QuadPart - a.QuadPart) * 1000.0 / (double)freq.QuadPart;
  return ms > 40.0;
}

BOOL DebuggerDetected() {
  if (IsDebuggedPEB()) return TRUE;
  if (ExtraAPIChecks()) return TRUE;
  if (NtQueryChecks()) return TRUE;
  if (TimingCheck()) return TRUE;
  return FALSE;
}

static const unsigned char base64_table[65] =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

unsigned char *base64_encode(const unsigned char *src, size_t len, size_t *out_len) {
  unsigned char *out, *pos;
  const unsigned char *end, *in;
  size_t olen;
  int line_len;

  olen = len * 4 / 3 + 4;
  olen += olen / 72;
  olen++;
  if (olen < len)
    return NULL;

  out = malloc(olen);
  if (out == NULL)
    return NULL;

  end = src + len;
  in = src;
  pos = out;
  line_len = 0;
  while (end - in >= 3) {
    *pos++ = base64_table[in[0] >> 2];
    *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
    *pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
    *pos++ = base64_table[in[2] & 0x3f];
    in += 3;
    line_len += 4;
    if (line_len >= 72) {
      *pos++ = '\n';
      line_len = 0;
    }
  }

  if (end - in) {
    *pos++ = base64_table[in[0] >> 2];
    if (end - in == 1) {
      *pos++ = base64_table[(in[0] & 0x03) << 4];
      *pos++ = '=';
    } else {
      *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
      *pos++ = base64_table[(in[1] & 0x0f) << 2];
    }
    *pos++ = '=';
    line_len += 4;
  }

  if (line_len)
    *pos++ = '\n';

  *pos = '\0';
  if (out_len)
    *out_len = pos - out;
  return out;
}

unsigned char *base64_decode(const unsigned char *src, size_t len, size_t *out_len) {
  unsigned char dtable[256], *out, *pos, block[4], tmp;
  size_t i, count, olen;
  int pad = 0;

  memset(dtable, 0x80, 256);
  for (i = 0; i < sizeof(base64_table) - 1; i++)
    dtable[base64_table[i]] = (unsigned char)i;
  dtable['='] = 0;

  count = 0;
  for (i = 0; i < len; i++) {
    if (dtable[src[i]] != 0x80)
      count++;
  }

  if (count == 0 || count % 4)
    return NULL;

  olen = count / 4 * 3;
  pos = out = malloc(olen);
  if (out == NULL)
    return NULL;

  count = 0;
  for (i = 0; i < len; i++) {
    tmp = dtable[src[i]];
    if (tmp == 0x80)
      continue;

    if (src[i] == '=')
      pad++;
    block[count] = tmp;
    count++;
    if (count == 4) {
      *pos++ = (block[0] << 2) | (block[1] >> 4);
      *pos++ = (block[1] << 4) | (block[2] >> 2);
      *pos++ = (block[2] << 6) | block[3];
      count = 0;
      if (pad) {
        if (pad == 1)
          pos--;
        else if (pad == 2)
          pos -= 2;
        else {
          free(out);
          return NULL;
        }
        break;
      }
    }
  }

  *out_len = pos - out;
  return out;
}

int encrypt(int x) {
  int random_offset = rand() % 256;
  return x ^ random_offset;
}

void printflag(char xor_key[]) {
  char decoded[100];
  int flag_len = sizeof(flag);
  int key_len = strlen(xor_key);
  
  for (int i = 0; i < flag_len; i++) {
    decoded[i] = flag[i] ^ xor_key[i % key_len];
  }
  decoded[flag_len] = '\0';

  char encoded[100];
  for (int i = 0; i < flag_len; i++) {
    int val = encrypt(decoded[i]);
    if (val == 0) val = '?';
    encoded[i] = (char)val;
  }
  encoded[flag_len] = '\0';

  printf("Flag: %s\n", encoded);
}

int main() {
  if (DebuggerDetected()) {
    puts("Maaf ya le tapi ga bisa segampang itu");
    fflush(stdout);

    FILE *f = fopen("antidbg", "w");
    if (f) {
      fputs("Maaf ya le tapi ga bisa segampang itu\n", f);
      fclose(f);
    }
    return 1;
  }

  size_t key_len;
  unsigned char *decoded = base64_decode(key, sizeof(key) - 1, &key_len);
  srand((unsigned) time(NULL));
  printflag(decoded);

  free(decoded);
  return 0;
}
