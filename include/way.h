#ifndef WAY_H
#define WAY_H

#include <stdbool.h>
#include <stdint.h>

#include <wayland-client.h>

#include "xdg-shell.h"
#include "wlr-layer-shell-unstable-v1.h"

#include <sys/mman.h>
#include <cairo/cairo.h>

struct zwl_context
{
    struct wl_display *display;
    struct wl_registry *registry;

    struct wl_compositor *compositor;
    struct wl_shm *shm;

    struct zwlr_layer_shell_v1 *layer_shell;

    struct wl_surface *surface;
    struct zwlr_layer_surface_v1 *layer_surface;

    int shm_fd;
    void *pixels;
    size_t shm_size;

    struct wl_shm_pool *pool;
    struct wl_buffer *buffer;

    bool shm_ready;
    bool cairo_ready;

    cairo_surface_t *cairo_surface;
    cairo_t *cr;
};

int zwl_init(struct zwl_context *ctx);

void zwl_dispatch(struct zwl_context *ctx);

void zwl_destroy(struct zwl_context *ctx);

#endif /* WAY_H */