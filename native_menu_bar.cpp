#include "native_menu_bar.h"

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
