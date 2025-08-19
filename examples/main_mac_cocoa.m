#import <Cocoa/Cocoa.h>

#include "common.inl"

/* Application delegate interface */
@interface AppDelegate : NSObject <NSApplicationDelegate>
@property (strong, nonatomic) NSWindow *window;
@end

/* Application delegate implementation */
@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    /* Create the main window */
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
    
    /* Add some basic content to the window */
    NSTextField *label = [[NSTextField alloc] initWithFrame:NSMakeRect(50, 300, 700, 50)];
    [label setStringValue:@"Hello from a native macOS window created with Objective-C++!"];
    [label setEditable:NO];
    [label setBezeled:NO];
    [label setDrawsBackground:NO];
    [label setFont:[NSFont systemFontOfSize:18]];
    [label setAlignment:NSTextAlignmentCenter];
    
    [self.window.contentView addSubview:label];

    createMenuBar(NULL);

    /* Not a proper main loop but illustrates the point */
    [NSTimer scheduledTimerWithTimeInterval:(1.0 / 60.0)
                                     target:self
                                   selector:@selector(updateFrame)
                                   userInfo:nil
                                    repeats:YES];
}

- (void)updateFrame
{
    handleEvents();
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

@end

/* Main function */
int main()
{
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
