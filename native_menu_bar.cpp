#include "native_menu_bar.h"

#ifdef _WIN32

#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace
{
	const size_t k_maxEvents = 64;

    struct Ctx
    {
        HWND hwnd = 0;
		HMENU menuBar = 0;
        WNDPROC originalWndProc = NULL;
        uintptr_t nextId = 1;

		size_t eventsHead = 0;
		size_t eventsTail = 0;
        nmb_Event_t events[k_maxEvents] = {};

        bool getEvent(nmb_Event_t& e)
        {
            if (eventsHead == eventsTail) return false; // No events available
            e = events[eventsHead];
            eventsHead = (eventsHead + 1) % k_maxEvents;
            return true;
		}

        void pushEvent(const nmb_Event_t& e)
        {
			// TODO: print a warning if the user isn't consuming events fast enough
            if ((eventsTail + 1) % k_maxEvents == eventsHead)
            {
                // Buffer is full, discard the oldest event
                eventsHead = (eventsHead + 1) % k_maxEvents;
            }
            events[eventsTail] = e;
            eventsTail = (eventsTail + 1) % k_maxEvents;
		}
    } g;

    LRESULT CALLBACK menuBarWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (uMsg == WM_COMMAND)
        {
            nmb_Event_t e;
            e.sender = (nmb_Handle)(LOWORD(wParam));
            e.event = nmb_EventType_itemTriggered;
			g.pushEvent(e);
            return 0;
        }

        CallWindowProc(g.originalWndProc, hWnd, uMsg, wParam, lParam);
	}
}

static_assert(sizeof(HWND) == sizeof(nmb_Handle), "Windows handles must be castable to void*");

nmb_Handle nmb_setup(void* hWnd)
{
	g.hwnd = (HWND)hWnd;
    g.originalWndProc = (WNDPROC)SetWindowLongPtr(g.hwnd, GWLP_WNDPROC, (LONG_PTR)menuBarWndProc);
    g.menuBar = CreateMenu();
    SetMenu(g.hwnd, g.menuBar);
    DrawMenuBar(g.hwnd);
    return g.menuBar;
}

nmb_Handle nmb_appendSubmenu(nmb_Handle parent, const char* caption)
{
    nmb_Handle submenu = CreatePopupMenu();
	BOOL result = AppendMenuA((HMENU)parent, MF_POPUP, (UINT_PTR)submenu, caption); // TODO: pretty sure we don't need to store g.nextId, just use the handle. Right?
    if (!result)
    {
		printf("Failed to append submenu '%s': %lu\n", caption, GetLastError());
		return nullptr;
    }
	DrawMenuBar(g.hwnd);
    return submenu;
}

nmb_Handle nmb_appendMenuItem(nmb_Handle parent, const char* caption)
{
	uintptr_t id = g.nextId++;
    BOOL result = AppendMenuA((HMENU)parent, MF_STRING, id, caption);
    if (!result)
    {
        printf("Failed to append submenu '%s': %lu\n", caption, GetLastError());
        return nullptr;
    }
    DrawMenuBar(g.hwnd);
    return (nmb_Handle)id;
}

void nmb_appendSeparator(nmb_Handle parent)
{
    AppendMenu((HMENU)parent, MF_SEPARATOR, 0, NULL);
    DrawMenuBar(g.hwnd);
}

bool nmb_pollEvent(nmb_Event_t* event)
{
	return g.getEvent(*event);
};

void nmb_setMenuItemChecked(nmb_Handle menuItem, bool checked)
{
    UINT flags = MF_BYCOMMAND | (checked ? MF_CHECKED : MF_UNCHECKED);
    CheckMenuItem(GetMenu(g.hwnd), (UINT)menuItem, flags);
    DrawMenuBar(g.hwnd);
}

bool nmb_isMenuItemChecked(nmb_Handle menuItem)
{
    UINT state = GetMenuState(g.menuBar, (UINT)menuItem, MF_BYCOMMAND);
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
    BOOL result = EnableMenuItem(g.menuBar, (UINT)menuItem, flags);
    if (result == -1)
    {
        printf("Failed to set menu item enabled state: %lu\n", GetLastError());
        return;
    }
    DrawMenuBar(g.hwnd);
}

bool nmb_isMenuItemEnabled(nmb_Handle menuItem)
{
    UINT state = GetMenuState(g.menuBar, (UINT)menuItem, MF_BYCOMMAND);
    if (state == (UINT)-1)
    {
        printf("Failed to get menu item state: %lu\n", GetLastError());
        return false;
    }
    return (state & MF_GRAYED) != MF_GRAYED;
}

#else

#include <vector>

#import <Cocoa/Cocoa.h>

@interface MenuHandler : NSObject
- (void)handleAction:(id)sender;
@end

namespace
{

struct Ctx
{
    MenuHandler* handler = nullptr;
    std::vector<nmb_Event_t> events;
} g;

}

@implementation MenuHandler
- (void)handleAction:(id)sender
{
    nmb_Event_t e;
    e.sender = sender;
    e.event = nmb_EventType_itemTriggered;
    g.events.push_back(e);
}
@end

nmb_Handle nmb_setup()
{
    g.handler = [[MenuHandler alloc] init];
    NSMenu *mainMenu = [[NSMenu alloc] init];
    [NSApp setMainMenu:mainMenu];
    return mainMenu;
}

nmb_Handle nmb_appendSubmenu(nmb_Handle parent, const char* caption)
{
    NSMenuItem* item = [(NSMenu*)parent addItemWithTitle:[NSString stringWithCString:caption encoding:NSUTF8StringEncoding] action:nil keyEquivalent:@""];
    NSMenu* menu = [[NSMenu alloc] init];
    [item setSubmenu:menu];
    return menu;
}

nmb_Handle nmb_appendMenuItem(nmb_Handle parent, const char* caption)
{
    NSMenuItem* item = [(NSMenu*)parent addItemWithTitle:[NSString stringWithCString:caption encoding:NSUTF8StringEncoding] action:@selector(handleAction:) keyEquivalent:@""];
    [item setTarget:g.handler];
    return item;
}

void nmb_appendSeparator(nmb_Handle parent)
{
    [(NSMenu*)parent addItem:[NSMenuItem separatorItem]];
}

bool nmb_pollEvent(nmb_Event_t* event)
{
    if(g.events.empty()) return false;
    *event = g.events.front();
    g.events.erase(g.events.begin());
    return true;
};

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