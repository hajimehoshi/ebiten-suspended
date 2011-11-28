#ifndef EBITEN_KERNEL_DETAIL_MACOSX_APPLICATION_MM
#define EBITEN_KERNEL_DETAIL_MACOSX_APPLICATION_MM

#import <Cocoa/Cocoa.h>
#undef check // solve a confliction with Boost

#include "ebiten/frames/frame.hpp"
#include <cassert>

@interface EbitenController : NSObject<NSApplicationDelegate> {
@private
  NSWindow* window_;
}

- (id)initWithWindow:(NSWindow*)window;
- (void)applicationDidFinishLaunching:(NSNotification*)aNotification;

@end

@implementation EbitenController

- (id)initWithWindow:(NSWindow*)window {
  self = [super init];
  if (self != nil) {
    self->window_ = window;
  }
  return self;
}

- (void)initMenu {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
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
  [mainMenu release];
  [appleMenu release];
  [pool release];
}

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification {
  (void)aNotification;
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  NSWindow* window = self->window_;
  assert(window);
  [window makeKeyAndOrderFront:nil];
  [self initMenu];
  [pool release];
}

@end

void
ebiten_kernel_detail_run_application(ebiten::frames::frame& frame) {
  NSWindow* window = frame.native_frame();
  assert(window != nil);
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  EbitenController* controller = [[EbitenController alloc] initWithWindow:window];
  NSApplication* app = [NSApplication sharedApplication];
  [app setDelegate:controller];
  [app finishLaunching];
  [app run];
  [pool release];
}

#endif
