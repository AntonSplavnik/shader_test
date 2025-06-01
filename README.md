# ressit

A minimal, cross-platform testbed for running OpenGL vertex & fragment shaders using [MLX42](https://github.com/codam-coding-college/MLX42).  
Easily load and visualize custom GLSL shaders in a simple, interactive window.

---

## ✨ Features

- **OpenGL 3.2+** context, cross-platform (macOS & Linux)
- Loads custom vertex and fragment shaders from file
- Passes mouse position and time as uniforms to shaders
- Simple shader hot-swapping (just change files & rebuild)
- Escape key closes the window

---

## 🖥️ Platform Support

- **macOS** (OpenGL 3.2+, Homebrew `glfw`, MLX42)
- **Linux** (OpenGL 3.2+, GLEW, GLFW, MLX42)

> ⚠️ Windows is not officially supported (but may work with tweaks)

---

## 🚀 Setup & Build

### Prerequisites

- C compiler (`gcc`, `clang`, etc.)
- `cmake`
- [MLX42 library](https://github.com/codam-coding-college/MLX42) (auto-handled by Makefile)
- **macOS:** [OpenGL](https://developer.apple.com/opengl/), `glfw` (`brew install glfw`)
- **Linux:** `libglew-dev`, `libglfw3-dev` (`sudo apt install libglew-dev libglfw3-dev`)

---

### Build

```bash
git clone <this-repo>
cd <this-repo>
make
```
---

## ▶️ Run
```bash
./raytracer
```

#### By default, it loads these shaders:

./shaders/animated/raytrace.vert
./shaders/animated/raytrace.frag

---

## 🔄 Running Different Shaders

1. Put your custom shaders anywhere (e.g., `./shaders/my.vert`, `./shaders/my.frag`)
2. Edit `main.c` in the `load_shaders()` function:
   ```c
   gl_init->vert_shader = load_file_to_string("./shaders/my.vert");
   gl_init->frag_shader = load_file_to_string("./shaders/my.frag");
   ```
3. Rebuild: `make`
4. Run again: `./raytracer`

---

## 🎮 Controls

ESC: Exit application

Mouse: Position is passed to shader as a uniform (mouse)

---

## 📁 Project Structure
``` bash
.
├── main.c
├── Makefile
├── MLX42/              # Auto-cloned submodule
├── shaders/
│   └── animated/
│       ├── raytrace.vert
│       └── raytrace.frag
```
---

## 🧹 Cleaning

``` bash
make clean      # Remove object files
make fclean     # Remove objects + binary
make distclean  # Full clean (including MLX42)
```
---

## 📝 Notes
Designed for fast shader prototyping, not production rendering.
If you see GLFW/OpenGL errors, double-check that dependencies are installed.
For custom window size, change the WIDTH/HEIGHT defines in main.c.

