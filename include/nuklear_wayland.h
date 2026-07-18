#ifndef NUKLEAR_WAYLAND_H
#define NUKLEAR_WAYLAND_H
#include "way.h"

/* Inicialización */
int zwl_nk_init(struct zwl_context *ctx);

/* Liberación de recursos */
void zwl_nk_destroy(struct zwl_context *ctx);

/* Render de la interfaz */
void zwl_nk_render(struct zwl_context *ctx);

#endif