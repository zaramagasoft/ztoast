CC = gcc

PKG_CFLAGS := $(shell pkg-config --cflags wayland-client cairo)
PKG_LIBS   := $(shell pkg-config --libs wayland-client cairo)

CFLAGS = -Wall -Wextra -std=c17 -g -Iinclude -Iprotocol $(PKG_CFLAGS)

LDLIBS = $(PKG_LIBS)

SRC = \
	src/main.c \
	src/way.c \
	src/shm.c \
	src/cairo_render.c \
	protocol/xdg-shell.c \
	protocol/wlr-layer-shell-unstable-v1.c

OBJ = $(SRC:.c=.o)

TARGET = ztoast

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run