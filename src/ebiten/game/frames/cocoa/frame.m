#import <Cocoa/Cocoa.h>

@interface EbitenWindowController : NSObject<NSWindowDelegate> {
}

- (void)alertDidEnd:(NSAlert*)alert
         returnCode:(NSInteger)returnCode
        contextInfo:(void*)contextInfo;
- (BOOL)windowShouldClose:(id)sender;

@end

@implementation EbitenWindowController

- (BOOL)windowShouldClose:(id)sender {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  if ([sender isDocumentEdited]) {
    NSAlert* alert = [NSAlert alertWithMessageText:@"Quit the game?"
                              defaultButton:@"Quit"
                              alternateButton:nil
                              otherButton:@"Cancel"
                              informativeTextWithFormat:@""];
    [alert beginSheetModalForWindow:sender
           modalDelegate:self
           didEndSelector:@selector(alertDidEnd:returnCode:contextInfo:)
           contextInfo:nil];
  }
  [pool release];
  return NO;
}

- (void)alertDidEnd:(NSAlert*)alert
         returnCode:(NSInteger)returnCode
        contextInfo:(void*)contextInfo {
  (void)alert;
  (void)contextInfo;
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  if (returnCode == NSAlertDefaultReturn) {
    [NSApp terminate:self];
  }
  [pool release];
}

@end

@interface EbitenController : NSObject<NSApplicationDelegate> {
@private
  size_t width_;
  size_t height_;
  NSWindow* window_;
}

- (id)initWithWidth:(size_t)width height:(size_t)height;
- (void)applicationDidFinishLaunching:(NSNotification*)aNotification;
- (NSWindow*)window;

@end

@implementation EbitenController

- (id)initWithWidth:(size_t)width height:(size_t)height {
  self = [super init];
  if (self != nil) {
    self->width_ = width;
    self->height_ = height;
    self->window_ = nil;
  }
  return self;
}

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification {
  (void)aNotification;
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  NSRect contentRect = NSMakeRect(0, 0, self->width_, self->height_);
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
  [window setDocumentEdited:YES];
  [window makeKeyAndOrderFront:nil];
  self->window_ = window;
  [pool release];
}

- (NSWindow*)window {
  return self->window_;
}

@end
