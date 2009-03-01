/*
 * Copyright (C) 2005  Terence M. Welsh
 *
 * This file is part of rsText.
 *
 * rsText is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * rsText is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


/*#include <rsText/rsText.h>
#include <rsText/fontmap.h>*/
#include "rsText.h"
#include "fontmap.h"



rsText::rsText(){
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_ALPHA, 512, 256, GL_ALPHA, GL_UNSIGNED_BYTE, fontmap);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 256, 0, GL_ALPHA, GL_UNSIGNED_BYTE, fontmap);

	float xorig, yorig;
	listbase = glGenLists(128);
	for(int i=0; i<128; ++i){
		xorig = float(i % 16) * 0.0625f;
		yorig = float(i / 16) * 0.125f;
		glNewList(listbase + i, GL_COMPILE);
			glBegin(GL_TRIANGLE_STRIP);
				glTexCoord2f(xorig, yorig + 0.125f);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glTexCoord2f(xorig + 0.0625f, yorig + 0.125f);
				glVertex3f(1.0f, 0.0f, 0.0f);
				glTexCoord2f(xorig, yorig);
				glVertex3f(0.0f, 1.0f, 0.0f);
				glTexCoord2f(xorig + 0.0625f, yorig);
				glVertex3f(1.0f, 1.0f, 0.0f);
			glEnd();
			glTranslatef(1.0f, 0.0f, 0.0f);
		/*glBitmap(512, 256,
			32.0f * float(i % 16), 32.0f * float(i / 16),
			32.0f, 0.0, (const GLubyte *)fontmap);*/
		glEndList();
	}
}


void rsText::draw(std::string &str){
	int character;

	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		for(unsigned int i=0; i<str.length(); ++i){
			// Character set only includes 128 characters starting
			// with ASCII number 32
			character = int((str.c_str())[i]) - 32;
			if(character >= 0 && character < 128)
				glCallList(character + listbase);
		}
	glPopAttrib();
}


void rsText::draw(std::vector<std::string> &strvec){
	int character;
	std::string* str;
	unsigned int j;
	unsigned int i;

	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		for(j=0; j<strvec.size(); j++){
			str = &(strvec[j]);
			for(i=0; i<str->length(); ++i){
				// Character set only includes 128 characters starting
				// with ASCII number 32
				character = int((str->c_str())[i]) - 32;
				if(character >= 0 && character < 128)
					glCallList(character + listbase);
			}
			glTranslatef(-float(i), -1.0f, 0.0f);
		}
	glPopAttrib();
}
