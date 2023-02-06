#include <string.h>
/* User include */
#include "commun.h"
#include "AES.h"
#include "load_data.h"
#include "pthread.h"

#define START_SBOX 2300
#define END_SBOX 4100

/* global array of texts*/
extern char texts[NB_DATA_SET][MSG_LEN];

/* global array for all traces */
extern double traces[NB_DATA_SET][NB_TRACE_VALUE];

/* global array for AES subkeys */
extern unsigned char AES_subkeys[16];

/** @brief Add a trace in a group to prepare average
 * @param[in] group
 * @param[in] id_text pass the id of the trace to add.
 * The trace itself is in the global array traces
 */
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

/** @brief Calculate the AES subkey
 * @param[in] arg pass the id of the subkey to calculate.
 * @note The subkey will be store in the global array AES_subkeys
 */
void calculate_AES_subkey(void *arg)
{
    /* Convert the parameters */
    uint8_t id_sub_key = *((uint8_t *)arg);
    uint8_t bit_index = 0; /* Set the bi index for all subkey */

    /* private variable */
    double avg_group0[NB_TRACE_VALUE], avg_group1[NB_TRACE_VALUE];
    uint32_t nb_value_in_group0 = 0, nb_value_in_group1 = 0;
    unsigned char best_key = 0;
    double max_diff = 0, min_diff = 0, max_cr_cr = 0;

    for (uint16_t key = 0; key <= 0xFF; key++)
    {
        nb_value_in_group0 = 0, nb_value_in_group1 = 0;
        (void)memset(avg_group0, 0, sizeof avg_group0);
        (void)memset(avg_group1, 0, sizeof avg_group1);

        /* for all texts */
        char res_aes;
        for (uint32_t id_text = 0; id_text < NB_DATA_SET; id_text++)
        {
            /* choose the group */
            res_aes = out_sbox(texts[id_text][id_sub_key], key);

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
            /* reuse variable avg_group0 to store difference */
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
        /* The most likely subkey hypothesis is the one with the highest peak to peak  */
        if (min_diff < 0)
        {
            min_diff = -min_diff;
        }
        if ((max_diff + min_diff) > max_cr_cr)
        {
            best_key = key;
            max_cr_cr = max_diff + min_diff;
        }
    }

    /* Save the best key, the main() will print it */
    AES_subkeys[id_sub_key] = best_key;
}