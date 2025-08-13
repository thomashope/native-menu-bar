#import <Cocoa/Cocoa.h>

// Application delegate interface
@interface AppDelegate : NSObject <NSApplicationDelegate>
@property (strong, nonatomic) NSWindow *window;
@end

// Application delegate implementation
@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Create the main window
    NSRect windowFrame = NSMakeRect(100, 100, 800, 600);
    
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
    
    // Add some basic content to the window
    NSTextField *label = [[NSTextField alloc] initWithFrame:NSMakeRect(50, 300, 700, 50)];
    [label setStringValue:@"Hello from a native macOS window created with Objective-C++!"];
    [label setEditable:NO];
    [label setBezeled:NO];
    [label setDrawsBackground:NO];
    [label setFont:[NSFont systemFontOfSize:18]];
    [label setAlignment:NSTextAlignmentCenter];
    
    [self.window.contentView addSubview:label];
    
    // Create and set up the menu bar
    [self setupMenuBar];
}

- (void)setupMenuBar {
    // Create the main menu bar
    NSMenu *menuBar = [[NSMenu alloc] init];
    
    // Create the Application menu
    NSMenu *appMenu = [[NSMenu alloc] init];
    NSMenuItem *appMenuItem = [[NSMenuItem alloc] init];
    [appMenuItem setSubmenu:appMenu];
    [menuBar addItem:appMenuItem];
    
    // Add items to Application menu
    NSString *appName = [[NSProcessInfo processInfo] processName];
    
    NSMenuItem *aboutItem = [[NSMenuItem alloc] initWithTitle:[NSString stringWithFormat:@"About %@", appName]
                                                       action:@selector(showAbout:)
                                                keyEquivalent:@""];
    [aboutItem setTarget:self];
    [appMenu addItem:aboutItem];
    
    [appMenu addItem:[NSMenuItem separatorItem]];
    
    NSMenuItem *quitItem = [[NSMenuItem alloc] initWithTitle:[NSString stringWithFormat:@"Quit %@", appName]
                                                      action:@selector(terminate:)
                                               keyEquivalent:@"q"];
    [quitItem setTarget:NSApp];
    [appMenu addItem:quitItem];
    
    // Create File menu
    NSMenu *fileMenu = [[NSMenu alloc] initWithTitle:@"File"];
    NSMenuItem *fileMenuItem = [[NSMenuItem alloc] initWithTitle:@"File" action:nil keyEquivalent:@""];
    [fileMenuItem setSubmenu:fileMenu];
    [menuBar addItem:fileMenuItem];
    
    NSMenuItem *newItem = [[NSMenuItem alloc] initWithTitle:@"New"
                                                     action:@selector(newDocument:)
                                              keyEquivalent:@"n"];
    [newItem setTarget:self];
    [fileMenu addItem:newItem];
    
    NSMenuItem *openItem = [[NSMenuItem alloc] initWithTitle:@"Open..."
                                                      action:@selector(openDocument:)
                                               keyEquivalent:@"o"];
    [openItem setTarget:self];
    [fileMenu addItem:openItem];
    
    [fileMenu addItem:[NSMenuItem separatorItem]];
    
    NSMenuItem *closeItem = [[NSMenuItem alloc] initWithTitle:@"Close Window"
                                                       action:@selector(performClose:)
                                                keyEquivalent:@"w"];
    [closeItem setTarget:nil]; // Responder chain will handle this
    [fileMenu addItem:closeItem];
    
    // Create Edit menu
    NSMenu *editMenu = [[NSMenu alloc] initWithTitle:@"Edit"];
    NSMenuItem *editMenuItem = [[NSMenuItem alloc] initWithTitle:@"Edit" action:nil keyEquivalent:@""];
    [editMenuItem setSubmenu:editMenu];
    [menuBar addItem:editMenuItem];
    
    NSMenuItem *cutItem = [[NSMenuItem alloc] initWithTitle:@"Cut"
                                                     action:@selector(cut:)
                                              keyEquivalent:@"x"];
    [editMenu addItem:cutItem];
    
    NSMenuItem *copyItem = [[NSMenuItem alloc] initWithTitle:@"Copy"
                                                      action:@selector(copy:)
                                               keyEquivalent:@"c"];
    [editMenu addItem:copyItem];
    
    NSMenuItem *pasteItem = [[NSMenuItem alloc] initWithTitle:@"Paste"
                                                       action:@selector(paste:)
                                                keyEquivalent:@"v"];
    [editMenu addItem:pasteItem];
    
    // Create Window menu
    NSMenu *windowMenu = [[NSMenu alloc] initWithTitle:@"Window"];
    NSMenuItem *windowMenuItem = [[NSMenuItem alloc] initWithTitle:@"Window" action:nil keyEquivalent:@""];
    [windowMenuItem setSubmenu:windowMenu];
    [menuBar addItem:windowMenuItem];
    
    NSMenuItem *minimizeItem = [[NSMenuItem alloc] initWithTitle:@"Minimize"
                                                          action:@selector(performMiniaturize:)
                                                   keyEquivalent:@"m"];
    [windowMenu addItem:minimizeItem];
    
    NSMenuItem *zoomItem = [[NSMenuItem alloc] initWithTitle:@"Zoom"
                                                      action:@selector(performZoom:)
                                               keyEquivalent:@""];
    [windowMenu addItem:zoomItem];
    
    // Set the menu bar as the application's main menu
    [NSApp setMainMenu:menuBar];
}

// Action methods for menu items
- (void)showAbout:(id)sender {
    NSAlert *alert = [[NSAlert alloc] init];
    [alert setMessageText:@"About This Application"];
    [alert setInformativeText:@"A simple macOS application created with Objective-C++\n\nThis demonstrates creating a native window with a custom menu bar."];
    [alert addButtonWithTitle:@"OK"];
    [alert runModal];
}

- (void)newDocument:(id)sender {
    NSAlert *alert = [[NSAlert alloc] init];
    [alert setMessageText:@"New Document"];
    [alert setInformativeText:@"New document functionality would be implemented here."];
    [alert addButtonWithTitle:@"OK"];
    [alert runModal];
}

- (void)openDocument:(id)sender {
    NSOpenPanel *openPanel = [NSOpenPanel openPanel];
    [openPanel setCanChooseFiles:YES];
    [openPanel setCanChooseDirectories:NO];
    [openPanel setAllowsMultipleSelection:NO];
    
    [openPanel beginWithCompletionHandler:^(NSInteger result) {
        if (result == NSModalResponseOK) {
            NSURL *selectedFile = [[openPanel URLs] firstObject];
            NSAlert *alert = [[NSAlert alloc] init];
            [alert setMessageText:@"File Selected"];
            [alert setInformativeText:[NSString stringWithFormat:@"Selected file: %@", [selectedFile lastPathComponent]]];
            [alert addButtonWithTitle:@"OK"];
            [alert runModal];
        }
    }];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

@end

// Main function
int main(int argc, const char * argv[]) {
    @autoreleasepool {
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