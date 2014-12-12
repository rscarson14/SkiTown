#version 330

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv;

out vec2 uv;
out vec4 normal;
out vec4 eye;
out vec4 coord;
out mat3 mMat;
out mat3 vMat;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 V;

void main () 
{
  uv = vertex_uv;
  gl_Position = MVP * vec4(vertex_position, 1.0);
  coord = MV * vec4(vertex_position, 1.0);
  
  mat3 mMat = inverse(mat3(V)) * mat3(MV);
  mat3 vMat = mat3(V);
  
  mat3 nM = mMat;
  mat3 it = transpose(inverse(nM)); // construct normal matrix
  normal = vec4(it * vec3(vertex_normal),1);
  eye = -vec4(MV * vec4(vertex_position, 1.0));
}