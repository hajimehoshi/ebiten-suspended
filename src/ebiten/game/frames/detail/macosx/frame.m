#import <Cocoa/Cocoa.h>
#undef check // conflicts with boost

@interface EbitenWindowController : NSObject<NSWindowDelegate>

- (void)alertDidEnd:(NSAlert*)alert
         returnCode:(NSInteger)returnCode
        contextInfo:(void*)contextInfo;
- (BOOL)windowShouldClose:(id)sender;

@end

@implementation EbitenWindowController

- (BOOL)windowShouldClose:(id)sender {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  if ([sender isDocumentEdited]) {
    // TODO: add the application's name
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

@interface EbitenWindow : NSWindow

- (id)initWithSize:(NSSize)size;

@end

@implementation EbitenWindow

- (id)initWithSize:(NSSize)size {
  [NSApplication sharedApplication];
  // initialize
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  NSRect contentRect = NSMakeRect(0, 0, size.width, size.height);
  NSUInteger const style = (NSTitledWindowMask | NSClosableWindowMask |
                            NSMiniaturizableWindowMask);
  NSRect windowRect = [NSWindow frameRectForContentRect:contentRect
                                styleMask:style];
  NSScreen* screen = [[NSScreen screens] objectAtIndex:0];
  NSSize screenSize = [screen visibleFrame].size;
  contentRect.origin = NSMakePoint((screenSize.width - windowRect.size.width) / 2,
                                   (screenSize.height - windowRect.size.height) / 2);
  self = [super initWithContentRect:contentRect
                styleMask:style backing:NSBackingStoreBuffered defer:YES];
  assert(self != nil);
  [self setReleasedWhenClosed:YES];
  EbitenWindowController* controller = [[EbitenWindowController alloc] init];
  [self setDelegate:controller];
  [self setDocumentEdited:YES];
  [pool release];
  return self;
}

@end
