#ifndef HYPERSPACE_H
#define HYPERSPACE_H

#include <OpenGL/gl.h>

class rsText;
class causticTextures;
class wavyNormalCubeMaps;
class splinePath;
class tunnel;
class goo;
class stretchedParticle;
class starBurst;

#define FLARESIZE 64

typedef struct HyperspaceSaverSettings
{
	// Parameters edited in the dialog box
	int dSpeed;
	int dStars;
	int dStarSize;
	int dResolution;
	int dDepth;
	int dFov;
	bool dShaders;
	bool kStatistics;
	
	// Additional globals:
	float frameTime;
	bool doingPreview;
	int xsize, ysize;
	float aspectRatio;
	rsText *textwriter;
	bool firstDraw;
	
	float unroll;
	float billboardMat[16];
	double modelMat[16];
	double projMat[16];
	GLint viewport[4];
	
	float camPos[3]/* = {0.0f, 0.0f, 0.0f}*/;
	float depth;
	int numAnimTexFrames/* = 20*/;
	causticTextures* theCausticTextures;
	wavyNormalCubeMaps* theWNCM; 
	int whichTexture/* = 0*/;
	splinePath* thePath;
	tunnel* theTunnel;
	goo* theGoo;
	float shiftx, shiftz;
	GLuint speckletex, spheretex, nebulatex;
	GLuint goo_vp, goo_fp, tunnel_vp, tunnel_fp;
	
	stretchedParticle** stars;
	stretchedParticle* sunStar;
	starBurst* theStarBurst;
	
	float goo_c[4];  // goo constants
	float goo_cp[4]/* = {0.0f, 1.0f, 2.0f, 3.0f}*/;  // goo constants phase
	float goo_cs[4];  // goo constants speed
	
	unsigned char flare1[FLARESIZE][FLARESIZE][4];
	unsigned char flare2[FLARESIZE][FLARESIZE][4];
	unsigned char flare3[FLARESIZE][FLARESIZE][4];
	unsigned char flare4[FLARESIZE][FLARESIZE][4];
	unsigned int flarelist[4];
	GLuint flaretex[4];
};

__private_extern__ void draw(HyperspaceSaverSettings *inSettings);
__private_extern__ void initSaver(int width, int height, HyperspaceSaverSettings *inSettings);
__private_extern__ void cleanUp(HyperspaceSaverSettings *inSettings);
__private_extern__ void setDefaults(HyperspaceSaverSettings *inSettings);

#endif
