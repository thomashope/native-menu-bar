#ifndef NATIVE_MENU_BAR_H
#define NATIVE_MENU_BAR_H

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
	nmb_Handle sender;
} nmb_Event;

/* On Windows you should pass the HWND of your main window. On macOS you should pass NULL. */
nmb_Handle nmb_setup(void* windowHandle);

bool nmb_pollEvent(nmb_Event* event);

nmb_Platform nmb_getPlatform(void);

nmb_Handle nmb_appendMenu(nmb_Handle parent, const char* caption);

nmb_Handle nmb_appendMenuItem(nmb_Handle parent, const char* caption);

void nmb_appendSeparator(nmb_Handle parent);

void nmb_setMenuItemChecked(nmb_Handle menuItem, bool checked);

bool nmb_isMenuItemChecked(nmb_Handle menuItem);

void nmb_setMenuItemEnabled(nmb_Handle menuItem, bool enabled);

bool nmb_isMenuItemEnabled(nmb_Handle menuItem);


#ifdef __cplusplus
}
#endif

#endif
