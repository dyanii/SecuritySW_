//key_schedule practice
#include "aes.h"
#include <stdio.h>
#include <cstring>
#define ROUND 16
#define N 4

void key_schedule(unsigned char *key, unsigned char round_key[ROUND + 1][16])
{
    memcpy(round_key[0], key, 16);
    unsigned char rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10,
                          0x20, 0x40, 0x80, 0x1B, 0x36};
    unsigned char *p = &round_key[1][0];


    for(int i=0; i<ROUND; i++)
    {
        //rotword
        *p = *(p-3); *(p+1) = *(p-2); *(p+2) = *(p-1); *(p+3) = *(p-4);

        //subword
        for(int j=0; j<4; j++)
            *(p + j) = sbox[*(p + j)];
        
        //rcons
        for(int j=0; j<4; j++, p++)
        {
            if(j==0)
                *p ^= rcon[4*i/N];
            *p ^= *(p-4*N);
        }

        //for 12bytes left
        for(int j=0; j<12; j++, p++)
            *p = *(p-4*N) ^ *(p-4);
    }
}