#include <stdio.h>
#include <string.h>

void printflag(char key[]) {
  unsigned char flag[] = {
    0x00, 0x1b, 0x0b, 0x13, 0x06, 0x19, 0x0b, 0x0a, 
    0x06, 0x53, 0x53, 0x59, 0x58, 0x02, 0x12, 0x43, 
    0x06, 0x00, 0x02, 0x5c, 0x69
  };
  int flag_len = sizeof(flag); // 21 bytes

  char decoded[50];
  int key_len = strlen(key);

  for (int i = 0; i < flag_len; i++) {
    decoded[i] = flag[i] ^ key[i % key_len];
  }
  decoded[flag_len] = '\0';
  
  printf("Flag: %s\n", decoded);
}

int main() {
  char username[50];
  char password[50];

  printf("Enter username: ");
  scanf("%49s", username);
  printf("Enter password: ");
  scanf("%49s", password);

  if (strcmp(username, "ristek") == 0 && strcmp(password, "ctfcompit2025gaming!") == 0) {
    printflag(password);
  } else {
    printf("Nope.\n");
  }

  return 0;
}