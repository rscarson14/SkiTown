#include "Snow.h"
#include "Behavior.H"
#include "Utilities/Texture.h"



Snow::Snow(uint numFlakes){
	srand(numFlakes);
	for (int i = 0; i < numFlakes; i++){
		float random1 = -600 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1200)));
		float random2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (500)));
		float random3 = -600 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1200)));
		vec3 newFlake = vec3(random1, random2, random3);
		snowPos.push_back(newFlake);
	}
}

void Snow::draw(DrawingState* dr){

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glLightfv(GL_LIGHT0, GL_POSITION, &(vec4(1.0)[0]));
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	fetchTexture("Snowflake.bmp");
	for (int i = 0; i < snowPos.size(); i++){	
		glPushMatrix();
		glTranslatef(snowPos[i].x, snowPos[i].y, snowPos[i].z);
		glBegin(GL_QUADS);
		glVertex3f(-1.5f, -1.5f, 0.0f); glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.5f, 1.5f, 0.0f); glTexCoord2f(0.0f, 1.0f);
		glVertex3f(1.5f, 1.5f, 0.0f); glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.5f, -1.5f, 0.0f); glTexCoord2f(1.0f, 0.0f);
		glEnd();
		glBegin(GL_QUADS);
		glVertex3f(0.0f, -1.5f, -1.5f); glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 1.5f, -1.5f); glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 1.5f, 1.5f); glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0f, -1.5f, 1.5f); glTexCoord2f(1.0f, 0.0f);
		glEnd();
		glPopMatrix();
	}
	
	glPopAttrib();
	glPopMatrix();
}


SnowFall::SnowFall(GrObject* o, float r) : Behavior(o), rate(r)
{
}
void SnowFall::simulateUntil(unsigned long t)
{

	Snow* snow = (Snow *) owner;
	vector<vec3>* flakes = &snow->snowPos;

	for (uint i = 0; i < flakes->size(); i++){
		(*(flakes))[i].y = (*(flakes))[i].y - rate*(t-lastV);
		if ((*(flakes))[i].y < 0.0f){
			(*(flakes))[i].y = 500.0f;
		}
	}

	lastV = t;
}