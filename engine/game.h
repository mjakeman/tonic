#pragma once

#include "opengl.h"

// Global game variable

class Game
{
public:
    virtual void Setup() = 0;
    virtual void Frame() = 0;
};

// You must implement this function
Game *Initialize(OpenGL *gl);