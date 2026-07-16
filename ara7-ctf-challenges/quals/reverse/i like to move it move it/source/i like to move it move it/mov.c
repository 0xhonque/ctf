#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "md5.h"

typedef struct 
{
    const char *ptr;
    size_t len;
} slice_t;

slice_t slice(const char *s, size_t start, size_t len) 
{
    slice_t ret;
    ret.ptr = s + start;
    ret.len = len;
    return ret;
}

char *base64_encode(const unsigned char *data, size_t input_length, size_t *output_length)
{
    static char encoding_table[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+/";
    static int mod_table[] = {0, 2, 1};
    char *encoded_data;

    int i, j;

    *output_length = 4 * ((input_length + 2) / 3);

    encoded_data = malloc(*output_length + 1);
    if (encoded_data == NULL) return NULL;

    for (i = 0, j = 0; i < input_length;)
    {
        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 18) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 12) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 6) & 0x3F];
        encoded_data[j++] = encoding_table[triple & 0x3F];
    }

    for (i = 0; i < mod_table[input_length % 3]; i++) encoded_data[*output_length - 1 - i] = '=';
    
    encoded_data[*output_length] = '\0';

    return encoded_data;
}

// unsigned char *base64_decode(const char *data, size_t input_length, size_t *output_length)
// {
//     static char decoding_table[256];
//     static int table_built = 0;

//     if (table_built == 0)
//     {
//         char encoding_table[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+/";
//         memset(decoding_table, 0, 256);
//         for (int i = 0; i < 64; i++) decoding_table[(unsigned char)encoding_table[i]] = i;
//         table_built = 1;
//     }

//     if (input_length % 4 != 0) return NULL;

//     *output_length = input_length / 4 * 3;
//     if (data[input_length - 1] == '=') (*output_length)--;
//     if (data[input_length - 2] == '=') (*output_length)--;

//     unsigned char *decoded_data = malloc(*output_length + 1);
//     if (decoded_data == NULL) return NULL;

//     for (int i = 0, j = 0; i < input_length;)
//     {
//         uint32_t sextet_a = data[i] == '=' ? 0 : decoding_table[(unsigned char)data[i]]; i++;
//         uint32_t sextet_b = data[i] == '=' ? 0 : decoding_table[(unsigned char)data[i]]; i++;
//         uint32_t sextet_c = data[i] == '=' ? 0 : decoding_table[(unsigned char)data[i]]; i++;
//         uint32_t sextet_d = data[i] == '=' ? 0 : decoding_table[(unsigned char)data[i]]; i++;

//         uint32_t triple = (sextet_a << 18) + (sextet_b << 12) + (sextet_c << 6) + sextet_d;

//         if (j < *output_length) decoded_data[j++] = (triple >> 16) & 0xFF;
//         if (j < *output_length) decoded_data[j++] = (triple >> 8) & 0xFF;
//         if (j < *output_length) decoded_data[j++] = (triple >> 0) & 0xFF;
//     }

//     decoded_data[*output_length] = '\0';

//     return decoded_data;
// }

__attribute__((noinline))
int strcmp_wrap(const char *a, const char *b, size_t len) 
{
    // printf("%s : %s\n", a, b);
    volatile int r = strncmp(a, b, len);
    return r;
}

void to_hex(char *buffer, uint8_t *p)
{
    int ptr = 0;
    unsigned int i;
    for(i = 0; i < 16; ++i) 
    {
        if (i > 0 && i % 2 == 0) ptr += sprintf(buffer + ptr, "-");
        ptr += sprintf(buffer + ptr, "%02X", p[i]);
    }
}

void xor_uname(uint8_t *buffer, uint8_t *in, char *u_name)
{
    size_t input_len = 16;
    size_t name_len = strlen(u_name);
    int i;
    if (name_len == 0) return; 

    for (i = 0; i < input_len; i++) buffer[i] = in[i] ^ u_name[i % name_len];
}

int rol8(int x, int n)
{
    return ((x << n) & 0xFF) | (x >> (8 - n));
}

void xor_mask(uint8_t *buffer, uint8_t *in)
{
    uint8_t mask[] = {0x13, 0x37, 0x61, 0x67, 0xAB, 0xCD};
    size_t input_len = 16;
    size_t mask_len = sizeof(mask);
    int i;

    for (i = 0; i < input_len; i++) buffer[i] = rol8(in[i] ^ mask[i % mask_len], 1);
}

int checker(char *u_name, char *u_key)
{
    uint8_t hash[16];
    char key[40];

    #define B64_CMP(slice_a, slice_b)                         \
    do {                                                      \
        size_t la, lb;                                        \
        char *ea = base64_encode((unsigned char*)slice_a.ptr, \
                                 slice_a.len, &la);           \
        char *eb = base64_encode((unsigned char*)slice_b.ptr, \
                                 slice_b.len, &lb);           \
        if (!ea || !eb || la != lb ||                         \
            strcmp_wrap(ea, eb, la) != 0) {                   \
            free(ea); free(eb);                               \
            return 0;                                         \
        }                                                     \
        free(ea); free(eb);                                   \
    } while (0)

    #define B64_CONST(slice_a, literal)                       \
    do {                                                      \
        size_t la, lb;                                        \
        char *ea = base64_encode((unsigned char*)slice_a.ptr, \
                                 slice_a.len, &la);           \
        char *eb = base64_encode((unsigned char*)literal,     \
                                 strlen(literal), &lb);       \
        if (!ea || !eb || la != lb ||                         \
            strcmp_wrap(ea, eb, la) != 0) {                   \
            free(ea); free(eb);                               \
            return 0;                                         \
        }                                                     \
        free(ea); free(eb);                                   \
    } while (0)

    // #define B64_CMP(slice_a, slice_b)                         \
    // do {                                                      \
    //     size_t la, lb;                                        \
    //     char *ea = base64_encode((unsigned char*)slice_a.ptr, \
    //                              slice_a.len, &la);           \
    //     char *eb = base64_encode((unsigned char*)slice_b.ptr, \
    //                              slice_b.len, &lb);           \
    //     printf("%.*s : %.*s\n", slice_a.len, slice_a.ptr,     \
    //                             slice_b.len, slice_b.ptr);    \
    //     if (!ea || !eb || la != lb ||                         \
    //         strcmp_wrap(ea, eb, la) != 0) {                   \
    //         free(ea); free(eb);                               \
    //         continue;                                         \
    //     }                                                     \
    //     free(ea); free(eb);                                   \
    // } while (0)

    // #define B64_CONST(slice_a, literal)                       \
    // do {                                                      \
    //     size_t la, lb;                                        \
    //     char *ea = base64_encode((unsigned char*)slice_a.ptr, \
    //                              slice_a.len, &la);           \
    //     char *eb = base64_encode((unsigned char*)literal,     \
    //                              strlen(literal), &lb);       \
    //     printf("%.*s : %s\n", slice_a.len, slice_a.ptr,       \
    //                             literal);                     \
    //     if (!ea || !eb || la != lb ||                         \
    //         strcmp_wrap(ea, eb, la) != 0) {                   \
    //         free(ea); free(eb);                               \
    //         continue;                                         \
    //     }                                                     \
    //     free(ea); free(eb);                                   \
    } while (0)

    slice_t prefix, pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8, suffix;
    slice_t p1, p2, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, s1, s2;
    slice_t tmp;

    func_11(u_name, hash);

    // ARACTF_XXXX-YYYY-ZZZZ-AAAA-BBBB-CCCC-DDDD-EEEE_7.0
    prefix = slice(u_key, 0, 7);  // ARACTF_
    pt1 = slice(u_key, 7, 4);     // XXXX
    pt2 = slice(u_key, 11, 6);    // -YYYY-
    pt3 = slice(u_key, 17, 4);    // ZZZZ
    pt4 = slice(u_key, 21, 6);    // -AAAA-
    pt5 = slice(u_key, 27, 4);    // BBBB
    pt6 = slice(u_key, 31, 6);    // -CCCC-
    pt7 = slice(u_key, 37, 4);    // DDDD
    pt8 = slice(u_key, 41, 5);    // -EEEE
    suffix = slice(u_key, 46, 4); // _7.0

    xor_uname(hash, hash, u_name);
    xor_mask(hash, hash);

    to_hex(key, hash);

    // p1 = slice(prefix.ptr, 0, 4); // ARAC
    // p2 = slice(prefix.ptr, 2, 5); // ACTF_
    // o1 = slice(key, 0, 1);  // X
    // o2 = slice(key, 1, 5);  // XXX-Y
    // o3 = slice(key, 6, 2);  // YY
    // o4 = slice(key, 8, 4);  // Y-ZZ
    // o5 = slice(key, 12, 4); // ZZ-A
    // o6 = slice(key, 16, 5); // AAA-B
    // o7 = slice(key, 21, 6); // BBB-CC
    // o8 = slice(key, 27, 5); // CC-DD
    // o9 = slice(key, 32, 4); // DD-E
    // o10 = slice(key, 36, 3); // EEE
    // s1 = slice(suffix.ptr, 0, 3); // _7.
    // s2 = slice(suffix.ptr, 1, 3); // 7.0

    // turn the bytes into the correct format: XXXX-YYYY-ZZZZ-AAAA-BBBB-CCCC-DDDD-EEEE
    //                                         012345678901234567890123456789012345678
    p1  = slice(prefix.ptr, 0, 4);   // ARAC
    p2  = slice(prefix.ptr, 2, 5);   // ACTF_
    o1  = slice(key, 0, 3);          // XXX
    o2  = slice(key, 1, 6);          // XXX-YY
    o3  = slice(key, 6, 5);          // YYY-Z
    o4  = slice(key, 8, 5);          // Y-ZZZ
    o5  = slice(key, 11, 5);         // ZZZ-A
    o6  = slice(key, 16, 6);         // AAA-BB
    o7  = slice(key, 20, 8);         // BBBB-CCC
    o8  = slice(key, 27, 6);         // CC-DDD
    o9  = slice(key, 32, 5);         // DD-EE
    o10 = slice(key, 34, 5);         // -EEEE
    s1  = slice(suffix.ptr, 0, 3);   // _7.
    s2  = slice(suffix.ptr, 1, 3);   // 7.0

    // { // 7.0
    //     char a[4] = {0};
    //     memcpy(a, s2.ptr, s2.len);
    //     // printf("%.*s\n", (int)s2.len, a);
    //     if (strcmp_wrap(a, "7.0", s2.len) != 0) return 1;
    // }

    // { // XXX
    //     char a[4] = {0}, b[4] = {0};
    //     memcpy(a, o1.ptr, o1.len);
    //     memcpy(b, pt1.ptr, o1.len);
    //     // printf("%.*s : %.*s\n", (int)o1.len, a, (int)o1.len, b);
    //     if (strcmp_wrap(a, b, o1.len) != 0) return 1;
    // }

    // { // ARAC
    //     char a[5] = {0};
    //     memcpy(a, p1.ptr, p1.len);
    //     // printf("%.*s\n", (int)p1.len, a);
    //     if (strcmp_wrap(a, "ARAC", p1.len) != 0) return 1;
    // }

    // { // DD-EE
    //     char a[6] = {0}, b[6] = {0};
    //     memcpy(a, o9.ptr, o9.len);
    //     memcpy(b, pt7.ptr + 2, o9.len);
    //     // printf("%.*s : %.*s\n", (int)o9.len, a, (int)o9.len, b);
    //     if (strcmp_wrap(a, b, o9.len) != 0) return 1;
    // }

    // { // BBBB-CCC
    //     char a[9] = {0}, b[9] = {0};
    //     memcpy(a, o7.ptr, o7.len);
    //     memcpy(b, pt5.ptr, o7.len);
    //     // printf("%.*s : %.*s\n", (int)o7.len, a, (int)o7.len, b);
    //     if (strcmp_wrap(a, b, o7.len) != 0) return 1;
    // }

    // { // ACTF_
    //     char a[6] = {0};
    //     memcpy(a, p2.ptr, p2.len);
    //     // printf("%.*s\n", (int)p2.len, a);
    //     if (strcmp_wrap(a, "ACTF_", p2.len) != 0) return 1;
    // }

    // { // Y-ZZZ
    //     char a[6] = {0}, b[6] = {0};
    //     memcpy(a, o4.ptr, o4.len);
    //     memcpy(b, pt2.ptr + 4, o4.len);
    //     // printf("%.*s : %.*s\n", (int)o4.len, a, (int)o4.len, b);
    //     if (strcmp_wrap(a, b, o4.len) != 0) return 1;
    // }

    // { // _7.
    //     char a[4] = {0};
    //     memcpy(a, s1.ptr, s1.len);
    //     // printf("%.*s\n", (int)s1.len, a);
    //     if (strcmp_wrap(a, "_7.", s1.len) != 0) return 1;
    // }

    // { // ZZZ-A
    //     char a[6] = {0}, b[6] = {0};
    //     memcpy(a, o5.ptr, o5.len);
    //     memcpy(b, pt3.ptr + 1, o5.len);
    //     // printf("%.*s : %.*s\n", (int)o5.len, a, (int)o5.len, b);
    //     if (strcmp_wrap(a, b, o5.len) != 0) return 1;
    // }

    // { // XXX-YY
    //     char a[7] = {0}, b[7] = {0};
    //     memcpy(a, o2.ptr, o2.len);
    //     memcpy(b, pt1.ptr + 1, o2.len);
    //     // printf("%.*s : %.*s\n", (int)o2.len, a, (int)o2.len, b);
    //     if (strcmp_wrap(a, b, o2.len) != 0) return 1;
    // }

    // { // CC-DDD
    //     char a[7] = {0}, b[7] = {0};
    //     memcpy(a, o8.ptr, o8.len);
    //     memcpy(b, pt6.ptr + 3, o8.len);
    //     // printf("%.*s : %.*s\n", (int)o8.len, a, (int)o8.len, b);
    //     if (strcmp_wrap(a, b, o8.len) != 0) return 1;
    // }

    // { // YYY-Z
    //     char a[5] = {0}, b[5] = {0};
    //     memcpy(a, o3.ptr, o3.len);
    //     memcpy(b, pt2.ptr + 2, o3.len);
    //     // printf("%.*s : %.*s\n", (int)o3.len, a, (int)o3.len, b);
    //     if (strcmp_wrap(a, b, o3.len) != 0) return 1;
    // }

    // { // AAA-BB
    //     char a[7] = {0}, b[7] = {0};
    //     memcpy(a, o6.ptr, o6.len);
    //     memcpy(b, pt4.ptr + 2, o6.len);
    //     // printf("%.*s : %.*s\n", (int)o6.len, a, (int)o6.len, b);
    //     if (strcmp_wrap(a, b, o6.len) != 0) return 1;
    // }

    // { // -EEEE
    //     char a[6] = {0}, b[6] = {0};
    //     memcpy(a, o10.ptr, o10.len);
    //     memcpy(b, pt8.ptr, o10.len);
    //     // printf("%.*s : %.*s\n", (int)o10.len, a, (int)o10.len, b);
    //     if (strcmp_wrap(a, b, o10.len) != 0) return 1;
    // }

    // printf("7.0\n");
    B64_CONST(s2, "7.0");
    // printf("XXX\n");
    tmp = slice(pt1.ptr, 0, o1.len);
    B64_CMP(o1, tmp);
    // printf("ARAC\n");
    B64_CONST(p1, "ARAC");

    // printf("DD-EE\n");
    tmp = slice(pt7.ptr + 2, 0, o9.len);
    B64_CMP(o9, tmp);

    // printf("BBBB-CCC\n");
    tmp = slice(pt5.ptr, 0, o7.len);
    B64_CMP(o7, tmp);
    // printf("ACTF_\n");
    B64_CONST(p2, "ACTF_");

    // printf("Y-ZZZ\n");
    tmp = slice(pt2.ptr + 4, 0, o4.len);
    B64_CMP(o4, tmp);

    // printf("_7.\n");
    B64_CONST(s1, "_7.");

    // printf("ZZZ-A\n");
    tmp = slice(pt3.ptr + 1, 0, o5.len);
    B64_CMP(o5, tmp);

    // printf("XXX-YY\n");
    tmp = slice(pt1.ptr + 1, 0, o2.len);
    B64_CMP(o2, tmp);

    // printf("CC-DDD\n");
    tmp = slice(pt6.ptr + 3, 0, o8.len);
    B64_CMP(o8, tmp);

    // printf("YYY-Z\n");
    tmp = slice(pt2.ptr + 2, 0, o3.len);
    B64_CMP(o3, tmp);

    // printf("AAA-BB\n");
    tmp = slice(pt4.ptr + 2, 0, o6.len);
    B64_CMP(o6, tmp);

    // printf("-EEEE\n");
    B64_CMP(o10, pt8);

    return 0;
}

int main(int argc, char **argv)
{
    char u_name[51], u_key[51];
    
    // whatever
    printf("Enter username: ");
    scanf("%50s", u_name);

    // ARACTF_XXXX-YYYY-ZZZZ-AAAA-BBBB-CCCC-DDDD-EEEE_7.0
    printf("Enter key: ");
    scanf("%50s", u_key);

    if (checker(u_name, u_key) == 0) printf("✅");
    else printf("❌");

    return 0;
}