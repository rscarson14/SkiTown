/// GraphicsTown2006 - CS559 Sample Code
// written by Michael Gleicher - originally written Fall, 2000
//
// updated Fall 2006 - more testing code
// updated Fall 2008 - switched some internal stuff (no more linked lists)

#include "GrTown_PCH.H"
#include "GraphicsTownUI.H"
#include "FlyCamera.H"
#include "GrWorld.H"
#include "DrawUtils.H"
#include "Examples/Objects.H"
#include "Examples/SimpleBehaviors.H"
#include "Examples/Cars.H"
#include "Skycube.h"

// for setting up shader paths and textures
#include "Utilities/ShaderTools.H"
#include "Utilities/Texture.H"


// Example code (objects and behaviors)
#include "Examples/Suburbs.H"

#include "Roads/Roads.H"
#include "Roads/Drive.H"


// define this to get 2 cars that always turn
// #define TESTCARS

const int numCars = 100;
const int nGrids = 5;

int main(int /*argc*/, char** /*argv*/)
{ 

  // put in some texture paths - look all over the place
	texturePaths.push_back(".");
	texturePaths.push_back("..");
	texturePaths.push_back("Textures/signs");
	texturePaths.push_back("../Textures/signs");
	texturePaths.push_back("Textures/Textures");
	texturePaths.push_back("../Textures/Textures");
	texturePaths.push_back("Textures/Objects");
	texturePaths.push_back("../Textures/Objects");

	shaderPaths.push_back("Shaders");
	shaderPaths.push_back("../Shaders");

	//add cabin
	GrObject* c1 = new Cabin;
	c1->interesting = true;
	c1->laX = 0; c1->laY = 0; c1->laZ = 0;
	c1->lfX = -50; c1->lfY = 100; c1->lfZ = 300;
	add(c1, -100, 0, 100, pi / 2.0f);

  /*// add some more stuff
  GrObject* o1 = new Church;
  o1->interesting = true;
  o1->laX = 0; o1->laY = 0; o1->laZ = 0;
  o1->lfX = -50; o1->lfY = 100; o1->lfZ = 300;
  add(o1,-100,0,100,pi/2.f);
  */

  // *****************************************************************
  //  Make your town here

  // first, some really simple things

  // cubes are particularly easy since they set their own position
  // we need to raise the cube since we're giving the position of the
  // center 
  GrObject* cube1 = new ShadedCube(-50,5,-50,10,   .7f, .6f, .3f);
  add(cube1);
  cube1->name = "Cube1";
  // make it an interesting object (so its easy to look at)
  // and give a place to look at it from
  // this is a good object to look at since it won't move
  cube1->interesting = true;
  cube1->laX = 0; cube1->laY = 0; cube1->laZ = 0;
  cube1->lfX = 70; cube1->lfY = 50; cube1->lfZ = 0;


  // *****************************************************************
  // now make a UI
  FlyCamera* fc = new FlyCamera;
  Map* m = new Map;

  add(fc);
  add(m);

  GraphicsTownUI grTown;
  grTown.make_window();
  grTown.townView->defaultCamera = fc;
  grTown.window->show();

  Fl::run();
  return 1;
}

// $Header: /p/course/cs559-gleicher/private/CVS/GrTown/Main.cpp,v 1.8 2009/11/10 22:40:03 gleicher Exp $
