#import <Cocoa/Cocoa.h>

#include "common.inl"

/* Application delegate interface */
@interface AppDelegate : NSObject <NSApplicationDelegate>
@property (strong, nonatomic) NSWindow *window;
@end

/* Application delegate implementation */
@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    /* Create the main window */
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

    /* Not a proper main loop but illustrates the point */
    [NSTimer scheduledTimerWithTimeInterval:(1.0 / 60.0)
                                     target:self
                                   selector:@selector(updateFrame)
                                   userInfo:nil
                                    repeats:YES];

    createMenuBar(NULL);
}

- (void)updateFrame
{
    handleEvents();
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

@end

int main()
{
    @autoreleasepool
    {
        /* Create the application */
        NSApplication *app = [NSApplication sharedApplication];
        [app setActivationPolicy:NSApplicationActivationPolicyRegular];
        
        /* Create and set the app delegate */
        AppDelegate *delegate = [[AppDelegate alloc] init];
        [app setDelegate:delegate];
        
        /* Activate the app (bring to front) */
        [app activateIgnoringOtherApps:YES];
        
        /* Run the application event loop */
        [app run];
    }
    
    return 0;
}
