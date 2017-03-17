# matrix-row-reducer
A simple command line guide for row-reducing matrices

Requires C++11. Practice Gauss-Jordan elimination or have the steps automatically calculated.

Build with Xcode or something along the lines of `clang main.cpp Matrix.cpp -std=c++11 -stdlib=libc++ -lc++`.

## Building with Emscripten Console

This code is runnable in a web browser (see [demo](http://gabemontague.com/matrix-row-reducer/index.html?command=rowreduce&loads=rowreduce%2Cmatrixmake) via [emscripten-console](github.com/montaguegabe/emscripten-console)). To build for web browser:
1) Install Emscripten
2) Clone the [emscripten-console](github.com/montaguegabe/emscripten-console) repository into this repository (it will be gitignore'd)
3) Run `grunt bc` (builds then serves) within the emscripten-console repository. Settings from Emscripten.config will be read, and the executable will be packaged into the online console prompt.
4) In the online console, type `load rowreduce` then `rowreduce` to run the main application. To generate a random matrix, similarly load the `matrixmake` command then run it.
