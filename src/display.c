#include <string.h>
/* User include */
#include "commun.h"

void display_AES_key(unsigned char *AES_subkeys)
{

    printf("\nThe AES key is : ");

    for (uint8_t i = 0; i < 16; i++)
    {
        printf("%02x ", AES_subkeys[i]);
    }

    printf("\n");
}
