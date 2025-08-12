## Goals

Desired User Features

- create main menu bar with menus and sub-menus
- regular text menu items
- separator lines
- check / uncheck mark next to menu items

Desired API features

- append items to menus
- insert items into menus at specific place
- delete items and entire menus/sub-menus
- enable/disable menu items

Platform specific / extra stuff

- keyboard shortcuts
- windows accelerator keys?
- easily create the default menus expected on each platform?
- Dear ImGui implementation?

## Architecture

Desired Platforms

- windows with native win32
- mac native
- SDL2 on windows and mac
- linux?

API

- forward actions back to the user as events, and let them respond on their own?
- OR, fire callbacks (but when do they get called? want it to be consistent across platforms)

## Resources

Info about Win32 menu system https://learn.microsoft.com/en-us/windows/win32/menurc/about-menus

- `AppendMenu` and `InsertMenu` functions are still supported, but `InsertMenuItem` should be used for new applications.