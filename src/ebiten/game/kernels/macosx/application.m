#import <Cocoa/Cocoa.h>
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

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification {
  (void)aNotification;
  NSWindow* window = self->window_;
  assert(window);
  [window makeKeyAndOrderFront:nil];
}

@end

