//CBC 구현 (근데 잘 안된것같음;;)
#include "aes.h"
#include <stdio.h>
#define BLOCK_SIZE 16

void AES_CBC_encrypt(unsigned char *ct, unsigned char *pmsg, int len,
                        unsigned char *iv, unsigned char *key);
void AES_CBC_decrypt(unsigned char *pmsg, unsigned char *ct, int len,
                        unsigned char *iv, unsigned char *key);


void AES_CBC_encrypt(unsigned char *ct, unsigned char *pmsg, int len,
                        unsigned char *iv, unsigned char *key)
{
    memcpy(ct, pmsg, len);

    for(int i=0; i<BLOCK_SIZE; i++)
        *(ct + 1) ^= iv[i]; //이거 ct+1인지 ct+i인지 제대로 못봄 직접 실행해서 찾아보기
    
    for(int i=1; i<(len / BLOCK_SIZE); i++) //(n-1)번만 돌아감 (왜냐면 마지막은 XOR계산없이 암호화만 진행)
    {
        AES_encrypt(ct, key, ct);
        for(int j=0; j<BLOCK_SIZE; j++, ct++) //for문이 한번 다 돌면 ct는 p2를 가리키고 있게됨(ct+16)
        //그러니까 다음 블럭을 가리키고 있다는 뜻
            *(ct + BLOCK_SIZE) ^= *ct;
    }
    AES_encrypt(ct, key, ct); //마지막 블록을 암호화만 하는 것임
}

void AES_CBC_decrypt(unsigned char *dmsg, unsigned char *ct, int len,
                        unsigned char *iv, unsigned char *key)
{
    memcpy(dmsg, ct, len);

    for(int i=0; i<len; i+=BLOCK_SIZE)
        AES_decrypt(dmsg+i, key, dmsg+i);
        
    for(int i=0; i<BLOCK_SIZE; i++, dmsg++)
        *dmsg ^= iv[i];
    
    for(int i=0; i<(len-BLOCK_SIZE); i++, dmsg++)
    {
        *dmsg ^= ct[i];
    }
}

int main()
{
    unsigned char msg[] = "Single block msg";
    unsigned char aes_128_key[] = {0x06, 0xa9, 0x21, 0x40, 0x36, 0xb8, 0xa1, 0x5b,
                                    0x51, 0x2e, 0x03, 0xd5, 0x34, 0x12, 0x00, 0x06};
    unsigned char iv[]          = {0x3d, 0xaf, 0xba, 0x42, 0x9d, 0x9e, 0xb4, 0x30,
                                    0xb4, 0x22, 0xda, 0x80, 0x2c, 0x9f, 0xac, 0x41};
    unsigned char pmsg[] = {};
    
    printf("메시지: \n");
    for(int i=0; i<BLOCK_SIZE; i++)
        printf("0x%02X ", msg[i]);

    int len = ((sizeof(msg) / BLOCK_SIZE) + 1) * BLOCK_SIZE; //length는 패딩이 된 후의 길이임
    unsigned char ct[len];
    unsigned char dmsg[len];
    int pad;

    len = ((sizeof(msg) / 16) + 1) * BLOCK_SIZE;
    pad = len - sizeof(msg);

    for(int i=0; i<sizeof(msg); i++)
        pmsg[i] = msg[i];
    for(int i=sizeof(msg); i<len; i++)
        pmsg[i] = pad;

    for(int i=0; i<len; i++)
        printf("0x%02X ", pmsg[i]);
    printf("\n");
    
    printf("암호화: \n");
    AES_CBC_encrypt(ct, pmsg, len, iv, aes_128_key);
    for(int i=0; i<BLOCK_SIZE; i++) printf("0x%02X ", ct[i]);
    printf("\n");

    printf("복호화: \n");
    AES_CBC_decrypt(dmsg, ct, len, iv, aes_128_key);
    for(int i=0; i<BLOCK_SIZE; i++) printf("0x%02X ", dmsg[i]);
    printf("\n");

    int num = pmsg[len-1];
    for(int i=0; i<(len-num); i++)
        printf("0x%02X ", pmsg[i]);
    printf("\n");
}