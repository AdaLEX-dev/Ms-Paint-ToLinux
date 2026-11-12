#define NOMINMAX
#include "wayland_backend.h"
#include "gdi_rasterizer.h"
#include "mfc_windows.h"
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <linux/input-event-codes.h>

WaylandState g_wayland;

// Wayland Registry Callbacks
static void registry_global(void* data, struct wl_registry* registry, uint32_t name, const char* interface, uint32_t version) {
    auto state = (WaylandState*)data;
    if (std::strcmp(interface, wl_compositor_interface.name) == 0) {
        state->compositor = (struct wl_compositor*)wl_registry_bind(registry, name, &wl_compositor_interface, 4);
    } else if (std::strcmp(interface, wl_shm_interface.name) == 0) {
        state->shm = (struct wl_shm*)wl_registry_bind(registry, name, &wl_shm_interface, 1);
    } else if (std::strcmp(interface, xdg_wm_base_interface.name) == 0) {
        state->xdg_wm_base = (struct xdg_wm_base*)wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);
    } else if (std::strcmp(interface, wl_seat_interface.name) == 0) {
        state->seat = (struct wl_seat*)wl_registry_bind(registry, name, &wl_seat_interface, 1);
    }
}

static void registry_global_remove(void* data, struct wl_registry* registry, uint32_t name) {}

static const struct wl_registry_listener registry_listener = {
    registry_global,
    registry_global_remove
};

// XDG WM Base Ping / Pong
static void xdg_wm_base_ping(void* data, struct xdg_wm_base* xdg_wm_base, uint32_t serial) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    xdg_wm_base_ping
};

// XDG Surface Configure
static void xdg_surface_configure(void* data, struct xdg_surface* xdg_surface, uint32_t serial) {
    auto state = (WaylandState*)data;
    xdg_surface_ack_configure(xdg_surface, serial);
    state->configured = true;
    state->RenderFrame();
}

static const struct xdg_surface_listener xdg_surface_listener = {
    xdg_surface_configure
};

// XDG Toplevel Configure / Close
static void xdg_toplevel_configure(void* data, struct xdg_toplevel* toplevel, int32_t width, int32_t height, struct wl_array* states) {
    auto state = (WaylandState*)data;
    if (width > 0 && height > 0 && (width != state->width || height != state->height)) {
        state->ResizeBuffer(width, height);
    }
}

static void xdg_toplevel_close(void* data, struct xdg_toplevel* toplevel) {
    auto state = (WaylandState*)data;
    state->running = false;
    CWnd* pMain = AfxGetMainWnd();
    if (pMain) {
        pMain->SendMessage(WM_CLOSE, 0, 0);
    }
}

static const struct xdg_toplevel_listener xdg_toplevel_listener = {
    xdg_toplevel_configure,
    xdg_toplevel_close
};

// ============================================================================
// Pointer Events -> Win32 Mouse Messages
// ============================================================================

static void DispatchMouseToWindows(UINT msg, WPARAM wParam, int screenX, int screenY) {
    AfxDispatchMouse(msg, wParam, screenX, screenY);
    g_wayland.RenderFrame();
}

static void pointer_enter(void* data, struct wl_pointer* pointer, uint32_t serial, struct wl_surface* surface, wl_fixed_t sx, wl_fixed_t sy) {
    auto state = (WaylandState*)data;
    state->mouse_x = wl_fixed_to_int(sx);
    state->mouse_y = wl_fixed_to_int(sy);
}

static void pointer_leave(void* data, struct wl_pointer* pointer, uint32_t serial, struct wl_surface* surface) {}

static void pointer_motion(void* data, struct wl_pointer* pointer, uint32_t time, wl_fixed_t sx, wl_fixed_t sy) {
    auto state = (WaylandState*)data;
    state->mouse_x = wl_fixed_to_int(sx);
    state->mouse_y = wl_fixed_to_int(sy);

    WPARAM wParam = 0;
    if (state->left_button_down) wParam |= MK_LBUTTON;
    if (state->right_button_down) wParam |= MK_RBUTTON;

    DispatchMouseToWindows(WM_MOUSEMOVE, wParam, state->mouse_x, state->mouse_y);
}

static void pointer_button(void* data, struct wl_pointer* pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state_w) {
    auto state = (WaylandState*)data;
    bool pressed = (state_w == WL_POINTER_BUTTON_STATE_PRESSED);

    if (button == BTN_LEFT) {
        state->left_button_down = pressed;
        WPARAM wParam = MK_LBUTTON;
        DispatchMouseToWindows(pressed ? WM_LBUTTONDOWN : WM_LBUTTONUP, wParam, state->mouse_x, state->mouse_y);
    } else if (button == BTN_RIGHT) {
        state->right_button_down = pressed;
        WPARAM wParam = MK_RBUTTON;
        DispatchMouseToWindows(pressed ? WM_RBUTTONDOWN : WM_RBUTTONUP, wParam, state->mouse_x, state->mouse_y);
    }
}

static void pointer_axis(void* data, struct wl_pointer* pointer, uint32_t time, uint32_t axis, wl_fixed_t value) {}
static void pointer_frame(void* data, struct wl_pointer* pointer) {}
static void pointer_axis_source(void* data, struct wl_pointer* pointer, uint32_t axis_source) {}
static void pointer_axis_stop(void* data, struct wl_pointer* pointer, uint32_t time, uint32_t axis) {}
static void pointer_axis_discrete(void* data, struct wl_pointer* pointer, uint32_t axis, int32_t discrete) {}

static const struct wl_pointer_listener pointer_listener = {
    pointer_enter,
    pointer_leave,
    pointer_motion,
    pointer_button,
    pointer_axis,
    pointer_frame,
    pointer_axis_source,
    pointer_axis_stop,
    pointer_axis_discrete
};

// Seat Listener
static void seat_capabilities(void* data, struct wl_seat* seat, uint32_t caps) {
    auto state = (WaylandState*)data;
    if ((caps & WL_SEAT_CAPABILITY_POINTER) && !state->pointer) {
        state->pointer = wl_seat_get_pointer(seat);
        wl_pointer_add_listener(state->pointer, &pointer_listener, state);
    } else if (!(caps & WL_SEAT_CAPABILITY_POINTER) && state->pointer) {
        wl_pointer_destroy(state->pointer);
        state->pointer = nullptr;
    }
}

static void seat_name(void* data, struct wl_seat* seat, const char* name) {}

static const struct wl_seat_listener seat_listener = {
    seat_capabilities,
    seat_name
};

// ============================================================================
// WaylandState Lifecycle & Buffer Management
// ============================================================================

bool WaylandState::Init(int w, int h) {
    width = w;
    height = h;

    display = wl_display_connect(nullptr);
    if (!display) {
        std::cerr << "[Wayland] Failed to connect to Wayland display!" << std::endl;
        return false;
    }

    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, this);
    wl_display_roundtrip(display);

    if (!compositor || !shm || !xdg_wm_base) {
        std::cerr << "[Wayland] Compositor missing required interfaces (wl_compositor, wl_shm, xdg_wm_base)!" << std::endl;
        return false;
    }

    xdg_wm_base_add_listener(xdg_wm_base, &xdg_wm_base_listener, this);

    if (seat) {
        wl_seat_add_listener(seat, &seat_listener, this);
    }

    surface = wl_compositor_create_surface(compositor);
    xdg_surface = xdg_wm_base_get_xdg_surface(xdg_wm_base, surface);
    xdg_surface_add_listener(xdg_surface, &xdg_surface_listener, this);

    xdg_toplevel = xdg_surface_get_toplevel(xdg_surface);
    xdg_toplevel_add_listener(xdg_toplevel, &xdg_toplevel_listener, this);
    xdg_toplevel_set_title(xdg_toplevel, "untitled - Paint");
    xdg_toplevel_set_app_id(xdg_toplevel, "mspaint");

    ResizeBuffer(width, height);
    wl_surface_commit(surface);
    wl_display_roundtrip(display);

    return true;
}

void WaylandState::ResizeBuffer(int w, int h) {
    if (w < 100) w = 100;
    if (h < 100) h = 100;
    width = w;
    height = h;

    if (pixel_buffer) {
        munmap(pixel_buffer, shm_size);
        pixel_buffer = nullptr;
    }
    if (wl_buffer) {
        wl_buffer_destroy(wl_buffer);
        wl_buffer = nullptr;
    }
    if (shm_pool) {
        wl_shm_pool_destroy(shm_pool);
        shm_pool = nullptr;
    }
    if (shm_fd >= 0) {
        close(shm_fd);
        shm_fd = -1;
    }

    int stride = width * 4;
    shm_size = stride * height;

    shm_fd = memfd_create("mspaint-shm", MFD_CLOEXEC);
    if (shm_fd < 0) {
        std::cerr << "[Wayland] memfd_create failed!" << std::endl;
        return;
    }
    ftruncate(shm_fd, shm_size);

    pixel_buffer = (uint32_t*)mmap(nullptr, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (pixel_buffer == MAP_FAILED) {
        std::cerr << "[Wayland] mmap failed!" << std::endl;
        close(shm_fd);
        shm_fd = -1;
        return;
    }

    // Initialize with XP buttonface background
    std::fill(pixel_buffer, pixel_buffer + (width * height), 0xFFECE9D8);

    shm_pool = wl_shm_create_pool(shm, shm_fd, shm_size);
    wl_buffer = wl_shm_pool_create_buffer(shm_pool, 0, width, height, stride, WL_SHM_FORMAT_XRGB8888);

    CWnd* pMain = AfxGetMainWnd();
    if (pMain) {
        pMain->SetWindowPos(nullptr, 0, 0, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

void WaylandState::RenderFrame() {
    if (!configured || !pixel_buffer || !surface || !wl_buffer) return;

    CWnd* pMain = AfxGetMainWnd();
    if (pMain) {
        HDC hdcMain = pMain->GetDC()->m_hDC;
        AfxPaintAllWindows(hdcMain);

        GDISurface* pMainSurf = GetDCSurface(hdcMain);
        if (pMainSurf) {
            int copyW = std::min(width, pMainSurf->width);
            int copyH = std::min(height, pMainSurf->height);
            for (int y = 0; y < copyH; ++y) {
                std::memcpy(pixel_buffer + y * width, pMainSurf->buffer + y * (pMainSurf->stride / 4), copyW * 4);
            }
        }
    }

    wl_surface_attach(surface, wl_buffer, 0, 0);
    wl_surface_damage_buffer(surface, 0, 0, width, height);
    wl_surface_commit(surface);
    wl_display_flush(display);
}

void WaylandState::DispatchEvents() {
    if (display) {
        wl_display_dispatch_pending(display);
        wl_display_flush(display);
    }
}

void WaylandState::Destroy() {
    if (pixel_buffer) munmap(pixel_buffer, shm_size);
    if (wl_buffer) wl_buffer_destroy(wl_buffer);
    if (shm_pool) wl_shm_pool_destroy(shm_pool);
    if (shm_fd >= 0) close(shm_fd);
    if (xdg_toplevel) xdg_toplevel_destroy(xdg_toplevel);
    if (xdg_surface) xdg_surface_destroy(xdg_surface);
    if (surface) wl_surface_destroy(surface);
    if (pointer) wl_pointer_destroy(pointer);
    if (seat) wl_seat_destroy(seat);
    if (xdg_wm_base) xdg_wm_base_destroy(xdg_wm_base);
    if (shm) wl_shm_destroy(shm);
    if (compositor) wl_compositor_destroy(compositor);
    if (registry) wl_registry_destroy(registry);
    if (display) wl_display_disconnect(display);
}
