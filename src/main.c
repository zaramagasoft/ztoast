#include <stdio.h>
#include <stdlib.h>

#include "way.h"

int main(void)
{
    struct zwl_context ctx;

    if (zwl_init(&ctx) != 0)
    {
        fprintf(stderr, "Error inicializando Wayland.\n");
        return EXIT_FAILURE;
    }
    printf("Init OK\n");
    zwl_destroy(&ctx);

    return EXIT_SUCCESS;
}