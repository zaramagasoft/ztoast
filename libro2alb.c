#include <stdio.h>
#include <stdlib.h>

int valinit[] = {0,1,22,3,4};
int * const punterocabron = &valinit[2]; // Esto es válido: punterocabron apunta a la dirección de valinit[0] y no puede cambiar a otra dirección
 int main(int argc, char const *argv[])
 {

    //int * const punterocabron;
    //*punterocabron = 5; 
    printf("Valor de pcabron: %d, y tamanio: %lu bytes\n", *(punterocabron+1), sizeof(*punterocabron));  
    return 0;
 }
 
