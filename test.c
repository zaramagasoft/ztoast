#include <wayland-client.h>

#define NK_IMPLEMENTATION
#include "include/nuklear.h"

int main(void)
{
    struct nk_context ctx;
    nk_init_default(&ctx, NULL);
}