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

#include "windows-private.h"
#include "windows-opengl.h"

#include <Windows.h>
#include <assert.h>

// See the following article for an overview
// https://mariuszbartosik.com/opengl-4-x-initialization-in-windows-without-a-framework/

// Returns 0 if successful, 1 if unsuccessful
Win32OpenGL *Win32OpenGL::Load()
{
    // In order to query opengl for extensions, we already need
    // to have an opengl context. However, we cannot change the
    // pixel format of a window once chosen, so we need to create
    // a "fake" window here to retrieve our extensions, and then use
    // those extensions later when creating the real window.

    const char *className = "DummyOpenGLContext";
    HINSTANCE instance = GetModuleHandle(0);

    WNDCLASSEXA wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = (WNDPROC)DefWindowProc;
    wcex.hInstance = instance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.lpszClassName = className;

    if (!RegisterClassExA(&wcex))
    {
        Log("Could not register window class\n");
        PrintLastError();
        return NULL;
    }

    // Create a 'fake' window
    HWND handle = CreateWindowExA(
        NULL, className, "OpenGLDummyWindow",
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, 0, 0,
        NULL, NULL, instance, NULL);

    if (!handle)
    {
        Log("Could not create window\n");
        PrintLastError();
        return NULL;
    }

    // Get a 'fake' device context
    HDC device_context = GetDC(handle);

    // Choose a suitable pixel format so we can start querying
    // opengl for extensions
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 24;

    int format = ChoosePixelFormat(device_context, &pfd);

    // TODO: https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)#Simple_Context_Creation
    // ALSO: https://mariuszbartosik.com/opengl-4-x-initialization-in-windows-without-a-framework/

    if (format == 0)
    {
        Log("Could not choose a pixel format\n");
        PrintLastError();
        return NULL;
    }

    if (!SetPixelFormat(device_context, format, &pfd))
    {
        Log("Could not set pixel format\n");
        PrintLastError();
        return NULL;
    }

    DescribePixelFormat(device_context, format, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    // Create an opengl context
    HGLRC gl_context = wglCreateContext(device_context);
    wglMakeCurrent(device_context, gl_context);

    // Use this context to query OpenGL/WGL Extensions
    auto loader = new Win32OpenGL();    
    loader->RetrieveExtensions();

    // Cleanup
    wglMakeCurrent(NULL, NULL);
    ReleaseDC(handle, device_context);
    wglDeleteContext(gl_context);
    DestroyWindow(handle);

    return loader;
}

#include <stdio.h>

void AssertProc(bool exp, const char *proc)
{
    if (!exp)
        printf ("Failed to load opengl function '%s'.\n", proc);

    assert (exp);
}

#define Win32GLGetProcAddress(Name, NAME) this->Name = (PFN##NAME##PROC) wglGetProcAddress(#Name); AssertProc(this->Name != NULL, #Name)

void Win32OpenGL::RetrieveExtensions()
{
#pragma GCC diagnostic ignored "-Wcast-function-type"

    this->wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
    this->wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    Win32GLGetProcAddress(glCreateShader, GLCREATESHADER);
    Win32GLGetProcAddress(glShaderSource, GLSHADERSOURCE);
    Win32GLGetProcAddress(glCompileShader, GLCOMPILESHADER);
    Win32GLGetProcAddress(glGetShaderiv, GLGETSHADERIV);
    Win32GLGetProcAddress(glGetShaderInfoLog, GLGETSHADERINFOLOG);
    Win32GLGetProcAddress(glCreateProgram, GLCREATEPROGRAM);
    Win32GLGetProcAddress(glAttachShader, GLATTACHSHADER);
    Win32GLGetProcAddress(glLinkProgram, GLLINKPROGRAM);
    Win32GLGetProcAddress(glGetProgramiv, GLGETPROGRAMIV);
    Win32GLGetProcAddress(glGetProgramInfoLog, GLGETPROGRAMINFOLOG);
    Win32GLGetProcAddress(glDeleteShader, GLDELETESHADER);
    Win32GLGetProcAddress(glGenVertexArrays, GLGENVERTEXARRAYS);
    Win32GLGetProcAddress(glGenBuffers, GLGENBUFFERS);
    Win32GLGetProcAddress(glBindVertexArray, GLBINDVERTEXARRAY);
    Win32GLGetProcAddress(glBindBuffer, GLBINDBUFFER);
    Win32GLGetProcAddress(glBufferData, GLBUFFERDATA);
    Win32GLGetProcAddress(glVertexAttribPointer, GLVERTEXATTRIBPOINTER);
    Win32GLGetProcAddress(glEnableVertexAttribArray, GLENABLEVERTEXATTRIBARRAY);
    Win32GLGetProcAddress(glUseProgram, GLUSEPROGRAM);

#pragma GCC diagnostic pop
}