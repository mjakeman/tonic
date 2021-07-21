#include "game.h"

#include <GL/gl.h>
#include <iostream>

#include <assert.h>

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

class TonicGame : public Game
{
private:
    OpenGL *gl;
    unsigned int shaderProgram;
    unsigned int VBO, VAO;

public:
    TonicGame(OpenGL *gl)
    {
        this->gl = gl;
    }

    void Setup()
    {
        // build and compile our shader program
        // ------------------------------------
        // vertex shader
        unsigned int vertexShader = gl->glCreateShader(GL_VERTEX_SHADER);
        gl->glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        gl->glCompileShader(vertexShader);
        // check for shader compile errors
        int success;
        char infoLog[512];
        gl->glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            gl->glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // fragment shader
        unsigned int fragmentShader = gl->glCreateShader(GL_FRAGMENT_SHADER);
        gl->glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        gl->glCompileShader(fragmentShader);
        // check for shader compile errors
        gl->glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            gl->glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // link shaders
        shaderProgram = gl->glCreateProgram();
        gl->glAttachShader(shaderProgram, vertexShader);
        gl->glAttachShader(shaderProgram, fragmentShader);
        gl->glLinkProgram(shaderProgram);
        // check for linking errors
        gl->glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            gl->glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        gl->glDeleteShader(vertexShader);
        gl->glDeleteShader(fragmentShader);

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            -0.5f, -0.5f, 0.0f, // left  
            0.5f, -0.5f, 0.0f, // right 
            0.0f,  0.5f, 0.0f  // top   
        }; 

        gl->glGenVertexArrays(1, &VAO);
        gl->glGenBuffers(1, &VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        gl->glBindVertexArray(VAO);

        gl->glBindBuffer(GL_ARRAY_BUFFER, VBO);
        gl->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        gl->glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        gl->glBindBuffer(GL_ARRAY_BUFFER, 0); 

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        gl->glBindVertexArray(0);
    }

    void Frame()
    {
        glClearColor(0.0, 1.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        gl->glUseProgram(shaderProgram);
        gl->glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time 
    }
};

Game *Initialize(OpenGL *gl)
{
    return new TonicGame(gl);
}