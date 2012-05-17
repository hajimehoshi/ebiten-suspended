#ifndef EBITEN_APPLICATION_DETAIL_MACOSX_RUN_MM
#define EBITEN_APPLICATION_DETAIL_MACOSX_RUN_MM

#import <Cocoa/Cocoa.h>
#undef check // solve a confliction with Boost

#include <cassert>

__attribute__((visibility("hidden")))
@interface EbitenController : NSObject<NSApplicationDelegate>

- (id)initWithWindow:(NSWindow*)window;
- (void)applicationDidFinishLaunching:(NSNotification*)aNotification;

@end

#ifndef EBITEN_WITHOUT_OBJC_IMPL
@implementation EbitenController {
@private
  NSWindow* window_;
}

- (id)initWithWindow:(NSWindow*)window {
  self = [super init];
  if (self != nil) {
    self->window_ = window;
  }
  return self;
}

- (void)initMenu {
  NSString* processName = [[NSProcessInfo processInfo] processName];
  NSMenu* mainMenu = [[NSMenu alloc] init];
  NSMenu* appleMenu = [[NSMenu alloc] init];
  [appleMenu addItemWithTitle:[@"Quit " stringByAppendingString:processName]
                       action:@selector(performClose:)
                keyEquivalent:@"q"];
  {
    NSMenuItem* menuItem = [mainMenu insertItemWithTitle:@""
                                                  action:nil
                                           keyEquivalent:@""
                                                 atIndex:0];
    [mainMenu setSubmenu:appleMenu forItem:menuItem];
  }
  [NSApp setMainMenu: mainMenu];
}

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification {
  (void)aNotification;
  NSWindow* window = self->window_;
  assert(window);
  [window makeKeyAndOrderFront:nil];
  [self initMenu];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication {
  (void)theApplication;
  return YES;
}

@end
#endif

#endif
