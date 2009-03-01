//
//  HyperspaceView.h
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

#import <ScreenSaver/ScreenSaver.h>
#import "Hyperspace.h"

@interface HyperspaceView : ScreenSaverView 
{
	NSOpenGLView *lView;
	HyperspaceSaverSettings lSettings;
	BOOL lMainScreen;
	BOOL lMainScreenOnly;
	BOOL lIsConfiguring;
	
	float lTimes[10];
    int lTimeindex;
    unsigned long long lLastTime;
	
	IBOutlet NSWindow *ibConfigureSheet;
	IBOutlet NSTextField *ibVersionTxt;
	IBOutlet NSSlider *ibResolution;
	IBOutlet NSTextField *ibResolutionTxt;
	IBOutlet NSSlider *ibStars;
	IBOutlet NSTextField *ibStarsTxt;
	IBOutlet NSSlider *ibSpeed;
	IBOutlet NSTextField *ibSpeedTxt;
	IBOutlet NSSlider *ibStarSize;
	IBOutlet NSTextField *ibStarSizeTxt;
	IBOutlet NSSlider *ibDepth;
	IBOutlet NSTextField *ibDepthTxt;
	IBOutlet NSSlider *ibFov;
	IBOutlet NSTextField *ibFovTxt;
	IBOutlet NSButton *ibShaderCbx;
	IBOutlet NSButton *ibMainScreenOnlyCbx;
}
- (IBAction)closeSheet:(id)sender;
- (IBAction)reset:(id)sender;
- (IBAction)setValueForSender:(id)sender;

@end
