#include "native_menu_bar.h"

#include <stdio.h>

#define MAX_EVENTS 64
#define UNUSED(x) (void)(x)

static struct
{
    size_t head;
    size_t tail;
    nmb_Event_t data[MAX_EVENTS];
} events;

static bool getEvent(nmb_Event_t* e)
{
    if (events.head == events.tail) return false; /* No events available */
    *e = events.data[events.head];
    events.head = (events.head + 1) % MAX_EVENTS;
    return true;
}

static void pushEvent(const nmb_Event_t* e)
{
    /* TODO: print a warning if the user isn't consuming events fast enough */
    if ((events.tail + 1) % MAX_EVENTS == events.head)
    {
        /* Buffer is full, overwrite the oldest event */
        events.head = (events.head + 1) % MAX_EVENTS;
    }
    events.data[events.tail] = *e;
    events.tail = (events.tail + 1) % MAX_EVENTS;
}

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static_assert(sizeof(HWND) == sizeof(nmb_Handle), "Window handles must be interchangeable with void*");
static_assert(sizeof(HMENU) == sizeof(nmb_Handle), "Menu handles must be interchangeable with void*");

static struct
{
	HWND hwnd;
	HMENU menuBar;
	WNDPROC originalWndProc;
	UINT nextId;
} g;

static LRESULT CALLBACK menuBarWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_COMMAND)
	{
		nmb_Event_t e;
		e.sender = (nmb_Handle)(uintptr_t)(LOWORD(wParam));
		e.event = nmb_EventType_itemTriggered;
		pushEvent(&e);
		return 0;
	}

	return CallWindowProc(g.originalWndProc, hWnd, uMsg, wParam, lParam);
}

nmb_Handle nmb_setup(void* hWnd)
{
	memset(&g, 0, sizeof(g));
	g.hwnd = (HWND)hWnd;
	g.originalWndProc = (WNDPROC)SetWindowLongPtr(g.hwnd, GWLP_WNDPROC, (LONG_PTR)menuBarWndProc);
	g.menuBar = CreateMenu();
	if (!SetMenu(g.hwnd, g.menuBar))
	{
		printf("Failed to set menu on window: %lu\n", GetLastError());
	}
	if (!DrawMenuBar(g.hwnd))
	{
		printf("Failed to draw menu bar on window: %lu\n", GetLastError());
	}
	return g.menuBar;
}

bool nmb_pollEvent(nmb_Event_t* event)
{
    return getEvent(event);
};

nmb_Platform_t nmb_getPlatform()
{
    return nmb_Platform_windows;
}

nmb_Handle nmb_appendMenu(nmb_Handle parent, const char* caption)
{
	nmb_Handle submenu = CreatePopupMenu();
	BOOL result = AppendMenuA((HMENU)parent, MF_POPUP, (UINT_PTR)submenu, caption); // TODO: pretty sure we don't need to store g.nextId, just use the handle. Right?
	if (!result)
	{
		printf("Failed to append submenu '%s': %lu\n", caption, GetLastError());
		return NULL;
	}
	DrawMenuBar(g.hwnd);
	return submenu;
}

nmb_Handle nmb_appendMenuItem(nmb_Handle parent, const char* caption)
{
	UINT id = g.nextId++;
	BOOL result = AppendMenuA((HMENU)parent, MF_STRING, id, caption);
	if (!result)
	{
		printf("Failed to append submenu '%s': %lu\n", caption, GetLastError());
		return NULL;
	}
	DrawMenuBar(g.hwnd);
	return (nmb_Handle)(uintptr_t)id;
}

void nmb_appendSeparator(nmb_Handle parent)
{
	AppendMenu((HMENU)parent, MF_SEPARATOR, 0, NULL);
	DrawMenuBar(g.hwnd);
}

void nmb_setMenuItemChecked(nmb_Handle menuItem, bool checked)
{
	UINT flags = MF_BYCOMMAND | (checked ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(GetMenu(g.hwnd), (UINT)(uintptr_t)menuItem, flags);
	DrawMenuBar(g.hwnd);
}

bool nmb_isMenuItemChecked(nmb_Handle menuItem)
{
	UINT state = GetMenuState(g.menuBar, (UINT)(uintptr_t)menuItem, MF_BYCOMMAND);
	if (state == (UINT)-1)
	{
		printf("Failed to get menu item state: %lu\n", GetLastError());
		return false;
	}
	return (state & MF_CHECKED) == MF_CHECKED;
}


void nmb_setMenuItemEnabled(nmb_Handle menuItem, bool enabled)
{
	UINT flags = MF_BYCOMMAND | (enabled ? MF_ENABLED : MF_GRAYED);
	BOOL result = EnableMenuItem(g.menuBar, (UINT)(uintptr_t)menuItem, flags);
	if (result == -1)
	{
		printf("Failed to set menu item enabled state: %lu\n", GetLastError());
		return;
	}
	DrawMenuBar(g.hwnd);
}

bool nmb_isMenuItemEnabled(nmb_Handle menuItem)
{
	UINT state = GetMenuState(g.menuBar, (UINT)(uintptr_t)menuItem, MF_BYCOMMAND);
	if (state == (UINT)-1)
	{
		printf("Failed to get menu item state: %lu\n", GetLastError());
		return false;
	}
	return (state & MF_GRAYED) != MF_GRAYED;
}

#else

#import <Cocoa/Cocoa.h>

@interface MenuHandler : NSObject
- (void)handleAction : (id)sender;
@end

static struct
{
    MenuHandler* handler;
} g;

@implementation MenuHandler
- (void)handleAction:(id)sender
{
	nmb_Event_t e;
	e.sender = sender;
	e.event = nmb_EventType_itemTriggered;
    pushEvent(&e);
}
@end

/*

 Mac features

Looks like both MAC and WINDOWS give the app some menus by default.

 Mac: App menu using the Bundle Name from the Info.plist + a Window menu with some stuff in it
 Windows: A default menu when you click on the app icon.

On mac you can access the App menu with [NSApp mainMenu], and presuambly modify it from there?
On windows you can apparently use GetSystenMenu()?

On mac we are likely to want to insert our custom menus after the App menu but before the Window menu, plus some after the Window menu (e.g. Help)

Idea
 appMenu = nmb_getAppMenu() // Returns the app menu on Mac, and system icon menu on windows
 fileMenu = nmb_insertMenuAfter(appMenu, "File")
 windowMenu = nmb_getMenu("Window");
 if(!windowMenu)
 {
    windowMenu = nmb_appendMennu("window")
    ... make the window menu
 }
 nmb_insertMenuAfter(windowMenu, "Help")

Result
 mac: App (default) / File / Window (default) / Help
 windows: Icon (default) / File / Window / Help

---

 - If you do [NSApp setMainMenu] on mac it override the default menus
 - mac uses the Bundle name from Info.plist for the App Menu name

 */

nmb_Handle nmb_setup(void* windowHandle /* unused on mac */)
{
    UNUSED(windowHandle);
    memset(&g, 0, sizeof(g));
	g.handler = [[MenuHandler alloc]init];
    NSMenu* menuBar = [NSApp mainMenu];
	return menuBar;
}

bool nmb_pollEvent(nmb_Event_t* event)
{
    return getEvent(event);
};

nmb_Platform_t nmb_getPlatform()
{
    return nmb_Platform_macos;
}

nmb_Handle nmb_appendMenu(nmb_Handle parent, const char* caption)
{
	NSMenuItem* item = [(NSMenu*)parent addItemWithTitle : [NSString stringWithCString : caption encoding : NSUTF8StringEncoding] action : nil keyEquivalent : @""];
	NSMenu* menu = [[NSMenu alloc]init];
	[item setSubmenu : menu] ;
	return menu;
}

nmb_Handle nmb_appendMenuItem(nmb_Handle parent, const char* caption)
{
	NSMenuItem* item = [(NSMenu*)parent addItemWithTitle : [NSString stringWithCString : caption encoding : NSUTF8StringEncoding] action : @selector(handleAction:) keyEquivalent:@""];
	[item setTarget : g.handler] ;
	return item;
}

void nmb_appendSeparator(nmb_Handle parent)
{
	[(NSMenu*)parent addItem : [NSMenuItem separatorItem] ] ;
}

void nmb_setMenuItemChecked(nmb_Handle menuItem, bool checked)
{
	((NSMenuItem*)menuItem).state = checked ? NSControlStateValueOn : NSControlStateValueOff;
}

bool nmb_isMenuItemChecked(nmb_Handle menuItem)
{
	return ((NSMenuItem*)menuItem).state == NSControlStateValueOn;
}


void nmb_setMenuItemEnabled(nmb_Handle menuItem, bool enabled)
{
	((NSMenuItem*)menuItem).action = enabled ? @selector(handleAction:) : nil;
}

bool nmb_isMenuItemEnabled(nmb_Handle menuItem)
{
	return ((NSMenuItem*)menuItem).enabled;
}

#endif
