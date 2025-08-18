#include <SDL.h>
#include <SDL_syswm.h>
#include <stdio.h>

#include "../native_menu_bar.h"

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

SDL_Window* g_window = nullptr;

void createMenuBar(void* nativeWindowHandle)
{
	nmb_Handle hMenuBar = nmb_setup(nativeWindowHandle);
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
			SDL_Log("New file selected\n");
		}
		else if (e.sender == g_hFileOpen)
		{
			SDL_Log("Open file selected\n");
		}
		else if (e.sender == g_hFileSave)
		{
			SDL_Log("Save file selected\n");
		}
		else if (e.sender == g_hFileEnabler)
		{
			nmb_setMenuItemEnabled(g_hFileEnablee, !nmb_isMenuItemEnabled(g_hFileEnablee));
		}
		else if (e.sender == g_hFileExit)
		{
			SDL_Event quitEvent;
			quitEvent.type = SDL_QUIT;
			SDL_PushEvent(&quitEvent);
		}
		else if (e.sender == g_hEditCopy)
		{
			SDL_Log("Copy selected\n");
		}
		else if (e.sender == g_hEditPaste)
		{
			SDL_Log("Paste selected\n");
		}
		else if (e.sender == g_hHelpAbout)
		{
			SDL_Log("SDL2 Window Demo\nVersion 1.0\n");
		}
		else
		{
			nmb_setMenuItemChecked(e.sender, !nmb_isMenuItemChecked(e.sender));
		}
	}
}

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
		return 1;
	}

	g_window = SDL_CreateWindow("Menu Bar Example - SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 300, SDL_WINDOW_SHOWN);

	if (!g_window)
	{
		SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

#if defined(_WIN32)
	// On Windows, get HWND for native_menu_bar
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	if (SDL_GetWindowWMInfo(g_window, &wmInfo))
	{
		createMenuBar((void*)wmInfo.info.win.window);
	}
	else
	{
		SDL_Log("Failed to get native window handle\n");
		SDL_DestroyWindow(g_window);
		SDL_Quit();
		return 1;
	}
#else
	// On macOS, pass nothing (native_menu_bar will handle it)
	createMenuBar(nullptr);
#endif

	bool running = true;
	while (running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					running = false;
				}
			}
		}

		handleEvents();

		// Clear window to black
		SDL_Surface* surface = SDL_GetWindowSurface(g_window);
		SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));
		SDL_UpdateWindowSurface(g_window);

		SDL_Delay(16); // ~60 FPS
	}

	SDL_DestroyWindow(g_window);
	SDL_Quit();
	return 0;
}