#include <cairo.h>
#include <math.h>

#ifdef _WIN32 // Windows-specific code
#include <windows.h>
#include <cairo-win32.h>

// Window procedure function that handles window messages
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Create a Cairo surface associated with the window's HDC (device context)
            cairo_surface_t *surface = cairo_win32_surface_create(hdc);
            // Create a Cairo drawing context for the surface
            cairo_t *cr = cairo_create(surface);

            // Set the background color to white
            cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // White
            cairo_paint(cr); // Fill the drawing area with the background color

            // Draw a green rectangle
            cairo_rectangle(cr, 50, 50, 100, 75);
            cairo_set_source_rgb(cr, 0.0, 0.5, 0.0); // Green color
            cairo_fill(cr); // Fill the rectangle with the current color

            // Draw a blue circle
            cairo_arc(cr, 200, 150, 50, 0, 2 * M_PI);
            cairo_set_source_rgb(cr, 0.0, 0.0, 0.5); // Blue color
            cairo_fill(cr); // Fill the circle with the current color

            // Cleanup: destroy the Cairo context and surface
            cairo_destroy(cr);
            cairo_surface_destroy(surface);

            // End painting the window
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_CLOSE:
            // Close the window when the close button is clicked
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            // Post a quit message to exit the application
            PostQuitMessage(0);
            break;
        default:
            // Handle other messages using the default window procedure
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Entry point of the program for Windows
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register the window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc; // Set the window procedure
    wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "CairoWinAppClass"; // Class name for the window
    RegisterClass(&wc); // Register the window class

    // Create the window with specified class name, title, and style
    HWND hwnd = CreateWindow(wc.lpszClassName, "Cairo WinApp", WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
                             NULL, NULL, wc.hInstance, NULL);

    // Show and update the window
    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    // Enter the main message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Return the exit code from the message loop
    return (int)msg.wParam;
}
#else // Linux-specific code
#include <X11/Xlib.h>
#include <cairo-xlib.h>

// Function to draw shapes using Cairo on Linux
void draw_shapes(Display *display, Window window) {
    // Open a connection to the X server
    display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Could not open display\n");
        exit(1);
    }

    // Get the default screen
    int screen = DefaultScreen(display);

    // Create a window
    window = XCreateSimpleWindow(display, RootWindow(display, screen),
                                 100, 100, 800, 600, 1,
                                 BlackPixel(display, screen),
                                 WhitePixel(display, screen));

    // Select the events to listen for
    XSelectInput(display, window, ExposureMask);

    // Map the window to the screen
    XMapWindow(display, window);

    // Create a Cairo surface for the X window
    cairo_surface_t *surface = cairo_xlib_surface_create(display, window,
                                                         DefaultVisual(display, screen),
                                                         800, 600);
    cairo_t *cr = cairo_create(surface);

    // Set the background color to white
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // White
    cairo_paint(cr); // Fill the drawing area with the background color

    // Draw a green rectangle
    cairo_rectangle(cr, 50, 50, 100, 75);
    cairo_set_source_rgb(cr, 0.0, 0.5, 0.0); // Green color
    cairo_fill(cr); // Fill the rectangle with the current color

    // Draw a blue circle
    cairo_arc(cr, 200, 150, 50, 0, 2 * M_PI);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.5); // Blue color
    cairo_fill(cr); // Fill the circle with the current color

    // Cleanup: destroy the Cairo context and surface
    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    // Clean up X11 resources
    XCloseDisplay(display);
}

// Entry point of the program for Linux
int main() {
    Display *display = NULL;
    Window window;

    // Draw the shapes on the Linux platform
    draw_shapes(display, window);

    // Return 0 to indicate successful execution
    return 0;
}
#endif

// Cross-platform entry point
int main() {
#ifdef _WIN32
    // Windows-specific entry point
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOWNORMAL);
#else
    // Linux-specific entry point
    return main();
#endif
}
