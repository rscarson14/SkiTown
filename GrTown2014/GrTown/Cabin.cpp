#define GLEW_STATIC 1

#include <GL/glew.h>


#include "Cabin.h"
#include "Utilities/Texture.h"
#include "Utilities/ShaderTools.H"
#include <FL/fl_ask.h>


Cabin::Cabin(){


}

void Cabin::draw(DrawingState *drst){

}


GLuint wallShader = 0;
GLuint triedWallShader = false;
void Cabin::drawAfter(DrawingState *drst){
	glewInit();

	if (wallShader == 0 && !triedWallShader){
		triedWallShader = true;
		char* error;
		if (!(wallShader = loadShader("Cabin.vert", "Cabin.frag", error))){
			std::string s = "Can't Load Cabin wall Shader:";
			s += error;
			fl_alert(s.c_str());
		}
		
	}

	day_side = fetchTexture("wood_texture.png", false, false);

	GLfloat points[] = {
			-1.0f, -1.0f, -1.0f, // triangle 1 : begin
			-1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f, // triangle 1 : end
			1.0f, 1.0f, -1.0f, // triangle 2 : begin
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f, // triangle 2 : end
	};

	GLfloat tex_coords[] = {
		// face 1
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,

		0.0, 1.0,
		0.0, 0.0,
		1.0, 0.0
	};

	glUseProgram(wallShader);
	
	glDisable(GL_CULL_FACE);

	GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};
	
	GLuint vertexBuffer;

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glDrawArrays(GL_TRIANGLES, 0, 18);
	glDisableVertexAttribArray(0);

	/*GLuint points_buffer;

	glGenBuffers(1, &points_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, points_buffer);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), &points, GL_STATIC_DRAW);
	
	GLuint points_vao;

	glEnableVertexAttribArray(0);
	glGenVertexArrays(1, &points_vao);
	glBindVertexArray(points_vao);
	glVertexAttribPointer(0, 18, GL_FLOAT, GL_FALSE, 0, NULL);

	glDrawArrays(GL_TRIANGLES, 0, 18);*/

	//glDisableVertexAttribArray(0);
//	glUseProgram(NULL);

	/*GLint texUniform = glGetUniformLocation(wallShader, "tex");

	if (texUniform < 0){
		printf("ERROR!!!!! loading texture uniform\n");
	}
	glUniform1i(texUniform, day_side->texName);

	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof (float), points, GL_STATIC_DRAW);

	GLuint colours_vbo = 0;
	glGenBuffers(1, &colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof (float), tex_coords, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);*/

	//glDrawArraysEXT(GL_TRIANGLES, 0, 18);
	
}