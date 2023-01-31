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
#include "decript.h"
#include "AES.h"

extern char texts[NB_DATA_SET][MSG_LEN];
extern double traces[NB_DATA_SET][NB_TRACE_VALUE];

int main(void)
{
    load_text();
    load_traces();
    for (uint8_t i = 0; i < 8; i++)
    {
        make_group_and_average(texts, traces, i);
    }

    // pthread_t th_reader;
    /* thread creation */
    // if (pthread_create(&th_reader, NULL, (void *)reader, NULL) < 0)
    // {
    //     fprintf(stderr, "pthread_create error for reader ");
    //     exit(0);
    // }

    // (void)pthread_join(th_convertor, &ret);

    exit(0);
}