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
    [NSApplication sharedApplication];
    self->window_ = nil;
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    NSRect contentRect = NSMakeRect(0, 0, width, height);
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
    [pool release];
    self->window_ = window;
  }
  return self;
}

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification {
  (void)aNotification;
  NSWindow* window = self->window_;
  [window makeKeyAndOrderFront:nil];
}

- (NSWindow*)window {
  return self->window_;
}

@end
