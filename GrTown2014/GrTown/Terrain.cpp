#include "Terrain.H"
#include "GrTown_PCH.H"

#include "GrWorld.H"
#include "DrawingState.H"
#include "DrawUtils.H"
#include <math.h>

#define PI 3.14159265

Terrain::Terrain(float a, float d, float l)
	: angle(a), div(d), length(l)
{

}

void Terrain::draw(DrawingState*d){
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1., 10);
	glPushMatrix();
	glTranslated(200, 0, 0);
	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	glColor4fv(color.operator());
	glVertex3d(-200, -1, 200);
	glVertex3d(100, -1, 200);
	glVertex3d(100, -1, -200);
	glVertex3d(-200, -1, -200);
	glEnd();
	glPopMatrix();

	double totalHeight = 0;
	double totalLength = 0;
	//double angle = 10;
	//double length = 15;
	//double div = 10;
	double maxA = (angle*div) / 2;
	for (double i = angle; i <= maxA; i += angle){
		double x = -length*(cos(i*PI / 180));
		double y = length*sin(i*PI / 180);
		glPushMatrix();
		glTranslated(x + totalLength, y + totalHeight, 0);
		glRotated(-i, 0, 0, 1);
		glBegin(GL_POLYGON);
		glNormal3f(0, 1, 0);
		glColor4fv(color.operator());
		glVertex3d(-length - 0.5, -1, 200);
		glVertex3d(length + 0.5, -1, 200);
		glVertex3d(length + 0.5, -1, -200);
		glVertex3d(-length - 0.5, -1, -200);
		glEnd();
		glPopMatrix();
		totalHeight += y * 2;
		totalLength += x * 2;
	}
	for (double i = maxA; i >= 0; i -= angle){
		double x = -length*(cos(i*PI / 180));
		double y = length*sin(i*PI / 180);
		glPushMatrix();
		glTranslated(x + totalLength, y + totalHeight, 0);
		glRotated(-i, 0, 0, 1);
		glBegin(GL_POLYGON);
		glNormal3f(0, 1, 0);
		glColor4fv(color.operator());
		glVertex3d(-length - 0.5, -1, 200);
		glVertex3d(length + 0.5, -1, 200);
		glVertex3d(length + 0.5, -1, -200);
		glVertex3d(-length - 0.5, -1, -200);
		glEnd();
		glPopMatrix();
		totalHeight += y * 2;
		totalLength += x * 2;
	}

	glDisable(GL_POLYGON_OFFSET_FILL);

}
