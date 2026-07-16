// i686-w64-mingw32-gcc captureme.c -o captureme.exe -lws2_32
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define FLAG "\xac\x1f\x8f\xc5\xc8\x14\xce\xf4\x3a\x6a\x76\x3a\x79\xd6\x6b\xe4\xf6\x5d\x74\xc6\x4f\x4a\x42\xde\xb8\x1f\xa9\x0f\x1b\xd4\xe3\x18\xb3\x61\x5f\x93\x85\xb4\x5a\x49\x92\xf7\xd0\x61\x02\xd0\xcd\xc5\xee\x08\x0a\x90\xbe\x06\x7a\xfb\x6e\x68\xd8\xd9\xe9\x31\x3f\xbb\xf6\x0e\x8c\xda\xa3\x3b\xbf\x4e\x5c\xf3\xcc\x8d\x07\x16\xad\x62"
// compit{jangan_tangkap_flag_doang_tapi_tangkap_juga_paketnya}

#define ENC_KEY "CML6ENrLy2G="
// rizztech

static const char b64_table[] =
  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+/";

#define N 256

void swap(unsigned char *a, unsigned char *b) {
  unsigned char tmp = *a;
  *a = *b;
  *b = tmp;
}

void KSA(const char *key, unsigned char *S) {
  int len = strlen(key);
  int j = 0;
  for (int i = 0; i < N; i++)
    S[i] = i;
  for (int i = 0; i < N; i++) {
    j = (j + S[i] + key[i % len]) % N;
    swap(&S[i], &S[j]);
  }
}

void PRGA(unsigned char *S, const unsigned char *in, unsigned char *out, int len) {
  int i = 0, j = 0;
  for (int n = 0; n < len; n++) {
    i = (i + 1) % N;
    j = (j + S[i]) % N;
    swap(&S[i], &S[j]);
    int rnd = S[(S[i] + S[j]) % N];
    out[n] = rnd ^ in[n];
  }
}

void RC4(const char *key, const unsigned char *in, unsigned char *out, int len) {
  unsigned char S[N];
  KSA(key, S);
  PRGA(S, in, out, len);
}

int b64_index(char c) {
  for (int i = 0; i < 64; i++) {
    if (b64_table[i] == c)
      return i;
  }
  return -1;
}

int base64_decode(const char *in, unsigned char *out) {
  int len = strlen(in);
  int i, j, out_index = 0;
  unsigned char arr4[4], arr3[3];

  for (i = 0; i < len;) {
    for (j = 0; j < 4 && i < len; j++) {
      if (in[i] == '=')
        arr4[j] = 0;
      else
        arr4[j] = b64_index(in[i]);
      i++;
    }

    arr3[0] = (arr4[0] << 2) | ((arr4[1] & 0x30) >> 4);
    arr3[1] = ((arr4[1] & 0xf) << 4) | ((arr4[2] & 0x3c) >> 2);
    arr3[2] = ((arr4[2] & 0x3) << 6) | arr4[3];

    for (j = 0; j < 3; j++) {
      out[out_index++] = arr3[j];
    }
  }

  return out_index;
}

int main() {
  WSADATA wsa;
  SOCKET sock;
  struct sockaddr_in server;

  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
    printf("WSAStartup failed\n");
    return 1;
  }

  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
    printf("Socket creation failed\n");
    WSACleanup();
    return 1;
  }

  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family = AF_INET;
  server.sin_port = htons(9000);

  if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
    printf("Connection failed\n");
    closesocket(sock);
    WSACleanup();
    return 1;
  }

  unsigned char key_decoded[128];
  int key_len = base64_decode(ENC_KEY, key_decoded);
  key_decoded[key_len] = '\0';

  unsigned char rc4_out[512];
  unsigned char decoded[512];

  int enc_len = strlen(FLAG);
  RC4((char *)key_decoded, (const unsigned char *)FLAG, rc4_out, enc_len);

  int dec_len = base64_decode((char *)rc4_out, decoded);
  decoded[dec_len] = '\0';

  while (1) {
    int offset = 0;
    while (offset < dec_len) {
      int chunk_size = (dec_len - offset > 4) ? 4 : (dec_len - offset);

      unsigned char buffer[5]; 
      memcpy(buffer, decoded + offset, chunk_size);
      buffer[chunk_size] = '\0';

      sendto(sock, (char *)buffer, chunk_size, 0, (struct sockaddr *)&server, sizeof(server));
      sendto(sock, "\n", 1, 0, (struct sockaddr *)&server, sizeof(server));

      // printf("[DEBUG] Sending: %s\n", buffer);

      offset += chunk_size;
    }

    Sleep(1000); 
  }

  closesocket(sock);
  WSACleanup();
  return 0;
}

