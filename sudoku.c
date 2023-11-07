#include <stdio.h>
#include <math.h>

#define DIM 9

typedef struct
{
    int optionArray[DIM]; // si la opcion es cero es el final de las opciones
    int options;
} Options;

void initZero(int (*arr)[], int dim)
{
    for (int i = 0; i < dim; i++)
    {
        (*arr)[i] = 0;
    }
}

void printSudoku(int sudoku[DIM][DIM])
{
    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }
}

Options getOptions(int nums[DIM])
{
    Options o;

    initZero(&o.optionArray, DIM);
    o.options = 0;

    for (int i = 1; i <= DIM; i++)
    {
        int isOption = 1;
        for (int n = 0; n < DIM; n++)
        {
            if (i == n)
            {
                isOption = 0;
                break;
            }
        }
        if (isOption)
        {
            o.optionArray[o.options] = i;
            o.options++;
        }
    }

    return o;
}

void solveSudoku(int sudoku[DIM][DIM], Options (*optionSudoku)[DIM][DIM])
{
    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            if (sudoku[i][j] != 0)
            {
                Options singleOption;
                singleOption.options = 1;
                singleOption.optionArray[1] = sudoku[i][j];
                singleOption.optionArray[2] = 0;

                (*optionSudoku)[i][j] = singleOption;
            }
            else
            {
                /* LOGICA PA CONOCER LAS OPCIONES EN CADA CUADRADO !! */
                int known[DIM], indice = 0;
                initZero(&known, DIM);

                // los numeros de esa columna
                for (int x = 0; x < DIM; x++)
                {
                    if (sudoku[x][j] != 0)
                    {
                        int add = 1;
                        for (int k = 0; k < indice; k++)
                        {
                            if (known[k] == sudoku[x][j])
                            {
                                add = 0;
                            }
                            break;
                        }
                        if (add)
                        {
                            known[indice] = sudoku[x][j];
                            indice++;
                        }
                    }
                }
                // numeros conocidos de esa fila
                for (int x = 0; x < DIM; x++)
                {
                    if (sudoku[i][x] != 0)
                    {
                        int add = 1;
                        for (int k = 0; k < indice; k++)
                        {
                            if (known[k] == sudoku[i][x])
                            {
                                add = 0;
                            }
                            break;
                        }
                        if (add)
                        {
                            known[indice] = sudoku[i][x];
                            indice++;
                        }
                    }
                }
                // numeros conocidos de ese cuadrado
                for (int x = 0; x < (int)sqrt(DIM); x++)
                {
                    for (int xx = 0; xx < (int)sqrt(DIM); xx++)
                    {
                        if (sudoku[i / (int)sqrt(DIM) + x][j / (int)sqrt(DIM) + xx] != 0)
                        {
                            int add = 1;
                            for (int k = 0; k < indice; k++)
                            {
                                if (known[k] == sudoku[i / (int)sqrt(DIM) + x][j / (int)sqrt(DIM) + xx])
                                {
                                    add = 0;
                                }
                                break;
                            }
                            if (add)
                            {
                                known[indice] = sudoku[i / (int)sqrt(DIM) + x][j / (int)sqrt(DIM) + xx];
                                indice++;
                            }
                        }
                    }
                }

                (*optionSudoku)[i][j] = getOptions(known);
            }
        }
    }
}

int main()
{
    int sudoku[DIM][DIM] = {
        {0, 4, 1, 0, 0, 7, 0, 5, 0},
        {0, 8, 0, 5, 0, 0, 6, 0, 0},
        {0, 0, 2, 0, 0, 1, 0, 0, 0},
        {0, 9, 3, 0, 0, 0, 0, 0, 4},
        {0, 0, 5, 2, 0, 0, 0, 9, 8},
        {2, 1, 4, 9, 0, 3, 7, 6, 0},
        {5, 0, 9, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 7, 9, 5, 2, 6},
        {1, 0, 6, 0, 0, 0, 0, 0, 0}};

    Options optionSudoku[DIM][DIM];

    solveSudoku(sudoku, &optionSudoku);

    return 0;
}