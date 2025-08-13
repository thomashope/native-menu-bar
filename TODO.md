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

- MacOS menu docs https://developer.apple.com/documentation/appkit/nsmenu?language=objc
- Win32 menu docs https://learn.microsoft.com/en-us/windows/win32/menurc/about-menus
	- `AppendMenu` and `InsertMenu` functions are still supported, but `InsertMenuItem` should be used for new applications.
- Sublime Text's cross platform menu definition format https://www.sublimetext.com/docs/menus.html
	- caption: The text of the menu entry.
	- mnemonic: The character to use as the key to press to activate the entry. Only applies to Windows and Linux. Must match the case of the character in the "caption".
	- command: A string of the command to execute when the entry is activated.
	- args: A JSON object of args to send to the command.
	- children: A JSON array of entries to create a submenu from.
	- id: A unique string for the menu entry. Used for menu entries with "children" to allow adding additional child entries.
	- platform: One of the strings: "OSX", "!OSX", "Windows", "!Windows", "Linux" or "!Linux". Controls what platforms the entry is shown on.
	- Each menu entry requires at minimum, the key "caption" for a non-functionality entry, or "command" for an entry that performs an action.