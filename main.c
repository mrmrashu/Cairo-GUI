#include <windows.h>
#include <cairo-win32.h>
#include <cairo.h>
#include <math.h>

// Window procedure function that handles window messages
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            // Begin painting the window
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

// Entry point of the program
int main() {
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

    MSG msg;
    // Enter the main message loop
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Return the exit code from the message loop
    return (int)msg.wParam;
}
