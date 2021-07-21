// The MIT License (MIT)

// Copyright (c) 2021 Matthew Jakeman

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.

#pragma once

#ifdef _WIN32
#include <gl/gl.h>
#endif

#ifdef __linux__
#include <EGL/egl.h>
#endif

// OpenGL Extension Headers
#include "dist/gl/glext.h"

#define GLDefineFunc(Name, NAME) PFN##NAME##PROC Name = NULL

class OpenGL
{
public:
    GLDefineFunc(glCreateShader, GLCREATESHADER);
    GLDefineFunc(glShaderSource, GLSHADERSOURCE);
    GLDefineFunc(glCompileShader, GLCOMPILESHADER);
    GLDefineFunc(glGetShaderiv, GLGETSHADERIV);
    GLDefineFunc(glGetShaderInfoLog, GLGETSHADERINFOLOG);
    GLDefineFunc(glCreateProgram, GLCREATEPROGRAM);
    GLDefineFunc(glAttachShader, GLATTACHSHADER);
    GLDefineFunc(glLinkProgram, GLLINKPROGRAM);
    GLDefineFunc(glGetProgramiv, GLGETPROGRAMIV);
    GLDefineFunc(glGetProgramInfoLog, GLGETPROGRAMINFOLOG);
    GLDefineFunc(glDeleteShader, GLDELETESHADER);
    GLDefineFunc(glGenVertexArrays, GLGENVERTEXARRAYS);
    GLDefineFunc(glGenBuffers, GLGENBUFFERS);
    GLDefineFunc(glBindVertexArray, GLBINDVERTEXARRAY);
    GLDefineFunc(glBindBuffer, GLBINDBUFFER);
    GLDefineFunc(glBufferData, GLBUFFERDATA);
    GLDefineFunc(glVertexAttribPointer, GLVERTEXATTRIBPOINTER);
    GLDefineFunc(glEnableVertexAttribArray, GLENABLEVERTEXATTRIBARRAY);
    GLDefineFunc(glUseProgram, GLUSEPROGRAM);
protected:
    OpenGL() {}
};