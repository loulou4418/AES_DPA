#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "commun.h"

char tmp_buffer[1024];
char texts[NB_DATA_SET][MSG_LEN];
double traces[NB_DATA_SET][NB_TRACE_VALUE];
char tmp_trace_buf[17 * NB_TRACE_VALUE];

char load_text(void)
{

    FILE *fp = fopen("data/texts.csv", "r");
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
            texts[id_col][id_letter] = *data;
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
    }

    fclose(fp);
    return 0;
}

void print_trace(double *buffer, uint32_t size)
{
    FILE *gnu_file = fopen("data/gnu_file", "w");
    if (!gnu_file)
    {
        printf("Can't create gnu file\n");
        return;
    }
    /* Add gnuplot command at the top of the file */
    fprintf(gnu_file, "plot '-' with lines\n");

    for (register uint32_t i = 0; i < size; i++)
    {
        fprintf(gnu_file, "%lf \n", buffer[i]);
    }

    fprintf(gnu_file, "e ");
    fflush(gnu_file);
    fclose(gnu_file);
    system("gnuplot -persistent data/gnu_file");
}

char load_traces(void)
{

    FILE *fp = fopen("data/traces_double.csv", "r");
    if (!fp)
    {
        printf("Can't open traces file\n");
        return -1;
    }

    /* get one line */
    uint32_t id_value = 0, id_col = 0, nb_value=0;
    register char *data;
    while (fgets(tmp_trace_buf, sizeof(tmp_trace_buf), fp) != NULL)
    {
        /* first call data first char */
        data = strtok(tmp_trace_buf, ",");

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

    printf("End of traces \n");

    fclose(fp);

  //  print_trace(traces[5], nb_value +1);

    return 0;
}
