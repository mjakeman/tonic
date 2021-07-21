#include "game.h"

#include <GL/gl.h>

class TonicGame : public Game
{
public:
    TonicGame()
    {
    }

    void Setup()
    {
    }

    void Frame()
    {
        glClearColor(0.0, 1.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
};

Game *Game::Create()
{
    return new TonicGame();   
}