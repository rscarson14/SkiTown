#ifndef _TERRAIN_H
#define _TERRAIN_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl\gl.h>										// Header File For The OpenGL32 Library
#include <gl\glu.h>										// Header File For The GLu32 Library
#include <math.h>
#include "GrObject.H"
#include "DrawingState.H"

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


#define MAP_SIZE		512				// This is the size of our .raw height map
#define STEP_SIZE		8					// This is width and height of each QUAD
#define HEIGHT_RATIO	1.0f				// This is the ratio that the Y is scaled according to the X and Z

// This returns the height (0 to 255) from a heightmap given an X and Y
float Height(BYTE *pHeightMap, int X, int Y);

// This loads a .raw file of a certain size from the file
void LoadRawFile(LPSTR strName, int nSize, BYTE *pHeightMap);

// This turns heightmap data into primitives and draws them to the screen
void RenderHeightMap(BYTE *pHeightMap, DrawingState* drst);



/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

#endif


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// This file holds all our defines and functions for dealing with the terrain. 
// As more tutorials are put out, we will add to this file to better handle
// terrain rendering.  This is actually a huge subject, so there is much to code.
//
// 
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// ©2000-2005 GameTutorials
//
//
