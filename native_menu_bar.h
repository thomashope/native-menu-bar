#ifndef NATIVE_MENU_BAR_H
#define NATIVE_MENU_BAR_H

typedef void* nmb_Handle;

enum nmb_EventType_t
{
    nmb_EventType_none,
    nmb_EventType_itemTriggered,
};

struct nmb_Event_t
{
    nmb_EventType_t event;
    nmb_Handle sender;
};

nmb_Handle nmb_setup(void* windowHandle = nullptr); // TODO: we only need this on windows, what to do on mac?

nmb_Handle nmb_appendSubmenu(nmb_Handle parent, const char* caption);

nmb_Handle nmb_appendMenuItem(nmb_Handle parent, const char* caption);

void nmb_appendSeparator(nmb_Handle parent);

bool nmb_pollEvent(nmb_Event_t* event);

void nmb_setMenuItemChecked(nmb_Handle menuItem, bool checked);

bool nmb_isMenuItemChecked(nmb_Handle menuItem);

void nmb_setMenuItemEnabled(nmb_Handle menuItem, bool enabled);

bool nmb_isMenuItemEnabled(nmb_Handle menuItem);

#endif
