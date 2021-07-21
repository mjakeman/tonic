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

#include <Windows.h>
#include <stdio.h>

// Prints a human readable version of the last error
void PrintLastError()
{
    LPTSTR errorText = NULL;

    FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        errorText,
        0, NULL);

    printf("Error Message: %s\n", errorText);

    LocalFree(errorText);
}

// Thanks to: https://gist.github.com/syu5-gh/eaa0018ed70836b7279b
void Log(const char *fmt, ...)
{
    va_list argp;
    va_start(argp, fmt);
    char dbg_out[4096];
    vsprintf_s(dbg_out, fmt, argp);
    va_end(argp);
    OutputDebugStringA(dbg_out);
}