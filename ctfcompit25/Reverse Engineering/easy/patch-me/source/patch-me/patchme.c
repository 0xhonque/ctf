#include <stdio.h>
#include <string.h>

#define flag "\x13\x0e\x19\x13\x01\x07\x1a\x1e\x00\x07\x01\x0d\x10\x2f\x11\x15\x17\x0b\x1b\x04\x1d\x3e\x03\x08\x1a\x0a\x18\x3e\x19\x06\x1b\x0e"
#define key "\x70\x61\x74\x63\x68\x73\x61\x6e\x61\x73\x69\x6e\x69"

void printflag(char xor_key[]) {
  char decoded[50];
  int flag_len = sizeof(flag) - 1;
  int key_len = strlen(xor_key);
  
  for (int i = 0; i < flag_len; i++) {
    decoded[i] = flag[i] ^ xor_key[i % key_len];
  }
  decoded[flag_len] = '\0';
  
  printf("Flag: %s\n", decoded);
}

int main() {
  int isAdmin = 0;
  if (isAdmin) {
    printflag(key);
  } else {
    printf("Coba patch dulu brok\n");
  }

  return 0;
}