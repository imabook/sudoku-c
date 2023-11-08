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

int isSolved(int sudoku[DIM][DIM])
{
    for (int i = 0; i < DIM; i++)
        for (int j = 0; j < DIM; j++)
            if (sudoku[i][j] == 0)
                return 0;

    return 1;
}

int isPossible(int sudoku[DIM][DIM])
{

    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            if (sudoku[i][j] == 0)
                continue;

            // los numeros de esa columna
            for (int x = 0; x < DIM; x++)
            {
                if (sudoku[x][j] == sudoku[i][j] && x != i)
                    return 0;
            }

            // los numeros de esa fila
            for (int x = 0; x < DIM; x++)
            {
                if (sudoku[i][x] == sudoku[i][j] && x != j)
                    return 0;
            }

            // numeros conocidos de ese cuadrado
            for (int x = 0; x < (int)sqrt(DIM); x++)
            {
                for (int xx = 0; xx < (int)sqrt(DIM); xx++)
                {
                    // que putisima barbaridad
                    if (sudoku[(int)sqrt(DIM) * (i / (int)sqrt(DIM)) + x][(int)sqrt(DIM) * (j / (int)sqrt(DIM)) + xx] == sudoku[i][j] && (int)sqrt(DIM) * (i / (int)sqrt(DIM)) + x != i && (int)sqrt(DIM) * (j / (int)sqrt(DIM)) + xx != j)
                        return 0;
                }
            }
        }
    }
    return 1;
}

void copyOptions(Options (*o1)[DIM][DIM], Options o2[DIM][DIM])
{
    for (int I = 0; I < DIM; I++)
        for (int J = 0; J < DIM; J++)
            (*o1)[I][J] = o2[I][J];
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
            if (i == nums[n])
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

void arrayToOptions(int sudoku[DIM][DIM], Options (*optionSudoku)[DIM][DIM])
{
    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            if (sudoku[i][j] != 0)
            {
                Options o;
                o.options = 1;
                initZero(&o.optionArray, DIM);
                o.optionArray[0] = sudoku[i][j];

                (*optionSudoku)[i][j] = o;
            }
            else
            {
                Options o;
                o.options = 9;
                for (int n = 0; n < DIM; n++)
                {
                    o.optionArray[n] = n + 1;
                }
                (*optionSudoku)[i][j] = o;
            }
        }
    }
}

void optionsToArray(Options optionSudoku[DIM][DIM], int (*sudoku)[DIM][DIM])
{

    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            if (optionSudoku[i][j].options != 1)
            {
                (*sudoku)[i][j] = 0;
            }
            else
            {
                (*sudoku)[i][j] = optionSudoku[i][j].optionArray[0];
            }
        }
    }
}

int compareStates(int actual[DIM][DIM], int previous[DIM][DIM])
{
    for (int i = 0; i < DIM; i++)
        for (int j = 0; j < DIM; j++)
            if (actual[i][j] != previous[i][j])
                return 0; // no son iguales

    return 1; // son iguales
}

int simplifySudoku(Options (*optionSudoku)[DIM][DIM])
{
    int resuelto = 1;

    int prevState[DIM][DIM];
    optionsToArray(*optionSudoku, &prevState);

    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {

            // TODA LA PUTISIMA LOGICA:

            // significa que no esta resuelto ese cuadrado
            if ((*optionSudoku)[i][j].options != 1)
            {
                resuelto = 0;
                /* LOGICA PA CONOCER LAS OPCIONES EN CADA CUADRADO !! */
                int known[DIM], indice = 0;
                initZero(&known, DIM);

                int sudoku[DIM][DIM];
                optionsToArray(*optionSudoku, &sudoku);

                // estos estan pa calcular los huecos restantes en una fila/columna/cuadrado
                int optionsColumna[DIM];
                initZero(&optionsColumna, DIM);
                int optionsFila[DIM];
                initZero(&optionsFila, DIM);
                int optionsCuadrado[DIM];
                initZero(&optionsCuadrado, DIM);

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
                                break;
                            }
                        }
                        if (add)
                        {
                            known[indice] = sudoku[x][j];
                            indice++;
                        }
                    }
                    else
                    {
                        // por cada opcion de esta casilla chequear opciones de la fila
                        for (int option = 0; option < (*optionSudoku)[i][j].options; option++)
                        {
                            for (int k = 0; k < (*optionSudoku)[x][j].options; k++)
                            {
                                if ((*optionSudoku)[i][j].optionArray[option] == (*optionSudoku)[x][j].optionArray[k])
                                    optionsColumna[option]++;
                            }
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
                                break;
                            }
                        }
                        if (add)
                        {
                            known[indice] = sudoku[i][x];
                            indice++;
                        }
                    }
                    else
                    {
                        // por cada opcion de esta casilla chequear opciones de la fila
                        for (int option = 0; option < (*optionSudoku)[i][j].options; option++)
                        {
                            for (int k = 0; k < (*optionSudoku)[i][x].options; k++)
                            {
                                if ((*optionSudoku)[i][j].optionArray[option] == (*optionSudoku)[i][x].optionArray[k])
                                    optionsFila[option]++;
                            }
                        }
                    }
                }
                // numeros conocidos de ese cuadrado
                for (int x = 0; x < (int)sqrt(DIM); x++)
                {
                    for (int xx = 0; xx < (int)sqrt(DIM); xx++)
                    {
                        // que putisima barbaridad
                        if (sudoku[(int)sqrt(DIM) * (i / (int)sqrt(DIM)) + x][(int)sqrt(DIM) * (j / (int)sqrt(DIM)) + xx] != 0)
                        {
                            int add = 1;
                            for (int k = 0; k < indice; k++)
                            {
                                if (known[k] == sudoku[(int)sqrt(DIM) * (i / (int)sqrt(DIM)) + x][(int)sqrt(DIM) * (j / (int)sqrt(DIM)) + xx])
                                {
                                    add = 0;
                                    break;
                                }
                            }
                            if (add)
                            {
                                known[indice] = sudoku[(int)sqrt(DIM) * (i / (int)sqrt(DIM)) + x][(int)sqrt(DIM) * (j / (int)sqrt(DIM)) + xx];
                                indice++;
                            }
                        }
                        else
                        {
                            // por cada opcion de esta casilla chequear opciones de la fila
                            for (int option = 0; option < (*optionSudoku)[i][j].options; option++)
                            {
                                for (int k = 0; k < (*optionSudoku)[(int)sqrt(DIM) * (i / (int)sqrt(DIM)) + x][(int)sqrt(DIM) * (j / (int)sqrt(DIM)) + xx].options; k++)
                                {
                                    if ((*optionSudoku)[i][j].optionArray[option] == (*optionSudoku)[(int)sqrt(DIM) * (i / (int)sqrt(DIM)) + x][(int)sqrt(DIM) * (j / (int)sqrt(DIM)) + xx].optionArray[k])
                                        optionsCuadrado[option]++;
                                }
                            }
                        }
                    }
                }
                Options finalOptions = getOptions(known);

                // podria hacerlo una sola vez con un forloop de 3 PERO NO ME SALE DE LA POLLA
                if (finalOptions.options != 1)
                {
                    Options tempOptions;
                    tempOptions.options = 9;

                    // chequeo de columna
                    for (int o = 0; o < DIM; o++)
                    {
                        if (optionsColumna[o] == 1)
                        {
                            tempOptions.options = 1;
                            initZero(&tempOptions.optionArray, DIM);
                            tempOptions.optionArray[0] = (*optionSudoku)[i][j].optionArray[o];
                        }
                    }

                    // chequeo de fila
                    for (int o = 0; o < DIM; o++)
                    {
                        if (optionsFila[o] == 1)
                        {
                            tempOptions.options = 1;
                            initZero(&tempOptions.optionArray, DIM);
                            tempOptions.optionArray[0] = (*optionSudoku)[i][j].optionArray[o];
                        }
                    }

                    // chequeo de cuadrao
                    for (int o = 0; o < DIM; o++)
                    {
                        if (optionsCuadrado[o] == 1)
                        {
                            tempOptions.options = 1;
                            initZero(&tempOptions.optionArray, DIM);
                            tempOptions.optionArray[0] = (*optionSudoku)[i][j].optionArray[o];
                        }
                    }

                    if (tempOptions.options == 1)
                    {
                        for (int numero = 0; numero < finalOptions.options; numero++)
                        {
                            if (finalOptions.optionArray[numero] == tempOptions.optionArray[0])
                            {
                                // significa que el numero si es valido para ser elegido
                                finalOptions = tempOptions;
                            }
                        }
                    }
                }
                (*optionSudoku)[i][j] = finalOptions;
            }
        }
    }

    int actualState[DIM][DIM];
    optionsToArray(*optionSudoku, &actualState);

    if (compareStates(actualState, prevState))
    {
        return -1;
    }

    return resuelto;
}

int solveSudoku(Options (*optionSudoku)[DIM][DIM], int i, int j)
{

    if (i == 8 && j == 9)
    {
        int sudoku[DIM][DIM];
        optionsToArray((*optionSudoku), &sudoku);

        printf("\n");
        printSudoku(sudoku);

        return 1;
    }
    else if (j == 9)
    {
        j = 0;
        i++;
    }

    Options opt[DIM][DIM];
    copyOptions(&opt, (*optionSudoku));

    if (opt[i][j].options == 1)
        solveSudoku(&opt, i, j + 1);
    else
        for (int o = 0; o < opt[i][j].options; o++)
        {
            Options optCopy[DIM][DIM];
            copyOptions(&optCopy, opt);

            initZero(&optCopy[i][j].optionArray, DIM);
            optCopy[i][j].options = 1;

            optCopy[i][j].optionArray[0] = opt[i][j].optionArray[o];

            int sudoku[DIM][DIM];
            optionsToArray(optCopy, &sudoku);

            if (isPossible(sudoku))
            {
                // printf("\n");
                // printSudoku(sudoku);
                if (solveSudoku(&optCopy, i, j + 1))
                    return 1;
            }
        }

    return 0;
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

    // int sudoku[DIM][DIM] = {
    //     {0, 4, 1, 0, 0, 7, 0, 5, 0},
    //     {0, 8, 7, 5, 0, 2, 6, 0, 0},
    //     {0, 0, 2, 0, 0, 1, 0, 0, 0},
    //     {0, 9, 3, 0, 0, 0, 2, 1, 4},
    //     {0, 0, 5, 2, 0, 0, 3, 9, 8},
    //     {2, 1, 4, 9, 8, 3, 7, 6, 5},
    //     {5, 0, 9, 0, 0, 0, 1, 0, 0},
    //     {4, 3, 8, 1, 7, 9, 5, 2, 6},
    //     {1, 0, 6, 0, 0, 0, 0, 0, 0}};

    Options optionSudoku[DIM][DIM];

    printf("el sudoku a resolver:\n");
    printSudoku(sudoku);

    arrayToOptions(sudoku, &optionSudoku);

    // bastante feo
    while (1)
    {
        int i = simplifySudoku(&optionSudoku);

        printf("\n");
        optionsToArray(optionSudoku, &sudoku);
        printSudoku(sudoku);

        if (i == 1)
        {
            printf("sudoku resuelto !!");
            return 0;
        }
        else if (i == -1)
        {
            printf("\n\nfuerza bruta:\n");

            solveSudoku(&optionSudoku, 0, 0);

            return 0;
        }
    }
}
