#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>
#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>

/* user inludes */
#include "load_data.h"
#include "decrypt.h"
#include "AES.h"
#include "display.h"

extern char texts[NB_DATA_SET][MSG_LEN];
extern double traces[NB_DATA_SET][NB_TRACE_VALUE];
extern unsigned char AES_subkeys[16];

int main(void)
{
    load_text();
    load_traces();

    printf("Please wait ");
    fflush(stdout);

    for (uint8_t i = 0; i < 16; i++)
    {
        calculate_AES_subkey(&i);
        
        printf(".");
        fflush(stdout);
    }

    printf("\n");

    display_AES_key(AES_subkeys);

    exit(0);
}