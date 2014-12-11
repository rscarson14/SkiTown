/// GraphicsTown2006 - CS559 Sample Code
// written by Michael Gleicher - originally written Fall, 2000
// updated Fall, 2005
// updated Fall, 2006 - new texture manager, improvements for behaviors

#include <GL/glew.h>

#include "../GrTown_PCH.H"
#include "Objects.H"
#include "../DrawUtils.H"
#include "Utilities/Texture.H"
#include "../MMatrix.H"
#include <FL/fl_ask.h>
#include "../FlyCamera.H"
#include <iostream> 
#include <vector> 
#include <glm/glm.hpp>
#include "SimpleBehaviors.H"

#include "Utilities/ShaderTools.H"
#include <random>

#define PI 3.14159
#define STEP_SIZE 0.01 //

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

Cable::Cable(float h, float z, float x1, float x2, float y1, float y2, float wr, float t) : tension(t)
{
	cntlPnts.push_back(vec3(x1, y1 + h, z - wr));
	cntlPnts.push_back(vec3(x1 + wr, y1 + h, z));
	cntlPnts.push_back(vec3(x1, y1 + h, z + wr));
	cntlPnts.push_back(vec3(x2, y2 + h, z + wr));
	cntlPnts.push_back(vec3(x2-wr, y2 + h, z));
	cntlPnts.push_back(vec3(x2, y2 + h, z - wr));
	GrObject*chair = new Chair(0);
}

void Cable::draw(DrawingState*){
	glLineWidth(3.0f);
	glBegin(GL_LINE_STRIP);
	int size = cntlPnts.size();
	for (int i = 0; i < size; i++){
		for (double t = 0; t <= 1; t += STEP_SIZE){ //draw 100 points between control points
			vec3 location;
			 //number of control points

			//define p1-p4 as which four control points will
			//be used to draw the curve between point and point + 1
			int p2 = i; //current point
			int p3 = (i + 1) % size; //next point
			int p1; //one behind current point (for tangents)
			if (i == 0) p1 = size - 1;
			else p1 = i - 1;
			int p4 = (i + 2) % size; //two ahead of current point (for tangents)
			vec3 start = cntlPnts[p2];
			vec3 end = cntlPnts[p3];
			vec3 tstart = cntlPnts[p1]; //start of tangent 1
			vec3 tend = cntlPnts[p4]; //end of tangent 2

				vec3 tangent1 = (1 - tension / 2)*(end + -1.0f * tstart); // tangent at p2 (line between p1 and p3)
				vec3 tangent2 = (1 - tension / 2)*(tend + -1.0f * start); // tangent at p3 (line between p2 and p4)

				float a1 = ((2 * pow(t, 3)) - (3 * pow(t, 2)) + 1);
				float a2 = ((-2 * pow(t, 3)) + (3 * pow(t, 2)));
				float a3 = ((pow(t, 3)) - (2 * pow(t, 2)) + t);
				float a4 = ((pow(t, 3)) - pow(t, 2));

					//calculate position using hermite polynomials and all four points
				location = a1*start + a2*end+ a3*tangent1+ a4*tangent2;
			glVertex3f(location.x, location.y, location.z);
		}
	}
	glEnd();
}

Hill::Hill(float a, float l, float d) : angle(a), length(l), div(d)
{
	firstDraw = true;
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

	int counter = 0;
	float totalHeight = 0;
	float totalLength = 0;
	if (firstDraw){
		hillPointsX.push_back(0.0);
		hillPointsY.push_back(0.0);
		std::cout << hillPointsX[counter] << ", " << hillPointsY[counter] << std::endl;
	}
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
		if (firstDraw){
			counter++;
			float random1 = -185 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (-100 + 185)));
			float random2 = 100 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (185 - 100)));
			hillPointsX.push_back(totalLength);
			hillPointsY.push_back(totalHeight);
			std::cout << hillPointsX[counter] << ", " << hillPointsY[counter] << std::endl;
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
		if (firstDraw){
			counter++;
			float random1 = -185 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (-100 + 185)));
			float random2 = 100 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (185 - 100)));
			hillPointsX.push_back(totalLength);
			hillPointsY.push_back(totalHeight);
			std::cout << hillPointsX[counter] << ", " << hillPointsY[counter] << std::endl;
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
	firstDraw = false;
}

Lift::Lift(float z){
	GrObject*chair = new Chair(0.0);
	sup1 = new Support(20, -1, z, 50, 4, 10);
	sup2 = new Support(-356., 169, z, 50, 4, 10);
	add(chair, 0, 20, 0);
	add(sup1, 20, -1, 50);
	add(sup2, -356., 169, 50);

}

Chair::Chair(float u){

}

void Chair::draw(DrawingState*){
	glColor3f(0.2, 0.1, 0);
	polygon(-4, -5., -2., 0., -5., 2., 0., 5., 2., 0., 5., -2., 0.);
	polygon(4, -5., -2., 1., -5., 2., 1., 5., 2., 1., 5., -2., 1.);
	polygon(-4, 5., 2., 0., 5., 2., 1., 5., -2., 1., 5., -2., 0.);
	polygon(4, -5., 2., 0., -5., 2., 1., -5., -2., 1., -5., -2., 0.);
	polygon(4, 5., 2., 0., 5., 2., 1., -5., 2., 1., -5., 2., 0.);
	polygon(-4, 5., -2., 0., 5., -2., 1., -5., -2., 1., -5., -2., 0.);

	glTranslated(0, -3, 3);
	glRotated(90, 1, 0, 0);
	glPushMatrix();
	polygon(-4, -5., -2., 0., -5., 2., 0., 5., 2., 0., 5., -2., 0.);
	polygon(4, -5., -2., 1., -5., 2., 1., 5., 2., 1., 5., -2., 1.);
	polygon(-4, 5., 2., 0., 5., 2., 1., 5., -2., 1., 5., -2., 0.);
	polygon(4, -5., 2., 0., -5., 2., 1., -5., -2., 1., -5., -2., 0.);
	polygon(4, 5., 2., 0., 5., 2., 1., -5., 2., 1., -5., 2., 0.);
	polygon(-4, 5., -2., 0., 5., -2., 1., -5., -2., 1., -5., -2., 0.);
	glPopMatrix();

}

Support::Support(float px, float py, float pz, float h, float rp, float rw) : x(px), y(py), z(pz), supHeight(h), poleRad(rp), wheelRad(rw)
{

}

void Support::draw(DrawingState*d){

	GLUquadricObj *quadObj = gluNewQuadric();
	GLUquadricObj *quadObjIn = gluNewQuadric();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);

	//GL_CULL_FACE is left enabled as disabling it leads to bizarre ripple effects



	glColor3f(0.5, 0.5, 0.5);
	GLfloat r;

	if (va_vertices.size() == 0) {


		GLfloat height;
		GLfloat angle;
		r = poleRad;
		GLfloat heightPoints = 500.0;
		GLfloat heightIncrement = supHeight / (heightPoints - 1);

		//The value to increment between the points is determined by the desired space to fill over the number
		//of points. In this case, a 1 is subtracted from the points, because 0 is counted as the first point
		//and the loop runs until the value equals the desired size.

		GLfloat anglePoints = 36.0;
		GLfloat angleIncrement = 360 / anglePoints;

		GLfloat radialDecreasePerHeightIncrement = 0.02f;

		for (height = 0.0; height <= supHeight; height += heightIncrement) {
			for (angle = 0; angle < 360; angle += angleIncrement){
				this->va_vertices.push_back(glm::vec3((r * cos(angle*(PI / 180))), height, (r * sin(angle*(PI / 180)))));
				this->va_tex_coords.push_back(vec2(angle / 360, height / supHeight));
			}
			if (height >= 20.0 && height <= 30.0){
				r -= radialDecreasePerHeightIncrement;
			}
		}

		GLfloat b = 0;
		GLfloat bMax = (heightPoints - 1) * anglePoints;

		for (b = 0.0; b < bMax; b++) {
			if (((int)(b) % (int)anglePoints) != ((int)anglePoints - 1)) {
				this->va_indices.push_back(glm::ivec3(b, b + anglePoints, b + anglePoints + 1));
				this->va_indices.push_back(glm::ivec3(b, b + anglePoints + 1, b + 1));
			}
			else {
				this->va_indices.push_back(glm::ivec3(b, b + anglePoints, b + 1));
				this->va_indices.push_back(glm::ivec3(b, b + 1, b - anglePoints + 1));
			}

			/*if (b < anglePoints) {
				this->va_normals.push_back(glm::vec3(0, 1, 0));
			}
			else {*/
				glm::vec3 one;
				glm::vec3 two;
				glm::vec3 three;
				glm::vec3 four;
				glm::vec3 five;
				glm::vec3 six;

				//get all points starting at point directly above and going clockwise
				one = va_vertices[GLuint(b - anglePoints)];
				if (((int)(b) % (int)anglePoints) != ((int)anglePoints - 1)) {
					two = va_vertices[GLuint(b + 1)];
					three = va_vertices[GLuint(b + anglePoints + 1)];
				}
				else {
					two = va_vertices[GLuint(b - anglePoints + 1)];
					three = va_vertices[GLuint(b + 1)];
				}
				four = va_vertices[GLuint(b + anglePoints)];
				if (((int)(b) % (int)anglePoints) != 0) {
					five = va_vertices[GLuint(b - 1)];
					six = va_vertices[GLuint(b - anglePoints - 1)];
				}
				else {
					five = va_vertices[GLuint(b + anglePoints - 1)];
					six = va_vertices[GLuint(b - 1)];
				}

				glm::vec3 sum = glm::cross(one - va_vertices[GLuint(b)], two - va_vertices[GLuint(b)]);
				sum += glm::cross(two - va_vertices[GLuint(b)], three - va_vertices[GLuint(b)]);
				sum += glm::cross(three - va_vertices[GLuint(b)], four - va_vertices[GLuint(b)]);
				sum += glm::cross(four - va_vertices[GLuint(b)], five - va_vertices[GLuint(b)]);
				sum += glm::cross(five - va_vertices[GLuint(b)], six - va_vertices[GLuint(b)]);
				sum += glm::cross(six - va_vertices[GLuint(b)], one - va_vertices[GLuint(b)]);
				this->va_normals.push_back(glm::normalize(sum));
			//}
		}

		for (b = 0; b < anglePoints; ++b) {
			for (angle = 0; angle < 360; angle += angleIncrement) {
				this->va_normals.push_back(glm::normalize(glm::vec3(r*cos(angle *(PI / 180)), 0, r*sin(angle *(PI / 180)))));
			}
		}

	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	fetchTexture("metal013.png", true, true);

	glVertexPointer(3, GL_FLOAT, 0, &va_vertices[0]);
	glNormalPointer(GL_FLOAT, 0, &va_normals[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &va_tex_coords[0]);

	glDrawElements(GL_TRIANGLES, 3 * va_indices.size(), GL_UNSIGNED_INT, &va_indices[0]);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glColor3f(0.5f, 0.5f, 0.5f);
	 
	glPushMatrix();
	glTranslated(0, supHeight, 0);
		glRotated(90, 1, 0, 0);

		glPushMatrix();
			gluCylinder(quadObj, wheelRad + 2, wheelRad+2, 3, 40, 10);
		glPopMatrix();

		glPushMatrix();
		gluQuadricOrientation(quadObjIn, GLU_INSIDE);
		gluCylinder(quadObjIn, wheelRad - 2, wheelRad - 2, 3, 40, 10);
		glPopMatrix();

		glPushMatrix();
			glTranslated(0, 10, 2);
			glRotated(90, 1, 0, 0);
			gluCylinder(quadObj, 0.4, 0.4, 20, 10, 10);
			glPushMatrix();
			glTranslated(-10, 0, 10);
			glRotated(90, 0, 1,	0);
			gluCylinder(quadObj, 0.4, 0.4, 20, 10, 10);
			glPopMatrix();
		glPopMatrix();

		glPushMatrix();
			glTranslated(0, 0, 2.5);
			gluDisk(quadObj, wheelRad - 2.1, wheelRad + 2, 40, 10);
		glPopMatrix();

		glPushMatrix();
			glRotated(180, 1, 0, 0);
			gluDisk(quadObj, wheelRad - 2.1, wheelRad + 2, 40, 10);
			gluDisk(quadObj, 0, 2.05, 20, 10);
		glPopMatrix();


	glPopMatrix();
	glPopAttrib();
	

}

Tree::Tree(float px, float py, float pz)
: x(px), y(py), z(pz)
{
	//transMatrix(transform, px, py, pz);

}


Texture tree_tex;
void Tree::draw(DrawingState* d){
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);

	float changez = 0;
	
	float xpos = ((FlyCamera*)d->camera)->posX - x;
	float zpos = ((FlyCamera*)d->camera)->posZ - z;
	float ypos = ((FlyCamera*)d->camera)->posY;

	if (zpos >= 0) changez -= 180;

	//rotate train on y axis to get correct orientation in xz plane
	glRotatef(((atan(xpos / zpos) * 180 / PI) + changez + 90), 0, 1, 0);

	tree_tex = *fetchTexture("pinetree.bmp", false, false);

	//glActiveTexture(GL_TEXTURE0);

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
	glPopAttrib();

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
