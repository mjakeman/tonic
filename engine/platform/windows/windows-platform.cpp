/* windows-platform.cpp
 *
 * Copyright 2021 Matthew Jakeman <mjakeman26@outlook.co.nz>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * SPDX-License-Identifier: MIT
 */

#include "windows-private.h"
#include "windows-platform.h"
#include "windows-opengl.h"

#include "../../internal.h"
#include "../../game.h"

#define SUBSYSTEM WINDOWS

#include <gl/GL.h>

static bool running = true;

static LRESULT CALLBACK WindowProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = 0;
    switch (msg)
    {
    case WM_DESTROY:
    {
        running = false;
        break;
    }

    case WM_SIZE:
    {
        UINT width = LOWORD(lparam);
        UINT height = HIWORD(lparam);
        glViewport(0, 0, width, height);
        break;
    }

    default:
    {
        // Let windows handle it for us
        result = DefWindowProc(handle, msg, wparam, lparam);
        break;
    }
    }

    return result;
}

int Win32Platform::Run(HINSTANCE instance, int show_code)
{
    Log("This is project '%s' - win32.\n", PROJECT_NAME);

    const char *class_name = PROJECT_NAME;
    const char *title = PROJECT_NAME;
    int width = 800;
    int height = 600;

    WNDCLASSEXA wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEXA);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = (WNDPROC)WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = instance;
    wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = class_name;

    if (!RegisterClassExA(&wcex))
    {
        Log("Could not register window class\n");
        PrintLastError();
        return -1;
    }

    HWND handle = CreateWindowExA(
        0, class_name, title,
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, instance, NULL);

    if (!handle)
    {
        Log("Could not create window\n");
        PrintLastError();
        return -1;
    }

    // Get OpenGL Extensions
    auto loader = Win32OpenGL::Load();

    if (loader == NULL)
        return -1;

    // TODO: https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)#Simple_Context_Creation
    // ALSO: https://mariuszbartosik.com/opengl-4-x-initialization-in-windows-without-a-framework/

    HDC deviceContext = GetDC(handle);

    // Retrieve a suitable Pixel Format from WGL
    const int pixelAttribs[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_ALPHA_BITS_ARB, 8,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
        WGL_SAMPLES_ARB, 4,
        0};

    int format;
    UINT numFormats;
    bool status = loader->wglChoosePixelFormatARB(deviceContext, pixelAttribs, NULL, 1, &format, &numFormats);

    if (status == false || numFormats == 0)
    {
        Log("wglChoosePixelFormatARB() failed (numFormats = %d).\n", numFormats);
        return -1;
    }

    // Create a Windows-style pfd from our WGL pixel format above

    PIXELFORMATDESCRIPTOR pfd;
    DescribePixelFormat(deviceContext, format, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    if (!SetPixelFormat(deviceContext, format, &pfd))
    {
        Log("Could not set pixel format\n");
        PrintLastError();
        return -1;
    }

    // Create OpenGL Core Context
    const int major_min = 4, minor_min = 5;
    int contextAttribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
        WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0};

    // Create the context
    HGLRC glContext = loader->wglCreateContextAttribsARB(deviceContext, 0, contextAttribs);
    if (glContext == NULL)
    {
        Log("wglCreateContextAttribsARB() failed.\n");
        return -1;
    }

    // Make the context current
    if (!wglMakeCurrent(deviceContext, glContext))
    {
        Log("wglMakeCurrent() failed.\n");
        return -1;
    }

    // And, we're done!
    ShowWindow(handle, show_code);

    auto game = Initialize(loader);
    game->Setup();

    while (running)
    {
        MSG message;
        if (PeekMessage(&message, handle, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        // Do frame code
        // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);
        game->Frame();

        // TODO: This uses legacy profiles
        // We want OpenGL 3+
        // glClear(GL_COLOR_BUFFER_BIT);
        // glBegin(GL_TRIANGLES);
        // glColor3f(1.0f, 0.0f, 0.0f);
        // glVertex2i(0,  1);
        // glColor3f(0.0f, 1.0f, 0.0f);
        // glVertex2i(-1, -1);
        // glColor3f(0.0f, 0.0f, 1.0f);
        // glVertex2i(1, -1);
        // glEnd();
        // glFlush();

        SwapBuffers(deviceContext);
    }

    wglMakeCurrent(NULL, NULL);
    ReleaseDC(handle, deviceContext);
    wglDeleteContext(glContext);
    DestroyWindow(handle);

    return 0;
}
