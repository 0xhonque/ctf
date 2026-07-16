#include <stdio.h>
#include "./sha256/sha256.h"
#include "./chacha20-c/chacha20.h"

// HMMM I WONDER WHERE PARTS 1 AND 2 IS...
unsigned char data[] = 
{
    0x2f, 0x2a, 0x2a, 0x2a, 0x47, 0x2e, 0x47, 0x30, 0x28, 0x29, 0x23, 0x22, 
    0x35, 0x47, 0x30, 0x2f, 0x22, 0x35, 0x22, 0x47, 0x37, 0x26, 0x35, 0x33, 
    0x34, 0x47, 0x56, 0x47, 0x26, 0x29, 0x23, 0x47, 0x55, 0x47, 0x2e, 0x34, 
    0x49, 0x49, 0x49
};
unsigned int data_len = sizeof(data) / sizeof(data[0]);

unsigned char enc_data[] = 
{
  0x8c, 0x40, 0x8b, 0x75, 0x90, 0xe9, 0x6f, 0xc4, 0x79, 0xf4, 0x10, 0xed,
  0x11, 0xa8, 0xcb, 0xcd, 0x4e, 0x59, 0x70, 0x39, 0xa1, 0xbe, 0x34, 0xa7,
  0xc1, 0x03, 0xdc, 0x62, 0x9a, 0x35, 0x5f, 0x7c, 0xc0, 0x85, 0xe8, 0x3c,
  0xa7, 0x08, 0x7b, 0x66, 0x07, 0xf8, 0x74, 0x3f, 0x23, 0x43, 0x4f, 0xef,
  0xf0, 0xe7, 0x37, 0xb7, 0x01, 0x2e, 0xa8, 0x85, 0xd2, 0xfe, 0x4c, 0x36,
  0xb1, 0x0d, 0xcf, 0xf5, 0x8e, 0x17, 0x18, 0x4d, 0xba, 0x26, 0x9f, 0x63,
  0xe1, 0xd4, 0xc2, 0x28, 0x35, 0x35, 0x32, 0xa8, 0x10
};
unsigned int enc_data_len = 81;

int main() 
{
    char pass[150] = {0};
    char temp[65];
    char pt3[40], pt2[100], pt1[10];
    uint8_t key[32] = {0};
    uint8_t nonce[12] = "this-a-nonce";
    
    struct chacha20_context ctx;

    printf(">> "); // FLARE-ON.COM
    fgets(pt1, sizeof(pt1), stdin);
    printf(">> "); // DOT COM ISNT JUST FOR WEBSITE BTW. MAYBE PIECE TOGETHER WHERE YOU ARE CURRENTLY AND THAT HINT???
    fgets(pt2, sizeof(pt2), stdin);
    printf(">> "); // HMMM I WONDER WHERE PARTS 1 AND 2 IS...
    fgets(pt3, sizeof(pt3), stdin);

    pt1[strcspn(pt1, "\n")] = 0;
    pt2[strcspn(pt2, "\n")] = 0;
    pt3[strcspn(pt3, "\n")] = 0;

    for (int i = 0; i < data_len; i++) 
    {        
        pt3[i] = pt3[i] ^ 0x67;

        if ((unsigned char)pt3[i] != data[i]) return 0;
    }

    unsigned int nonce_len = sizeof(nonce) / sizeof(nonce[0]);

    for (int i = 0; i < nonce_len; i++) nonce[i] = nonce[i] ^ 0xCD;

    strcat(pass, pt1);
    strcat(pass, "|");
    strcat(pass, pt2);
    strcat(pass, "|");
    strcat(pass, pt3);

    sha256_easy_hash_hex(pass, strlen(pass), temp);
    temp[64] = '\0';

    for (int i = 0; i < 32; i++) sscanf(&temp[i * 2], "%2hhx", &key[i]);

    chacha20_init_context(&ctx, key, nonce, 0);
    chacha20_xor(&ctx, enc_data, enc_data_len);

    char filename[] = {0x9d, 0x9a, 0x81, 0x82, 0x8a, 0xc1, 0x8c, 0x80, 0x82, 0xef};
    unsigned int name_len = sizeof(filename) / sizeof(filename[0]);
    for (int i = 0; i < name_len; i++) filename[i] = filename[i] ^ 0xEF;

    FILE *fp = fopen(filename, "wb");
    if (!fp) 
    {
        perror("Failed to open file");
        return 1;
    }

    fwrite(enc_data, 1, enc_data_len, fp);
    fclose(fp);
    printf(
"                                      ---------------\n"
"                                 -++==-----------------==-\n"
"                                ****+-----------------=+***+\n"
"                                **+=---------:..:------=+**@\n"
"                                +==-------*@@@@@@@@@@*==+**@\n"
"                             -@@@@@@@*++*@@@@@@@@@@@@@%****@\n"
"                            @@@@@@@@@@@**@@@@@@*++*%#@@****@\n"
"                           *@@@*-*@@@@@=*@@@@=@@@@@  @@@***@\n"
"                           -@@@@@@- @@@ -*@@*@@@@@@@.+@@@**%\n"
"                            @@@@@@*.*@* -+**%@@@@@@@%@*:-=*@\n"
"                            @@@@@@@@@#=:-++==*@@@@@*-  :--*@.\n"
"                           :+*@@@*++*%-.-+**+===-----==***%@:\n"
"                          .%*=--=**@#*------*@@@@@@@@@@@@@@@-\n"
"                           *@@@@@@@@@@@@@@@@@**@@@@@@@@@@@@@-\n"
"                            +@@@@@@*@@@@@@@@=  .+@@@@@@@@@@@=\n"
"                              #*@@@%@@@@@@@@@@@@@%*%@@@@@@@@+\n"
"                               :*@@@@@@@@@***%@@*+**#@@@@@@@+\n"
"                                -@@@@@@@@@@@@@*++***%@@@@@@@+\n"
"                                 :@@@@@@@@**+++***%@@#%@@@@@*\n"
"                                  @@@@@@@@%%#@@@@@@%***#@@@@*\n"
"                                 .%@@@@@@@@@@@@@#*******#%@@*\n"
"                                 -+@@@@@@@@@@#***********--*+\n"
"                                 #=@@@@@@@#************@@***=\n"
"                                #@*@@@@@%************%%@@@**-\n"
"                                @@@#@@@@*************%%@@@**=\n"
"                                @@@+@@@@#************#%@@@@**\n"
"                                @@@+@@@@@%#***********%@@@@%*+\n"
"                               :@@@+@@@@@@%##*********#@@@%-+=\n"
"                               %@@@@@@@@@%%#**********#@@@*=+\n"
"                               @@@@@@@@@@%%##*********#@@%=+:\n"
"                               *@@@@@@@@@%%###*******#%@@*+*\n"
"                               @@+ @@@@@@@%%%###**###%@@*-*.\n"
"                               =@+ @@@@@@@%%%#######%#****#\n"
"                              .@%@@@@@@@@@@%%%%#####*@@@*%@\n"
"                               @@%@@@@@@@@@@%%###%@@@@@*@@@\n"
"                               .*#@@@@@@@@@@@%%%%@@@@@@@@\n"
"                              :+*@+ @@@@@@@*..=%@@@@@@@=\n"
"                             =+*@:  %@@@@@@     @@@%@@@\n"
"                           .+*#%    +@@@@@@     #****@%\n"
"                          ++*%*     :@#@@@@     *****@*\n"
"                        =+**@+      :@*#@@@     *****@*\n"
"                       +++*@:       =@**@@@     -****@+\n"
"                     =++*%@          @@@@@%      %@@@@*\n"
"                    +++*@*           @@@@@@      *@@@@@\n"
"                  +++*#@=            #@@@@@      :@@@%@-\n"
"                =****@@.              @@@@@       @@%@@*\n"
"              -**+*#@@                #@@@@:      #@@@@*\n"
"            -*****@@#                 :@@@@*      *@@@@=\n"
"          .******@@*                   @@@@*      +@@@@\n"
"         ******%@@+                    @@@@#      .@@@@\n"
"       *******%@@.                     @%@@@       @@@@\n"
"     :******#@@@                      -**%@@%     +%%%@\n"
"    *#****#%@@*                      -+**%@@@@    *+**@*\n"
"  =#*****%@@@=                    :-=****%@@@@@@ -=+***@=\n"
" #@%**#%@@@@          .**+=-:::--=+****@@@@@@@@*-==+*****\n"
"#@@@@@@@@@%          ***+***+=+*****@@@@@@#***===++++++**+\n"
"   +@@@@@           :@@#******#@@@@@:       -+********+***+\n"
"                        *%@%%*.           :=**#***@**@*#@*@:\n"
"                                          #@@@**@@**#*-+: \n"
        );
    return 67;
}
