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

	if (normal_map == NULL || day_side == NULL || day_roof == NULL){
		printf("ERROR! didn't load cabin stuff correctly\n");
	}

	int posZ_index = 0;
	int negZ_index = 18;
	int posX_index = 36;
	int negX_index = 54;
	int v2_offset = 3;
	int v3_offset = 6;
	
	// Cube vertices. Each set makes a triangle for a face. UV coords are mapped appropriately
	GLfloat g_vertex_buffer_data[] = {

		-10.0f, 10.0f, 10.0f, // +Z
		-10.0f, -10.0f, 10.0f,
		10.0f, -10.0f, 10.0f,
		
		10.0f, 10.0f, 10.0f, // +Z
		-10.0f, 10.0f, 10.0f,
		10.0f, -10.0f, 10.0f,

		-10.0f, 10.0f, -10.0f, // -Z
		-10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,

		10.0f, 10.0f, -10.0f, // -Z
		-10.0f, 10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,

		10.0f, 10.0f, -10.0f, // +X
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, 10.0f,

		10.0f, 10.0f, 10.0f, // +X
		10.0f, 10.0f, -10.0f,
		10.0f, -10.0f, 10.0f,

		-10.0f, 10.0f, -10.0f, // -X
		-10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f, 10.0f,

		-10.0f, 10.0f, 10.0f, // -X
		-10.0f, 10.0f, -10.0f,
		-10.0f, -10.0f, 10.0f,
	};


	GLfloat g_normal_buffer_data[sizeof(g_vertex_buffer_data)/3]; // one normal for every vertex
	
	//to calculate normals we take the cross product two of the triangles edges. 
	// we can do this once for each face of the cube (or every two triangles)
	vec3 edge1, edge2;
	vec3 normal;

	edge1.x= g_vertex_buffer_data[posZ_index] - g_vertex_buffer_data[posZ_index + v2_offset];
	edge1.y= g_vertex_buffer_data[posZ_index + 1] - g_vertex_buffer_data[posZ_index + v2_offset + 1];
	edge1.z = g_vertex_buffer_data[posZ_index + 2] - g_vertex_buffer_data[posZ_index + v2_offset + 2];

	edge2.x = g_vertex_buffer_data[posZ_index] - g_vertex_buffer_data[posZ_index + v3_offset];
	edge2.y = g_vertex_buffer_data[posZ_index + 1] - g_vertex_buffer_data[posZ_index + v3_offset + 1];
	edge2.z = g_vertex_buffer_data[posZ_index + 2] - g_vertex_buffer_data[posZ_index + v3_offset + 2];

	normal = normalize(cross(edge1, edge2));
	
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < 3; j++){
			g_normal_buffer_data[posZ_index + i*3 + j] = normal[j];
		}
	}

	edge1.x = g_vertex_buffer_data[negZ_index] - g_vertex_buffer_data[negZ_index + v2_offset];
	edge1.y = g_vertex_buffer_data[negZ_index + 1] - g_vertex_buffer_data[negZ_index + v2_offset + 1];
	edge1.z = g_vertex_buffer_data[negZ_index + 2] - g_vertex_buffer_data[negZ_index + v2_offset + 2];

	edge2.x = g_vertex_buffer_data[negZ_index] - g_vertex_buffer_data[negZ_index + v3_offset];
	edge2.y = g_vertex_buffer_data[negZ_index + 1] - g_vertex_buffer_data[negZ_index + v3_offset + 1];
	edge2.z = g_vertex_buffer_data[negZ_index + 2] - g_vertex_buffer_data[negZ_index + v3_offset + 2];

	normal = normalize(cross(edge2, edge1));

	for (int i = 0; i < 6; i++){
		for (int j = 0; j < 3; j++){
			g_normal_buffer_data[negZ_index + i * 3 + j] = normal[j];
		}
	}

	edge1.x = g_vertex_buffer_data[posX_index] - g_vertex_buffer_data[posX_index + v2_offset];
	edge1.y = g_vertex_buffer_data[posX_index + 1] - g_vertex_buffer_data[posX_index + v2_offset + 1];
	edge1.z = g_vertex_buffer_data[posX_index + 2] - g_vertex_buffer_data[posX_index + v2_offset + 2];

	edge2.x = g_vertex_buffer_data[posX_index] - g_vertex_buffer_data[posX_index + v3_offset];
	edge2.y = g_vertex_buffer_data[posX_index + 1] - g_vertex_buffer_data[posX_index + v3_offset + 1];
	edge2.z = g_vertex_buffer_data[posX_index + 2] - g_vertex_buffer_data[posX_index + v3_offset + 2];

	normal = normalize(cross(edge1, edge2));

	for (int i = 0; i < 6; i++){
		for (int j = 0; j < 3; j++){
			g_normal_buffer_data[posX_index + i * 3 + j] = normal[j];
		}
	}

	edge1.x = g_vertex_buffer_data[negX_index] - g_vertex_buffer_data[negX_index + v2_offset];
	edge1.y = g_vertex_buffer_data[negX_index + 1] - g_vertex_buffer_data[negX_index + v2_offset + 1];
	edge1.z = g_vertex_buffer_data[negX_index + 2] - g_vertex_buffer_data[negX_index + v2_offset + 2];

	edge2.x = g_vertex_buffer_data[negX_index] - g_vertex_buffer_data[negX_index + v3_offset];
	edge2.y = g_vertex_buffer_data[negX_index + 1] - g_vertex_buffer_data[negX_index + v3_offset + 1];
	edge2.z = g_vertex_buffer_data[negX_index + 2] - g_vertex_buffer_data[negX_index + v3_offset + 2];

	normal = normalize(cross(edge2, edge1));

	for (int i = 0; i < 6; i++){
		for (int j = 0; j < 3; j++){
			g_normal_buffer_data[negX_index + i * 3 + j] = normal[j];
		}
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords), tex_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_normal_buffer_data), g_normal_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);

	glDrawArrays(GL_TRIANGLES, 0, sizeof(g_vertex_buffer_data));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, day_roof->texName);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal_map->texName);


	posZ_index = 0;
	posX_index = 9;
	negZ_index = 18;
	negX_index = 27;
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

	 GLfloat g_roof_normal_data[sizeof(g_roof_buffer_data) / 3];

	 edge1.x = g_vertex_buffer_data[posZ_index] - g_vertex_buffer_data[posZ_index + v2_offset];
	 edge1.y = g_vertex_buffer_data[posZ_index + 1] - g_vertex_buffer_data[posZ_index + v2_offset + 1];
	 edge1.z = g_vertex_buffer_data[posZ_index + 2] - g_vertex_buffer_data[posZ_index + v2_offset + 2];

	 edge2.x = g_vertex_buffer_data[posZ_index] - g_vertex_buffer_data[posZ_index + v3_offset];
	 edge2.y = g_vertex_buffer_data[posZ_index + 1] - g_vertex_buffer_data[posZ_index + v3_offset + 1];
	 edge2.z = g_vertex_buffer_data[posZ_index + 2] - g_vertex_buffer_data[posZ_index + v3_offset + 2];

	 normal = normalize(cross(edge1, edge2));

	 for (int i = 0; i < 3; i++){
		 for (int j = 0; j < 3; j++){
			 g_roof_normal_data[posZ_index + i * 3 + j] = normal[j];
		 }
	 }

	 edge1.x = g_vertex_buffer_data[posX_index] - g_vertex_buffer_data[posX_index + v2_offset];
	 edge1.y = g_vertex_buffer_data[posX_index + 1] - g_vertex_buffer_data[posX_index + v2_offset + 1];
	 edge1.z = g_vertex_buffer_data[posX_index + 2] - g_vertex_buffer_data[posX_index + v2_offset + 2];

	 edge2.x = g_vertex_buffer_data[posX_index] - g_vertex_buffer_data[posX_index + v3_offset];
	 edge2.y = g_vertex_buffer_data[posX_index + 1] - g_vertex_buffer_data[posX_index + v3_offset + 1];
	 edge2.z = g_vertex_buffer_data[posX_index + 2] - g_vertex_buffer_data[posX_index + v3_offset + 2];

	 normal = normalize(cross(edge1, edge2));

	 for (int i = 0; i < 3; i++){
		 for (int j = 0; j < 3; j++){
			 g_roof_normal_data[posX_index + i * 3 + j] = normal[j];
		 }
	 }

	 edge1.x = g_vertex_buffer_data[negZ_index] - g_vertex_buffer_data[negZ_index + v2_offset];
	 edge1.y = g_vertex_buffer_data[negZ_index + 1] - g_vertex_buffer_data[negZ_index + v2_offset + 1];
	 edge1.z = g_vertex_buffer_data[negZ_index + 2] - g_vertex_buffer_data[negZ_index + v2_offset + 2];

	 edge2.x = g_vertex_buffer_data[negZ_index] - g_vertex_buffer_data[negZ_index + v3_offset];
	 edge2.y = g_vertex_buffer_data[negZ_index + 1] - g_vertex_buffer_data[negZ_index + v3_offset + 1];
	 edge2.z = g_vertex_buffer_data[negZ_index + 2] - g_vertex_buffer_data[negZ_index + v3_offset + 2];

	 normal = normalize(cross(edge1, edge2));

	 for (int i = 0; i < 3; i++){
		 for (int j = 0; j < 3; j++){
			 g_roof_normal_data[negZ_index + i * 3 + j] = normal[j];
		 }
	 }

	 edge1.x = g_vertex_buffer_data[negX_index] - g_vertex_buffer_data[negX_index + v2_offset];
	 edge1.y = g_vertex_buffer_data[negX_index + 1] - g_vertex_buffer_data[negX_index + v2_offset + 1];
	 edge1.z = g_vertex_buffer_data[negX_index + 2] - g_vertex_buffer_data[negX_index + v2_offset + 2];

	 edge2.x = g_vertex_buffer_data[negX_index] - g_vertex_buffer_data[negX_index + v3_offset];
	 edge2.y = g_vertex_buffer_data[negX_index + 1] - g_vertex_buffer_data[negX_index + v3_offset + 1];
	 edge2.z = g_vertex_buffer_data[negX_index + 2] - g_vertex_buffer_data[negX_index + v3_offset + 2];

	 normal = normalize(cross(edge1, edge2));

	 for (int i = 0; i < 3; i++){
		 for (int j = 0; j < 3; j++){
			 g_roof_normal_data[negX_index + i * 3 + j] = normal[j];
		 }
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


	 glGenBuffers(2, buffers);

	 glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(g_roof_buffer_data), g_roof_buffer_data, GL_STATIC_DRAW);
	 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(0);

	 glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(g_roof_tex_data), g_roof_tex_data, GL_STATIC_DRAW);
	 glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(1);

	 glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(g_roof_normal_data), g_roof_normal_data, GL_STATIC_DRAW);
	 glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(2);

	 glDrawArrays(GL_TRIANGLES, 0, sizeof(g_roof_buffer_data));

	 glDisableVertexAttribArray(0);
	 glDisableVertexAttribArray(1);

	glUseProgram(NULL);
	glPopMatrix();
	
}


void Cabin::drawAfter(DrawingState* drst){

}