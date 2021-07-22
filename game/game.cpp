#include "game.h"

#include <iostream>
#include <assert.h>
#include <math.h>

class TonicGame : public Game
{
private:
    OpenGL *gl;
    unsigned int shaderProgram;
    unsigned int VBO, VAO;
    float timeValue = 0.0f;

public:
    TonicGame(OpenGL *gl)
    {
        this->gl = gl;
    }

    void Setup()
    {
        std::string vertexShaderStr = platform->ReadFileToString("/share/tonic/shader.vert");
        const char* vertexShaderSource = vertexShaderStr.c_str();
        // printf("%s", vertexShaderSource);

        std::string fragmentShaderStr = platform->ReadFileToString("/share/tonic/shader.frag");
        const char* fragmentShaderSource = fragmentShaderStr.c_str();
        // printf("%s", fragmentShaderSource);
        
        // printf("%s\n", fragmentShaderSource);

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
        glClearColor(0.0, 17.0f/256, 43.0f/256, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        timeValue += 0.001f;
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = gl->glGetUniformLocation(shaderProgram, "ourColor");
        gl->glUseProgram(shaderProgram);
        gl->glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        gl->glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time 
    }
};

Game *Initialize(OpenGL *gl)
{
    return new TonicGame(gl);
}