#define _GNU_SOURCE

#include "shm.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/memfd.h>
#include <limits.h>

static int
create_shm_file(void)
{
    int fd = memfd_create("ztoast-shm", MFD_CLOEXEC);

    if (fd < 0)
    {
        perror("memfd_create");
        return -1;
    }

    return fd;
}
int zwl_shm_init(struct zwl_context *ctx, int width, int height)
{
    ctx->shm_fd = create_shm_file();

    if (ctx->shm_fd < 0)
        return -1;

    size_t stride = width * 4;
    ctx->shm_size = stride * height;

    if (ftruncate(ctx->shm_fd, ctx->shm_size) < 0)
    {
        perror("ftruncate");
        close(ctx->shm_fd);
        ctx->shm_fd = -1;
        return -1;
    }

    printf("SHM creada\n");
    printf("FD     : %d\n", ctx->shm_fd);
    printf("Stride : %zu bytes\n", stride);
    printf("Size   : %zu bytes\n", ctx->shm_size);

    ctx->pixels = mmap(NULL,
                       ctx->shm_size,
                       PROT_READ | PROT_WRITE,
                       MAP_SHARED,
                       ctx->shm_fd,
                       0);

    if (ctx->pixels == MAP_FAILED)
    {
        perror("mmap");
        close(ctx->shm_fd);
        ctx->shm_fd = -1;
        return -1;
    }

    printf("mmap OK\n");
    printf("Pixels : %p\n", ctx->pixels);
    ctx->pool = wl_shm_create_pool(
        ctx->shm,
        ctx->shm_fd,
        ctx->shm_size);
    if (!ctx->pool)
    {
        fprintf(stderr, "Error creando wl_shm_pool\n");
        return -1;
    }

    printf("wl_shm_pool creada correctamente.\n");
    ctx->buffer =
        wl_shm_pool_create_buffer(
            ctx->pool,
            0, // offset dentro del pool
            width,
            height,
            width * 4,
            WL_SHM_FORMAT_ARGB8888);
    if (!ctx->buffer)
    {
        fprintf(stderr, "Error creando wl_buffer\n");
        return -1;
    }

    printf("wl_buffer creado correctamente.\n");
    
    return 0;
}