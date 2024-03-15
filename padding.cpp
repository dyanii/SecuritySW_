//패딩 연습 (다시 봐야함)
#include <stdio.h>
#define BLOCK_SIZE 16

int main()
{
    unsigned char msg[] = "Hello this is a test";
    int len, pad;

    /*padding on*/
    len = ((sizeof(msg) / 16) + 1) * BLOCK_SIZE;
    unsigned char pmsg[len];
    pad = len - sizeof(msg);

    for(int i=0; i<sizeof(msg); i++)
        pmsg[i] = msg[i];
    for(int i=sizeof(msg); i<len; i++)
        pmsg[i] = pad;

    for(int i=0; i<len; i++)
        printf("%d ", pmsg[i]);
    printf("\n");

    /*padding off*/
    int num = pmsg[len-1];
    for(int i=0; i<(len-num); i++)
        printf("%d ", pmsg[i]);
    printf("\n");
}