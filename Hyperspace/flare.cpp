/*
 * Copyright (C) 2005-2010  Terence M. Welsh
 *
 * This file is part of Hyperspace.
 *
 * Hyperspace is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
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


#include <Hyperspace/flare.h>



#define FLARESIZE 64


/*unsigned char flare1[FLARESIZE][FLARESIZE][4];
unsigned char flare2[FLARESIZE][FLARESIZE][4];
unsigned char flare3[FLARESIZE][FLARESIZE][4];
unsigned char flare4[FLARESIZE][FLARESIZE][4];
unsigned int flarelist[4];
GLuint flaretex[4];*/

/*extern int xsize, ysize;
extern float aspectRatio;
extern float billboardMat[16];
extern double modelMat[16];
extern double projMat[16];
extern GLint viewport[4];
// Calculated in main draw routine each frame
extern float frameTime;
extern float camPos[3];*/



void initFlares(HyperspaceSaverSettings *inSettings){
	int i, j;
	float x, y;
	float temp;
	
	glGenTextures(4, inSettings->flaretex);
	
	// First flare:  basic sphere
	for(i=0; i<FLARESIZE; i++){
		for(j=0; j<FLARESIZE; j++){
			inSettings->flare1[i][j][0] = 255;
			inSettings->flare1[i][j][1] = 255;
			inSettings->flare1[i][j][2] = 255;
			x = float(i - FLARESIZE / 2) / float(FLARESIZE / 2);
			y = float(j - FLARESIZE / 2) / float(FLARESIZE / 2);
			temp = 1.0f - ((x * x) + (y * y));
			if(temp > 1.0f)
				temp = 1.0f;
			if(temp < 0.0f)
				temp = 0.0f;
			temp = temp * temp;
			inSettings->flare1[i][j][3] = (unsigned char)(255.0f * temp);
		}
	}
	glBindTexture(GL_TEXTURE_2D, inSettings->flaretex[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, FLARESIZE, FLARESIZE, 0, GL_RGBA,
				 GL_UNSIGNED_BYTE, inSettings->flare1);
	
	// Second flare:  flattened sphere
	for(i=0; i<FLARESIZE; i++){
		for(j=0; j<FLARESIZE; j++){
			inSettings->flare2[i][j][0] = 255;
			inSettings->flare2[i][j][1] = 255;
			inSettings->flare2[i][j][2] = 255;
			x = float(i - FLARESIZE / 2) / float(FLARESIZE / 2);
			y = float(j - FLARESIZE / 2) / float(FLARESIZE / 2);
			temp = 2.5f * (1.0f - ((x * x) + (y * y)));
			if(temp > 1.0f)
				temp = 1.0f;
			if(temp < 0.0f)
				temp = 0.0f;
			//temp = temp * temp * temp * temp;
			inSettings->flare2[i][j][3] = (unsigned char)(255.0f * temp);
		}
	}
	glBindTexture(GL_TEXTURE_2D, inSettings->flaretex[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, FLARESIZE, FLARESIZE, 0, GL_RGBA,
				 GL_UNSIGNED_BYTE, inSettings->flare2);
	
	// Third flare:  torus
	for(i=0; i<FLARESIZE; i++){
		for(j=0; j<FLARESIZE; j++){
			inSettings->flare3[i][j][0] = 255;
			inSettings->flare3[i][j][1] = 255;
			inSettings->flare3[i][j][2] = 255;
			x = float(i - FLARESIZE / 2) / float(FLARESIZE / 2);
			y = float(j - FLARESIZE / 2) / float(FLARESIZE / 2);
			temp = 4.0f * ((x * x) + (y * y)) * (1.0f - ((x * x) + (y * y)));
			if(temp > 1.0f)
				temp = 1.0f;
			if(temp < 0.0f)
				temp = 0.0f;
			temp = temp * temp * temp * temp;
			inSettings->flare3[i][j][3] = (unsigned char)(255.0f * temp);
		}
	}
	glBindTexture(GL_TEXTURE_2D, inSettings->flaretex[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, FLARESIZE, FLARESIZE, 0, GL_RGBA,
				 GL_UNSIGNED_BYTE, inSettings->flare3);
	
	// Fourth flare:  weird flare
	for(i=0; i<FLARESIZE; i++){
		for(j=0; j<FLARESIZE; j++){
			x = float(i - FLARESIZE / 2) / float(FLARESIZE / 2);
			if(x < 0.0f)
				x = -x;
			y = float(j - FLARESIZE / 2) / float(FLARESIZE / 2);
			if(y < 0.0f)
				y = -y;
			inSettings->flare4[i][j][0] = 255;
			inSettings->flare4[i][j][1] = 255;
			temp = 0.14f * (1.0f - max(x, y)) / max((x * y), 0.05f);
			if(temp > 1.0f)
				temp = 1.0f;
			if(temp < 0.0f)
				temp = 0.0f;
			inSettings->flare4[i][j][2] = (unsigned char)(255.0f * temp);
			temp = 0.1f * (1.0f - max(x, y)) / max((x * y), 0.1f);
			if(temp > 1.0f)
				temp = 1.0f;
			if(temp < 0.0f)
				temp = 0.0f;
			inSettings->flare4[i][j][3] = (unsigned char)(255.0f * temp);
		}
	}
	glBindTexture(GL_TEXTURE_2D, inSettings->flaretex[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, FLARESIZE, FLARESIZE, 0, GL_RGBA,
				 GL_UNSIGNED_BYTE, inSettings->flare4);
	
	// Build display lists
	inSettings->flarelist[0] = glGenLists(4);
	inSettings->flarelist[1] = inSettings->flarelist[0] + 1;
	inSettings->flarelist[2] = inSettings->flarelist[0] + 2;
	inSettings->flarelist[3] = inSettings->flarelist[0] + 3;
	for(i=0; i<4; i++){
		glNewList(inSettings->flarelist[i], GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, inSettings->flaretex[i]);
		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-0.5f, -0.5f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.5f, -0.5f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-0.5f, 0.5f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.5f, 0.5f, 0.0f);
		glEnd();
		glEndList();
	}
}


// Draw a flare at a specified (x,y) location on the screen
// Screen corners are at (0,0) and (1,1)
// alpha = 0.0 for lowest intensity; alpha = 1.0 for highest intensity
void flare(float *pos, float red, float green, float blue, float alpha, HyperspaceSaverSettings *inSettings){
	double winx, winy, winz;  // in screen coordinates
	float x, y, dx, dy;
	float fadewidth, temp;
	static float flicker = 0.95f;

	gluProject(pos[0], pos[1], pos[2],
		inSettings->modelMat, inSettings->projMat, inSettings->viewport,
		&winx, &winy, &winz);
	x = (float(winx) / float(inSettings->xsize)) * inSettings->aspectRatio;
	y = float(winy) / float(inSettings->ysize);
	float diff[3] = {pos[0] - inSettings->camPos[0], pos[1] - inSettings->camPos[1], pos[2] - inSettings->camPos[2]};
	if(diff[0] * inSettings->billboardMat[8] + diff[1] * inSettings->billboardMat[9] + diff[2] * inSettings->billboardMat[10] > 0.0f)
		return;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	// Fade alpha if source is off edge of screen
	fadewidth = float(inSettings->xsize) / 10.0f;
	if(y < 0){
		temp = fadewidth + y;
		if(temp < 0.0f)
			return;
		alpha *= temp / fadewidth;
	}
	if(y > inSettings->ysize){
		temp = fadewidth - y + inSettings->ysize;
		if(temp < 0.0f)
			return;
		alpha *= temp / fadewidth;
	}
	if(x < 0){
		temp = fadewidth + x;
		if(temp < 0.0f)
			return;
		alpha *= temp / fadewidth;
	}
	if(x > inSettings->xsize){
		temp = fadewidth - x + inSettings->xsize;
		if(temp < 0.0f)
			return;
		alpha *= temp / fadewidth;
	}

	// Find lens flare vector
	// This vector runs from the light source through the screen's center
	dx = 0.5f * inSettings->aspectRatio - x;
	dy = 0.5f - y;
	
	// Setup projection matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, inSettings->aspectRatio, 0, 1.0f);

	// Update fractal flickering
	flicker += inSettings->frameTime * (rsRandf(2.0f) - 1.0f);
	if(flicker < 0.9f)
		flicker = 0.9f;
	if(flicker > 1.1f)
		flicker = 1.1f;
	alpha *= flicker;

	// Draw stuff
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	glScalef(0.1f * flicker, 0.1f * flicker, 1.0f);
	glColor4f(red, green, blue * 0.8f, alpha);
	glCallList(inSettings->flarelist[0]);

	// wide flare
	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	glScalef(5.0f * alpha, 0.05f * alpha, 1.0f);
	glColor4f(red * 0.3f, green * 0.3f, blue, alpha);
	glCallList(inSettings->flarelist[0]);

	// torus
	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	glScalef(0.5f, 0.2f, 1.0f);
	glColor4f(red, green * 0.5f, blue * 0.5f, alpha * 0.4f);
	glCallList(inSettings->flarelist[2]);

	// 3 blueish dots
	glLoadIdentity();
	glTranslatef(x + dx * 0.35f, y + dy * 0.35f, 0.0f);
	glScalef(0.06f, 0.06f, 1.0f);
	glColor4f(red * 0.85f, green * 0.85f, blue, alpha * 0.5f);
	glCallList(inSettings->flarelist[1]);

	glLoadIdentity();
	glTranslatef(x + dx * 0.45f, y + dy * 0.45f, 0.0f);
	glScalef(0.09f, 0.09f, 1.0f);
	glColor4f(red * 0.7f, green * 0.7f, blue, alpha * 0.4f);
	glCallList(inSettings->flarelist[1]);

	glLoadIdentity();
	glTranslatef(x + dx * 0.55f, y + dy * 0.55f, 0.0f);
	glScalef(0.12f, 0.12f, 1.0f);
	glColor4f(red * 0.55f, green * 0.55f, blue, alpha * 0.3f);
	glCallList(inSettings->flarelist[1]);

	// 4 more dots
	glLoadIdentity();
	glTranslatef(x + dx * 0.75f, y + dy * 0.75f, 0.0f);
	glScalef(0.14f, 0.07f, 1.0f);
	glColor4f(red * 0.3f, green * 0.3f, blue * 0.3f, alpha);
	glCallList(inSettings->flarelist[3]);

	glLoadIdentity();
	glTranslatef(x + dx * 0.78f, y + dy * 0.78f, 0.0f);
	glScalef(0.06f, 0.06f, 1.0f);
	glColor4f(red * 0.3f, green * 0.4f, blue * 0.4f, alpha * 0.5f);
	glCallList(inSettings->flarelist[1]);

	glLoadIdentity();
	glTranslatef(x + dx * 1.25f, y + dy * 1.25f, 0.0f);
	glScalef(0.1f, 0.1f, 1.0f);
	glColor4f(red * 0.3f, green * 0.4f, blue * 0.3f, alpha * 0.5f);
	glCallList(inSettings->flarelist[1]);

	glLoadIdentity();
	glTranslatef(x + dx * 1.3f, y + dy * 1.3f, 0.0f);
	glScalef(0.07f, 0.07f, 1.0f);
	glColor4f(red * 0.6f, green * 0.45f, blue * 0.3f, alpha * 0.5f);
	glCallList(inSettings->flarelist[1]);

	// stretched weird flare
	glLoadIdentity();
	glTranslatef(x + dx * 1.45f, y + dy * 1.45f, 0.0f);
	glScalef(0.8f, 0.2f, 1.0f);
	glRotatef(x * 70.0f, 0, 0, 1);
	glColor4f(red, green, blue, alpha * 0.4f);
	glCallList(inSettings->flarelist[3]);

	// circle
	glLoadIdentity();
	glTranslatef(x + dx * 2.0f, y + dy * 2.0f, 0.0f);
	glScalef(0.3f, 0.3f, 1.0f);
	glColor4f(red, green, blue, alpha * 0.2f);
	glCallList(inSettings->flarelist[1]);

	// big weird flare
	glLoadIdentity();
	glTranslatef(x + dx * 2.4f, y + dy * 2.4f, 0.0f);
	glRotatef(y * 40.0f, 0, 0, 1);
	glScalef(0.7f, 0.7f, 1.0f);
	glColor4f(red, green, blue, alpha * 0.3f);
	glCallList(inSettings->flarelist[3]);

	// Unsetup projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
