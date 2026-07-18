#include "cairo_render.h"

#include <stdio.h>

// #include <cairo/cairo.h>
int zwl_cairo_init(struct zwl_context *ctx,
                   int width,
                   int height)
{
    ctx->cairo_surface =
        cairo_image_surface_create_for_data(
            (unsigned char *)ctx->pixels,
            CAIRO_FORMAT_ARGB32,
            width,
            height,
            width * 4);

    if (cairo_surface_status(ctx->cairo_surface) != CAIRO_STATUS_SUCCESS)
    {
        fprintf(stderr,
                "Error creando cairo_surface\n");
        return -1;
    }

    ctx->cr = cairo_create(ctx->cairo_surface);

    if (cairo_status(ctx->cr) != CAIRO_STATUS_SUCCESS)
    {
        fprintf(stderr,
                "Error creando cairo context\n");
        cairo_surface_destroy(ctx->cairo_surface);
        ctx->cairo_surface = NULL;
        return -1;
    }

    printf("Cairo inicializado correctamente.\n");

    return 0;
}
void zwl_cairo_draw(struct zwl_context *ctx)
{
    /* Fondo */

   /*  cairo_set_source_rgb(ctx->cr,
                         0.15,
                         0.15,
                         0.15);
 */
   cairo_set_source_rgba(ctx->cr,
                      0.039,
                      0.059,
                      0.039,
                      0.650);

    cairo_paint(ctx->cr);

    /* Texto */

    cairo_select_font_face(
        ctx->cr,
        "3270 Nerd Font Propo",
        CAIRO_FONT_SLANT_NORMAL,
        CAIRO_FONT_WEIGHT_NORMAL);

    cairo_set_font_size(ctx->cr, 28);

    cairo_set_source_rgb(ctx->cr,
                     0.20,
                     1.00,
                     0.20);


    cairo_move_to(ctx->cr,
                  30,
                  60);

    cairo_show_text(ctx->cr,
                    "Hola Cairo");

    cairo_surface_flush(ctx->cairo_surface);
}