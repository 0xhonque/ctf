#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define REG_COUNT 4

uint8_t regs[REG_COUNT];  // 4 registers
uint8_t flag_zero = 0;

void run(uint8_t *code, size_t len) {
  size_t pc = 0;
  static char input[128];   // buffer for user input
  static int got_input = 0; // read only once

  while (pc < len) {
    uint8_t op = code[pc++];

    switch (op) {
      case 0x01: { // MOV reg, imm
        uint8_t r = code[pc++];
        uint8_t imm = code[pc++];
        regs[r] = imm;
        break;
      }
      case 0x02: { // ADD regA, regB
        uint8_t a = code[pc++];
        uint8_t b = code[pc++];
        regs[a] += regs[b];
        break;
      }
      case 0x03: { // XOR regA, regB
        uint8_t a = code[pc++];
        uint8_t b = code[pc++];
        regs[a] ^= regs[b];
        break;
      }
      case 0x04: { // CMP reg, imm
        uint8_t r = code[pc++];
        uint8_t imm = code[pc++];
        flag_zero = (regs[r] == imm);
        break;
      }
      case 0x05: { // JMP addr
        uint8_t addr = code[pc++];
        pc = addr;
        break;
      }
      case 0x06: { // JNE addr
        uint8_t addr = code[pc++];
        if (!flag_zero) pc = addr;
        break;
      }
      case 0x07: { // PRINT reg
        uint8_t r = code[pc++];
        putchar(regs[r]);
        break;
      }
      case 0x08: { // INPUT reg
        uint8_t r = code[pc++];
        regs[r] = getchar();   // waits for one char each time
        break;
      }
      case 0xFF: // HALT
        return;
      default:
        printf("Unknown opcode: %02x\n", op);
        return;
    }
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("usage: %s <program.bin>\n", argv[0]);
    return 1;
  }

  FILE *fp = fopen(argv[1], "rb");
  if (!fp) {
    perror("fopen");
    return 1;
  }

  fseek(fp, 0, SEEK_END);
  size_t len = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  uint8_t *code = malloc(len);
  fread(code, 1, len, fp);
  fclose(fp);

  run(code, len);

  printf("\n");
  free(code);
  return 0;
}
