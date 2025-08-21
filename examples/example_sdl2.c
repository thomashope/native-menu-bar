#include <SDL.h>
#include <SDL_syswm.h>
#include <stdio.h>
#include <stdbool.h>

#include "common.inl"

SDL_Window* g_window = NULL;
SDL_Renderer* g_renderer = NULL;

int main(int argc, char* argv[])
{
	UNUSED(argc);
	UNUSED(argv);

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

	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!g_renderer)
	{
		SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(g_window);
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
		SDL_DestroyRenderer(g_renderer);
		SDL_DestroyWindow(g_window);
		SDL_Quit();
		return 1;
	}
#else
	// On macOS, pass nothing (native_menu_bar will handle it)
	createMenuBar(NULL);
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

		SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
		SDL_RenderClear(g_renderer);
		SDL_RenderPresent(g_renderer);

		SDL_Delay(16); // ~60 FPS
	}

	nmb_shutdown();
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	SDL_Quit();
	return 0;
}
