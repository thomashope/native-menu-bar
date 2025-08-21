#include "native_menu_bar.h"

#include <stdio.h>

#define MAX_EVENTS 64
#define ERROR_BUFFER_SIZE 128
#define UNUSED(x) (void)(x)

static char errorBuffer[ERROR_BUFFER_SIZE];

static struct
{
    size_t head;
    size_t tail;
    nmb_Event data[MAX_EVENTS];
} events;

static bool getEvent(nmb_Event* e)
{
    if (events.head == events.tail) return false; /* No events available */
    *e = events.data[events.head];
    events.head = (events.head + 1) % MAX_EVENTS;
    return true;
}

static void pushEvent(const nmb_Event* e)
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

const char* nmb_getLastError(void)
{
	return errorBuffer;
}

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define CAPTION_BUFFER_SIZE 256

static_assert(sizeof(HWND) == sizeof(nmb_Handle), "Window handles must be interchangeable with void*");
static_assert(sizeof(HMENU) == sizeof(nmb_Handle), "Menu handles must be interchangeable with void*");

static struct
{
	HWND hwnd;
	HMENU menuBar;
	WNDPROC originalWndProc;
	UINT nextId;
	WCHAR wcharBuffer[CAPTION_BUFFER_SIZE];
} g;

static LRESULT CALLBACK menuBarWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_COMMAND)
	{
		nmb_Event e;
		e.sender = (nmb_Handle)(uintptr_t)(LOWORD(wParam));
		e.event = nmb_EventType_itemTriggered;
		pushEvent(&e);
		return 0;
	}

	return CallWindowProc(g.originalWndProc, hWnd, uMsg, wParam, lParam);
}

static WCHAR* utf8ToWide(const char* utf8)
{
	if (!utf8) return NULL;
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, g.wcharBuffer, CAPTION_BUFFER_SIZE);
	return g.wcharBuffer;
}

nmb_Handle nmb_setup(void* hWnd)
{
	memset(&g, 0, sizeof(g));
	errorBuffer[0] = 0;
	g.nextId = 1;
	g.hwnd = (HWND)hWnd;
	g.originalWndProc = (WNDPROC)SetWindowLongPtr(g.hwnd, GWLP_WNDPROC, (LONG_PTR)menuBarWndProc);
	g.menuBar = CreateMenu();
	if (!SetMenu(g.hwnd, g.menuBar))
	{
		snprintf(errorBuffer, ERROR_BUFFER_SIZE, "Failed to set menu on window: %lu\n", GetLastError());
	}
	if (!DrawMenuBar(g.hwnd))
	{
		snprintf(errorBuffer, ERROR_BUFFER_SIZE, "Failed to draw menu bar on window: %lu\n", GetLastError());
	}
	return g.menuBar;
}

bool nmb_pollEvent(nmb_Event* event)
{
    return getEvent(event);
};

nmb_Platform nmb_getPlatform()
{
    return nmb_Platform_windows;
}

nmb_Handle nmb_appendMenu(nmb_Handle parent, const char* caption)
{
	return nmb_insertMenu(parent, -1, caption);
}

/* TODO: allow passing negative indices to insert from the end of the menu */
nmb_Handle nmb_insertMenu(nmb_Handle parent, int index, const char* caption)
{
	if (index < -1)
	{
		snprintf(errorBuffer, ERROR_BUFFER_SIZE, "Invalid index '%d' passed to '%s'\n", index, __func__);
		return NULL;
	}

	if (!parent)
	{
		parent = g.menuBar;
	}

	nmb_Handle submenu = CreatePopupMenu();
	BOOL result = InsertMenuW((HMENU)parent, (UINT)index, MF_BYPOSITION | MF_POPUP, (UINT_PTR)submenu, utf8ToWide(caption));
	if (!result)
	{
		snprintf(errorBuffer, ERROR_BUFFER_SIZE, "Failed to insert submenu '%s'. Windows error %lu\n", caption, GetLastError());
		return NULL;
	}
	DrawMenuBar(g.hwnd);
	return submenu;
}

nmb_Handle nmb_appendMenuItem(nmb_Handle parent, const char* caption)
{
	return nmb_insertMenuItem(parent, -1, caption);
}

/* TODO: allow passing negative indices to insert from the end of the menu */
nmb_Handle nmb_insertMenuItem(nmb_Handle parent, int index, const char* caption)
{
	if (index < -1)
	{
		snprintf(errorBuffer, ERROR_BUFFER_SIZE, "Invalid index '%d' passed to '%s'\n", index, __func__);
		return NULL;
	}

	if (!parent)
	{
		snprintf(errorBuffer, ERROR_BUFFER_SIZE, "Failed to create menu item because parent was NULL\n");
		return NULL;
	}

	UINT id = g.nextId++;
	BOOL result = InsertMenuW((HMENU)parent, (UINT)index, MF_BYPOSITION | MF_STRING, id, utf8ToWide(caption));
	if (!result)
	{
		snprintf(errorBuffer, ERROR_BUFFER_SIZE, "Failed to insert menu item '%s'. Windows error %lu\n", caption, GetLastError());
		return NULL;
	}
	DrawMenuBar(g.hwnd);
	return (nmb_Handle)(uintptr_t)id;
}

void nmb_appendSeparator(nmb_Handle parent)
{
	nmb_insertSeparator(parent, -1);
}

void nmb_insertSeparator(nmb_Handle parent, int index)
{
	if (index < -1)
	{
		snprintf(errorBuffer, ERROR_BUFFER_SIZE, "Invalid index '%d' passed to '%s'\n", index, __func__);
		return;
	}

	if (!parent)
	{
		snprintf(errorBuffer, ERROR_BUFFER_SIZE, "Failed to create separator because parent was NULL\n");
		return;
	}

	InsertMenu((HMENU)parent, (UINT)index, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
	DrawMenuBar(g.hwnd);
}

void nmb_setMenuItemChecked(nmb_Handle menuItem, bool checked)
{
	if (!menuItem) return;

	UINT flags = MF_BYCOMMAND | (checked ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(GetMenu(g.hwnd), (UINT)(uintptr_t)menuItem, flags);
	DrawMenuBar(g.hwnd);
}

bool nmb_isMenuItemChecked(nmb_Handle menuItem)
{
	if (!menuItem) return false;

	UINT state = GetMenuState(g.menuBar, (UINT)(uintptr_t)menuItem, MF_BYCOMMAND);
	if (state == (UINT)-1)
	{
		snprintf(errorBuffer, ERROR_BUFFER_SIZE, "Failed to get menu item state: %lu\n", GetLastError());
		return false;
	}
	return (state & MF_CHECKED) == MF_CHECKED;
}


void nmb_setMenuItemEnabled(nmb_Handle menuItem, bool enabled)
{
	if (!menuItem) return;

	UINT flags = MF_BYCOMMAND | (enabled ? MF_ENABLED : MF_GRAYED);
	BOOL result = EnableMenuItem(g.menuBar, (UINT)(uintptr_t)menuItem, flags);
	if (result == -1)
	{
		snprintf(errorBuffer, ERROR_BUFFER_SIZE, "Failed to set menu item enabled state: %lu\n", GetLastError());
		return;
	}
	DrawMenuBar(g.hwnd);
}

bool nmb_isMenuItemEnabled(nmb_Handle menuItem)
{
	if (!menuItem) return false;

	UINT state = GetMenuState(g.menuBar, (UINT)(uintptr_t)menuItem, MF_BYCOMMAND);
	if (state == (UINT)-1)
	{
		snprintf(errorBuffer, ERROR_BUFFER_SIZE, "Failed to get menu item state: %lu\n", GetLastError());
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
    NSMenu* mainMenu; /* AKA the menu bar */
} g;

@implementation MenuHandler
- (void)handleAction:(id)sender
{
	nmb_Event e;
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

static NSString* getApplicationName(void)
{
    NSString *appName = nil;

    // check the plist
    if (!appName)
    {
        appName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleDisplayName"];
    }

    if (!appName)
    {
        appName = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleName"];
    }

    // failing that, use the process name
    if (!appName || [appName length] == 0)
    {
        appName = [[NSProcessInfo processInfo] processName];
    }

    return appName;
}

static void createDefaultMenus(void)
{
    if (NSApp == nil)
        return;

    /* Create the app menu */
    NSString* appName = getApplicationName();
    NSMenu* appMenu = [[NSMenu alloc] initWithTitle:@""];

    /* Add some minimal default menu items (the HIG actually want us to add quite a few more) */
    [appMenu addItemWithTitle:[@"About " stringByAppendingString:appName] action:@selector(orderFrontStandardAboutPanel:) keyEquivalent:@""];
    [appMenu addItem:[NSMenuItem separatorItem]];
    [appMenu addItemWithTitle:[@"Quit " stringByAppendingString:appName] action:@selector(terminate:) keyEquivalent:@"q"];

    /* Attach it the app */
    NSMenuItem* appMenuItem = [[NSMenuItem alloc] init];
    [appMenuItem setSubmenu:appMenu];
    [[NSApp mainMenu] addItem:appMenuItem];
}

nmb_Handle nmb_setup(void* windowHandle /* unused on mac */)
{
    UNUSED(windowHandle);
    memset(&g, 0, sizeof(g));
	errorBuffer[0] = 0;
	g.handler = [[MenuHandler alloc]init];

    /* Check if someone else (e.g. SDL) already built the app menu */
    NSInteger numItemsInAppleMenu = [[[[NSApp mainMenu] itemAtIndex:0] submenu] numberOfItems];
    bool addDefaultMenuItems = numItemsInAppleMenu == 0;

    /* If not, add some default menu items */
    if(addDefaultMenuItems)
    {
        /* To add a custom app menu, we have to create our own main menu (aka menu bar) */
        g.mainMenu = [[NSMenu alloc] init];
        [NSApp setMainMenu:g.mainMenu];
        createDefaultMenus();
    }
    else
    {
        g.mainMenu = [NSApp mainMenu];
    }

    return g.mainMenu;
}

bool nmb_pollEvent(nmb_Event* event)
{
    return getEvent(event);
}

nmb_Platform nmb_getPlatform(void)
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
