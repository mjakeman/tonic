#include "internal.h"
#include "game.h"

#include <Windows.h>
#include "gl/GL.h"

LRESULT CALLBACK WindowProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LRESULT result;
    switch (msg)
    {
        default:
        {
            // Let windows handle it for us
            result = DefWindowProc(handle, msg, wparam, lparam);
            break;
        }
    }

    return result;
}

// Prints a human readable version of the last error
static void print_last_error()
{
    LPTSTR errorText = NULL;

    FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM|
        FORMAT_MESSAGE_ALLOCATE_BUFFER|
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        errorText,
        0, NULL
    );

    printf("Error Message: %s\n", errorText);

    LocalFree(errorText);
}

int WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int show_code)
{
    printf("This is project '%s' - win32.\n", PROJECT_NAME);

    char *class_name = PROJECT_NAME;
    char *title = PROJECT_NAME;
    int width = 800;
    int height = 600;    

    WNDCLASSEX wcex = {};
    wcex.cbSize        = sizeof (WNDCLASSEX);
    wcex.style         = CS_OWNDC;
	wcex.lpfnWndProc   = (WNDPROC)WindowProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = instance;
	wcex.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
	wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName  = NULL;
	wcex.lpszClassName = class_name;

    if (!RegisterClassEx (&wcex))
    {
        printf("Could not register window class\n");
        print_last_error();
        return -1;
    }

    HWND handle = CreateWindowExA(
        NULL, class_name, title,
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, instance, NULL
    );
    
    if (!handle)
    {
        printf("Could not create window\n");
        print_last_error();
        return -1;
    }

    HDC device_context = GetDC(handle);

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cColorBits   = 32;

    int format = ChoosePixelFormat(device_context, &pfd);
    
    if (format == 0)
    {
        printf("Could not choose a pixel format\n");
        print_last_error();
        return -1;
    }

    if (!SetPixelFormat(device_context, format, &pfd))
    {
        printf("Could not set pixel format\n");
        print_last_error();
        return -1;
    }

    DescribePixelFormat(device_context, format, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    HGLRC gl_context = wglCreateContext(device_context);
    wglMakeCurrent(device_context, gl_context);

    ShowWindow(handle, show_code);

    setup();

    while (true)
    {
        MSG message;
        if (PeekMessage(&message, handle, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        // Do frame code
        // glClearColor (1.0f, 1.0f, 1.0f, 1.0f);
        // glClear (GL_COLOR_BUFFER_BIT);
        // frame();

        // TODO: This uses legacy profiles
        // We want OpenGL 3+
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2i(0,  1);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2i(-1, -1);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2i(1, -1);
        glEnd();
        glFlush();

        SwapBuffers (device_context);
    }

    wglMakeCurrent(NULL, NULL);
    ReleaseDC(handle, device_context);
    wglDeleteContext(gl_context);
    DestroyWindow(handle);

    return 0;
}
