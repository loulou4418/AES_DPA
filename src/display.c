#include <string.h>

/* User include */
#include "commun.h"

/**
 * @brief Display the full key from the subkeys array
 * @param[in] the subkeys array in number format
 */
void display_AES_key(unsigned char *AES_subkeys)
{

    printf("\nThe AES key is : ");

    for (uint8_t i = 0; i < 16; i++)
    {
        printf("%02x ", AES_subkeys[i]);
    }

    printf("\n");
}

/**
 * @brief Print trace using gnuplot program
 * @param buffer data to print
 * @param size number of data
 * @note exemple of use
 *  print_trace(traces[5], 5000);
 */
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

#warning "You need to install gnuplot to use this function"
    if (system("gnuplot -persistent data/gnu_file") == -1)
    {
        printf("Can't open gnuplot, make sure gnuplot is installed \n");
    }
}