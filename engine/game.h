#pragma once

// TODO: Find a cross platform way of doing this
#include "windows-opengl.h"

// Global game variable

class Game
{
public:
    virtual void Setup() = 0;
    virtual void Frame() = 0;
};

// You must implement this function
Game *Initialize(OpenGL *gl);