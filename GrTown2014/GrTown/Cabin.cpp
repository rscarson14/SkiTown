#define GLEW_STATIC 1

#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Cabin.h"
#include "Utilities/Texture.h"
#include "Utilities/ShaderTools.H"
#include <FL/fl_ask.h>

using namespace glm;

Texture* day_side;
Texture* normal_map;
Texture* night_side;
Texture* day_roof;

Cabin::Cabin(){
	Texture* day_side = NULL;
	Texture* normal_map = NULL;
	Texture* night_side = NULL;
	Texture* day_roof = NULL;
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

	if (day_side == NULL) { day_side = fetchTexture("wood_texture.png", false, false); }
	if (normal_map == NULL) { normal_map = fetchTexture("wood_normals.png", false, false); }
	if (day_roof == NULL) { day_roof = fetchTexture("roof3.png", false, false); } 

	if (normal_map == NULL || day_side == NULL || day_roof == NULL){
		printf("ERROR! didn't load cabin stuff correctly\n");
	}
	else{	
		printf("Cabin stuff laoded great\n");
	}


	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	GLfloat g_vertex_buffer_data[] = {
		-10.0f, -10.0f, -10.0f, // triangle 1 : begin
		-10.0f, -10.0f, 10.0f,
		-10.0f, 10.0f, 10.0f, // triangle 1 : end

		10.0f, 10.0f, -10.0f, // triangle 2 : begin
		-10.0f, -10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f, // triangle 2 : end
		
		10.0f, -10.0f, 10.0f, //
		-10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,

		10.0f, 10.0f, -10.0f, //
		10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f, -10.0f,

		-10.0f, -10.0f, -10.0f, //
		-10.0f, 10.0f, 10.0f,
		-10.0f, 10.0f, -10.0f,

		10.0f, -10.0f, 10.0f, //
		-10.0f, -10.0f, 10.0f,
		-10.0f, -10.0f, -10.0f,

		-10.0f, 10.0f, 10.0f, //
		-10.0f, -10.0f, 10.0f,
		10.0f, -10.0f, 10.0f,
		
		10.0f, 10.0f, 10.0f, //*
		10.0f, -10.0f, -10.0f,
		10.0f, 10.0f, -10.0f,
		
		10.0f, -10.0f, -10.0f, //
		10.0f, 10.0f, 10.0f,
		10.0f, -10.0f, 10.0f,
		
		10.0f, 10.0f, 10.0f, //
		10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,
		
		10.0f, 10.0f, 10.0f, //
		-10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, 10.0f,
		
		10.0f, 10.0f, 10.0f, //
		-10.0f, 10.0f, 10.0f,
		10.0f, -10.0f, 10.0f,
	};

	GLfloat tex_coords[] = {
		// face 1
		0.0f, 0.0f,//
		1.0f, 0.0f,
		1.0f, 1.0f,
		
		0.0f, 1.0f,//
		1.0f, 0.0f,
		1.0f, 1.0f,

		1.0f, 0.0f, //
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 1.0f, //
		0.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 0.0f, // 
		0.0f, 1.0f,
		1.0f, 1.0f,

		1.0f, 1.0f, //
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 1.0f, //
		0.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 1.0f, //*
		1.0f, 0.0f,
		0.0f, 1.0f,

		1.0f, 0.0f, //
		0.0f, 1.0f,
		0.0f, 0.0f,

		1.0f, 0.0f, //
		1.0f, 1.0f,
		0.0f, 1.0f,

		1.0f, 0.0f, //
		0.0f, 1.0f,
		0.0f, 0.0f,

		1.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
	};

	glUseProgram(wallShader);
	glDisable(GL_CULL_FACE);
	
	glPushMatrix();
	glTranslated(0, 10, 0);

	mat4 pMat = mat4(0.0f);
	mat4 vMat = mat4(0.0f);

	glGetFloatv(GL_PROJECTION_MATRIX, &pMat[0][0]);
	glGetFloatv(GL_MODELVIEW_MATRIX, &vMat[0][0]);

	mat4 MVP = pMat * vMat;
	
	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	GLuint MatrixID = glGetUniformLocation(wallShader, "MVP");
	GLuint TextureID = glGetUniformLocation(wallShader, "tex");
	GLuint NormalID = glGetUniformLocation(wallShader, "normals");

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	// For each model you render, since the MVP will be different (at least the M part)
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniform1i(TextureID, 0);
	glUniform1i(NormalID, 1);

	//if (day_side == NULL){ printf("Problems....\n"); }
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, day_side->texName);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal_map->texName);

	GLuint buffers[2]; // 0 = vertex, 1 = uv

	glGenBuffers(2, buffers);
	
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords), tex_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, sizeof(g_vertex_buffer_data));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, day_roof->texName);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal_map->texName);

	 GLfloat g_roof_buffer_data[] = {
		-12.0f, 8.0f, 12.0f,
		0.0f, 20.0f, 0.0f,
		12.0f, 8.0f, 12.0f,

		12.0f, 8.0f, -12.0f,
		0.0f, 20.0f, 0.0f,
		12.0f, 8.0f, 12.0f,

		-12.0f, 8.0f, -12.0f,
		0.0f, 20.0f, 0.0f,
		12.0f, 8.0f, -12.0f,

		-12.0f, 8.0f, -12.0f,
		0.0f, 20.0f, 0.0f,
		-12.0f, 8.0f, 12.0f,
	};

	 GLfloat g_roof_tex_data[] = {
		0.0f, 0.0f,
		0.5f, 1.0f,
		1.0f, 0.0f,

		0.0f, 0.0f,
		0.5f, 1.0f,
		1.0f, 0.0f,

		0.0f, 0.0f,
		0.5f, 1.0f,
		1.0f, 0.0f,

		0.0f, 0.0f,
		0.5f, 1.0f,
		1.0f, 0.0f,
	 };


	 glGenBuffers(2, buffers);

	 glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(g_roof_buffer_data), g_roof_buffer_data, GL_STATIC_DRAW);
	 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(0);

	 glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(g_roof_tex_data), g_roof_tex_data, GL_STATIC_DRAW);
	 glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(1);

	 glDrawArrays(GL_TRIANGLES, 0, sizeof(g_roof_buffer_data));

	 glDisableVertexAttribArray(0);
	 glDisableVertexAttribArray(1);

	glUseProgram(NULL);
	glActiveTexture(NULL);
	glPopMatrix();
	
}