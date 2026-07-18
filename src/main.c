#include <stdio.h>
#include <stdlib.h>

#include "way.h"

#define _GNU_SOURCE

#include "shm.h"

int main(void)
{
    struct zwl_context ctx;

    if (zwl_init(&ctx) != 0)
    {
        fprintf(stderr, "Error inicializando Wayland.\n");
        return EXIT_FAILURE;
    }
    printf("Init OK\n");
    while (1)
    {
        zwl_dispatch(&ctx);
    }

    zwl_destroy(&ctx);

    return EXIT_SUCCESS;
}