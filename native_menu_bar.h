#ifndef NATIVE_MENU_BAR_H
#define NATIVE_MENU_BAR_H

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

typedef void* nmb_Handle;

typedef enum nmb_Platform
{
    nmb_Platform_windows,
    nmb_Platform_macos,
} nmb_Platform_t;

typedef enum nmb_EventType
{
	nmb_EventType_none,
	nmb_EventType_itemTriggered,
} nmb_EventType_t;

typedef struct nmb_Event
{
	nmb_EventType_t event;
	nmb_Handle sender;
} nmb_Event_t;

/* On Windows you should pass the HWND of your main window. On macOS you should pass NULL. */
nmb_Handle nmb_setup(void* windowHandle);

bool nmb_pollEvent(nmb_Event_t* event);

nmb_Platform_t nmb_getPlatform();

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
