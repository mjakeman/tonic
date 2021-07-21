#include "internal.h"

#include <stdio.h>
#include <cstring>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>

#include "game.h"

#include "linux-platform.h"
#include "linux-opengl.h"

// Special thanks to:
// https://github.com/eyelash/tutorials/blob/master/wayland-egl.c
// For setting up an EGL context on Wayland

static wl_compositor *compositor = NULL;
static struct wl_shell *shell = NULL;
static struct wl_egl_window *egl_window = NULL;

static void
on_registry_add_object (void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version)
{
    if (strcmp (interface, "wl_compositor") == 0)
        compositor = (wl_compositor *) wl_registry_bind (registry, name, &wl_compositor_interface, 1);
    else if (strcmp (interface, "wl_shell") == 0)
        shell = (wl_shell *) wl_registry_bind (registry, name, &wl_shell_interface, 1);
}

static void
on_registry_remove_object (void *data, struct wl_registry *registry, uint32_t name)
{
}

static struct wl_registry_listener registry_listener = { &on_registry_add_object, &on_registry_remove_object };



static void
shell_surface_ping (void *data, struct wl_shell_surface *shell_surface, uint32_t serial)
{
	wl_shell_surface_pong (shell_surface, serial);
}

static void
shell_surface_configure (void *data, struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height)
{
	wl_egl_window_resize (egl_window, width, height, 0, 0);
	glViewport (0, 0, width, height);
}

static void
shell_surface_popup_done (void *data, struct wl_shell_surface *shell_surface)
{

}
static struct wl_shell_surface_listener shell_surface_listener = {&shell_surface_ping, &shell_surface_configure, &shell_surface_popup_done};

int LinuxPlatform::Run()
{
    printf("This is project '%s' - linux.\n", PROJECT_NAME);

    struct wl_display *display = wl_display_connect (NULL);
    struct wl_registry *registry = wl_display_get_registry (display);
	wl_registry_add_listener (registry, &registry_listener, NULL);
    wl_display_roundtrip (display);

    EGLDisplay egl_display = eglGetDisplay (display);

    if (!eglInitialize (egl_display, NULL, NULL))
    {
        printf("Could not initialise egl");
        return -1;
    }

    // Create window
    eglBindAPI(EGL_OPENGL_API);
    EGLint attributes[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_NONE
    };

    EGLConfig config;
    EGLint num_config;

    eglChooseConfig(egl_display, attributes, &config, 1, &num_config);
    EGLContext egl_context = eglCreateContext(egl_display, config, EGL_NO_CONTEXT, NULL);

    // TODO: Use xdg_surface instead
    wl_surface *surface = wl_compositor_create_surface (compositor);
    wl_shell_surface * shell_surface = wl_shell_get_shell_surface(shell, surface);
    wl_shell_surface_add_listener(shell_surface, &shell_surface_listener, NULL);
    wl_shell_surface_set_toplevel (shell_surface);

    egl_window = wl_egl_window_create (surface, 800, 600);
    EGLSurface egl_surface = eglCreateWindowSurface (egl_display, config, egl_window, NULL);
    eglMakeCurrent (egl_display, egl_surface, egl_surface, egl_context);

    // Run game setup
    auto game = Initialize(LinuxOpenGL::Load());
    game->Setup ();

    // Run
    while (true)
    {
        // Handle events
        wl_display_dispatch_pending (display);

        // Next frame
        game->Frame ();

        // Finally swap buffers
        eglSwapBuffers (egl_display, egl_surface);
    }

    // Cleanup
    eglDestroySurface (egl_display, egl_surface);
	wl_egl_window_destroy (egl_window);
	wl_shell_surface_destroy (shell_surface);
	wl_surface_destroy (surface);
	eglDestroyContext (egl_display, egl_context);

    eglTerminate (egl_display);
    wl_display_disconnect (display);

    return 0;
}
