#define NOMINMAX
#include "stdafx.h"
#include "global.h"
#include "pbrush.h"
#include "toolbox.h"
#include "imgcolor.h"
#include "bar.h"
#include "wayland/wayland_backend.h"
#include <iostream>
#include <string>

extern int __argc;
extern char** __argv;

int main(int argc, char* argv[]) {
    bool bTestStroke = false;
    bool bDumpWindows = false;

    char* clean_argv[argc + 1];
    int clean_argc = 0;
    clean_argv[clean_argc++] = argv[0];

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            std::cout << "Authentic Windows XP MS Paint (Native Arch Wayland Port)\n"
                      << "Usage: ./mspaint [OPTIONS] [FILE]\n\n"
                      << "Options:\n"
                      << "  --help, -h         Show this help message\n"
                      << "  --test-stroke      Run simulated drawing stroke and export paint_drawn.ppm\n"
                      << "  --dump-windows     Print Win32/MFC window hierarchy and coordinates\n";
            return 0;
        } else if (arg == "--test-stroke") {
            bTestStroke = true;
        } else if (arg == "--dump-windows") {
            bDumpWindows = true;
        } else {
            clean_argv[clean_argc++] = argv[i];
        }
    }
    clean_argv[clean_argc] = nullptr;
    __argc = clean_argc;
    __argv = clean_argv;

    std::cout << "========================================" << std::endl;
    std::cout << "Starting Native Arch Wayland MS Paint..." << std::endl;
    std::cout << "Codebase: Authentic Leaked Windows XP" << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. Connect and initialize Wayland Compositor & XDG Shell
    if (!g_wayland.Init(1024, 768)) {
        std::cerr << "Failed to initialize Wayland subsystem! Ensure WAYLAND_DISPLAY is set." << std::endl;
        return 1;
    }
    std::cout << "[Wayland] Connected to display & initialized 1024x768 XDG toplevel." << std::endl;

    // 2. Initialize authentic Windows XP MS Paint Application
    if (!theApp.InitInstance()) {
        std::cerr << "theApp.InitInstance() returned FALSE!" << std::endl;
        g_wayland.Destroy();
        return 1;
    }
    std::cout << "[MS Paint] theApp.InitInstance() succeeded!" << std::endl;

    if (bDumpWindows) {
        AfxDumpWindows();
    }

    // Initial render
    g_wayland.RenderFrame();
    std::cout << "[MS Paint] Initial frame rendered into Wayland SHM buffer!" << std::endl;

    if (bTestStroke) {
        std::cout << "[MS Paint Test] Simulating drawing a stroke with pencil..." << std::endl;
        AfxDispatchMouse(WM_LBUTTONDOWN, MK_LBUTTON, 200, 200);
        for (int i = 0; i <= 100; i += 5) {
            AfxDispatchMouse(WM_MOUSEMOVE, MK_LBUTTON, 200 + i, 200 + i);
        }
        AfxDispatchMouse(WM_LBUTTONUP, 0, 300, 300);

        g_wayland.RenderFrame();

        if (g_wayland.pixel_buffer) {
            FILE* fp = fopen("paint_drawn.ppm", "wb");
            if (fp) {
                fprintf(fp, "P6\n%d %d\n255\n", g_wayland.width, g_wayland.height);
                for (int i = 0; i < g_wayland.width * g_wayland.height; ++i) {
                    uint32_t px = g_wayland.pixel_buffer[i];
                    uint8_t r = (px >> 16) & 0xFF;
                    uint8_t g = (px >> 8) & 0xFF;
                    uint8_t b = px & 0xFF;
                    fputc(r, fp);
                    fputc(g, fp);
                    fputc(b, fp);
                }
                fclose(fp);
                std::cout << "[MS Paint] Saved drawn test screenshot to paint_drawn.ppm" << std::endl;
            }
        }
    }

    // 3. Main event loop
    while (g_wayland.running) {
        int ret = wl_display_dispatch(g_wayland.display);
        if (ret < 0) break;
    }

    std::cout << "[MS Paint] Exiting cleanly..." << std::endl;
    theApp.ExitInstance();
    g_wayland.Destroy();
    return 0;
}
