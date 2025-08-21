## Plan

MVP Features

- [x] create main menu bar with menus and sub-menus
- [x] append regular text menu items
- [x] separator lines
- [x] check / uncheck mark next to menu items
- [x] enable/disable menu items
- [x] simplified mac and windows examples

Release Work

- [x] add license
- [x] compile as C99 to latest C, C++98 to latest C++
- [ ] compile on all of the above without warnings on windows and mac
- [ ] make sure i'm doing cleanup and proper memory management and API and examples
- [x] cross platform SDL2 example
- [ ] inline documentation in .h
- [x] double check the API is using typical C practice
	(e.g. enum and type names, should I use stdbool.h or ints for returns, name conventions / case, etc.)
- [ ] decide on text encoding (UTF8 everywhere?) and implement

Further Work (prioritise top to bottom)

- [ ] Keyboard shortcuts (key equivalents on mac)
- [ ] Windows accelerator keys (the thing that happens when you press Alt)
- [ ] insert items into menus at specific place
	insertMenuAfter(handle, "new item")
	insertMenuItemAfter()
	insertMenuBefore()
	insertMenuItemBefore()
- [ ] cross platform SFML example (or something else popular)
- [ ] delete items and entire menus/sub-menus
	remove(handle menu or menuItem)

Extra ideas

- Add the searchable help menu on mac
- easily create the default menus expected on each platform?
- Dear ImGui implementation
- Linux

API

- forward actions back to the user as events, and let them respond on their own?
- OR, fire callbacks (but when do they get called? want it to be consistent across platforms)

## Resources

- MacOS menu docs https://developer.apple.com/documentation/appkit/nsmenu?language=objc
- Apple HIG on menus https://developer.apple.com/design/human-interface-guidelines/the-menu-bar
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

## References

- https://sourceforge.net/projects/tinyfiledialogs/files/