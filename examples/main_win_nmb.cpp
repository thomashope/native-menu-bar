#include <windows.h>		/* must include this before GL/gl.h */
#include <GL/gl.h>			/* OpenGL header file */
#include <GL/glu.h>			/* OpenGL utilities header file */
#include <stdio.h>

#pragma comment(lib, "opengl32.lib")

#include "../native_menu_bar.h"

HWND g_hwnd = NULL;

// Menu item IDs
nmb_Handle idFileNew;
nmb_Handle idFileOpen;
nmb_Handle idFileSave;
nmb_Handle idFileExit;
nmb_Handle idEditCopy;
nmb_Handle idEditPaste;
nmb_Handle idHelpAbout;

void createMenuBar(HWND hWnd)
{
    nmb_Handle hMenuBar = nmb_setup(hWnd);
    nmb_Handle hFileMenu = nmb_appendSubmenu(hMenuBar, "File");
    nmb_Handle hEditMenu = nmb_appendSubmenu(hMenuBar, "Edit");
    nmb_Handle hHelpMenu = nmb_appendSubmenu(hMenuBar, "Help");

    // File menu
    idFileNew = nmb_appendMenuItem(hFileMenu, "New");
    idFileOpen = nmb_appendMenuItem(hFileMenu, "Open...");
    idFileSave = nmb_appendMenuItem(hFileMenu, "Save");
    nmb_appendSeparator(hFileMenu);
    idFileExit = nmb_appendMenuItem(hFileMenu, "Exit");

    // Edit menu
    idEditCopy = nmb_appendMenuItem(hEditMenu, "Copy");
    idEditPaste = nmb_appendMenuItem(hEditMenu, "Paste");

    // Help menu
    idHelpAbout = nmb_appendMenuItem(hHelpMenu, "About...");
}

void drawTriangle()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2i(0, 1);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2i(-1, -1);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2i(1, -1);
    glEnd();
    glFlush();
}

void handleEvents()
{
    nmb_Event_t e;
    while (nmb_pollEvent(&e))
    {
        if (e.sender == idFileNew)
        {
            MessageBox(g_hwnd, L"New file selected", L"Menu", MB_OK);
        }
        else if (e.sender == idFileOpen)
        {
            MessageBox(g_hwnd, L"Open file selected", L"Menu", MB_OK);
        }
        else if (e.sender == idFileSave)
        {
            MessageBox(g_hwnd, L"Save file selected", L"Menu", MB_OK);
        }
        else if (e.sender == idFileExit)
        {
            PostQuitMessage(0);
        }
        else if (e.sender == idEditCopy)
        {
            MessageBox(g_hwnd, L"Copy selected", L"Menu", MB_OK);
        }
        else if (e.sender == idEditPaste)
        {
            MessageBox(g_hwnd, L"Paste selected", L"Menu", MB_OK);
        }
        else if (e.sender == idHelpAbout)
        {
            MessageBox(g_hwnd, L"OpenGL Triangle Demo\nVersion 1.0", L"About", MB_OK);
        }
    }
}

LONG WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static PAINTSTRUCT ps;

    switch (uMsg)
    {
    case WM_PAINT:
        drawTriangle();
        BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        return 0;

    case WM_SIZE:
        glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
        PostMessage(hWnd, WM_PAINT, 0, 0);
        return 0;

    case WM_CHAR:
        switch (wParam)
        {
        case 27: // ESC key
            PostQuitMessage(0);
            break;
        }
        return 0;

    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HWND createOpenGLWindow(wchar_t* title, int x, int y, int width, int height, BYTE type, DWORD flags)
{
    int         pf;
    HDC         hDC;
    HWND        hWnd;
    WNDCLASS    wc;
    PIXELFORMATDESCRIPTOR pfd;
    static HINSTANCE hInstance = 0;

    /* only register the window class once - use hInstance as a flag. */
    if (!hInstance)
    {
        hInstance = GetModuleHandle(NULL);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = (WNDPROC)WindowProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = NULL;
        wc.lpszMenuName = NULL;
        wc.lpszClassName = L"OpenGL";

        if (!RegisterClass(&wc)) {
            MessageBox(NULL, L"RegisterClass() failed:  "
                "Cannot register window class.", L"Error", MB_OK);
            return NULL;
        }
    }

    hWnd = CreateWindow(L"OpenGL", title, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        x, y, width, height, NULL, NULL, hInstance, NULL);

    if (hWnd == NULL)
    {
        MessageBox(NULL, L"CreateWindow() failed:  Cannot create a window.",
            L"Error", MB_OK);
        return NULL;
    }

	g_hwnd = hWnd;

    hDC = GetDC(hWnd);

    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | flags;
    pfd.iPixelType = type;
    pfd.cColorBits = 32;

    pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == 0)
    {
        MessageBox(NULL, L"ChoosePixelFormat() failed: Cannot find a suitable pixel format.", L"Error", MB_OK);
        return 0;
    }

    if (SetPixelFormat(hDC, pf, &pfd) == FALSE)
    {
        MessageBox(NULL, L"SetPixelFormat() failed: Cannot set format specified.", L"Error", MB_OK);
        return 0;
    }

    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    ReleaseDC(hWnd, hDC);

    return hWnd;
}

int APIENTRY WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst, LPSTR lpszCmdLine, int nCmdShow)
{
    HDC hDC;				/* device context */
    HGLRC hRC;				/* opengl context */
    HWND  hWnd;				/* window */
    MSG   msg;				/* message */

    hWnd = createOpenGLWindow(L"Menu Bar Example", 0, 0, 256, 256, PFD_TYPE_RGBA, 0);
    if (hWnd == NULL)
        exit(1);

    hDC = GetDC(hWnd);
    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    ShowWindow(hWnd, nCmdShow);

    createMenuBar(hWnd);

	bool running = true;
	while (running)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

		handleEvents();
    }

    wglMakeCurrent(NULL, NULL);
    ReleaseDC(hWnd, hDC);
    wglDeleteContext(hRC);
    DestroyWindow(hWnd);

    return msg.wParam;
}