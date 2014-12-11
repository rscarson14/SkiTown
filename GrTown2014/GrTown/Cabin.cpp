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
Texture* roof_normal;
Texture* day_roof;

Cabin::Cabin(){
	/*Texture* day_side = NULL;
	Texture* normal_map = NULL;
	Texture* night_side = NULL;
	Texture* day_roof = NULL;*/
}


GLuint wallShader = 0;
GLuint triedWallShader = false;
void Cabin::draw(DrawingState *drst){
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

	if (day_side == NULL) { day_side = fetchTexture("wood_texture.bmp", false, false); }
	if (normal_map == NULL) { normal_map = fetchTexture("wood_normals.png", false, false); }
	if (day_roof == NULL) { day_roof = fetchTexture("snowroof3.png", false, false); } 
	if (roof_normal == NULL) { roof_normal = fetchTexture("roof_normals.png", false, false); }

	if (normal_map == NULL || day_side == NULL || day_roof == NULL){
		printf("ERROR! didn't load cabin stuff correctly\n");
	}
	
	// Cube vertices. Each set makes a triangle for a face. UV coords are mapped appropriately
	vec3 g_vertex_buffer_data[] = {

		vec3(-10.0f, 10.0f, 10.0f), // +Z
		vec3(-10.0f, -10.0f, 10.0f),
		vec3(10.0f, -10.0f, 10.0f),

		vec3(10.0f, 10.0f, 10.0f), // +Z
		vec3(-10.0f, 10.0f, 10.0f),
		vec3(10.0f, -10.0f, 10.0f),

		vec3(-10.0f, 10.0f, -10.0f), // -Z
		vec3(-10.0f, -10.0f, -10.0f),
		vec3(10.0f, -10.0f, -10.0f),

		vec3(10.0f, 10.0f, -10.0f), // -Z
		vec3(-10.0f, 10.0f, -10.0f),
		vec3(10.0f, -10.0f, -10.0f),

		vec3(10.0f, 10.0f, -10.0f), // +X
		vec3(10.0f, -10.0f, -10.0f),
		vec3(10.0f, -10.0f, 10.0f),

		vec3(10.0f, 10.0f, 10.0f), // +X
		vec3(10.0f, 10.0f, -10.0f),
		vec3(10.0f, -10.0f, 10.0f),

		vec3(-10.0f, 10.0f, -10.0f), // -X
		vec3(-10.0f, -10.0f, -10.0f),
		vec3(-10.0f, -10.0f, 10.0f),

		vec3(-10.0f, 10.0f, 10.0f), // -X
		vec3(-10.0f, 10.0f, -10.0f),
		vec3(-10.0f, -10.0f, 10.0f),

	};


	//GLfloat g_normal_buffer_data[sizeof(g_vertex_buffer_data)/3]; // one normal for every triangle

	vec3 g_normal_buffer_data[24];
	
	//to calculate normals we take the cross product two of the triangles edges. 
	// we can do this once for each face of the cube (or every two triangles)
	vec3 p1, p2, p3;
	int j = 0;
	for (int i = 0; i < 4; i++){
		p1 = g_vertex_buffer_data[j];
		p2 = g_vertex_buffer_data[j + 1];
		p3 = g_vertex_buffer_data[j + 2];

		vec3 e1 = normalize(p2 - p1);
		vec3 e2 = normalize(p2 - p3);

		vec3 norm = normalize(cross(e1, e2));
		
		for (int k = 0; k < 6; k++){
			if (i % 2 == 0){
				g_normal_buffer_data[i * 6 + k] = vec3(norm);
			}
		}

		j += 6;
	}

	GLfloat tex_coords[] = {

		0.0f, 1.0f, // +Z
		0.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 1.0f, // +Z
		0.0f, 1.0f,
		1.0f, 0.0f,

		0.0f, 1.0f, // -Z
		0.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 1.0f, // -Z
		0.0f, 1.0f,
		1.0f, 0.0f,

		0.0f, 1.0f, // +X
		0.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 1.0f, // +X
		0.0f, 1.0f,
		1.0f, 0.0f,

		0.0f, 1.0f, // -X
		0.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 1.0f, // -X
		0.0f, 1.0f,
		1.0f, 0.0f,
	};

	glUseProgram(wallShader);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
	glDisable(GL_CULL_FACE);
	
	glPushMatrix();
	glTranslated(0, 10, 0);
	//glRotated(45.0f, 0.0f, 1.0f, 0.0f);

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
	GLuint SunID1 = glGetUniformLocation(wallShader, "sun.lookFrom");
	GLuint SunID2 = glGetUniformLocation(wallShader, "sun.lookTowards");
	GLuint SunID3 = glGetUniformLocation(wallShader, "sun.ambientColor");

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	// For each model you render, since the MVP will be different (at least the M part)
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniform1i(TextureID, 0);
	glUniform1i(NormalID, 1);
	glUniform4fv(SunID1, 1, &(drst->sun.lookFrom[0]));
	glUniform4fv(SunID2, 1, &(drst->sun.lookTowards[0]));
	glUniform4fv(SunID3, 1, &(drst->sun.ambientColor[0]));
	//if (day_side == NULL){ printf("Problems....\n"); }
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, day_side->texName);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal_map->texName);

	GLuint buffers[3]; // 0 = vertex, 1 = uv

	glGenBuffers(3, buffers);
	
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_normal_buffer_data), g_normal_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords), tex_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);

	glDrawArrays(GL_TRIANGLES, 0, sizeof(g_vertex_buffer_data)/sizeof(vec3));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, day_roof->texName);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, roof_normal->texName);

 	 vec3 g_roof_buffer_data[] = {
		vec3(-12.0f, 8.0f, 12.0f),
		vec3(0.0f, 20.0f, 0.0f),
		vec3(12.0f, 8.0f, 12.0f),

		vec3(12.0f, 8.0f, 12.0f),
		vec3(0.0f, 20.0f, 0.0f),
		vec3(12.0f, 8.0f, -12.0f),

		vec3(12.0f, 8.0f, -12.0f),
		vec3(0.0f, 20.0f, 0.0f),
		vec3(-12.0f, 8.0f, -12.0f),

		vec3(-12.0f, 8.0f, -12.0f),
		vec3(0.0f, 20.0f, 0.0f),
		vec3(-12.0f, 8.0f, 12.0f),
	};

	 vec3 g_roof_normal_data[12];

	 j = 0;
	 for (int i = 0; i < 4; i++){
		 p1 = g_roof_buffer_data[j];
		 p2 = g_roof_buffer_data[j + 1];
		 p3 = g_roof_buffer_data[j + 2];

		 vec3 e1 = normalize(p2 - p1);
		 vec3 e2 = normalize(p2 - p3);

		 vec3 norm = normalize(cross(e2, e1));

		 for (int k = 0; k < 3; k++){
			 g_roof_normal_data[i * 3 + k] = vec3(norm);
		 }

		 j += 3;
	 }

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

	 GLuint buffers1[3];

	 glGenBuffers(3, buffers1);

	 glBindBuffer(GL_ARRAY_BUFFER, buffers1[0]);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(g_roof_buffer_data), g_roof_buffer_data, GL_STATIC_DRAW);
	 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(0);


	 glBindBuffer(GL_ARRAY_BUFFER, buffers1[1]);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(g_roof_normal_data), g_roof_normal_data, GL_STATIC_DRAW);
	 glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(1);

	 glBindBuffer(GL_ARRAY_BUFFER, buffers1[2]);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(g_roof_tex_data), g_roof_tex_data, GL_STATIC_DRAW);
	 glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(2);

	 glDrawArrays(GL_TRIANGLES, 0, sizeof(g_roof_buffer_data));

	 glDisableVertexAttribArray(0);
	 glDisableVertexAttribArray(1);
	 glDisableVertexAttribArray(2);

	glUseProgram(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopAttrib();
	glPopClientAttrib();
	glPopMatrix();
	
}


void Cabin::drawAfter(DrawingState* drst){

}