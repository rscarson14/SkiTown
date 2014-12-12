#version 330

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv;

out vec2 uv;
out vec3 normal;
out mat3 mMat;
out mat3 vMat;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 V;

void main () 
{
  uv = vertex_uv;
  gl_Position = MVP * vec4(vertex_position, 1.0);
  
  mat3 nM = mMat;
  mat3 it = transpose(inverse(mat3(MV))); // construct normal matrix
  normal = vec3(it * vec3(vertex_normal));

}