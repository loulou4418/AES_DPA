// See LICENSE for license details.

/* user inludes */
#include "commun.h"
#include "load_data.h"
#include "decrypt.h"
#include "AES.h"
#include "display.h"

/* global array of texts*/
extern char texts[NB_DATA_SET][MSG_LEN];

/* global array for all traces */
extern double traces[NB_DATA_SET][NB_TRACE_VALUE];

/* global array for AES subkeys */
extern unsigned char AES_subkeys[16];

int main(void)
{
    load_text("data/texts.csv");
    load_traces("data/traces_double.csv");

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