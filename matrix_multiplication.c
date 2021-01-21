#include <stdio.h>
#include <stdlib.h>
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

int main(int argc, char *argv[])
{

    int **MatrixA;
    int **MatrixB;
    int size = 10;

    Lab1_loadinput(&MatrixA, &MatrixB, &size);
    int ***result = multiply_sequential(&MatrixA, &MatrixB, size);
    Lab1_saveoutput(*result, &size, 0.0);
}