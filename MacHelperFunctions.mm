//
//  MacHelperFunctions.mm
//  Hyperspace
//
//  Created by Nick Zitzmann on 3/19/06.
//  Copyright 2006 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MacHelperFunctions.h"
#import <OpenGL/glext.h>

bool RSSShadersSupported()
{
	long osVersion;
	NSArray *glExtensions = [[NSString stringWithUTF8String:(const char *)glGetString(GL_EXTENSIONS)] componentsSeparatedByString:@" "];
	
	Gestalt(gestaltSystemVersion, &osVersion);
	if (osVersion < 0x1043)
		return false;
	else if ([glExtensions containsObject:@"GL_ARB_fragment_program"] == NO || [glExtensions containsObject:@"GL_ARB_vertex_program"] == NO || [glExtensions containsObject:@"GL_ARB_multitexture"] == NO || [glExtensions containsObject:@"GL_ARB_texture_cube_map"] == NO)
		return false;
	return true;
}
