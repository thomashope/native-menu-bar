#include <windows.h>
#include <stdio.h>

#include "../native_menu_bar.h"

HWND g_hWnd = NULL;

// Menu item IDs
nmb_Handle g_hFileNew;
nmb_Handle g_hFileOpen;
nmb_Handle g_hFileSave;
nmb_Handle g_hFileEnabler;
nmb_Handle g_hFileEnablee;
nmb_Handle g_hFileExit;
nmb_Handle g_hEditCopy;
nmb_Handle g_hEditPaste;
nmb_Handle g_hHelpAbout;

void createMenuBar(HWND hWnd)
{
    nmb_Handle hMenuBar = nmb_setup(hWnd);
    nmb_Handle hFileMenu = nmb_appendSubmenu(hMenuBar, "File");
    nmb_Handle hEditMenu = nmb_appendSubmenu(hMenuBar, "Edit");
    nmb_Handle hHelpMenu = nmb_appendSubmenu(hMenuBar, "Help");

    // File menu
    g_hFileNew = nmb_appendMenuItem(hFileMenu, "New");
    g_hFileOpen = nmb_appendMenuItem(hFileMenu, "Open...");
    g_hFileSave = nmb_appendMenuItem(hFileMenu, "Save");
    nmb_appendMenuItem(hFileMenu, "Check Me");
    g_hFileEnabler = nmb_appendMenuItem(hFileMenu, "Enabler");
    g_hFileEnablee = nmb_appendMenuItem(hFileMenu, "Enablee");
    nmb_appendSeparator(hFileMenu);
    g_hFileExit = nmb_appendMenuItem(hFileMenu, "Exit");

    // Edit menu
    g_hEditCopy = nmb_appendMenuItem(hEditMenu, "Copy");
    g_hEditPaste = nmb_appendMenuItem(hEditMenu, "Paste");

    nmb_Handle submenu = nmb_appendSubmenu(hEditMenu, "Submenu");
    nmb_appendMenuItem(submenu, "Submenu Item 1");

    // Help menu
    g_hHelpAbout = nmb_appendMenuItem(hHelpMenu, "About...");
}

void handleEvents()
{
    nmb_Event_t e;
    while (nmb_pollEvent(&e))
    {
        if (e.sender == g_hFileNew)
        {
            MessageBox(g_hWnd, L"New file selected", L"Menu", MB_OK);
        }
        else if (e.sender == g_hFileOpen)
        {
            MessageBox(g_hWnd, L"Open file selected", L"Menu", MB_OK);
        }
        else if (e.sender == g_hFileSave)
        {
            MessageBox(g_hWnd, L"Save file selected", L"Menu", MB_OK);
        }
        else if (e.sender == g_hFileEnabler)
        {
            nmb_setMenuItemEnabled(g_hFileEnablee, !nmb_isMenuItemEnabled(g_hFileEnablee));
        }
        else if (e.sender == g_hFileExit)
        {
            PostQuitMessage(0);
        }
        else if (e.sender == g_hEditCopy)
        {
            MessageBox(g_hWnd, L"Copy selected", L"Menu", MB_OK);
        }
        else if (e.sender == g_hEditPaste)
        {
            MessageBox(g_hWnd, L"Paste selected", L"Menu", MB_OK);
        }
        else if (e.sender == g_hHelpAbout)
        {
            MessageBox(g_hWnd, L"Win32 Window Demo\nVersion 1.0", L"About", MB_OK);
        }
        else
        {
            nmb_setMenuItemChecked(e.sender, !nmb_isMenuItemChecked(e.sender));
        }
    }
}

LRESULT WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Fill the window with black
        RECT rect;
        GetClientRect(hWnd, &rect);
        HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(hdc, &rect, blackBrush);
        DeleteObject(blackBrush);

        EndPaint(hWnd, &ps);
        return 0;
    }

    case WM_SIZE:
        InvalidateRect(hWnd, NULL, FALSE);
        return 0;

    case WM_CHAR:
        if (wParam == 27) // ESC key
            PostQuitMessage(0);
        return 0;

    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HWND createWindow(wchar_t* title, int x, int y, int width, int height)
{
    static HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASS wc = {};
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"Win32WindowClass";

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"RegisterClass() failed", L"Error", MB_OK);
        return NULL;
    }

    HWND hWnd = CreateWindow(
        L"Win32WindowClass", title,
        WS_OVERLAPPEDWINDOW,
        x, y, width, height,
        NULL, NULL, hInstance, NULL
    );

    if (!hWnd) {
        MessageBox(NULL, L"CreateWindow() failed", L"Error", MB_OK);
        return NULL;
    }

    g_hWnd = hWnd;
    return hWnd;
}

int APIENTRY WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst, LPSTR lpszCmdLine, int nCmdShow)
{
    HWND hWnd = createWindow(L"Menu Bar Example", 0, 0, 400, 300);
    if (!hWnd)
        return 1;

    ShowWindow(hWnd, nCmdShow);
    createMenuBar(hWnd);

    MSG msg;
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

    DestroyWindow(hWnd);
    return (int)msg.wParam;
}