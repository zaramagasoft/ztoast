#include <stdio.h>
#include <stdlib.h>

int *punterocabron = NULL;
// La función solo recibe el puntero y lo rellena
void fillArray(int *arr, int size, int value)
{
    if (arr != NULL)
    {
        for (int i = 0; i < size; i++)
        {
            arr[i] = value; // Esto es válido: el contenido SÍ puede cambiar
        }
    }
}
void printArray(int *vector)
{
    if (vector != NULL)
    {
        for (int i = 0; i < 5; i++)
        {
            printf("%d ", vector[i]);
        }
        printf("\n");
    }
}

int main(int argc, char const *argv[])
{
    // 1. Reservamos la memoria primero
    // 2. Definimos el puntero como CONSTANTE (no puede cambiar a dónde apunta)
    int *const vector = malloc(5 * sizeof(int));

    // Si intentaras hacer: vector = malloc(...); de nuevo, el compilador daría error.

    // Ahora podemos modificar el contenido de la dirección reservada
    fillArray(vector, 5, 45);

    printf("Dirección fija: %p\n", (void *)vector);
    printf("Primer valor: %d\n", vector[0]);

    printArray(vector);
    fillArray(vector, 5, 100);
    printf("Dirección fija: %p\n", (void *)vector);
    printf("Primer valor: %d\n", vector[0]);

    printArray(vector);
    punterocabron = vector; // Esto es válido: punterocabron apunta a la misma dirección que vector
    // free(vector); // Liberamos al final
    printArray(punterocabron); // Evitamos dangling pointer
    free(vector);              // Liberamos la memoria al final
    punterocabron = NULL;      // Evitamos dangling pointer
    return 0;
}