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

#include "linux-opengl.h"

#include <EGL/egl.h>

#include <assert.h>
#include <stdio.h>

void AssertProc(bool exp, const char *proc)
{
    if (!exp)
        printf ("Failed to load opengl function '%s'.\n", proc);

    assert (exp);
}

#define LinuxGLGetProcAddress(Name, NAME) this->Name = (PFN##NAME##PROC) eglGetProcAddress(#Name); AssertProc(this->Name != NULL, #Name)

// Returns 0 if successful, 1 if unsuccessful
LinuxOpenGL *LinuxOpenGL::Load()
{
    // Use this context to query OpenGL/EGL Extensions
    auto loader = new LinuxOpenGL();
    loader->RetrieveExtensions();
    return loader;
}

void LinuxOpenGL::RetrieveExtensions()
{
#pragma GCC diagnostic ignored "-Wcast-function-type"

    LinuxGLGetProcAddress(glCreateShader, GLCREATESHADER);
    LinuxGLGetProcAddress(glShaderSource, GLSHADERSOURCE);
    LinuxGLGetProcAddress(glCompileShader, GLCOMPILESHADER);
    LinuxGLGetProcAddress(glGetShaderiv, GLGETSHADERIV);
    LinuxGLGetProcAddress(glGetShaderInfoLog, GLGETSHADERINFOLOG);
    LinuxGLGetProcAddress(glCreateProgram, GLCREATEPROGRAM);
    LinuxGLGetProcAddress(glAttachShader, GLATTACHSHADER);
    LinuxGLGetProcAddress(glLinkProgram, GLLINKPROGRAM);
    LinuxGLGetProcAddress(glGetProgramiv, GLGETPROGRAMIV);
    LinuxGLGetProcAddress(glGetProgramInfoLog, GLGETPROGRAMINFOLOG);
    LinuxGLGetProcAddress(glDeleteShader, GLDELETESHADER);
    LinuxGLGetProcAddress(glGenVertexArrays, GLGENVERTEXARRAYS);
    LinuxGLGetProcAddress(glGenBuffers, GLGENBUFFERS);
    LinuxGLGetProcAddress(glBindVertexArray, GLBINDVERTEXARRAY);
    LinuxGLGetProcAddress(glBindBuffer, GLBINDBUFFER);
    LinuxGLGetProcAddress(glBufferData, GLBUFFERDATA);
    LinuxGLGetProcAddress(glVertexAttribPointer, GLVERTEXATTRIBPOINTER);
    LinuxGLGetProcAddress(glEnableVertexAttribArray, GLENABLEVERTEXATTRIBARRAY);
    LinuxGLGetProcAddress(glUseProgram, GLUSEPROGRAM);
    LinuxGLGetProcAddress(glGetUniformLocation, GLGETUNIFORMLOCATION);
    LinuxGLGetProcAddress(glUniform4f, GLUNIFORM4F);
    LinuxGLGetProcAddress(glUniform1f, GLUNIFORM1F);
    LinuxGLGetProcAddress(glUniform1i, GLUNIFORM1I);

#pragma GCC diagnostic pop
}
