#ifndef EBITEN_FRAMES_DETAIL_MACOSX_NATIVE_FRAME_MM
#define EBITEN_FRAMES_DETAIL_MACOSX_NATIVE_FRAME_MM

#import <Cocoa/Cocoa.h>
#undef check // solve a confliction with Boost

#include <cstddef>

@interface EbitenWindow : NSWindow<NSWindowDelegate>

- (id)initWithSize:(NSSize)size;
- (void)alertDidEnd:(NSAlert*)alert
         returnCode:(NSInteger)returnCode
        contextInfo:(void*)contextInfo;
- (BOOL)windowShouldClose:(id)sender;

@end

#ifndef EBITEN_WITHOUT_OBJC_IMPL
@implementation EbitenWindow

- (id)initWithSize:(NSSize)size {
  [NSApplication sharedApplication];
  NSUInteger const style = (NSTitledWindowMask | NSClosableWindowMask |
                            NSMiniaturizableWindowMask);
  NSRect const windowRect =
    [NSWindow frameRectForContentRect:NSMakeRect(0, 0, size.width, size.height)
                            styleMask:style];
  NSScreen* screen = [[NSScreen screens] objectAtIndex:0];
  NSSize const screenSize = [screen visibleFrame].size;
  // Reference: Mac OS X Human Interface Guidelines: UI Element Guidelines: Windows
  // http://developer.apple.com/library/mac/#documentation/UserExperience/Conceptual/AppleHIGuidelines/Windows/Windows.html#//apple_ref/doc/uid/20000961-TP9
  NSRect const contentRect = NSMakeRect((screenSize.width - windowRect.size.width) / 2,
                                        (screenSize.height - windowRect.size.height) * 2 / 3,
                                        size.width, size.height);
  self = [super initWithContentRect:contentRect
                          styleMask:style
                            backing:NSBackingStoreBuffered defer:YES];
  assert(self != nil);
  [self setReleasedWhenClosed:YES];
  [self setDelegate:self];
  [self setDocumentEdited:YES];
  return self;
}

- (BOOL)windowShouldClose:(id)sender {
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
  return NO;
}

- (void)alertDidEnd:(NSAlert*)alert
         returnCode:(NSInteger)returnCode
        contextInfo:(void*)contextInfo {
  (void)alert;
  (void)contextInfo;
  if (returnCode == NSAlertDefaultReturn) {
    [NSApp terminate:self];
  }
}

@end
#endif

static NSWindow*
ebiten_frame_detail_generate_native_frame(std::size_t width, std::size_t height) {
  EbitenWindow* window = [[EbitenWindow alloc]
                           initWithSize:NSMakeSize(width, height)];
  assert(window != nil);

  NSRect const rect = NSMakeRect(0, 0, width, height);
  NSOpenGLPixelFormatAttribute const attributes[] = {
    NSOpenGLPFAWindow,
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFAAccelerated,
    NSOpenGLPFADepthSize, 32,
    nil,
  };
  NSOpenGLPixelFormat* format = [[NSOpenGLPixelFormat alloc]
                                  initWithAttributes:attributes];
  EbitenOpenGLView* glView = [[EbitenOpenGLView alloc] initWithFrame:rect
                                                         pixelFormat:format];
  [window setContentView:glView];
  //[window makeFirstResponder:glView];

  return window;
}

#endif
