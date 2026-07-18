#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_FONT

#define NK_IMPLEMENTATION
#include "nuklear.h"

#include "nuklear_wayland.h"

#include <stdio.h>
#include <stdlib.h>
int zwl_nk_init(struct zwl_context *ctx)
{
    ctx->nk = malloc(sizeof(struct nk_context));

    if (!ctx->nk)
        return -1;

    nk_init_default(ctx->nk, NULL);
    

    printf("Nuklear inicializado.\n");

    return 0;
}

void zwl_nk_destroy(struct zwl_context *ctx)
{
    if (!ctx->nk)
        return;

    nk_free(ctx->nk);

    free(ctx->nk);

    ctx->nk = NULL;
}

void
zwl_nk_render(struct zwl_context *ctx)
{
    if (!ctx || !ctx->nk)
        return;

    struct nk_context *nk = ctx->nk;

    nk_input_begin(nk);
    nk_input_end(nk);

    /* Aquí irán los widgets cuando tengamos la fuente */

    const struct nk_command *cmd;

    nk_foreach(cmd, nk)
{
    switch (cmd->type)
    {
        case NK_COMMAND_RECT:
            printf("RECT\n");
            break;

        case NK_COMMAND_TEXT:
            printf("TEXT\n");
            break;

        case NK_COMMAND_LINE:
            printf("LINE\n");
            break;

        default:
            break;
    }
}

    nk_clear(nk);
}