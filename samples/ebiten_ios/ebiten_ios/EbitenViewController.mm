//
//  EbitenViewController.m
//  ebiten_ios
//
//  Created by Hajime Hoshi on 1/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "EbitenViewController.h"

class game : private ebiten::noncopyable {
public:
    bool
    update(ebiten::graphics::texture_factory&,
           ebiten::input const&) {
        return false;
    }
    void
    draw(ebiten::graphics::graphics_context& g,
         ebiten::graphics::texture& offscreen) {
        g.set_offscreen(offscreen);
        g.fill(128, 128, 255, 255);
    }
};

@interface EbitenViewController () {
@private
    ebiten::kernel* _kernel;
    game* _game;
}

@end

@implementation EbitenViewController

- (id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if (self) {
        self->_game = nil;
        self->_kernel = nil;
    }
    return self;
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView
{
}
*/

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad
{
    [super viewDidLoad];
    NSBundle* bundle = [NSBundle mainBundle];
    NSString* ns_path = [bundle pathForResource:@"sprites.nut" ofType:nil];
    std::string path([ns_path UTF8String]);
    self->_game = new game();
    auto game_update = std::bind(&game::update,
                                 self->_game,
                                 std::placeholders::_1,
                                 std::placeholders::_2);
    auto game_draw = std::bind(&game::draw,
                               self->_game,
                               std::placeholders::_1,
                               std::placeholders::_2);
    std::size_t const width  = static_cast<std::size_t>(self.view.frame.size.width);
    std::size_t const height = static_cast<std::size_t>(self.view.frame.size.height);
    self->_kernel = new ebiten::kernel(game_update,
                                       game_draw,
                                       width / 2, height / 2, 2, 60,
                                       (GLKView*)self.view);
}

- (void)viewDidUnload
{
    if (self->_game) {
        delete self->_game;
        self->_game = nil;
    }
    if (self->_kernel) {
        delete self->_kernel;
        self->_kernel = nil;
    }
    [super viewDidUnload];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

@end
