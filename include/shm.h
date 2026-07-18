#ifndef SHM_H
#define SHM_H

#include "way.h"


int zwl_shm_init(struct zwl_context *ctx,
                 int width,
                 int height);

void zwl_shm_destroy(struct zwl_context *ctx);
int zwl_shm_init(struct zwl_context *ctx,
                 int width,
                 int height);

void zwl_shm_destroy(struct zwl_context *ctx);

#endif