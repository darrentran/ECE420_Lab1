#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

#include "lab1_IO.h"

int ***multiply_sequential(int ***A, int ***B, int size)
{

    int **C = malloc(size * sizeof(int *));

    for (int i = 0; i < size; i++)
    {
        C[i] = malloc(size * sizeof(int));
    }

    for (int i = 0; i < size; i++)
    {

        for (int j = 0; j < size; j++)
        {

            C[i][j] = 0;

            for (int k = 0; k < size; k++)
            {
                C[i][j] += (*A)[i][k] * (*B)[k][j];
            }
        }
    }

    return &C;
}

struct arg_struct
{
    int ***A;
    int ***B;
    int ***C;
    int size;
    int left_i;
    int right_i;
    int left_j;
    int right_j;
};

void *calculate_c_i_j(void *arg_p)
{
    struct arg_struct *args = arg_p;

    for (int i = args->left_i; i <= args->right_i; i++)
    {
        for (int j = args->left_j; j <= args->right_j; j++)
        {
            (*args->C)[i][j] = 0;

            for (int k = 0; k < args->size; k++)
            {
                (*args->C)[i][j] += (*args->A)[i][k] * (*args->B)[k][j];
                printf((*args->A)[i][k]);
                printf((*args->B)[k][j]);
            }
        }
    }
}

int ***multiply_parallel(int ***A, int ***B, int size, int num_threads)
{

    // Allocate memory for output matrix
    int **C = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++)
    {
        C[i] = malloc(size * sizeof(int));
    }

    pthread_t thread_array[num_threads];

    // Perform matrix multiplcation
    for (int k = 0; k < num_threads; k++)
    {
        int x = floor(k / sqrt(num_threads));
        int y = k % (int)sqrt(num_threads);

        int left_i = size / sqrt(num_threads) * x;
        int right_i = (size / sqrt(num_threads) * (x + 1)) - 1;

        int left_j = size / sqrt(num_threads) * y;
        int right_j = (size / sqrt(num_threads) * (y + 1)) - 1;

        struct arg_struct *args;
        args->A = A;
        args->B = B;
        args->C = &C;
        args->left_i = left_i;
        args->right_i = right_i;
        args->left_j = left_j;
        args->right_j = right_j;
        args->size = size;

        pthread_create(&thread_array[k], NULL, calculate_c_i_j, (void *)args);
    }

    for (int k = 0; k < num_threads; k++)
    {
        pthread_join(thread_array[k], NULL);
    }

    return &C;
}

int main(int argc, char *argv[])
{
    int **MatrixA;
    int **MatrixB;
    int size;
    int option;
    int num_threads;

    Lab1_loadinput(&MatrixA, &MatrixB, &size);

    while ((option = getopt(argc, argv, "t:")) != -1)
        switch (option)
        {
        case 't':
            num_threads = strtol(optarg, NULL, size * size);
            break;
        case '?':
            printf("Unexpected Options. \n");
            return -1;
        }

    int ***result = multiply_parallel(&MatrixA, &MatrixB, size, num_threads);
    Lab1_saveoutput(*result, &size, 0.0);
}