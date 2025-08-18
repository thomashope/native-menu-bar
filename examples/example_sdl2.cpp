#include <windows.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_syswm.h>

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")

// Menu item IDs
#define ID_FILE_NEW     1001
#define ID_FILE_OPEN    1002
#define ID_FILE_SAVE    1003
#define ID_FILE_EXIT    1004
#define ID_EDIT_COPY    2001
#define ID_EDIT_PASTE   2002
#define ID_HELP_ABOUT   3001

// Global variables
SDL_Window* g_window = NULL;
SDL_Renderer* g_renderer = NULL;
HWND g_hwnd = NULL;
WNDPROC g_originalWndProc = NULL;

void render()
{
    // Clear the screen with black
    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
    SDL_RenderClear(g_renderer);

	float t = SDL_GetTicks() * 0.001f;
	int c = (int)(128 + 127 * sin(t));

    SDL_SetRenderDrawColor(g_renderer, c, c, c, 255);
    SDL_Rect rect = { 100, 100, 200, 150 };
    SDL_RenderFillRect(g_renderer, &rect);

    SDL_RenderPresent(g_renderer);
}

HMENU createMenuBar()
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

LRESULT CALLBACK CustomWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_COMMAND)
    {
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
        {
            SDL_Event quit_event;
            quit_event.type = SDL_QUIT;
            SDL_PushEvent(&quit_event);
        }
        break;
        case ID_EDIT_COPY:
            MessageBox(hWnd, L"Copy selected", L"Menu", MB_OK);
            break;
        case ID_EDIT_PASTE:
            MessageBox(hWnd, L"Paste selected", L"Menu", MB_OK);
            break;
        case ID_HELP_ABOUT:
            MessageBox(hWnd, L"SDL2 Renderer Demo\nVersion 1.0", L"About", MB_OK);
            break;
        }
        return 0;
    }

    // Call the original SDL window procedure for other messages
    if (g_originalWndProc)
        return CallWindowProc(g_originalWndProc, hWnd, uMsg, wParam, lParam);

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HWND GetWin32WindowHandleFromSDL(SDL_Window* window)
{
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);

    if (SDL_GetWindowWMInfo(window, &wmInfo))
    {
        if (wmInfo.subsystem == SDL_SYSWM_WINDOWS)
        {
            return wmInfo.info.win.window;
        }
    }
    return NULL;
}

int InitializeSDL()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    // Create window
    g_window = SDL_CreateWindow("SDL2 Renderer with Win32 Menu",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800, 600,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (g_window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    // Create renderer
    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (g_renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    // Get the Win32 window handle
    g_hwnd = GetWin32WindowHandleFromSDL(g_window);
    if (g_hwnd == NULL)
    {
        printf("Could not get Win32 window handle!\n");
        return 0;
    }

    // Subclass the window to handle menu messages
    g_originalWndProc = (WNDPROC)SetWindowLongPtr(g_hwnd, GWLP_WNDPROC, (LONG_PTR)CustomWndProc);

    // Create and set the menu
    HMENU hMenu = createMenuBar();
    SetMenu(g_hwnd, hMenu);

    return 1;
}

void CleanupSDL()
{
    if (g_renderer)
    {
        SDL_DestroyRenderer(g_renderer);
        g_renderer = NULL;
    }

    if (g_window)
    {
        SDL_DestroyWindow(g_window);
        g_window = NULL;
    }

    SDL_Quit();
}

int main(int argc, char* argv[])
{
    if (!InitializeSDL())
    {
        return 1;
    }

    SDL_Event e;
    bool quit = false;

    // Main loop
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_ESCAPE)
                    quit = true;
                break;
            }
        }

        render();

        SDL_Delay(10);// Small delay to prevent excessive CPU usage
    }

    CleanupSDL();
    return 0;
}