orbit
=====

A game engine in C++ and OpenGL 3.1.

This is a personal and educational project of mine, so I will not accept pull requests. However, feel free to fork the code and go ham on it. Also, feel free to send me questions or feedback.

how to compile manually for linux
=====

*step 1: download dependencies*
  * OpenGL ≥3.1 - Update graphics drivers or download [mesa ≥10.0](mesa3d.org) for software emulation.
  * [glfw](glfw.org)
  * [glew] (glew.sourceforge.net)

*step 2: clone*

```
git clone https://github.com/vondel/orbit
```

*step 3: compile*

go to the 'orbit' folder and run
```
clang++ -o orbit -std=c++11 -g -Wall -W -lglfw -lGL -lGLEW -lm  src/main.cpp
```

*step 4: run*

```
./orbit
```
