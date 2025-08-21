## Plan

### MVP Features

- [x] create main menu bar with menus and sub-menus
- [x] append regular text menu items
- [x] separator lines
- [x] check / uncheck mark next to menu items
- [x] enable/disable menu items
- [x] simplified mac and windows examples

### Release Work

- [x] add license
- [x] compile as C99 to latest C, C++98 to latest C++
- [x] compile on all of the above without warnings on windows and mac
- [x] make sure i'm doing cleanup and reasonable-ish memory management
- [x] insert items into menus at specific index
- [x] cross platform SDL2 example
- [x] inline documentation in .h
- [x] double check the API is using typical C practice
	(e.g. enum and type names, should I use stdbool.h or ints for returns, name conventions / case, etc.)

### Further Work

- [ ] Keyboard shortcuts (key equivalents on mac)
	
	Desired behaviour is to forward an event to the event loop (with a flag to say it was triggered by a shortcut) when the shortcut was pressed.

	On mac this should be straight forward using key equivalents?

	On windows it might require creating an accelerator table by populating ACCEL structures. See https://learn.microsoft.com/en-us/windows/win32/menurc/using-keyboard-accelerators#creating-a-run-time-accelerator-table . This may require updating the table when new entries with keyboard shortcuts are added


- [ ] Windows accelerator keys (the thing that happens when you press Alt)

	In a standard Win32 app pressing ALT moves focus to the menu bar, then you can press a key to jump to specific menus, specified by adding an & to the caption.

	Some apps may disable this ALT beahviour (e.g. SDL) so they can use the ALT key for other stuff. In this case I'm not sure if we can manually re-enable the standard ALT behaviour somehow.

	Options:
	1. Do nothing. Accelerator keys are not a cross platform feature anyway.
	2. Allow users to have & in the item caption to get accelerator key behaviour on windows. Strip them out on mac. If they don't work (e.g. because of SDL) assume that's intended and it's up to the user to solve it (e.g. if they DO want accelerator keys in their SDL app).
	3. Have our own API function to enable/disable accelerator keys that works independently of the backend (win32, sdl2, sfml, etc.) so the user gets an easy choice.

- [ ] Review robustness of approach to UTF-8 text on Windows.

	Windows options:
	1. Use the W versions of functions, manually convert captions from UTF8 to UTF16 https://learn.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar
	2. Use the A versions of functions, expect the user will have to the codepage to UTF8

		this is normally done in the manifest

		- [ ] find out if we can set the codepage to UTF8 in code?

	For now I've gone with option 1 on Windows, but may be worth revisiting.
	
- Additional API for menu item queries and traversal

	- [ ] get number of child items
	- [ ] get type of each item (menu, item, separator)
	- [ ] find handle by name
	- [ ] get handle index within parent

- [ ] more cross platform examples (SFML, sokol, etc.)

- [ ] delete items and entire menus/sub-menus

	remove(handle menu or menuItem)

	If this gets implemented, fancier cleanup will be needed (e.g. the HMENU handles on windows).

### Extra ideas

- Add the searchable help menu on mac
- easily create the default menus expected on each platform?
- Dear ImGui implementation
- Linux
- Web (through ImGui fallback?)

## Resources

- MacOS menu docs https://developer.apple.com/documentation/appkit/nsmenu?language=objc
- Apple HIG on menus https://developer.apple.com/design/human-interface-guidelines/the-menu-bar
- Win32 menu docs https://learn.microsoft.com/en-us/windows/win32/menurc/about-menus
	- `AppendMenu` and `InsertMenu` functions are still supported, but `InsertMenuItem` should be used for new applications.
- Windows and UTF-8 https://learn.microsoft.com/en-us/windows/apps/design/globalizing/use-utf8-code-page
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