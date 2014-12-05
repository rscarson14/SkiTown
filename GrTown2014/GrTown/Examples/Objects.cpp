/// GraphicsTown2006 - CS559 Sample Code
// written by Michael Gleicher - originally written Fall, 2000
// updated Fall, 2005
// updated Fall, 2006 - new texture manager, improvements for behaviors

#include "../GrTown_PCH.H"
#include "Objects.H"
#include "../DrawUtils.H"
#include "Utilities/Texture.H"
#include "../MMatrix.H"
#include <FL/fl_ask.h>
#include "../FlyCamera.H"
#include <iostream> 
#include <vector> 

#include "Utilities/ShaderTools.H"
#include <random>

#define PI 3.14159

using std::vector;

// some very basic classes of objects...
// mainly for debugging
Cube::Cube(float x, float y, float z, float s, 
		   float r, float g, float b)
  : color(r,g,b), size(s)
{
  transMatrix(transform,x,y,z);
}
void Cube::draw(DrawingState*)
{
  glColor4fv(&color.r);
  cube(0,0,0,size);
}

Hill::Hill(float a, float l, float d) : angle(a), length(l), div(d)
{
	randCalc = false;
	std::cout << "New Hill!" << std::endl;

}

void Hill::draw(DrawingState* d){
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1., 10);
	glPushMatrix();
	glTranslated(200, 0, 0);
	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	glColor4fv(&d->ground.r);
	glVertex3d(-200, -1, 200);
	glVertex3d(100, -1, 200);
	glVertex3d(100, -1, -200);
	glVertex3d(-200, -1, -200);
	glEnd();
	glPopMatrix();

	float totalHeight = 0;
	float totalLength = 0;
	hillPoints.push_back({ 0, 0 });
	float maxA = (angle*div) / 2;
	for (float i = angle; i <= maxA; i += angle){
		float x = -length*(cos(i*PI / 180));
		float y = length*sin(i*PI / 180);
		glPushMatrix();
		glTranslated(x + totalLength, y + totalHeight, 0);
		glRotated(-i, 0, 0, 1);
		glBegin(GL_POLYGON);
		glNormal3f(0, 1, 0);
		glColor4fv(&d->ground.r);
		glVertex3d(-length - 0.5, -1, 200);
		glVertex3d(length + 0.5, -1, 200);
		glVertex3d(length + 0.5, -1, -200);
		glVertex3d(-length - 0.5, -1, -200);
		glEnd();
		glPopMatrix();
		totalHeight += y * 2;
		totalLength += x * 2;
		if (!randCalc){
			float random1 = -185 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (-100 + 185)));
			float random2 = 100 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (185 - 100)));
			//hillPoints.push_back({ totalLength, totalHeight, random1 });
			//hillPoints.push_back({ totalLength, totalHeight, random2 });
			add(new Tree(totalLength, totalHeight, random1), totalLength, totalHeight, random1);
			add(new Tree(totalLength, totalHeight, random2), totalLength, totalHeight, random2);
		}


	}
	for (float i = maxA; i >= 0; i -= angle){
		float x = -length*(cos(i*PI / 180));
		float y = length*sin(i*PI / 180);
		glPushMatrix();
		glTranslated(x + totalLength, y + totalHeight, 0);
		glRotated(-i, 0, 0, 1);
		glBegin(GL_POLYGON);
		glNormal3f(0, 1, 0);
		glColor4fv(&d->ground.r);
		glVertex3d(-length - 0.5, -1, 200);
		glVertex3d(length + 0.5, -1, 200);
		glVertex3d(length + 0.5, -1, -200);
		glVertex3d(-length - 0.5, -1, -200);
		glEnd();
		glPopMatrix();
		totalHeight += y * 2;
		totalLength += x * 2;
		if (!randCalc){
			float random1 = -185 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (-100 + 185)));
			float random2 = 100 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (185 - 100)));
			//hillPoints.push_back({ totalLength, totalHeight, random1 });
			//hillPoints.push_back({ totalLength, totalHeight, random2 });
			add(new Tree(totalLength, totalHeight, random1), totalLength, totalHeight, random1);
			add(new Tree(totalLength, totalHeight, random2), totalLength, totalHeight, random2);
		}
	}

	glPushMatrix();
	glTranslated(totalLength -50, totalHeight, 0);
	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	glColor4fv(&d->ground.r);
	glVertex3d(-50, -1, 200);
	glVertex3d(50, -1, 200);
	glVertex3d(50, -1, -200);
	glVertex3d(-50, -1, -200);
	glEnd();
	glPopMatrix();
	glDisable(GL_POLYGON_OFFSET_FILL);
	randCalc = true;
}

Tree::Tree(float px, float py, float pz)
: x(px), y(py), z(pz)
{
	//transMatrix(transform, px, py, pz);

}

void Tree::draw(DrawingState* d){
	float changez = 0;
	
	float xpos = ((FlyCamera*)d->camera)->posX - x;
	float zpos = ((FlyCamera*)d->camera)->posZ - z;
	float ypos = ((FlyCamera*)d->camera)->posY;

	if (zpos >= 0) changez -= 180;

	//rotate train on y axis to get correct orientation in xz plane
	glRotatef(((atan(xpos / zpos) * 180 / PI) + changez + 90), 0, 1, 0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	fetchTexture("pinetree.bmp");
	glBegin(GL_POLYGON);
	glNormal3f(1, 0, 0);
	glColor3f(1, 1, 1);
	glTexCoord2f(0, 1); glVertex3i(0, 30, -15);
	glTexCoord2f(1, 1); glVertex3i(0, 30, 15);
	glTexCoord2f(1, 0); glVertex3i(0, 0, 15);
	glTexCoord2f(0, 0); glVertex3i(0, 0, -15);
	glEnd();


	// back of the sign
	glBindTexture(GL_TEXTURE_2D, 0);

}

/***********************************************************************/
TestCubes::TestCubes()
{
}

void TestCubes::draw(DrawingState*)
{
  glColor3f(1,0,0);
  cube( 20,0,0, 3);
  cube(-20,0,0, 3);
  cube(0, 20,0, 3);
  cube(0,-20,0, 3);
  cube(0,0, 20, 3);
  cube(0,0,-20, 3);

  glColor3f(1,1,0);
  cube(20,10,20,3);
  glColor3f(0,1,1);
  cube(-20,10,20,3);
  glColor3f(1,0,1);
  cube(-20,10,-20,3);
  glColor3f(1,1,1);
  cube(20,10,-20,3);
}

// some very basic classes of objects...
// mainly for debugging
ShadedCube::ShadedCube(float x, float y, float z, float s, 
					   float r, float g, float b)
  : color(r,g,b), size(s)
{
  transMatrix(transform,x,y,z);
}

// load the shader in the first time the cube draws (since that way we
// know we have a GL context)
// we only try to load it once
static unsigned int shadedCubeShader = 0;
static bool triedCubeShader=false;

void ShadedCube::draw(DrawingState*)
{
	if (!shadedCubeShader && !triedCubeShader) {
		triedCubeShader = true;
		char* error;
		if (!(shadedCubeShader=loadShader("ShadedCubeTest.vert","ShadedCubeTest.frag",error))) {
			std::string s = "Can't Load Cube Shader:";
			s += error;
			fl_alert(s.c_str());
		}
	}

	glUseProgram(shadedCubeShader);
	glColor4fv(&color.r);
	cube(0,0,0,size);
	glUseProgram(0);
}

// $Header: /p/course/cs559-gleicher/private/CVS/GrTown/Examples/Objects.cpp,v 1.6 2009/11/10 22:40:03 gleicher Exp $
