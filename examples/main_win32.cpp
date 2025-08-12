#include <windows.h>		/* must include this before GL/gl.h */
#include <GL/gl.h>			/* OpenGL header file */
#include <GL/glu.h>			/* OpenGL utilities header file */
#include <stdio.h>

#pragma comment(lib, "opengl32.lib")

// Menu item IDs
#define ID_FILE_NEW     1001
#define ID_FILE_OPEN    1002
#define ID_FILE_SAVE    1003
#define ID_FILE_EXIT    1004
#define ID_EDIT_COPY    2001
#define ID_EDIT_PASTE   2002
#define ID_HELP_ABOUT   3001

void display()
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

HMENU CreateMenuBar()
{
    HMENU hMenuBar = CreateMenu();
    HMENU hFileMenu = CreatePopupMenu();
    HMENU hEditMenu = CreatePopupMenu();
    HMENU hHelpMenu = CreatePopupMenu();

    // File menu
    AppendMenu(hFileMenu, MF_STRING, ID_FILE_NEW, L"&New");
    AppendMenu(hFileMenu, MF_STRING, ID_FILE_OPEN, L"&Open...");
    AppendMenu(hFileMenu, MF_STRING, ID_FILE_SAVE, L"&Save");
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hFileMenu, MF_STRING, ID_FILE_EXIT, L"E&xit");

    // Edit menu
    AppendMenu(hEditMenu, MF_STRING, ID_EDIT_COPY, L"&Copy");
    AppendMenu(hEditMenu, MF_STRING, ID_EDIT_PASTE, L"&Paste");

    // Help menu
    AppendMenu(hHelpMenu, MF_STRING, ID_HELP_ABOUT, L"&About...");

    // Add popup menus to menu bar
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFileMenu, L"&File");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hEditMenu, L"&Edit");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hHelpMenu, L"&Help");

    return hMenuBar;
}

LONG WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static PAINTSTRUCT ps;

    switch (uMsg)
    {
    case WM_PAINT:
        display();
        BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        return 0;

    case WM_SIZE:
        glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
        PostMessage(hWnd, WM_PAINT, 0, 0);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_FILE_NEW:
            MessageBox(hWnd, L"New file selected", L"Menu", MB_OK);
            break;
        case ID_FILE_OPEN:
            MessageBox(hWnd, L"Open file selected", L"Menu", MB_OK);
            break;
        case ID_FILE_SAVE:
            MessageBox(hWnd, L"Save file selected", L"Menu", MB_OK);
            break;
        case ID_FILE_EXIT:
            PostQuitMessage(0);
            break;
        case ID_EDIT_COPY:
            MessageBox(hWnd, L"Copy selected", L"Menu", MB_OK);
            break;
        case ID_EDIT_PASTE:
            MessageBox(hWnd, L"Paste selected", L"Menu", MB_OK);
            break;
        case ID_HELP_ABOUT:
            MessageBox(hWnd, L"OpenGL Triangle Demo\nVersion 1.0", L"About", MB_OK);
            break;
        }
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

HWND CreateOpenGLWindow(wchar_t* title, int x, int y, int width, int height, BYTE type, DWORD flags)
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

    // Create and set the menu
    HMENU hMenu = CreateMenuBar();
    SetMenu(hWnd, hMenu);

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

    hWnd = CreateOpenGLWindow(L"minimal", 0, 0, 256, 256, PFD_TYPE_RGBA, 0);
    if (hWnd == NULL)
        exit(1);

    hDC = GetDC(hWnd);
    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    ShowWindow(hWnd, nCmdShow);

    while (GetMessage(&msg, hWnd, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    wglMakeCurrent(NULL, NULL);
    ReleaseDC(hWnd, hDC);
    wglDeleteContext(hRC);
    DestroyWindow(hWnd);

    return msg.wParam;
}