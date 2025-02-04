# RigelRenderer
An OpenGL renderer written in C++. I mostly used [LearnOpenGL](https://learnopengl.com/)
to learn the basics of OpenGL, advanced lighting techniques and shadow mapping, however 
the code has been restructured and rewritten to integrate into RigelRenderer rendering
library. For deferred rendering ,I followed ogldev's [Deferred Shading](https://www.ogldev.org/www/tutorial35/tutorial35.html)
chapters, adapting the code into my renderer.

![Sponza photo](sponza_screenshot.png)

## Build
The project uses CMake, however currently it can only be build for
windows with Visual Studio 17. To generate CMake files, use the following command
``` bash
cmake -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 17 2022" -S . -B CMakeBuild
```
Then you can build the project using Visual Studio.

## Features
- Blinn-Phong lighting
- Deferred rendering
- Light volume culling 
- Shadow mapping for directional and spotlights
- Model loading with assimp
- Input processing
- Debug GUI drawing with Dear ImGui

## License
[MIT](https://choosealicense.com/licenses/mit/)
