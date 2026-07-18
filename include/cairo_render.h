#ifndef CAIRO_RENDER_H
#define CAIRO_RENDER_H

#include "way.h"
#include <cairo/cairo.h>
int zwl_cairo_init(struct zwl_context *ctx,
                   int width,
                   int height);

void zwl_cairo_destroy(struct zwl_context *ctx);

void zwl_cairo_draw(struct zwl_context *ctx);

#endif