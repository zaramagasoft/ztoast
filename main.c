//
// Created by alb on 15/7/26.
//
#define _GNU_SOURCE
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>

#include <poll.h>
#include <sys/mman.h>
#include <wayland-client.h>
#include <cairo.h>
#include "wlr-layer-shell-unstable-v1.h"

#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <pthread.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/timerfd.h>
// --- CONFIGURACIÓN NUKLEAR ---
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR

#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_SOFTWARE_FONT

// IMPORTANTE: Definimos la implementación y cargamos la librería
#define NK_IMPLEMENTATION
#include "nuklear.h"

// AHORA definimos una guarda para que zui.h no intente re-implementar nada
#undef NK_IMPLEMENTATION
#include "zmetrics.h"
#include "zui22.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // Tienes que bajar este .h y ponerlo en tu carpeta
#include "logo_data.h"
