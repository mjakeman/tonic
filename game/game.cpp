#include "game.h"

#include <iostream>
#include <assert.h>
#include <math.h>

#include "renderer/shader.h"

class TonicGame : public Game
{
private:
    OpenGL *gl;
    unsigned int VBO, VAO;
    float timeValue = 0.0f;
    Shader *shader;

public:
    TonicGame(OpenGL *gl)
    {
        this->gl = gl;
    }

    void Setup()
    {
        std::string vertShader = platform->ReadFileToString("/share/tonic/shaders/basic.vert");
        std::string fragShader = platform->ReadFileToString("/share/tonic/shaders/basic.frag");
        
        shader = new Shader(platform, gl, vertShader, fragShader);

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        
        float vertices[] = {
            // positions         // colors
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
        };

        gl->glGenVertexArrays(1, &VAO);
        gl->glGenBuffers(1, &VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        gl->glBindVertexArray(VAO);

        gl->glBindBuffer(GL_ARRAY_BUFFER, VBO);
        gl->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        // position attribute
        gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        gl->glEnableVertexAttribArray(0);
        // color attribute
        gl->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
        gl->glEnableVertexAttribArray(1);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        gl->glBindBuffer(GL_ARRAY_BUFFER, 0); 

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        gl->glBindVertexArray(0);
    }

    void Frame(float deltaTime)
    {
        glClearColor(0.0, 17.0f/256, 43.0f/256, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        gl->glUseProgram(shader->shaderId);

        gl->glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time 
    }
};

Game *Initialize(OpenGL *gl)
{
    return new TonicGame(gl);
}