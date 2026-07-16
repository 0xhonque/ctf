typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned long long size_t;

int check_flag(const char *input);

int entry(const char *input)
{
    return check_flag(input);
}

void* memcpy(void *dest, const void *src, size_t n) 
{
    char *d = dest;
    const char *s = src;
    for (size_t i = 0; i < n; i++) d[i] = s[i];
    return dest;
}

void* memset(void *s, int c, size_t n) 
{
    unsigned char *p = s;
    for (size_t i = 0; i < n; i++) p[i] = (unsigned char)c;
    return s;
}

int memcmp(const void *s1, const void *s2, size_t n) 
{
    const unsigned char *p1 = s1;
    const unsigned char *p2 = s2;
    for (size_t i = 0; i < n; i++) if (p1[i] != p2[i]) return p1[i] < p2[i] ? -1 : 1;
    return 0;
}

size_t strlen(const char *s) 
{
    size_t len = 0;
    while (s[len]) len++;
    return len;
}


struct chacha20_context 
{
    uint32_t keystream32[16];
    size_t position;
    uint8_t key[32];
    uint8_t nonce[12];
    uint64_t counter;
    uint32_t state[16];
};

// void str_obf(const char *in, unsigned char *out, size_t len) 
// {
//     for (size_t i = 0; i < len; i++) out[i] = in[i] ^ 41;
// }

void str_obf(const uint8_t *in, uint8_t *out, size_t len)
{
    const uint8_t k = 0x41;

    for (size_t i = 0; i < len; i++) 
    {
        uint8_t x = in[i];
        out[i] = (uint8_t)(x + k - ((x & k) << 1));
    }
}

// static uint32_t rotl32(uint32_t x, int n) 
// {
//     return (x << n) | (x >> (32 - n));
// }

uint32_t rotl32(uint32_t x, unsigned n)
{
    n &= 31;

    uint32_t a = x << n;
    uint32_t b = x >> ((32 - n) & 31);

    return a | b;
}


static uint32_t pack4(const uint8_t *a) 
{
    uint32_t res = 0;
    res |= (uint32_t)a[0] << 0;
    res |= (uint32_t)a[1] << 8;
    res |= (uint32_t)a[2] << 16;
    res |= (uint32_t)a[3] << 24;
    return res;
}

static void chacha20_init_block(struct chacha20_context *ctx, uint8_t key[], uint8_t nonce[]) 
{
    uint8_t magic_constant[16];
    unsigned char constant_src[] =
    {
        '$','9','1',' ','/','%','a','w','v','l','#','8','5','$','a','*', 0
    };

    str_obf((char*)constant_src, magic_constant, 16);
    memcpy(ctx->key, key, 32);
    memcpy(ctx->nonce, nonce, 12);
    ctx->state[0]  = pack4(magic_constant + 0);
    ctx->state[1]  = pack4(magic_constant + 4);
    ctx->state[2]  = pack4(magic_constant + 8);
    ctx->state[3]  = pack4(magic_constant + 12);
    ctx->state[4]  = pack4(key + 0);
    ctx->state[5]  = pack4(key + 4);
    ctx->state[6]  = pack4(key + 8);
    ctx->state[7]  = pack4(key + 12);
    ctx->state[8]  = pack4(key + 16);
    ctx->state[9]  = pack4(key + 20);
    ctx->state[10] = pack4(key + 24);
    ctx->state[11] = pack4(key + 28);
    ctx->state[12] = 0;
    ctx->state[13] = pack4(nonce + 0);
    ctx->state[14] = pack4(nonce + 4);
    ctx->state[15] = pack4(nonce + 8);
}

static void chacha20_block_set_counter(struct chacha20_context *ctx, uint64_t counter) 
{
    ctx->state[12] = (uint32_t)counter;
    ctx->state[13] = pack4(ctx->nonce) + (uint32_t)(counter >> 32);
}

static void chacha20_block_next(struct chacha20_context *ctx) 
{
    for (int i = 0; i < 16; i++) ctx->keystream32[i] = ctx->state[i];
#define QR(x,a,b,c,d) \
    x[a]+=x[b]; x[d]=rotl32(x[d]^x[a],16); \
    x[c]+=x[d]; x[b]=rotl32(x[b]^x[c],12); \
    x[a]+=x[b]; x[d]=rotl32(x[d]^x[a],8);  \
    x[c]+=x[d]; x[b]=rotl32(x[b]^x[c],7);
    for (int i = 0; i < 10; i++) 
    {
        QR(ctx->keystream32,0,4,8,12); QR(ctx->keystream32,1,5,9,13);
        QR(ctx->keystream32,2,6,10,14); QR(ctx->keystream32,3,7,11,15);
        QR(ctx->keystream32,0,5,10,15); QR(ctx->keystream32,1,6,11,12);
        QR(ctx->keystream32,2,7,8,13); QR(ctx->keystream32,3,4,9,14);
    }
    for (int i = 0; i < 16; i++) ctx->keystream32[i] += ctx->state[i];
    ctx->state[12]++;
    if (!ctx->state[12]) ctx->state[13]++;
}

void chacha20_init_context(struct chacha20_context *ctx, uint8_t key[], uint8_t nonce[], uint64_t counter) 
{
    memset(ctx, 0, sizeof(*ctx));
    chacha20_init_block(ctx, key, nonce);
    chacha20_block_set_counter(ctx, counter);
    ctx->counter = counter;
    ctx->position = 64;
}

void chacha20_xor(struct chacha20_context *ctx, uint8_t *bytes, size_t n_bytes) 
{
    uint8_t *keystream8 = (uint8_t*)ctx->keystream32;
    for (size_t i = 0; i < n_bytes; i++) 
    {
        if (ctx->position >= 64) 
        {
            chacha20_block_next(ctx);
            ctx->position = 0;
        }
        bytes[i] ^= keystream8[ctx->position++];
    }
}

void size_up(const char *plaintext, char *buffer) 
{
    size_t len = strlen(plaintext);
    if (len >= 50) memcpy(buffer, plaintext, 50);
    if (len < 50)
    {
        memcpy(buffer, plaintext, len);
        memset(buffer + len, ' ', 50 - len);
    }
}


int check_flag(const char *input) 
{
    // ARA7{__asm__.JMPing_backwards_AND_JMPing_fORwards}
    unsigned char data[50];

    data[0]  = 0x26; data[1]  = 0xc0; data[2]  = 0xb7; data[3]  = 0xa5;
    data[4]  = 0x9e; data[5]  = 0x1b; data[6]  = 0x83; data[7]  = 0x82;
    data[8]  = 0xa0; data[9]  = 0x89; data[10] = 0x00; data[11] = 0xa1;
    data[12] = 0x7d; data[13] = 0xba; data[14] = 0x34; data[15] = 0x70;
    data[16] = 0x9b; data[17] = 0xaa; data[18] = 0x93; data[19] = 0x53;
    data[20] = 0x4f; data[21] = 0x93; data[22] = 0x46; data[23] = 0x62;
    data[24] = 0xfb; data[25] = 0x58; data[26] = 0x57; data[27] = 0x22;
    data[28] = 0x89; data[29] = 0x75; data[30] = 0x25; data[31] = 0xa8;
    data[32] = 0x14; data[33] = 0xc7; data[34] = 0x2e; data[35] = 0x73;
    data[36] = 0x28; data[37] = 0xce; data[38] = 0x13; data[39] = 0x32;
    data[40] = 0x39; data[41] = 0xc2; data[42] = 0xa3; data[43] = 0x8d;
    data[44] = 0x3d; data[45] = 0x8b; data[46] = 0x9c; data[47] = 0xcb;
    data[48] = 0x2d; data[49] = 0xb1;

    struct chacha20_context ctx;
    char key[32], nonce[12], buffer[50];

    unsigned char key_src[] = 
    { 
        'A',' ','R','e','n','e','w','a','l',' ','A','g','e','n','t',' ',
        'C','a','p','t','u','r','e','T','h','e','F','l','a','g',' ','7', 0 
    };

    unsigned char nonce_src[] =
    {
        'P','r','o','p','e','r',' ','M','e','l','t','t', 0
    };

    str_obf((char*)key_src, (unsigned char*)key, 32);
    str_obf((char*)nonce_src, (unsigned char*)nonce, 12);

    size_up(input, buffer);
    
    chacha20_init_context(&ctx, (uint8_t*)key, (uint8_t*)nonce, 0);
    chacha20_xor(&ctx, (uint8_t*)buffer, 50);

    if (memcmp(buffer, data, 50) != 0) return 1; 

    return 0;
}