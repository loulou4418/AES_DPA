// See LICENSE for license details.

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "commun.h"

/* global array of texts*/
char texts[NB_DATA_SET][MSG_LEN];

/* global array for all traces */
double traces[NB_DATA_SET][NB_TRACE_VALUE];

/* temporary buffer use to store one line of csv file */
char tmp_trace_buf[17 * NB_TRACE_VALUE]; // TODO 17 should be define

/* temprary buffer to store the trace that will be displayed with gnuplot */
char tmp_buffer[1024];

/**
 * @brief Copy text from csv file to array
 * @param[in] file_name
 * @return 0 if ok
 * @return -1 if error
*/
char load_text(char * file_name)
{

    FILE *fp = fopen(file_name, "r");
    if (!fp)
    {
        printf("Can't open texts file\n");
        return -1;
    }

    register char *data;
    uint32_t id_letter = 0, id_col = 0;
    while (fgets(tmp_buffer, sizeof(tmp_buffer), fp) != NULL)
    {
        /* first call data first char */
        data = strtok(tmp_buffer, ",");
        while (1)
        {
            if (id_col >= NB_DATA_SET || id_letter > MSG_LEN)
            {
                /* error out of array  */
                printf("Error out of array text \n");
                return -1;
            }
            texts[id_col][id_letter] = atoi(data);
            /* strtok return NULL if data = \n */
            data = strtok(NULL, ",");

            if (data == NULL)
            {
                /* End of line */
                id_letter = 0;
                break;
            }
            id_letter++;
        }
        id_col++;

        if (id_col == MAX_DATA_SET){
            break;
        }
    }

    fclose(fp);
    return 0;
}

/**
 * @brief Load traces from csv file to array
 * @param file_name 
 * @return 0 if ok
 * @return -1 if error
 */
char load_traces(char * file_name)
{
    FILE *fp = fopen(file_name, "r");
    if (!fp)
    {
        printf("Can't open traces file\n");
        return -1;
    }

    /* get one line */
    uint32_t id_value = 0, id_col = 0, nb_value = 0;
    register char *data;
    /* get one line */
    while (fgets(tmp_trace_buf, sizeof(tmp_trace_buf), fp) != NULL)
    {
        /* Extract values between ',' */
        data = strtok(tmp_trace_buf, ",");
        /* first call data first char */

        while (1)
        {
            if (id_col >= NB_DATA_SET || id_value >= NB_TRACE_VALUE)
            {
                /* error out of array  */
                printf("Load data, Error out of array traces, id_col:%d, id_value:%d\n", id_col, id_value);
                fclose(fp);

                return -1;
            }
            traces[id_col][id_value] = (double)atof(data);
            /* strtok return NULL if data = \n */
            data = strtok(NULL, ",");

            if (data == NULL)
            { /* End of line */
                nb_value = id_value;
                id_value = 0;
                break;
            }
            id_value++;
        }
        id_col++;
    }

    printf("Success loading data \n");

    fclose(fp);

    return 0;
}
