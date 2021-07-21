#pragma once

// Global game variable

class Game
{
public:
    // You must implement these functions
    static Game *Create();
    virtual void Setup() = 0;
    virtual void Frame() = 0;
};