#include <stdio.h>
#include <string.h>

#define flag "\x13\x0e\x0e\x1b\x0c\x06\x0e\x1a\x15\x0f\x04\x18\x2f\x14\x07\x0a\x0d\x2d\x11\x07\x2f\x14\x0d\x1b\x11\x02\x08\x34\x07\x1b\x1b\x0f\x02\x18\x0d\x12\x11\x3e\x1a\x03\x18"

void printflag(char key[]) {
  char decoded[50];
  int flag_len = strlen(flag);
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

  if (strcmp(username, "compit") == 0 && strcmp(password, "packerunpack") == 0) {
    printflag(password);
  } else {
    printf("Unpack dulu coba bre.\n");
  }

  return 0;
}