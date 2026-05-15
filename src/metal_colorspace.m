#import <AppKit/AppKit.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#import <CoreGraphics/CoreGraphics.h>
#import <Foundation/Foundation.h>

void bgfx_fix_metal_colorspace(void *ns_window) {
    NSWindow *window = (__bridge NSWindow *)ns_window;
    CAMetalLayer *metalLayer = (CAMetalLayer *)window.contentView.layer;
    if (!metalLayer)
        return;

    NSScreen *screen = window.screen ?: [NSScreen mainScreen];
    CGColorSpaceRef displayCS = screen.colorSpace.CGColorSpace;
    metalLayer.colorspace = displayCS;
}
