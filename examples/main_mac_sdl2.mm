#include <SDL2/SDL.h>
#include <iostream>
#import <Cocoa/Cocoa.h>

@interface MenuHandler : NSObject
- (void)handleAction:(id)sender;
@end

@implementation MenuHandler
- (void)handleAction:(id)sender {
    SDL_Log("Recieved action from: %p", (void*)sender);
}
@end

void createMenuBar()
{
    // Create the main menu bar
    NSMenu *menuBar = [[NSMenu alloc] init];
    MenuHandler* handler = [[MenuHandler alloc] init];

    // Create the Application menu
    NSMenu *appMenu = [[NSMenu alloc] init];
    NSMenuItem *appMenuItem = [[NSMenuItem alloc] init];
    [appMenuItem setSubmenu:appMenu];
    [menuBar addItem:appMenuItem];

    // Add items to Application menu
    NSString *appName = [[NSProcessInfo processInfo] processName];

    NSMenuItem *aboutItem = [[NSMenuItem alloc] initWithTitle:[NSString stringWithFormat:@"About %@",
                                                               appName]
                                                       action:@selector(handleAction:)
                                                keyEquivalent:@""];
    [aboutItem setTarget:handler];
    [appMenu addItem:aboutItem];

    [appMenu addItem:[NSMenuItem separatorItem]];

    NSMenuItem *quitItem = [[NSMenuItem alloc] initWithTitle:[NSString stringWithFormat:@"Quit %@",
                                                              appName]
                                                      action:@selector(handleAction:)
                                               keyEquivalent:@"q"];
    [quitItem setTarget:NSApp];
    [appMenu addItem:quitItem];

    // Create File menu
    NSMenu *fileMenu = [[NSMenu alloc] initWithTitle:@"File"];
    NSMenuItem *fileMenuItem = [[NSMenuItem alloc] initWithTitle:@"File" action:nil keyEquivalent:@""];
    [fileMenuItem setSubmenu:fileMenu];
    [menuBar addItem:fileMenuItem];

    NSMenuItem *newItem = [[NSMenuItem alloc] initWithTitle:@"New"
                                                     action:@selector(handleAction:)
                                              keyEquivalent:@"n"];
    [newItem setTarget:handler];
    [fileMenu addItem:newItem];

    NSMenuItem *openItem = [[NSMenuItem alloc] initWithTitle:@"Open..."
                                                      action:@selector(handleAction:)
                                               keyEquivalent:@"o"];
    [openItem setTarget:handler];
    [fileMenu addItem:openItem];

    [fileMenu addItem:[NSMenuItem separatorItem]];

    NSMenuItem *clearItem = [[NSMenuItem alloc] initWithTitle:@"Clear Screen"
                                                       action:@selector(handleAction:)
                                                keyEquivalent:@"k"];
    [clearItem setTarget:handler];
    [fileMenu addItem:clearItem];

    // Create View menu with SDL-specific options
    NSMenu *viewMenu = [[NSMenu alloc] initWithTitle:@"View"];
    NSMenuItem *viewMenuItem = [[NSMenuItem alloc] initWithTitle:@"View" action:nil keyEquivalent:@""];
    [viewMenuItem setSubmenu:viewMenu];
    [menuBar addItem:viewMenuItem];

    NSMenuItem *fullscreenItem = [[NSMenuItem alloc] initWithTitle:@"Toggle Fullscreen"
                                                            action:@selector(handleAction:)
                                                     keyEquivalent:@"f"];
    [fullscreenItem setTarget:handler];
    [viewMenu addItem:fullscreenItem];

    NSMenuItem *changeColorItem = [[NSMenuItem alloc] initWithTitle:@"Change Background Color"
                                                             action:@selector(handleAction:)
                                                      keyEquivalent:@"b"];
    [changeColorItem setTarget:handler];
    [viewMenu addItem:changeColorItem];

    // Create Window menu
    NSMenu *windowMenu = [[NSMenu alloc] initWithTitle:@"Window"];
    NSMenuItem *windowMenuItem = [[NSMenuItem alloc] initWithTitle:@"Window" action:nil keyEquivalent:@""];
    [windowMenuItem setSubmenu:windowMenu];
    [menuBar addItem:windowMenuItem];

    NSMenuItem *minimizeItem = [[NSMenuItem alloc] initWithTitle:@"Minimize"
                                                          action:@selector(handleAction:)
                                                   keyEquivalent:@"m"];
    [minimizeItem setTarget:handler];
    [windowMenu addItem:minimizeItem];

    // Set the menu bar as the application's main menu
    [NSApp setMainMenu:menuBar];
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("Minimal SDL2 Window",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN);
    
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window,
                                                -1,
                                                SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    createMenuBar();

    // Main loop
    bool running = true;
    SDL_Event event;

    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Clear screen with dark blue
        SDL_SetRenderDrawColor(renderer, 0, 0, 64, 255);
        SDL_RenderClear(renderer);
        
        // Present the rendered frame
        SDL_RenderPresent(renderer);
        
        // Small delay to prevent excessive CPU usage
        SDL_Delay(16);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
