#define GLEW_STATIC 1

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

#include "Skycube.h"
#include "Utilities/Texture.h"
#include "Utilities/ShaderTools.H"
#include <FL/fl_ask.h>

using namespace glm;

Texture* sky_tex;

Skycube::Skycube(){


}

GLfloat counter = 0.0f;
GLuint skyShader = 0;
bool triedSkyShader = false;
void Skycube::draw(DrawingState* st, GrObject *camera){
	
	glewInit();
	glEnable(GL_TEXTURE_2D);
	if (skyShader == 0 && !triedSkyShader){
		triedSkyShader = true;
		char* error;
		if (!(skyShader = loadShader("Sky.vert", "Sky.frag", error))){
			std::string s = "Can't Load Cabin wall Shader:";
			s += error;
			fl_alert(s.c_str());
		}
	}
	
	if (sky_tex == NULL){ sky_tex = fetchTexture("sky-ss018.png", true, true); }

	// Cube vertices. Each set makes a triangle for a face. UV coords are mapped appropriately
	GLfloat g_vertex_buffer_data[] = {

		-1.0f, 1.0f, 1.0f, // +Z
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		1.0f, 1.0f, 1.0f, // +Z
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f, // -Z
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		1.0f, 1.0f, -1.0f, // -Z
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		1.0f, 1.0f, -1.0f, // +X
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,

		1.0f, 1.0f, 1.0f, // +X
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f, // -X
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, 1.0f, // -X
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,

	};

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

	counter += 0.001f;
	if (counter > 1.0f){
		counter = 0.0f;
	}
	printf("counter %f\n", counter);

	glUseProgram(skyShader);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	FlyCamera* cam = (FlyCamera*)camera;


	glPushMatrix();
	//glRotatef(-cam->direction*(180 / 3.1458), 0.0, 1.0, 0.0f);
	glTranslatef(cam->posX, cam->posY, cam->posZ);

	mat4 pMat = mat4(0.0f);
	mat4 vMat = mat4(1.0f);

	glGetFloatv(GL_PROJECTION_MATRIX, &pMat[0][0]);
	glGetFloatv(GL_MODELVIEW_MATRIX, &vMat[0][0]);
	
	/*vMat[0][3] = 0.0f;
	vMat[1][3] = 0.0f;
	vMat[2][3] = 0.0f;
	vMat[3][3] = 1.0f;*/

	//vMat = rotate(vMat, (-cam->direction*(180.0f / 3.1458f)), vec3(0, 1, 0));

	mat4 MVP = pMat * vMat;

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	GLuint MatrixID = glGetUniformLocation(skyShader, "MVP");
	GLuint TextureID = glGetUniformLocation(skyShader, "tex");
	GLuint CountID = glGetUniformLocation(skyShader, "count");

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniform1i(TextureID, 0);
	glUniform1f(CountID, counter);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sky_tex->texName);

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

	glPopMatrix();
	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
}

void Skycube::drawAfter(DrawingState* st){

}