# Rotating Pak Vramroro

This project is an OpenGL multimedia application that renders a rotating, textured octagon and plays background music. It utilizes several libraries to manage graphics, audio, and mathematics.

## Features

- **OpenGL Rendering:** Uses GLFW and GLAD to create a window and load OpenGL functions.
- **Shaders:** Implements custom vertex and fragment shaders for rendering.
- **Texture Loading:** Uses stb_image to load image files for texture mapping.
- **Transformations:** Uses GLM to handle transformations and rotations.
- **Audio Playback:** Integrates SDL2_mixer to load and play background music.
- **Interactive Controls:** Allows the user to exit the application by pressing the Escape key.

## Prerequisites

Before building and running the project, ensure that you have:

- A C++ compiler (e.g., GCC/G++ from MSYS2)
- [GLFW](https://www.glfw.org/)
- [GLAD](https://glad.dav1d.de/)
- [GLM](https://glm.g-truc.net/)
- [stb_image](https://github.com/nothings/stb) (included in the project)
- [SDL2](https://www.libsdl.org/) and [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/)
- A working audio system on your Windows machine

## Build Instructions

1. **Configure Tasks in VS Code:**

   The provided `.vscode/tasks.json` file includes tasks for building the project using either `gcc.exe` or `g++.exe`. Ensure that the paths to your compiler and libraries are correctly set.

2. **Compile the Code:**

   You can build the project from the VS Code terminal. For example, using the command:
   
   ```
   cmd /c chcp 65001>nul && C:\msys64\mingw64\bin\g++.exe -fdiagnostics-color=always -g -m64 d:\Alprot\Semester_4\om_gemoy.cpp -o d:\Alprot\Semester_4\om_gemoy.exe -L. -lglfw3 -lglad -lopengl32 -lgdi32 -lfreeglut -lglu32 -lSDL2_mixer -lSDL2main -lSDL2
   ```

3. **Run the Application:**

   Once compiled, run the generated executable (`om_gemoy.exe`). Make sure that the required assets, such as `gemoy.png` for textures and `Pak_Vramroro.mp3` for background music, are located in the same directory as the executable or adjust the paths accordingly.

## Project Structure

- **om_gemoy.cpp:** Contains the main code that initializes OpenGL, loads shaders, sets up vertex data, manages texture loading, and handles audio playback.
- **tasks.json:** Configuration file for Visual Studio Code tasks to build the project.
- **README.md:** This file, providing an overview and instructions.

## Code Overview

- **Initialization:**
  - GLFW is initialized for window creation and OpenGL context.
  - SDL2 and SDL2_mixer are initialized for audio.
  
- **Shader Compilation:**
  - Custom vertex and fragment shaders are created and compiled.
  
- **Texture Loading:**
  - The `stb_image` library is used to load the texture from `gemoy.png`.
  
- **Rendering Loop:**
  - The application enters a loop where it processes input, clears the screen, applies a rotation transformation, and renders a textured octagon using OpenGL.
  
- **Audio:**
  - Background music (`Pak_Vramroro.mp3`) is loaded and played if audio initialization is successful.
  
- **Cleanup:**
  - Resources (shaders, textures, buffers, music) are properly released before termination.

## Troubleshooting

- **Linker Error (`undefined reference to WinMain`):**
  - Ensure that `#define SDL_MAIN_HANDLED` is defined **before** including `SDL2/SDL.h` to prevent SDL from overriding `main()`.
  
- **Library Paths:**
  - Verify that your library paths (in the build tasks and compiler flags) match your local setup.

## License

Place your license information here (if applicable).

## Acknowledgments

- [GLFW](https://www.glfw.org/)
- [GLAD](https://glad.dav1d.de/)
- [GLM](https://glm.g-truc.net/)
- [stb_image](https://github.com/nothings/stb)
- [SDL2](https://www.libsdl.org/)
- [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/)

Enjoy exploring and modifying the code!
