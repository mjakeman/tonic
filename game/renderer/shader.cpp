#include "shader.h"

#include "../../engine/platform.h"

Shader::Shader(Platform *platform, OpenGL *gl, std::string vertexData, std::string fragmentData)
{
    this->platform = platform;
    this->gl = gl;

    const char *vertexSource = vertexData.c_str();
    const char *fragmentSource = fragmentData.c_str();

    // Setup Vertex Shader
    unsigned int vertexShader = gl->glCreateShader(GL_VERTEX_SHADER);
    gl->glShaderSource(vertexShader, 1, &vertexSource, NULL);
    gl->glCompileShader(vertexShader);

    // Check for errors
    int success;
    char infoLog[512];
    gl->glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        gl->glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        platform->Log("Error: Vertex Shader Compilation Failed\n%s\n", infoLog);
    }

    // Setup Fragment Shader
    unsigned int fragmentShader = gl->glCreateShader(GL_FRAGMENT_SHADER);
    gl->glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    gl->glCompileShader(fragmentShader);

    // Check for errors
    gl->glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        gl->glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        platform->Log("Error: Fragment Shader Compilation Failed\n%s\n", infoLog);
    }

    // Link Shaders
    shaderId = gl->glCreateProgram();
    gl->glAttachShader(shaderId, vertexShader);
    gl->glAttachShader(shaderId, fragmentShader);
    gl->glLinkProgram(shaderId);

    // Check for errors
    gl->glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
    if (!success)
    {
        gl->glGetProgramInfoLog(shaderId, 512, NULL, infoLog);
        platform->Log("Error: Shader Program Linking Failed\n%s\n", infoLog);
    }

    // Cleanup
    gl->glDeleteShader(vertexShader);
    gl->glDeleteShader(fragmentShader);
}

void Shader::setBool(const std::string &name, bool value) const
{         
    gl->glUniform1i(gl->glGetUniformLocation(shaderId, name.c_str()), (int)value); 
}

void Shader::setInt(const std::string &name, int value) const
{ 
    gl->glUniform1i(gl->glGetUniformLocation(shaderId, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const
{ 
    gl->glUniform1f(gl->glGetUniformLocation(shaderId, name.c_str()), value); 
} 