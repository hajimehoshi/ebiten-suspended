//
//  EbitenViewController.h
//  Ebiten-iOS
//
//  Created by Hajime Hoshi on 1/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#include "ebiten_script/squirrel/game.hpp"
#include "ebiten/ebiten.hpp"

@interface EbitenViewController : GLKViewController {
    ebiten::kernel* _kernel;
    ebiten_script::squirrel::game* _game;
}

@end
