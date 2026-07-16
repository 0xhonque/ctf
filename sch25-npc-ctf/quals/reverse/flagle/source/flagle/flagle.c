// gcc flagle.c -o flagle -lssl -lcrypto -lz -lzstd -ldl -static; sstrip flagle
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <setjmp.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <sys/ptrace.h>
#include <time.h>
#include <sys/stat.h>

// just realized this shit was obsolete bruhh shouldve used MBA
#define ONE 1
#define TWO 2
#define THREE 3
#define FOUR (TWO * TWO)
#define FIVE 5
#define SIX (TWO * THREE)
#define SEVEN 7
#define TWELVE (FOUR * THREE)
#define TWENTYONE (SEVEN * THREE)
#define FOURTYTWO (SIX * SEVEN)
#define OBFUSCATED_LENGTH ((((SEVEN * THREE) + (TWENTYONE % FOUR)) ^ ((SEVEN + SEVEN) << ONE)) + (((THREE ^ THREE) + (SEVEN % THREE)) * ((SEVEN - THREE) / TWO)) + (((FOURTYTWO / SIX) << TWO) - ((TWENTYONE / THREE) + (SEVEN ^ SEVEN))) + (((FIVE * FIVE) + (SEVEN ^ SEVEN)) - ((TWELVE >> ONE) + (SIX % FOUR))) + ((TWO * FOUR) + ONE))
#define PENALTY_THRESHOLD (THREE ^ ONE)
#define CHUNK_SIZE (1 << 10)

#define KEY 0xAB
#define WHITE "⬜"
#define GREEN "🟩"

#define ANOMALY_SIGNATURE 0xCC
#define QUANTUM_THRESHOLD (double)ONE / (double)(FIVE * FIVE * FOUR)

#define NOISE_FACTOR 0x12345678
#define OBFUSCATION_KEY 0xDEADBEEF

static inline uint64_t rdtsc() {
  uint32_t lo, hi;
  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  return ((uint64_t)hi << 32) | lo;
}

#define OBFUSCATED_CALL(func, ...) do { \
  volatile int entropy1 = (int)(rdtsc() & 0xFFFF) ^ NOISE_FACTOR; \
  volatile int entropy2 = __LINE__ ^ OBFUSCATION_KEY; \
  __asm__ __volatile__("nop; nop; nop"); \
  func(__VA_ARGS__); \
  __asm__ __volatile__("xor %%eax, %%eax" : : : "eax"); \
  volatile int dummy = entropy1 ^ entropy2; \
} while(0)

#define OBFUSCATED_CALL_RET(func, ret_var, ...) do { \
  volatile int entropy1 = (int)(rdtsc() & 0xFFFF) ^ NOISE_FACTOR; \
  volatile int entropy2 = __LINE__ ^ OBFUSCATION_KEY; \
  __asm__ __volatile__("nop; nop; nop"); \
  ret_var = func(__VA_ARGS__); \
  __asm__ __volatile__("xor %%eax, %%eax" : : : "eax"); \
  volatile int dummy = entropy1 ^ entropy2; \
} while(0)

jmp_buf fatal_error_handler;

typedef struct { char a; int b; int c; } Pair;

char flag_str_xored[] = { 0x9d, 0x93, 0x98, 0x92, 0x9d, 0x92, 0x9f, 0x9f, 0x9e, 0x9d, 0x9d, 0x9f, 0x9f, 0x93, 0x9f, 0x92, 0x98, 0x93, 0x9f, 0x9e, 0x9f, 0xc9, 0x9e, 0x98, 0x9e, 0x9d, 0x9c, 0x93, 0x9d, 0xc9, 0x99, 0xcd, 0x98, 0x9b, 0x9e, 0x92, 0x9c, 0x92, 0x9c, 0x9c, 0x9f, 0x9e, 0x9e, 0x9f, 0x9c, 0x98, 0x9d, 0x9d, 0x9e, 0xca, 0x9d, 0xc9, 0x9c, 0x9b, 0x9f, 0x9c, 0x9f, 0x98, 0x98, 0x9e, 0x9f, 0x98, 0x98, 0x9e, 0x9c, 0x93, 0x9f, 0xcf, 0x9e, 0x9d, 0x9e, 0x92, 0x98, 0x99, 0x9c, 0x9b, 0x9c, 0x93, 0x9d, 0x9d, 0x9d, 0x9a, 0x9d, 0xc8, 0x9f, 0xcd, 0x9d, 0xc9, 0x9f, 0x9d, 0x9f, 0x98, 0x9f, 0xce, 0x9e, 0x9f, 0x9e, 0x9b, 0x9c, 0xca, 0x9d, 0xce, 0x9f, 0xcf, 0x9e, 0x9f, 0x98, 0x9d, 0x9d, 0xcd, 0x9f, 0x99, 0x9c, 0x92, 0x9d, 0x9e, 0x99, 0xcd, 0x9d, 0xc8, 0x9c, 0x9f, 0x99, 0xcd, 0x9e, 0x9c, 0x9e, 0x9b, 0x98, 0x93, 0x9e, 0x9d, 0x9f, 0xcd, 0x98, 0x9d, 0x9e, 0x9f, 0x98, 0x98, 0x9d, 0x9a, 0x98, 0x9f, 0x98, 0x9d, 0x9d, 0x9f, 0x9f, 0x9f, 0x9f, 0x9d, 0x9f, 0x92, 0x9c, 0x9e, 0x9f, 0xcd, 0x98, 0xcf, 0x00 };
char key_str_xored[] = { 0xf8, 0xc8, 0xc3, 0xce, 0xc6, 0xca, 0xdf, 0xc2, 0xc8, 0xd8, 0xe8, 0xff, 0xed, 0x99, 0x9b, 0x99, 0x9e, 0x00 };
char table_str_xored[] = { 0xca, 0xc9, 0xc8, 0xcf, 0xce, 0xcd, 0xcc, 0xc3, 0xc2, 0xc1, 0xc0, 0xc7, 0xc5, 0xc6, 0xc4, 0xdb, 0xda, 0xd9, 0xd8, 0xdf, 0xde, 0xdc, 0xdd, 0xd3, 0xd2, 0xd1, 0xea, 0xe9, 0xe8, 0xef, 0xee, 0xed, 0xec, 0xe3, 0xe2, 0xe1, 0xe0, 0xe7, 0xe5, 0xe6, 0xe4, 0xfb, 0xfa, 0xf9, 0xf8, 0xff, 0xfe, 0xfc, 0xfd, 0xf3, 0xf2, 0xf1, 0x9b, 0x9a, 0x99, 0x98, 0x9f, 0x9e, 0x9d, 0x9c, 0x93, 0x92, 0x80, 0x84, 0x00 };

typedef struct
{
  enum { INIT, GET_INPUT, PROCESS_INPUT, CHECK_WIN, APPLY_PENALTY, FAIL, SUCCESS, DONE } state;
  int attempts;
  int last_correct_count;
  char input[OBFUSCATED_LENGTH * TWO + 1];
  Pair pairs[OBFUSCATED_LENGTH];
  char* flag_str;
  char* key_str;
  char* table_str;
  bool sovereignty_established;
} Context;

#define GENERATE_WRAPPER(wrapper_name, target_func, param_types, params, args) \
static inline void wrapper_name##_w3 param_types; \
static inline void wrapper_name##_w2 param_types; \
static inline void wrapper_name##_w1 param_types; \
static inline void wrapper_name param_types; \
static inline void wrapper_name##_w1 param_types { \
  __asm__ __volatile__("nop; nop; nop"); \
  volatile int dummy_entropy = rand() % 256; \
  wrapper_name##_w2 args; \
} \
static inline void wrapper_name##_w2 param_types { \
  volatile int dummy = rand() % 100; \
  __asm__ __volatile__("xor %%eax, %%eax" : : : "eax"); \
  if (dummy < 50) { \
    wrapper_name##_w3 args; \
  } else { \
    wrapper_name##_w3 args; \
  } \
} \
static inline void wrapper_name##_w3 param_types { \
  volatile char stack_noise[32]; \
  memset((void*)stack_noise, 0xAA ^ (rand() & 0xFF), 32); \
  target_func args; \
} \
static inline void wrapper_name param_types { \
  wrapper_name##_w1 args; \
}

#define GENERATE_WRAPPER_RET(wrapper_name, target_func, return_type, param_types, params, args) \
static inline return_type wrapper_name##_w3 param_types; \
static inline return_type wrapper_name##_w2 param_types; \
static inline return_type wrapper_name##_w1 param_types; \
static inline return_type wrapper_name param_types; \
static inline return_type wrapper_name##_w1 param_types { \
  __asm__ __volatile__("nop; nop; nop"); \
  volatile int dummy_entropy = rand() % 256; \
  return wrapper_name##_w2 args; \
} \
static inline return_type wrapper_name##_w2 param_types { \
  volatile int dummy = rand() % 100; \
  __asm__ __volatile__("xor %%eax, %%eax" : : : "eax"); \
  if (dummy < 50) { \
    return wrapper_name##_w3 args; \
  } else { \
    return wrapper_name##_w3 args; \
  } \
} \
static inline return_type wrapper_name##_w3 param_types { \
  volatile char stack_noise[32]; \
  memset((void*)stack_noise, 0xAA ^ (rand() & 0xFF), 32); \
  return target_func args; \
} \
static inline return_type wrapper_name param_types { \
  return wrapper_name##_w1 args; \
}

void state_init(Context* ctx);
void state_get_input(Context* ctx);
void state_process_input(Context* ctx);
void state_check_win(Context* ctx);
void state_apply_penalty(Context* ctx);
void state_fail(Context* ctx);
void state_success(Context* ctx);
void state_done(Context* ctx);
void real_check(const char* input, Pair* p, const char* flag_str, const char* key_str, const char* table_str);

void establish_sovereignty(Context* ctx)
{
  if (!ctx->sovereignty_established)
  {
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1) longjmp(fatal_error_handler, 3);
    ctx->sovereignty_established = true;
  }
}

void verify_temporal_flow()
{
  long start = clock();
  for (volatile int i = 0; i < (1 << (FOUR * FOUR)) - 1; i++);
  {
    double duration = ((double)(clock() - start) / CLOCKS_PER_SEC);
    if (duration > QUANTUM_THRESHOLD) longjmp(fatal_error_handler, 4);
  }
}

void inspect_code_integrity(void* addr, size_t size)
{
  unsigned char* p = (unsigned char*)addr;
  for (size_t i = 0; i < size; i++) if (p[i] == ANOMALY_SIGNATURE) longjmp(fatal_error_handler, 5);
}

uintptr_t state_handlers[] =
{
  (uintptr_t)&state_init, (uintptr_t)&state_get_input, (uintptr_t)&state_process_input,
  (uintptr_t)&state_check_win, (uintptr_t)&state_apply_penalty, (uintptr_t)&state_fail,
  (uintptr_t)&state_success, (uintptr_t)&state_done
};

void stack_shenanigans(int entropy) { __asm__ __volatile__("sub %0, %%rsp\n\tadd %0, %%rsp" : : "r"((long)(32 + entropy)) : "memory"); }

void generate_position_table(int position, const char* key, unsigned char* table) {
  char hash_input[256];
  snprintf(hash_input, sizeof(hash_input), "%s_pos_%d_v2", key, position);

  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256((unsigned char*)hash_input, strlen(hash_input), hash);

  for (int i = 0; i < 256; i++) {
    table[i] = i;
  }

  for (int i = 255; i > 0; i--) {
    int j = (hash[i % SHA256_DIGEST_LENGTH] +
            (hash[(i * 3) % SHA256_DIGEST_LENGTH] << 8)) % (i + 1);
    unsigned char temp = table[i];
    table[i] = table[j];
    table[j] = temp;
  }
}

char encrypt(char input, int i, const char* key) {
  unsigned char table[256];
  generate_position_table(i, key, table);
  return table[(unsigned char)input];
}

int deobfuscate(char* str, char key, int mode) { for (int i = 0; str[i] != 0; i++) str[i] ^= key; return 0; }

void get_state_path(char* path, size_t n)
{
  char* home = getenv("HOME");
  if (home) snprintf(path, n, "%s/.sh", home);
  else snprintf(path, n, ".sh");
}

void helper(const char* bad_input, int wrong_chars)
{
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256((unsigned char*)bad_input, strlen(bad_input), hash);
  long iterations = 1000000 + (wrong_chars * 100000);

  for (long i = 0; i < iterations; i++)
  {
    SHA256(hash, sizeof(hash), hash);
    SHA256(hash, sizeof(hash), hash);
    SHA256(hash, sizeof(hash), hash);
    SHA256(hash, sizeof(hash), hash);
    SHA256(hash, sizeof(hash), hash);
    SHA256(hash, sizeof(hash), hash);
    SHA256(hash, sizeof(hash), hash);
    SHA256(hash, sizeof(hash), hash);
    SHA256(hash, sizeof(hash), hash);
    SHA256(hash, sizeof(hash), hash);
    SHA256(hash, sizeof(hash), hash);
    SHA256(hash, sizeof(hash), hash);
  }
}

void recalibrate_crypto_engine(const char* bad_input, int wrong_chars)
{
  helper(bad_input, wrong_chars);
  helper(bad_input, wrong_chars);
  helper(bad_input, wrong_chars);
  helper(bad_input, wrong_chars);
  helper(bad_input, wrong_chars);
  helper(bad_input, wrong_chars);
  helper(bad_input, wrong_chars);
  helper(bad_input, wrong_chars);
  helper(bad_input, wrong_chars);
  helper(bad_input, wrong_chars);
  helper(bad_input, wrong_chars);
  helper(bad_input, wrong_chars);
}

int hex_char_to_int(char c)
{
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  return -1;
}

unsigned char* hex_decode(const char* hex_string, size_t input_length, size_t* output_length)
{
  if (input_length % 2 != 0) longjmp(fatal_error_handler, 1);
  *output_length = input_length / 2;
  unsigned char* decoded_data = malloc(*output_length + 1);
  if (decoded_data == NULL) longjmp(fatal_error_handler, 1);

  for (size_t i = 0; i < *output_length; i++)
  {
    int high = hex_char_to_int(hex_string[i * 2]);
    int low = hex_char_to_int(hex_string[i * 2 + 1]);
    if (high == -1 || low == -1)
    {
      free(decoded_data);
      longjmp(fatal_error_handler, 1);
    }
    decoded_data[i] = (high << 4) | low;
  }

  decoded_data[*output_length] = '\0';
  return decoded_data;
}

unsigned char* b64_decode(const char* data, size_t input_length, size_t* output_length, const char* table)
{
  static char decoding_table[256];
  static int table_built = 0;

  if (table_built == 0)
  {
    memset(decoding_table, 0, 256);
    for (int i = 0; i < 64; i++) decoding_table[(unsigned char)table[i]] = i;
    table_built = 1;
  }

  if (input_length % 4 != 0) longjmp(fatal_error_handler, 2);
  *output_length = input_length / 4 * 3;
  if (data[input_length - 1] == '=') (*output_length)--;
  if (data[input_length - 2] == '=') (*output_length)--;
  unsigned char* decoded_data = malloc(*output_length + 1);
  if (decoded_data == NULL) longjmp(fatal_error_handler, 2);

  for (int i = 0, j = 0; i < input_length;)
  {
    uint32_t sextet_a = data[i] == '=' ? 0 : decoding_table[(unsigned char)data[i]]; i++;
    uint32_t sextet_b = data[i] == '=' ? 0 : decoding_table[(unsigned char)data[i]]; i++;
    uint32_t sextet_c = data[i] == '=' ? 0 : decoding_table[(unsigned char)data[i]]; i++;
    uint32_t sextet_d = data[i] == '=' ? 0 : decoding_table[(unsigned char)data[i]]; i++;
    uint32_t triple = (sextet_a << 18) + (sextet_b << 12) + (sextet_c << 6) + sextet_d;
    if (j < *output_length) decoded_data[j++] = (triple >> 16) & 0xFF;
    if (j < *output_length) decoded_data[j++] = (triple >> 8) & 0xFF;
    if (j < *output_length) decoded_data[j++] = (triple >> 0) & 0xFF;
  }

  decoded_data[*output_length] = '\0';
  return decoded_data;
}

void real_check_impl(const char* input, Pair* p, const char* flag_str, const char* key_str, const char* table_str)
{
  if (setjmp(fatal_error_handler) == 0)
  {
    bool flag_used[OBFUSCATED_LENGTH] = {false};
    size_t hex_output_length, b64_output_length;

    char* flag_hex_decoded;
    OBFUSCATED_CALL_RET(hex_decode, flag_hex_decoded, flag_str, strlen(flag_str), &hex_output_length);

    char* flag_b64_decoded;
    OBFUSCATED_CALL_RET(b64_decode, flag_b64_decoded, flag_hex_decoded, hex_output_length, &b64_output_length, table_str);

    char temp[OBFUSCATED_LENGTH];

    for (int i = 0; i < OBFUSCATED_LENGTH; i++)
    {
      char encrypted_char;
      OBFUSCATED_CALL_RET(encrypt, encrypted_char, input[i], i, key_str);
      temp[i] = encrypted_char;

      if (temp[i] == flag_b64_decoded[i])
      {
        p[i].c = 0;
        flag_used[i] = true;
      }
      else p[i].c = 2;
    }
    free(flag_hex_decoded);
    free(flag_b64_decoded);
  }
  else
    return;
}

void real_check(const char* input, Pair* p, const char* flag_str, const char* key_str, const char* table_str)
{
  real_check_impl(input, p, flag_str, key_str, table_str);
}

GENERATE_WRAPPER(wrapped_verify_temporal, verify_temporal_flow,
  (void), (), ())

GENERATE_WRAPPER(wrapped_inspect_integrity, inspect_code_integrity,
  (void* addr, size_t size), (addr, size), (addr, size))

GENERATE_WRAPPER(wrapped_real_check, real_check, 
  (const char* input, Pair* p, const char* flag_str, const char* key_str, const char* table_str),
  (input, p, flag_str, key_str, table_str),
  (input, p, flag_str, key_str, table_str))

GENERATE_WRAPPER(wrapped_recalibrate_crypto, recalibrate_crypto_engine,
  (const char* bad_input, int wrong_chars), (bad_input, wrong_chars), (bad_input, wrong_chars))

GENERATE_WRAPPER(wrapped_establish_sovereignty, establish_sovereignty,
  (Context* ctx), (ctx), (ctx))

__attribute__((naked))
void obfuscated_caller(const char* input, Pair* p, const char* flag_str, const char* key_str, const char* table_str, void* func)
{
  __asm__ __volatile__(
    "pop %%rbx\n\t"
    "add $5, %%rbx\n\t"
    "call *%%r9\n\t"
    "sub $5, %%rbx\n\t"
    "jmp *%%rbx\n\t"
    :
    :
    : "rbx", "rax", "rdi", "rsi", "rdx", "rcx", "r8", "r9"
  );
}

void execute_obfuscated_check(Context* ctx)
{
  OBFUSCATED_CALL(wrapped_inspect_integrity, &real_check, 64);
  OBFUSCATED_CALL(stack_shenanigans, ctx->attempts);
  OBFUSCATED_CALL(obfuscated_caller, ctx->input, ctx->pairs, ctx->flag_str, ctx->key_str, ctx->table_str, &wrapped_real_check);
}

void state_init(Context* ctx)
{
  OBFUSCATED_CALL(wrapped_establish_sovereignty, ctx);
  OBFUSCATED_CALL(wrapped_verify_temporal);

  char path[256];
  get_state_path(path, sizeof(path));
  FILE* f = fopen(path, "rb");
  int penalty = 0;

  if (f)
  {
    fread(&penalty, sizeof(int), 1, f);
    fclose(f);
  }

  if (penalty > PENALTY_THRESHOLD)
  {
    printf("banned 🥀\n");
    exit(1);
  }

  ctx->attempts = 0;
  OBFUSCATED_CALL(deobfuscate, ctx->flag_str, KEY, 0);
  OBFUSCATED_CALL(deobfuscate, ctx->key_str, KEY, 0);
  OBFUSCATED_CALL(deobfuscate, ctx->table_str, KEY, 0);

  OBFUSCATED_CALL(wrapped_inspect_integrity, state_handlers, sizeof(state_handlers));

  ctx->state = GET_INPUT;
}

void state_get_input(Context* ctx)
{
  OBFUSCATED_CALL(wrapped_verify_temporal);
  if (ctx->attempts >= 3)
  {
    ctx->state = FAIL;
    return;
  }
  printf("Attempt %d/3\nEnter your guess (A-Z, a-z, 0-9, _{}): \n", ctx->attempts + 1);
  fgets(ctx->input, sizeof(ctx->input), stdin);
  ctx->state = PROCESS_INPUT;
}

void state_process_input(Context* ctx)
{
  if (strlen(ctx->input) == OBFUSCATED_LENGTH && ctx->input[OBFUSCATED_LENGTH - 1] != '\n')
  {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
  }
  size_t len = strlen(ctx->input);
  if (len > 0 && ctx->input[len - 1] == '\n')
  {
    ctx->input[len - 1] = '\0';
    len--;
  }
  if (len != OBFUSCATED_LENGTH)
  {
    printf("nuh uh\n");
    ctx->state = GET_INPUT;
    return;
  }
  for (int j = 0; j < OBFUSCATED_LENGTH; j++) ctx->pairs[j] = (Pair){ctx->input[j], j, 0};

  execute_obfuscated_check(ctx);

  ctx->state = CHECK_WIN;
}

void color(const Pair* p, int* n)
{
  if (p->c == 0) { printf("%s", GREEN); (*n)++; }
  else printf("%s", WHITE);
}

void state_check_win(Context* ctx)
{
  int correct = 0;
  for (int j = 0; j < OBFUSCATED_LENGTH; j++) color(&ctx->pairs[j], &correct);
  printf("\n");

  if (correct == OBFUSCATED_LENGTH)
    ctx->state = SUCCESS;
  else
  {
    ctx->last_correct_count = correct;
    ctx->state = APPLY_PENALTY;
  }
}

void state_apply_penalty(Context* ctx)
{
  printf("skill issue </3\n");
  fflush(stdout);

  OBFUSCATED_CALL(wrapped_verify_temporal);

  int wrong_chars = OBFUSCATED_LENGTH - ctx->last_correct_count;
  OBFUSCATED_CALL(wrapped_recalibrate_crypto, ctx->input, wrong_chars);

  char path[256];
  get_state_path(path, sizeof(path));
  FILE* f = fopen(path, "rb");
  int penalty = 0;
  if (f)
  {
    fread(&penalty, sizeof(int), 1, f);
    fclose(f);
  }
  penalty++;
  f = fopen(path, "wb");
  if (f)
  {
    fwrite(&penalty, sizeof(int), 1, f);
    fclose(f);
  }

  ctx->attempts++;
  ctx->state = GET_INPUT;
}

void lose(Context* ctx) { printf("vro 👅\n"); }

void state_fail(Context* ctx) { lose(ctx); ctx->state = DONE; }
void state_success(Context* ctx) { printf("yuh uh\n"); ctx->state = DONE; }
void state_done(Context* ctx) {  }

int main(int argc, char** argv)
{
  Context ctx = {
    .state = INIT, .flag_str = flag_str_xored, .key_str = key_str_xored,
    .table_str = table_str_xored, .sovereignty_established = false
  };

  uintptr_t handler_key = 0xDEADBEEFCAFEBABE;
  for (int i = 0; i < sizeof(state_handlers) / sizeof(uintptr_t); i++) state_handlers[i] ^= handler_key;

  while (ctx.state != DONE)
  {
    void (*handler)(Context*) = (void (*)(Context*))(state_handlers[ctx.state] ^ handler_key);
    handler(&ctx);
  }

  return 0;
}
