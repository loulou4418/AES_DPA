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

extern char texts[NB_DATA_SET][MSG_LEN];
extern double traces[NB_DATA_SET][NB_TRACE_VALUE];

int main(void)
{
    load_text();
    load_traces();

    pthread_t th_reader[16];
    for (uint8_t i = 0; i < 16; i++)
    {
        /* thread creation */
        if (pthread_create(&th_reader[i], NULL, (void *)calculate_AES_subkey, &i) < 0)
        {
            fprintf(stderr, "pthread_create error for reader ");
            exit(0);
        }
        (void)pthread_join(th_reader[i], NULL);
    }

    exit(0);
}