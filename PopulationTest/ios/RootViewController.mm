//
//  PopulationTestAppController.h
//  PopulationTest
//
//  Created by Serious Games on 5/3/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#import "cocos2d.h"
#import "RootViewController.h"
#import "GameHUD.h"
#import "AppDelegate.h"
#import "Senario.h"
#import "SplashScene.h"
#import "SenarioChooseScene.h"

using namespace cocos2d;

@implementation RootViewController

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
*/

// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
#if GAME_AUTOROTATION==kGameAutorotationNone
	return ( interfaceOrientation == UIInterfaceOrientationPortrait );
	
#elif GAME_AUTOROTATION==kGameAutorotationCCDirector
    CCDirector *director = [CCDirector sharedDirector];
	if( interfaceOrientation == UIInterfaceOrientationLandscapeLeft ) {
		[director setDeviceOrientation: kCCDeviceOrientationLandscapeRight];
	} else if( interfaceOrientation == UIInterfaceOrientationLandscapeRight) {
		[director setDeviceOrientation: kCCDeviceOrientationLandscapeLeft];
	}
	// Since this method should return YES in at least 1 orientation,
	// we return YES only in the Portrait orientation
	return ( interfaceOrientation == UIInterfaceOrientationPortrait );
	
#elif GAME_AUTOROTATION == kGameAutorotationUIViewController
	return ( UIInterfaceOrientationIsLandscape( interfaceOrientation ) );
	
#else
#error Unknown value in GAME_AUTOROTATION
	
#endif // GAME_AUTOROTATION
	
	// Should not happen
	return NO;
}

-(BOOL)shouldAutorotate
{
    return YES;
}

// Before rotation
#if GAME_AUTOROTATION == kGameAutorotationUIViewController
-(void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
	//overridden! I don't intend for the game to be at full Retina, so I'm gonna let Retina resize it for me. ARRR.
    //
	// Assuming that the main window has the size of the screen
	// BUG: This won't work if the EAGLView is not fullscreen
	///
 //   CGRect winRect = [[UIScreen mainScreen] bounds];
//	winRect.size.width = AppDelegate::getThis()->width;
 //   winRect.size.height = AppDelegate::getThis()->height;
    
    CGSize winRect;
	CGSize screenRect;
	if(toInterfaceOrientation == UIInterfaceOrientationPortrait || toInterfaceOrientation == UIInterfaceOrientationPortraitUpsideDown)
    {
        winRect = CGSizeMake(AppDelegate::getThis()->width, AppDelegate::getThis()->height);
        screenRect =CGSizeMake(AppDelegate::getThis()->appWidth,AppDelegate::getThis()->appHeight);
        
    }
	else if(toInterfaceOrientation == UIInterfaceOrientationLandscapeLeft || toInterfaceOrientation == UIInterfaceOrientationLandscapeRight)
    {
        winRect = CGSizeMake(AppDelegate::getThis()->height, AppDelegate::getThis()->width);
        screenRect =CGSizeMake(AppDelegate::getThis()->appHeight,AppDelegate::getThis()->appWidth);

        
    }
    CCDirector *director = CCDirector::sharedDirector();
    CCEGLView *glView = director->getOpenGLView();
  
   // float contentScaleFactor = director->getContentScaleFactor();
    
	//if( contentScaleFactor != 1 ) {
//		rect.size.width *= contentScaleFactor;
//		rect.size.height *= contentScaleFactor;
//	}
    
    [self prepareGUIOrientationChange];

    
    
    glView->setFrameSize(winRect.width, winRect.height);
    
    glView->setDesignResolutionSize(screenRect.width, screenRect.height, kResolutionExactFit);
    
    [self updateGUIOnOrientationChange];
 
}
#endif // GAME_AUTOROTATION == kGameAutorotationUIViewController


//After rotation
-(void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation{
    

}

-(void)prepareGUIOrientationChange{
    GameHUD* gameHUDScript = GameHUD::getThis();
    
    if (gameHUDScript)
    {
    }
}

-(void)updateGUIOnOrientationChange{
    //Not used yet
  //  UIInterfaceOrientation currentOrientation = self.interfaceOrientation;
    
    SplashScene* splashScript = SplashScene::getThis();
    GameScene* gameScene = GameScene::getThis();
    GameHUD* gameHUDScript = GameHUD::getThis();
    Senario* senarioScript = Senario::getThis();
    SenarioChooseScene* senarioChooseScript = SenarioChooseScene::getThis();
    
    if(splashScript)
    {
        splashScript->onOrientationChanged();
    }
    
    if (gameScene)
    {
        gameScene->onOrientationChanged();
    }
    
    if (senarioChooseScript)
    {
        senarioChooseScript->onOrientateChange();
    }
    
    if (gameHUDScript)
    {
        gameHUDScript->onOrientationChanged();
        for (int i = 1; i < gameHUDScript->getChildrenCount(); ++i)
        {
            CCObject* objPtr = gameHUDScript->getChildren()->objectAtIndex(i);
            if (InfoBar* derivedPtr = dynamic_cast<InfoBar*>(objPtr))
                derivedPtr->onOrientationChanged();
            else if (PopupMenu* derivedPtr = dynamic_cast<PopupMenu*>(objPtr))
                derivedPtr->onOrientationChanged();
        }
    }
    
    if (senarioScript)
    {
        senarioScript->onOrientationChanged();
    }
}


// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
    // return UIInterfaceOrientationMaskAll;
    return UIInterfaceOrientationMaskAllButUpsideDown;
}


- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (void)dealloc {
    [super dealloc];
}

@end
