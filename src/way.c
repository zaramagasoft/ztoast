/*
 * ============================================================================
 * way.c
 * ============================================================================
 *
 * Módulo de inicialización de Wayland para ZToast.
 *
 * Responsabilidades:
 *
 *   - Conectar con el servidor Wayland.
 *   - Obtener el registry global.
 *   - Descubrir las interfaces disponibles.
 *   - Hacer bind de las interfaces necesarias.
 *   - Crear la wl_surface y la layer_surface.
 *   - Gestionar el ciclo de vida de los objetos Wayland.
 *
 * Este módulo NO dibuja.
 * Este módulo NO usa Cairo.
 * Este módulo NO crea buffers SHM.
 *
 * Todo el renderizado se implementará en módulos posteriores.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "way.h"
#include "shm.h"
#include "cairo_render.h"
/*
 * ---------------------------------------------------------------------------
 * registry_global()
 * ---------------------------------------------------------------------------
 *
 * Callback ejecutado por Wayland cada vez que el compositor anuncia
 * una interfaz global disponible.
 *
 * Aquí descubrimos e inicializamos únicamente las interfaces que
 * necesita ZToast.
 *
 * Actualmente:
 *
 *   - wl_compositor
 *   - wl_shm
 *   - zwlr_layer_shell_v1
 *
 * El parámetro 'data' contiene el puntero al contexto de la aplicación
 * (struct zwl_context) que fue pasado a wl_registry_add_listener().
 *
 */
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
    else if (strcmp(interface, wl_shm_interface.name) == 0)
    {
        ctx->shm =
            wl_registry_bind(registry,
                             name,
                             &wl_shm_interface,
                             version < 1 ? version : 1);
    }
    else if (strcmp(interface, zwlr_layer_shell_v1_interface.name) == 0)
    {
        ctx->layer_shell =
            wl_registry_bind(registry,
                             name,
                             &zwlr_layer_shell_v1_interface,
                             version < 1 ? version : 1);
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
static void
layer_surface_configure(void *data,
                        struct zwlr_layer_surface_v1 *layer_surface,
                        uint32_t serial,
                        uint32_t width,
                        uint32_t height)
{
    struct zwl_context *ctx = data;
    (void)ctx;

    printf("Configure recibido\n");
    printf("Serial: %u\n", serial);
    printf("Size  : %ux%u\n", width, height);

    zwlr_layer_surface_v1_ack_configure(layer_surface, serial);
    /* Crear SHM solo la primera vez */
    if (!ctx->shm_ready)
    {
        if (zwl_shm_init(ctx, width, height) != 0)
        {
            fprintf(stderr, "Error inicializando SHM\n");
            return;
        }

        ctx->shm_ready = true;
    }
    if (!ctx->cairo_ready)
    {
        if (zwl_cairo_init(ctx, width, height) != 0)
            return;

        ctx->cairo_ready = true;
    }
    zwl_cairo_draw(ctx);

    wl_surface_attach(ctx->surface,
                      ctx->buffer,
                      0,
                      0);

    wl_surface_damage_buffer(ctx->surface,
                             0,
                             0,
                             width,
                             height);

    wl_surface_commit(ctx->surface);
}
static void
layer_surface_closed(void *data,
                     struct zwlr_layer_surface_v1 *layer_surface)
{
    struct zwl_context *ctx = data;
    (void)ctx;
    (void)layer_surface;

    printf("Layer cerrada.\n");
}

void zwl_dispatch(struct zwl_context *ctx)
{
    wl_display_dispatch(ctx->display);
}
static const struct zwlr_layer_surface_v1_listener layer_listener =
    {
        .configure = layer_surface_configure,
        .closed = layer_surface_closed,
};

/*
 * ---------------------------------------------------------------------------
 * zwl_init()
 * ---------------------------------------------------------------------------
 *
 * Inicializa el subsistema Wayland.
 *
 * Flujo:
 *
 *   1. Conecta con wl_display.
 *   2. Obtiene wl_registry.
 *   3. Registra los callbacks.
 *   4. Ejecuta un roundtrip para descubrir los globals.
 *   5. Comprueba que compositor, SHM y layer-shell existen.
 *
 * Devuelve:
 *
 *   0  -> correcto.
 *  -1  -> error.
 *
 */
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
    /*
     * Wayland envía el contenido del registry de forma asíncrona.
     * El roundtrip fuerza a esperar hasta haber recibido todos los
     * objetos globales antes de continuar la inicialización.
     */
    wl_display_roundtrip(ctx->display);
    // wl_display_roundtrip(ctx->display);
    if (!ctx->compositor)
    {
        fprintf(stderr, "Error: wl_compositor no disponible\n");
        return -1;
    }

    if (!ctx->shm)
    {
        fprintf(stderr, "Error: wl_shm no disponible\n");
        return -1;
    }

    if (!ctx->layer_shell)
    {
        fprintf(stderr, "Error: zwlr_layer_shell_v1 no disponible\n");
        return -1;
    }

    printf("Wayland OK zwl-init\n");
    // printf("Conectado a Wayland.\n");
    ctx->surface = wl_compositor_create_surface(ctx->compositor);

    if (!ctx->surface)
    {
        fprintf(stderr, "Error creando wl_surface\n");
        return -1;
    }

    printf("wl_surface creada correctamente.\n");
    ctx->layer_surface =
        zwlr_layer_shell_v1_get_layer_surface(
            ctx->layer_shell,
            ctx->surface,
            NULL, // Salida (output). NULL = que el compositor elija.
            ZWLR_LAYER_SHELL_V1_LAYER_OVERLAY,
            "ztoast");
    if (!ctx->layer_surface)
    {
        fprintf(stderr, "Error creando layer_surface\n");
        return -1;
    }

    printf("layer_surface creada correctamente.\n");

    zwlr_layer_surface_v1_add_listener(
        ctx->layer_surface,
        &layer_listener,
        ctx);
    // el layer_surface se ancla a la parte superior derecha de la pantalla
    zwlr_layer_surface_v1_set_anchor(
        ctx->layer_surface,
        ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP |
            ZWLR_LAYER_SURFACE_V1_ANCHOR_RIGHT);
    // el tamaño del layer_surface se establece en 400x120 píxeles
    zwlr_layer_surface_v1_set_size(
        ctx->layer_surface,
        400,
        120);
    // los márgenes del layer_surface se establecen en 20 píxeles
    zwlr_layer_surface_v1_set_margin(
        ctx->layer_surface,
        20, // top
        20, // right
        0,  // bottom
        0); // left
    // nuestros amigos commit y roundtrip para que el compositor procese los cambios
    wl_surface_commit(ctx->surface);
    // wl_display_roundtrip(ctx->display);
    return 0;
}
void zwl_destroy(struct zwl_context *ctx)
{
    (void)ctx;
}