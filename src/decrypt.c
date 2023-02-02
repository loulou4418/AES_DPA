#include <string.h>
/* User include */
#include "commun.h"
#include "AES.h"
#include "load_data.h"
#include "pthread.h"

#define START_SBOX 2300
#define END_SBOX 4100

double tem_trace[NB_TRACE_VALUE];

extern char texts[NB_DATA_SET][MSG_LEN];
extern double traces[NB_DATA_SET][NB_TRACE_VALUE];

unsigned char AES_subkeys[16];

void add_trace(double *group, uint32_t id_text)
{
    /* Add the trace to prepare average */
    for (register uint32_t id_value = START_SBOX; id_value < END_SBOX; id_value++)
    {
        if (id_text >= NB_DATA_SET || id_value >= NB_TRACE_VALUE)
        {
            /* error out of array  */
            printf("Error out of array traces, id_col:%d, id_value:%d\n", id_text, id_value);
            return;
        }
        group[id_value] += traces[id_text][id_value];
    }
}

void calculate_AES_subkey(void *arg)
{
    uint8_t id_sub_key = *((uint8_t *)arg);
    uint8_t bit_index = 0;
    double avg_group0[NB_TRACE_VALUE], avg_group1[NB_TRACE_VALUE];
    uint32_t nb_value_in_group0 = 0, nb_value_in_group1 = 0;
    unsigned char best_key = 0;
    double max_diff = 0, min_diff = 0, max_cr_cr = 0;
    char res_aes;
    for (uint16_t key = 0; key <= 0xFF; key++)
    {
        nb_value_in_group0 = 0, nb_value_in_group1 = 0;
        (void)memset(avg_group0, 0, sizeof avg_group0);
        (void)memset(avg_group1, 0, sizeof avg_group1);

        /* for all texts */
        for (uint32_t id_text = 0; id_text < NB_DATA_SET; id_text++)
        {
            /* choose the group */
            res_aes = out_sbox(texts[id_text][id_sub_key /* id sous-clée */], key);

            if ((res_aes & (0x1 << bit_index)) == 0)
            {
                add_trace(avg_group0, id_text);
                nb_value_in_group0++;
            }
            else
            {
                add_trace(avg_group1, id_text);
                nb_value_in_group1++;
            }
        }

        /* Real average  */
        max_diff = __DBL_MIN__, min_diff = __DBL_MAX__;
        for (register uint32_t id_value = START_SBOX; id_value < END_SBOX; id_value++)
        {
            register double diff;
            avg_group0[id_value] /= nb_value_in_group0;
            avg_group1[id_value] /= nb_value_in_group1;
            /* reuse varaible avg_group0 to store difference */
            diff = avg_group0[id_value] - avg_group1[id_value];
            if (diff < min_diff)
            {
                min_diff = diff;
            }
            /* find the maximum value and save the key */
            else if (diff > max_diff)
            {
                max_diff = diff;
            }
        }
        if (min_diff < 0)
        {
            min_diff = -min_diff;
        }
        if ((max_diff + min_diff) > max_cr_cr)
        {
            best_key = key;
            max_cr_cr = max_diff + min_diff;
        }
        // printf("Compute key %d\tgr0:%d \t gr1:%d \t with crcr %lf \t best key is %d with max cr cr %lf\n", key, nb_value_in_group0, nb_value_in_group1, (max_diff + min_diff), best_key, max_cr_cr);
    }

    AES_subkeys[id_sub_key] = best_key;

    // print_trace(tem_trace, 5000);
}