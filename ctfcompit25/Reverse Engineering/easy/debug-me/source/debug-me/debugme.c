#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define flag "\x07\x0a\x0f\x05\x0e\x13\x1e\x00\x11\x0b\x16\x1c\x0a\x02\x3a\x16\x01\x07\x17\x12\x00\x0e\x0b\x15\x3b\x16\x0a\x10\x09\x06\x0b\x1b\x03\x02\x03\x1b\x14\x1a"
#define key "debugger"

void printflag(char xor_key[]) {
  char decoded[50];
  int flag_len = sizeof(flag) - 1;
  int key_len = strlen(xor_key);
  
  for (int i = 0; i < flag_len; i++) {
    decoded[i] = flag[i] ^ xor_key[i % key_len];
  }
  decoded[flag_len] = '\0';

  char encoded[50];
  for (int i = 0; i < flag_len; i++) {
    encoded[i] = decoded[i] ^ decoded[i];
  }
  encoded[flag_len] = '\0';

  printf("Flag: %s\n", encoded);
}

int main() {
  printflag(key);
  return 0;
}