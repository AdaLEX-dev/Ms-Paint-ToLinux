# ============================================================================
# Makefile for Native Arch Linux Wayland MS Paint (Authentic Windows XP Port)
# ============================================================================

CXX ?= g++
CC ?= gcc

CXXFLAGS ?= -std=c++17 -O2 -g -Wall \
	-Wno-format-security \
	-Wno-unused-variable \
	-Wno-unused-function \
	-Wno-narrowing \
	-Wno-conversion-null \
	-Wno-pointer-arith \
	-Wno-write-strings \
	-Icompat \
	-Ixp-mspaint \
	-Iwayland \
	-Iwayland_proto

CFLAGS ?= -O2 -g -Iwayland_proto

LDFLAGS ?= -lwayland-client -lwayland-cursor -lm

TARGET = mspaint

XP_SRCS = $(wildcard xp-mspaint/*.cpp)
COMPAT_SRCS = compat/mfc_runtime.cpp compat/gdi_rasterizer.cpp
WAYLAND_SRCS = wayland/wayland_backend.cpp
WAYLAND_C_SRCS = wayland_proto/xdg-shell-protocol.c
MAIN_SRCS = main.cpp

XP_OBJS = $(XP_SRCS:.cpp=.o)
COMPAT_OBJS = $(COMPAT_SRCS:.cpp=.o)
WAYLAND_OBJS = $(WAYLAND_SRCS:.cpp=.o)
WAYLAND_C_OBJS = $(WAYLAND_C_SRCS:.c=.o)
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)

ALL_OBJS = $(XP_OBJS) $(COMPAT_OBJS) $(WAYLAND_OBJS) $(WAYLAND_C_OBJS) $(MAIN_OBJS)

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(ALL_OBJS)
	$(CXX) $(ALL_OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	timeout 2s ./$(TARGET) --test-stroke --dump-windows || [ $$? -eq 124 ]

clean:
	rm -f $(ALL_OBJS) $(TARGET) paint_screenshot.ppm paint_screenshot.png paint_drawn.ppm paint_drawn.png
