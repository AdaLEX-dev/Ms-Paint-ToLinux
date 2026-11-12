# Ms-Paint-ToLinux

A native **Windows 10 MS Paint** implementation built for **Arch Linux & Wayland** in modern C++17 and Qt6.

This repository also preserves the authentic leaked **Windows XP MS Paint** codebase ported directly to native Wayland without Wine or X11 (available on branch [`xp-mspaint`](../../tree/xp-mspaint)).

---

## Features

### 1. Authentic Windows 10 Ribbon Interface
- **Blue File Button (Backstage Menu)**:
  - New (`Ctrl+N`), Open (`Ctrl+O`), Save (`Ctrl+S`), Save As (PNG, JPEG, BMP, GIF)
  - Image Properties (`Ctrl+E`) for units and resolution customization
  - About Paint and Exit
- **Home Tab**:
  - **Clipboard**: Paste (with *Paste From*), Cut (`Ctrl+X`), Copy (`Ctrl+C`)
  - **Image**: Rectangular selection, Free-form selection, Select All (`Ctrl+A`), Delete (`Del`), Transparent selection, Crop, Resize and Skew (`Ctrl+W`), Rotate (90° CW, 90° CCW, 180°, Flip Horizontal, Flip Vertical)
  - **Tools**: Pencil, Fill with color (flood fill), In-place Text tool, Eraser, Color picker (eyedropper), Magnifier
  - **Brushes**: All 9 authentic Windows 10 artistic brushes:
    1. *Brush* (smooth anti-aliased round)
    2. *Calligraphy brush 1* (45° angled ribbon)
    3. *Calligraphy brush 2* (135° angled ribbon)
    4. *Airbrush* (spray particles with realistic distribution)
    5. *Oil brush* (textured bristle stroke)
    6. *Crayon* (chalky textured wax)
    7. *Marker* (semi-translucent highlighter chisel)
    8. *Natural pencil* (fine graphite texture)
    9. *Watercolor brush* (soft feathered wash)
  - **Shapes**: All 23 vector shapes with Outline styles (*Solid, Crayon, Marker, Oil, Natural pencil, Watercolor*) and Fill styles:
    - Line, Curve (Bezier), Oval/Circle, Rectangle/Square, Rounded Rectangle, Polygon
    - Triangle, Right Triangle, Diamond, Pentagon, Hexagon
    - Right Arrow, Left Arrow, Up Arrow, Down Arrow
    - 4-point Star, 5-point Star, 6-point Star
    - Rounded Rectangular Callout, Oval Callout, Cloud Callout
    - Heart, Lightning Bolt
  - **Size**: Line width selector (1px, 2px, 3px, 5px, 8px)
  - **Colors**: Dual Color 1 (Foreground) & Color 2 (Background) swatches + 30-color Windows 10 palette (20 standard + 10 custom slots) + *Edit Colors* dialog
- **View Tab**:
  - Zoom In, Zoom Out, 100% reset
  - Toggle Rulers, Pixel Gridlines (visible at zoom ≥ 400%), and Status Bar
  - Full screen toggle (`F11`)

### 2. High-Performance Canvas Engine
- Native Wayland scaling and multi-monitor fractional scale support.
- Infinite scrollable canvas workspace.
- 8 interactive border grab handles for dynamic canvas resizing.
- Multi-level Undo/Redo history stack (`Ctrl+Z`, `Ctrl+Y`).
- In-place canvas text editing with customizable font, size, and styling.
- Responsive status bar with live cursor coordinates, selection bounding box, canvas dimensions, and interactive zoom slider (12.5% to 800%).

---

## Building and Running

### Prerequisites on Arch Linux
```bash
sudo pacman -S base-devel cmake ninja qt6-base
```

### Build
```bash
make
```
*(Or manually via CMake)*:
```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Run
```bash
# Launch interactive Paint
./build/mspaint

# Or open an image file directly
./build/mspaint /path/to/image.png
```

### Test
```bash
make test
```

---

## Repository Branches
- **`main`**: Windows 10 MS Paint (Modern Ribbon UI, C++17, Qt6, Native Wayland)
- **`xp-mspaint`**: Authentic leaked Windows XP MS Paint ported directly to Wayland using custom GDI software rasterizers and MFC runtime.
