#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "way.h"

static void
registry_global(void *data,
                struct wl_registry *registry,
                uint32_t name,
                const char *interface,
                uint32_t version)
{
    struct zwl_context *ctx = data;

    printf("[%u] %s (v%u)\n",
           name,
           interface,
           version);

    if (strcmp(interface, wl_compositor_interface.name) == 0)
    {
        ctx->compositor =
            wl_registry_bind(registry,
                             name,
                             &wl_compositor_interface,
                             version < 4 ? version : 4);
    }
}

static void
registry_global_remove(void *data,
                       struct wl_registry *registry,
                       uint32_t name)
{
    (void)data;
    (void)registry;
    (void)name;
}
static const struct wl_registry_listener registry_listener = {
    .global = registry_global,
    .global_remove = registry_global_remove,
};
int zwl_init(struct zwl_context *ctx)
{
    memset(ctx, 0, sizeof(*ctx));

    ctx->display = wl_display_connect(NULL);

    if (!ctx->display)
    {
        fprintf(stderr, "No se pudo conectar a Wayland.\n");
        return -1;
    }

    ctx->registry = wl_display_get_registry(ctx->display);

    wl_registry_add_listener(
        ctx->registry,
        &registry_listener,
        ctx);

    wl_display_roundtrip(ctx->display);
    printf("Conectado a Wayland.\n");
    return 0;
}
void zwl_destroy(struct zwl_context *ctx)
{
    (void)ctx;
}