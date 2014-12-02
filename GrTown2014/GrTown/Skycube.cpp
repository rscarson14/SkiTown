#define GLEW_STATIC 1

#include <GL/glew.h>


#include "Skycube.h"
#include "Utilities/Texture.h"
#include "Utilities/ShaderTools.H"
#include <FL/fl_ask.h>


Skycube::Skycube(){

	

}

GLuint skyShader = 0;
bool loadedSkyShader = false;
void Skycube::draw(DrawingState* st){
	glDisable(GL_DEPTH_TEST);
	front_tex = fetchTexture("suv-front.png");	// fetch does the bind
	// front, back
	if (!front_tex)
		front_tex = fetchTexture("suv-front.png");	// fetch does the bind
	else
		front_tex->bind();

	glewInit();
	char* error;
	
	if (!loadedSkyShader && !skyShader){
		loadedSkyShader = false;
		if (!(skyShader = loadShader("Sky.vert", "Sky.frag", error))) {
			std::string s = "Can't Load sky shader:";
			s += error;
			fl_alert(s.c_str());
		}
		else{
			printf("loaded skyShader!\n");
		}
	}

	float points[] = {
		-10.0f, 10.0f, -10.0f,
		-10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,

		-10.0f, -10.0f, 10.0f,
		-10.0f, -10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, 10.0f,
		-10.0f, -10.0f, 10.0f,

		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,

		-10.0f, -10.0f, 10.0f,
		-10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, -10.0f, 10.0f,
		-10.0f, -10.0f, 10.0f,

		-10.0f, 10.0f, -10.0f,
		10.0f, 10.0f, -10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		-10.0f, 10.0f, 10.0f,
		-10.0f, 10.0f, -10.0f,

		-10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f, 10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f, 10.0f,
		10.0f, -10.0f, 10.0f
	};
	glUseProgram(skyShader);
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof (float), &points, GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glUseProgram(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);
	glEnable(GL_DEPTH_TEST);

}

void Skycube::drawAfter(DrawingState* st){
	float points[] = {
		-10.0f, 10.0f, -10.0f,
		-10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,

		-10.0f, -10.0f, 10.0f,
		-10.0f, -10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, 10.0f,
		-10.0f, -10.0f, 10.0f,

		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,

		-10.0f, -10.0f, 10.0f,
		-10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, -10.0f, 10.0f,
		-10.0f, -10.0f, 10.0f,

		-10.0f, 10.0f, -10.0f,
		10.0f, 10.0f, -10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		-10.0f, 10.0f, 10.0f,
		-10.0f, 10.0f, -10.0f,

		-10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f, 10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f, 10.0f,
		10.0f, -10.0f, 10.0f
	};
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof (float), &points, GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}