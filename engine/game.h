#pragma once

#include "opengl.h"
#include "platform.h"

// Global game variable

class Game
{
public:
    virtual void Setup() = 0;
    virtual void Frame() = 0;
    virtual ~Game() {}

    Platform *platform;
};

// You must implement this function
Game *Initialize(OpenGL *gl);