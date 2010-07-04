//
//  HyperspaceView.m
//  Hyperspace
//
//  Created by Nick Zitzmann on 2/19/06.
//  Copyright (c) 2006, Nick Zitzmann. All rights reserved.
//

/*
 * This file is part of Hyperspace.
 *
 * Hyperspace is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation.
 *
 * Hyperspace is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#import "HyperspaceView.h"
#import "Hyperspace.h"
#import "flare.h"
#import <sys/time.h>
#import <OpenGL/OpenGL.h>
#import "Hyperspace/extensions.h"

FOUNDATION_STATIC_INLINE bool RSSShadersSupported(void)
{
	SInt32 osVersion;
	
	Gestalt(gestaltSystemVersion, &osVersion);
	return (osVersion >= 0x1043 && queryExtension("GL_ARB_multitexture") && queryExtension("GL_ARB_texture_cube_map") && queryExtension("GL_ARB_shader_objects"));
}

@interface HyperspaceView (Private)
- (void)readDefaults:(ScreenSaverDefaults *)inDefaults;
- (void)setDialogValue;
@end

@implementation HyperspaceView

- (id)initWithFrame:(NSRect)frame isPreview:(BOOL)isPreview
{
	NSString *identifier = [[NSBundle bundleForClass:[self class]] bundleIdentifier];
    ScreenSaverDefaults *defaults = [ScreenSaverDefaults defaultsForModuleWithName:identifier];
	
    self = [super initWithFrame:frame isPreview:isPreview];
    if (self)
	{
		lSettings.doingPreview = isPreview;
		if (isPreview)
			lMainScreen = YES;
		else
			lMainScreen = (frame.origin.x == 0 && frame.origin.y == 0) ? YES : NO;
		lIsConfiguring = NO;
		lMainScreenOnly = [defaults boolForKey:@"MainScreenOnly"];
		if (lMainScreen || !lMainScreenOnly)
		{
			NSOpenGLPixelFormatAttribute attribs[] = 
			{
				NSOpenGLPFAAccelerated, (NSOpenGLPixelFormatAttribute)YES,
				NSOpenGLPFAColorSize, (NSOpenGLPixelFormatAttribute)32,
				NSOpenGLPFADoubleBuffer, (NSOpenGLPixelFormatAttribute)YES,
				NSOpenGLPFAMinimumPolicy, (NSOpenGLPixelFormatAttribute)YES,
				(NSOpenGLPixelFormatAttribute)0
			};
            NSOpenGLPixelFormat *format = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attribs] autorelease];
            
            if (format)
            {
                lView = [[[NSOpenGLView alloc] initWithFrame:NSZeroRect pixelFormat:format] autorelease];
                [self addSubview:lView];
				
                lSettings.frameTime = 0;
                
				[[lView openGLContext] makeCurrentContext];	// must do this before RSSShadersSupported() will work
				setDefaults(&lSettings);
                if ([defaults objectForKey:@"Speed"])
                {
                    [self readDefaults:defaults];
                }
                
                [self setAnimationTimeInterval:1/30.0];
				lSettings.first = 1;
            }
		}
    }
    return self;
}


- (void)setFrameSize:(NSSize)size
{
	[super setFrameSize:size];
	if (lView)
		[lView setFrameSize:size];
	reshape(int(size.width), int(size.height), &lSettings);
}


- (void)startAnimation
{
    [super startAnimation];
	
	if (lIsConfiguring == NO && lView)
    {
        if (lMainScreen || !lMainScreenOnly)
        {
			GLint interval = 1;
            
            [self lockFocus];
            [[lView openGLContext] makeCurrentContext];
            
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glFlush();
            CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &interval);	// don't allow screen tearing
            [[lView openGLContext] flushBuffer];
            
            initSaver(&lSettings);
			gettimeofday(&timer.prev_tv, NULL);	// reset the timer
			[self unlockFocus];
        }
    }
}


- (void)stopAnimation
{
    [super stopAnimation];
	
	if (lIsConfiguring == NO && lView)
    {
        if (lMainScreen || !lMainScreenOnly)
		{
			[[lView openGLContext] makeCurrentContext];
			cleanUp(&lSettings);
            lSettings.frameTime=0;
		}
	}
}


- (void)drawRect:(NSRect)rect
{
    [[NSColor blackColor] set];
    NSRectFill(rect);
    
    if (lView == nil)
    {    
		if (lMainScreen || !lMainScreenOnly)
        {
            NSRect tFrame=[self frame];
            NSRect tStringFrame;
            NSDictionary * tAttributes;
            NSString * tString;
            NSMutableParagraphStyle * tParagraphStyle;
            
            tParagraphStyle=[[NSParagraphStyle defaultParagraphStyle] mutableCopy];
            [tParagraphStyle setAlignment:NSCenterTextAlignment];
            
            tAttributes = [NSDictionary dictionaryWithObjectsAndKeys:[NSFont systemFontOfSize:[NSFont systemFontSize]],NSFontAttributeName,[NSColor whiteColor],NSForegroundColorAttributeName,tParagraphStyle,NSParagraphStyleAttributeName,nil];
            
            [tParagraphStyle release];
            
            tString=NSLocalizedStringFromTableInBundle(@"Minimum OpenGL requirements\rfor this Screen Effect\rnot available\ron your graphic card.",@"Localizable",[NSBundle bundleForClass:[self class]],@"No comment");
            
            tStringFrame.origin=NSZeroPoint;
            tStringFrame.size=[tString sizeWithAttributes:tAttributes];
            
            tStringFrame=SSCenteredRectInRect(tStringFrame,tFrame);
            
            [tString drawInRect:tStringFrame withAttributes:tAttributes];
        }
    }
}


- (void)animateOneFrame
{
	if (lIsConfiguring == NO && lView)
    {
        if (lMainScreen || !lMainScreenOnly)
        {
			lSettings.frameTime = timer.tick();
			if (lSettings.readyToDraw)
			{
				[[lView openGLContext] makeCurrentContext];
				draw(&lSettings);
				[[lView openGLContext] flushBuffer];
			}
        }
    }
}


- (BOOL)hasConfigureSheet
{
    return (lView != nil);
}


- (NSWindow*)configureSheet
{
    lIsConfiguring = YES;
	if (ibConfigureSheet == nil)
	{
		if ([NSBundle loadNibNamed:@"ConfigureSheet" owner:self])
		{
			[ibVersionTxt setStringValue:[[[NSBundle bundleForClass:[self class]] infoDictionary] objectForKey:@"CFBundleVersion"]];	// set the version text
			[self setDialogValue];
			if (RSSShadersSupported() == false)
				[ibShaderCbx setEnabled:NO];
		}
		else
			NSLog(@"Warning: %@ couldn't load ConfigureSheet.nib.", [self className]);
	}
	return ibConfigureSheet;
}


#pragma mark -


- (IBAction)closeSheet:(id)sender
{
	NSString *identifier = [[NSBundle bundleForClass:[self class]] bundleIdentifier];
    ScreenSaverDefaults *inDefaults = [ScreenSaverDefaults defaultsForModuleWithName:identifier];
	
	[inDefaults setInteger:lSettings.dSpeed forKey:@"Speed"];
	[inDefaults setInteger:lSettings.dStars forKey:@"Stars"];
	[inDefaults setInteger:lSettings.dStarSize forKey:@"StarSize"];
	[inDefaults setInteger:lSettings.dResolution forKey:@"Resolution"];
	[inDefaults setInteger:lSettings.dDepth forKey:@"Depth"];
	[inDefaults setInteger:lSettings.dFov forKey:@"Fov"];
	[inDefaults setBool:lSettings.dShaders forKey:@"UseShaders"];
	[inDefaults setBool:lMainScreenOnly forKey:@"MainScreenOnly"];
	
	lIsConfiguring = NO;
	if ([self isAnimating])
	{
		[self stopAnimation];
		[self startAnimation];
	}
	[NSApp endSheet:ibConfigureSheet];
	[ibConfigureSheet orderOut:self];
}


- (IBAction)reset:(id)sender
{
	setDefaults(&lSettings);
	[self setDialogValue];
}


- (IBAction)setValueForSender:(id)sender
{
	switch ([sender tag])
	{
		case 1:	// resolution of geometry
			lSettings.dResolution = [sender intValue];
			[ibResolutionTxt setIntValue:[sender intValue]];
			break;
		case 2:	// number of stars
			lSettings.dStars = [sender intValue];
			[ibStarsTxt setIntValue:[sender intValue]];
			break;
		case 3:	// speed
			lSettings.dSpeed = [sender intValue];
			[ibSpeedTxt setIntValue:[sender intValue]];
			break;
		case 4:	// size of stars
			lSettings.dStarSize = [sender intValue];
			[ibStarSizeTxt setIntValue:[sender intValue]];
			break;
		case 5:	// view depth
			lSettings.dDepth = [sender intValue];
			[ibDepthTxt setIntValue:[sender intValue]];
			break;
		case 6:	// field of view
			lSettings.dFov = [sender intValue];
			[ibFovTxt setIntValue:[sender intValue]];
			break;
		case 7:	// shaders
			lSettings.dShaders = ([sender state] == NSOnState) ? true : false;
			break;
		case 8:	// main screen
			lMainScreenOnly = ([sender state] == NSOnState) ? true : false;
			break;
	}
}

@end


@implementation HyperspaceView (Private)

- (void)readDefaults:(ScreenSaverDefaults *)inDefaults
{
    lSettings.dSpeed = int([inDefaults integerForKey:@"Speed"]);
	lSettings.dStars = int([inDefaults integerForKey:@"Stars"]);
	lSettings.dStarSize = int([inDefaults integerForKey:@"StarSize"]);
	lSettings.dResolution = int([inDefaults integerForKey:@"Resolution"]);
	lSettings.dDepth = int([inDefaults integerForKey:@"Depth"]);
	lSettings.dFov = int([inDefaults integerForKey:@"Fov"]);
	lSettings.dShaders = [inDefaults integerForKey:@"UseShaders"];
	lMainScreenOnly = [inDefaults boolForKey:@"MainScreenOnly"];
}


- (void)setDialogValue
{
	[ibResolution setIntValue:lSettings.dResolution];
	[ibResolutionTxt setIntValue:lSettings.dResolution];
	[ibStars setIntValue:lSettings.dStars];
	[ibStarsTxt setIntValue:lSettings.dStars];
	[ibSpeed setIntValue:lSettings.dSpeed];
	[ibSpeedTxt setIntValue:lSettings.dSpeed];
	[ibStarSize setIntValue:lSettings.dStarSize];
	[ibStarSizeTxt setIntValue:lSettings.dStarSize];
	[ibDepth setIntValue:lSettings.dDepth];
	[ibDepthTxt setIntValue:lSettings.dDepth];
	[ibFov setIntValue:lSettings.dFov];
	[ibFovTxt setIntValue:lSettings.dFov];
	[ibShaderCbx setState:(lSettings.dShaders) ? NSOnState : NSOffState];
	[ibMainScreenOnlyCbx setState:(lMainScreenOnly) ? NSOnState : NSOffState];
}

@end
