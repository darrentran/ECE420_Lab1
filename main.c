#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

#include "lab1_IO.h"
#include "timer.h"

// int ***multiply_sequential(int ***A, int ***B, int size)
// {

//     int **C = malloc(size * sizeof(int *));

//     for (int i = 0; i < size; i++)
//     {
//         C[i] = malloc(size * sizeof(int));
//     }

//     for (int i = 0; i < size; i++)
//     {

//         for (int j = 0; j < size; j++)
//         {

//             C[i][j] = 0;

//             for (int k = 0; k < size; k++)
//             {
//                 C[i][j] += (*A)[i][k] * (*B)[k][j];
//             }
//         }
//     }

//     return &C;
// }

struct arg_struct
{
    int **A;
    int **B;
    int **C;
    int size;
    int left_i;
    int right_i;
    int left_j;
    int right_j;
};

void free_matrix(int **matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

void *calculate_c_i_j(void *arg_p)
{
    struct arg_struct *args = (struct arg_struct *)arg_p;

    for (int i = args->left_i; i <= args->right_i; i++)
    {
        for (int j = args->left_j; j <= args->right_j; j++)
        {
            (args->C)[i][j] = 0;

            for (int k = 0; k < args->size; k++)
            {
                (args->C)[i][j] += (args->A)[i][k] * (args->B)[k][j];
            }
        }
    }
    free(args);
    return NULL;
}

int **multiply_parallel(int **A, int **B, int size, int num_threads)
{

    // Allocate memory for output matrix
    int **C = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++)
    {
        C[i] = malloc(size * sizeof(int));
    }
    pthread_t *thread_array = malloc(num_threads * sizeof(pthread_t *));

    // Perform matrix multiplcation
    for (int k = 0; k < num_threads; k++)
    {
        int x = floor(k / sqrt(num_threads));
        int y = k % (int)sqrt(num_threads);

        int left_i = size / sqrt(num_threads) * x;
        int right_i = (size / sqrt(num_threads) * (x + 1)) - 1;

        int left_j = size / sqrt(num_threads) * y;
        int right_j = (size / sqrt(num_threads) * (y + 1)) - 1;

        struct arg_struct *args = malloc(sizeof(struct arg_struct));
        args->A = A;
        args->B = B;
        args->C = C;
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
    free(thread_array);
    return C;
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Please indicate number of threads to create.\n");
        return -1;
    }
    int **MatrixA;
    int **MatrixB;
    int size;
    int num_threads = atoi(argv[1]);
    double start, end;

    Lab1_loadinput(&MatrixA, &MatrixB, &size);
    GET_TIME(start);
    int **MatrixC = multiply_parallel(MatrixA, MatrixB, size, num_threads);
    GET_TIME(end);
    Lab1_saveoutput(MatrixC, &size, end - start);

    free_matrix(MatrixA, size);
    free_matrix(MatrixB, size);
    free_matrix(MatrixC, size);
}