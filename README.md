# sdl-sandbox

Just a collection of toy projects I've written in SDL

## Building a game

Building a game requires a couple steps. The games directory
contains a cmake project which defines how to build each game.
However, these games have a lot of dependencies, including libraries
defined in this `sdl-sandbox` project. You can install them globally,
or you can install them locally to the games directory.

If you open the terminal in the root directory of `sdl-sandbox`,
you can follow these steps to build the project.

```commandline
cd pi
mkdir build
cd build
cmake ..
cmake --build .
cmake --install . --prefix ../../games
```
This will install the pi engine into the games directory.

To build the games you can then follow these steps:
```commandline
cd ../../games
mkdir build
cd build
cmake ..
cmake --build .
```
