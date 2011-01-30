#import <Cocoa/Cocoa.h>

@interface EbitenWindowController : NSObject<NSWindowDelegate> {
}

- (BOOL)windowShouldClose:(id)sender;

@end

@implementation EbitenWindowController

- (BOOL)windowShouldClose:(id)sender {
  (void)sender;
  [NSApp terminate:self];
  return YES;
}

@end

@interface EbitenController : NSObject<NSApplicationDelegate> {
@private
  NSWindow* window_;
}

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification;

@end

@implementation EbitenController

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  (void)aNotification;
  NSRect contentRect = NSMakeRect(0, 0, 320 * 2, 240 * 2);
  const NSUInteger style = (NSTitledWindowMask | NSClosableWindowMask |
                            NSMiniaturizableWindowMask);
  NSRect windowRect = [NSWindow frameRectForContentRect:contentRect
                                styleMask:style];
  NSScreen* screen = [[NSScreen screens] objectAtIndex:0];
  NSSize screenSize = [screen visibleFrame].size;
  contentRect.origin = NSMakePoint((screenSize.width - windowRect.size.width) / 2,
                                   (screenSize.height - windowRect.size.height) / 2);
  NSWindow* window = [[NSWindow alloc] initWithContentRect:contentRect
                      styleMask:style backing:NSBackingStoreBuffered defer:YES];
  [window setReleasedWhenClosed:YES];
  EbitenWindowController* controller = [[EbitenWindowController alloc] init];
  [window setDelegate:controller];
  [window makeKeyAndOrderFront:NSApp];
  self->window_ = window;
  [pool release];
}

@end
