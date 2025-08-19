#import <Cocoa/Cocoa.h>

#include "../native_menu_bar.h"

namespace
{

nmb_Handle enabler = 0;
nmb_Handle enablee = 0;

}

void setupMenuBar()
{
    nmb_Handle menuBar = nmb_setup();

    // TODO: is the name here determined by the Info.plist ? Is there some reccomended way of setting it in code? Should the API account for this somehow?
    nmb_Handle appMenu = nmb_appendMenu(menuBar, "caption ignored on mac"); // first submenu on mac always recieves the app name
    nmb_Handle aboutItem = nmb_appendMenuItem(appMenu, "About My App");

    nmb_Handle fileMenu = nmb_appendMenu(menuBar, "File");
    nmb_Handle newItem = nmb_appendMenuItem(fileMenu, "New");
    nmb_appendSeparator(fileMenu);
    enabler = nmb_appendMenuItem(fileMenu, "enabler");
    enablee = nmb_appendMenuItem(fileMenu, "enablee");

    nmb_Handle editMenu = nmb_appendMenu(menuBar, "Edit");
    nmb_Handle cutItem = nmb_appendMenuItem(editMenu, "Cut");
    nmb_Handle copyItem = nmb_appendMenuItem(editMenu, "Copy");
    nmb_Handle pasteItem = nmb_appendMenuItem(editMenu, "Paste");
}

// Application delegate interface
@interface AppDelegate : NSObject <NSApplicationDelegate>
@property (strong, nonatomic) NSWindow *window;
@end

// Application delegate implementation
@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Create the main window
    NSRect windowFrame = NSMakeRect(100, 100, 400, 300);

    self.window = [[NSWindow alloc] initWithContentRect:windowFrame
                                              styleMask:(NSWindowStyleMaskTitled | 
                                                        NSWindowStyleMaskClosable | 
                                                        NSWindowStyleMaskMiniaturizable | 
                                                        NSWindowStyleMaskResizable)
                                                backing:NSBackingStoreBuffered
                                                  defer:NO];
    
    [self.window setTitle:@"Native macOS Window with Menu Bar"];
    [self.window setReleasedWhenClosed:NO];
    [self.window center];
    [self.window makeKeyAndOrderFront:nil];

    setupMenuBar();

    // Not a proper main loop but illustrates the point
    [NSTimer scheduledTimerWithTimeInterval:(1.0 / 60.0)
                                     target:self
                                   selector:@selector(updateFrame)
                                   userInfo:nil
                                    repeats:YES];
}

- (void)updateFrame
{
    nmb_Event_t e;
    while(nmb_pollEvent(&e))
    {
        if(e.sender == enabler)
        {
            nmb_setMenuItemEnabled(enablee, !nmb_isMenuItemEnabled(enablee));
        }
        else
        {
            nmb_setMenuItemChecked(e.sender, !nmb_isMenuItemChecked(e.sender));
        }
    }
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

@end

// Main function
int main(int argc, const char * argv[])
{
    @autoreleasepool
    {
        // Create the application
        NSApplication *app = [NSApplication sharedApplication];
        [app setActivationPolicy:NSApplicationActivationPolicyRegular];
        
        // Create and set the app delegate
        AppDelegate *delegate = [[AppDelegate alloc] init];
        [app setDelegate:delegate];
        
        // Activate the app (bring to front)
        [app activateIgnoringOtherApps:YES];
        
        // Run the application event loop
        [app run];
    }
    
    return 0;
}
