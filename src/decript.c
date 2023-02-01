#include <string.h>
/* User include */
#include "commun.h"
#include "AES.h"
#include "load_data.h"

#define START_SBOX 2300
#define END_SBOX 4100

double tem_trace[NB_TRACE_VALUE];

extern char texts[NB_DATA_SET][MSG_LEN];
extern double traces[NB_DATA_SET][NB_TRACE_VALUE];

void make_group_and_average(void *arg)
{
    uint8_t id_sub_key = *((uint8_t *)arg);
    uint8_t bit_index = 0;
    double moy_group0[NB_TRACE_VALUE], moy_group1[NB_TRACE_VALUE];
    double *moy_group;
    uint32_t nb_value_in_group0 = 0, nb_value_in_group1 = 0;
    unsigned char best_key = 0;
    double max_diff = 0, min_diff = 0, max_cr_cr = 0;
    char res_aes;
    for (uint16_t key = 0; key <= 0xFF; key++)
    {
        nb_value_in_group0 = 0, nb_value_in_group1 = 0;
        (void)memset(moy_group0, 0, sizeof moy_group0);
        (void)memset(moy_group1, 0, sizeof moy_group1);

        /* for all texts */
        for (uint32_t id_text = 0; id_text < NB_DATA_SET; id_text++)
        {
            /* choose the group */
            res_aes = fist_step_AES(texts[id_text][id_sub_key /* id sous-clée */], key);

            if ((res_aes & (0x1 << bit_index)) == 0)
            {
                moy_group = moy_group0;
                nb_value_in_group0++;
            }
            else
            {
                moy_group = moy_group1;
                nb_value_in_group1++;
            }
            /* Add the trace to prepare average */
            for (register uint32_t id_value = START_SBOX; id_value < END_SBOX; id_value++)
            {
                if (id_text >= NB_DATA_SET || id_value >= NB_TRACE_VALUE)
                {
                    /* error out of array  */
                    printf("Error out of array traces, id_col:%d, id_value:%d\n", id_text, id_value);
                    return;
                }
                moy_group[id_value] += traces[id_text][id_value];
            }
        }

        /* Real average  */
        max_diff = -10, min_diff = 10;
        for (register uint32_t id_value = START_SBOX; id_value < END_SBOX; id_value++)
        {
            register double diff;
            moy_group0[id_value] /= nb_value_in_group0;
            moy_group1[id_value] /= nb_value_in_group1;
            /* reuse varaible moy_group0 to store difference */
            diff = moy_group0[id_value] - moy_group1[id_value];
            if (diff < min_diff)
            {
                min_diff = diff;
            }
            /* find the maximum value and save the key */
            if (diff > max_diff)
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
    printf("The best key is %d\n", best_key);
    pthread_exit(0);
    // print_trace(tem_trace, 5000);
}