#pragma once
#include <wayland-client.h>
#include "../wayland_proto/xdg-shell-client-protocol.h"
#include <stdint.h>
#include <stddef.h>
#include "afxwin.h"

struct WaylandState {
    struct wl_display* display = nullptr;
    struct wl_registry* registry = nullptr;
    struct wl_compositor* compositor = nullptr;
    struct wl_shm* shm = nullptr;
    struct xdg_wm_base* xdg_wm_base = nullptr;
    struct wl_seat* seat = nullptr;
    struct wl_pointer* pointer = nullptr;
    struct wl_keyboard* keyboard = nullptr;

    struct wl_surface* surface = nullptr;
    struct xdg_surface* xdg_surface = nullptr;
    struct xdg_toplevel* xdg_toplevel = nullptr;

    int width = 1024;
    int height = 768;
    bool configured = false;
    bool running = true;

    // SHM Buffer
    int shm_fd = -1;
    size_t shm_size = 0;
    uint32_t* pixel_buffer = nullptr;
    struct wl_shm_pool* shm_pool = nullptr;
    struct wl_buffer* wl_buffer = nullptr;

    // Input state
    int mouse_x = 0;
    int mouse_y = 0;
    bool left_button_down = false;
    bool right_button_down = false;

    bool Init(int w = 1024, int h = 768);
    void ResizeBuffer(int w, int h);
    void RenderFrame();
    void DispatchEvents();
    void Destroy();
};

extern WaylandState g_wayland;
