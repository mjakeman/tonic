#pragma once

#include "../../engine/opengl.h"
#include "../../engine/platform.h"

#include <string>

class Shader
{
private:
    // TODO: Try and avoid having these here. Maybe move this to a Renderer class?
    OpenGL *gl;
    Platform *platform;

public:
    unsigned int shaderId;

    Shader(Platform *platform, OpenGL *gl, std::string vertexData, std::string fragmentData);

    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
};