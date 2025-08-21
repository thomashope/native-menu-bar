#ifndef NATIVE_MENU_BAR_H
#define NATIVE_MENU_BAR_H

/*

	Native Menu Bar - A simple cross-platform C library for adding native menu bars to your game or app.

EXAMPLE USAGE

	in your app setup function:

		// Initialise the Native Menu Bar

		if(nmb_getPlatform() == nmb_Platform_windows)
		{
			nmb_setup(hWnd); // On Wondows, pass the HWND of your main window
		}
		else if(nmb_getPlatform() == nmb_Platform_macos)
		{
			nmb_setup(NULL); // Pass NULL on macOS
		}

		// build your menu bar

		nmb_Handle hFileMenu = nmb_appendMenu(hMenuBar, "File");
		nmb_Handle hEditMenu = nmb_appendMenu(hMenuBar, "Edit");

		g_hFileNew = nmb_appendMenuItem(hFileMenu, "New");
		g_hFileOpen = nmb_appendMenuItem(hFileMenu, "Open...");
		g_hFileSave = nmb_appendMenuItem(hFileMenu, "Save");
		
		if(nmb_getPlatform() == nmb_Platform_windows)
		{
			nmb_appendSeparator(hFileMenu);
			g_hFileExit = nmb_appendMenuItem(hFileMenu, "Exit");
		}

		g_hEditCopy = nmb_appendMenuItem(hEditMenu, "Copy");
		g_hEditPaste = nmb_appendMenuItem(hEditMenu, "Paste");

	In your frame loop:

		bool running = true;
		while(running)
		{
			// Respond to menu events
			nmb_Event e;
			while (nmb_pollEvent(&e))
			{
				if (e.sender == g_hFileNew)
				{
					printf("New file selected\n");
				}
				else if (e.sender == g_hFileOpen)
				{
					printf("Open file selected\n");
				}
				else if (e.sender == g_hFileSave)
				{
					printf("Save file selected\n");
				}
				else if (e.sender == g_hFileExit)
				{
					running = false; // Exit the app
				}
				else if (e.sender == g_hEditCopy)
				{
					printf("Copy selected\n");
				}
				else if (e.sender == g_hEditPaste)
				{
					printf("Paste selected\n");
				}
			}


			// ... your code ...
		}
*/

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* nmb_Handle;

typedef enum nmb_Platform
{
    nmb_Platform_windows,
    nmb_Platform_macos
} nmb_Platform;

typedef enum nmb_EventType
{
	nmb_EventType_none,
	nmb_EventType_itemTriggered
} nmb_EventType;

typedef struct nmb_Event
{
	nmb_EventType event;
	nmb_Handle sender; /* The handle of the menu item that issued the event. */
} nmb_Event;

/** Call once at the start of your application to set up Native Menu Bar.
*
* @param windowHandle On Windows you should pass the HWND of your main window. On macOS you should pass NULL.
*/
void nmb_setup(void* windowHandle);

/** Every frame (or other regular interval) call this function in a loop to retrieve events sent by menus and respond to them. Once it returns false, all current events have been processed.
*
* @param event Pointer to an nmb_Event structure to be populated.
* @return true if an event was written to the event pointer, false if there are no events to process at the moment.
*/
bool nmb_pollEvent(nmb_Event* event);

/** Get the platform on which the application is running. You may call this function before calling nmb_setup().
*
* @return The platform enum value indicating the current platform.
*/
nmb_Platform nmb_getPlatform(void);

/** Append a new submenu to the given parent menu.
*
* @param parent The parent menu handle, or NULL to create a top-level menu.
* @param caption The name of the submenu.
* @return The handle to the newly created submenu, or NULL on failure.
*/
nmb_Handle nmb_appendMenu(nmb_Handle parent, const char* caption);

/** Creates a new submenu as a child of the parent menu.
*
* @param parent The parent menu handle, or NULL to create a top-level menu.
* @param index The index at which to insert the submenu. If -1, it will be appended to the end.
* @param caption The name of the submenu.
* @return The handle to the newly created submenu, or NULL on failure.
*/
nmb_Handle nmb_insertMenu(nmb_Handle parent, int index, const char* caption);

/** Append a new menu item to the given parent menu. You should store the returned handle so you can respond to events from this menu item later.
*
* @param parent The parent menu handle.
* @param caption The name of the menu item.
* @return The handle to the newly created menu item, or NULL on failure.
*/
nmb_Handle nmb_appendMenuItem(nmb_Handle parent, const char* caption);

/** Creates a new menu item as a child of the parent menu. You should store the returned handle so you can respond to events from this menu item later.
*
* @param parent The parent menu handle.
* @param index The index at which to insert the menu item. If -1, it will be appended to the end.
* @param caption The name of the menu item.
* @return The handle to the newly created menu item, or NULL on failure.
*/
nmb_Handle nmb_insertMenuItem(nmb_Handle parent, int index, const char* caption);

/** Appends a separator line to the given parent menu. */
void nmb_appendSeparator(nmb_Handle parent);

/** Inserts a separator line at the specified index in the parent menu. If index is -1, it will be appended to the end. */
void nmb_insertSeparator(nmb_Handle parent, int index);

/** Sets the checked state of the given menu item. */
void nmb_setMenuItemChecked(nmb_Handle menuItem, bool checked);

/** Returns true if the given menu item is checked. */
bool nmb_isMenuItemChecked(nmb_Handle menuItem);

/** Sets the enabled state of the given menu item. When an item is disabled it is greyed out and cannot be clicked. */
void nmb_setMenuItemEnabled(nmb_Handle menuItem, bool enabled);

/** Returns true if the given menu item is enabled. */
bool nmb_isMenuItemEnabled(nmb_Handle menuItem);

/** If a function fails, call this to get a null-terminated string describing the error. The memory for this string is managed internally, do not free it. */
const char* nmb_getLastError(void);

#ifdef __cplusplus
}
#endif

#endif
