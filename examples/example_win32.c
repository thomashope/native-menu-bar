#include <windows.h>
#include <stdio.h>

#include "../native_menu_bar.h"

#include "common.inl"

HWND g_hWnd = NULL;

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
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASS wc;
	memset(&wc, 0, sizeof(WNDCLASS));
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
    UNUSED(hCurrentInst);
	UNUSED(hPreviousInst);
	UNUSED(lpszCmdLine);

    HWND hWnd = createWindow(L"Menu Bar Example - Win32", 100, 100, 400, 300);
    if (!hWnd)
        return 1;

    ShowWindow(hWnd, nCmdShow);
    createMenuBar(hWnd);

    MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
    bool running = true;
    while (running)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
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

    nmb_shutdown();
    DestroyWindow(hWnd);
    return (int)msg.wParam;
}