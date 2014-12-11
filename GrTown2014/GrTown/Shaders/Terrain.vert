#version 330

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv;

out vec2 uv;
out vec3 normal;
out vec3 eye;

uniform mat4 MVP;
uniform mat4 MV;

void main () 
{
  uv = vertex_uv;
  gl_Position = MVP * vec4(vertex_position, 1.0);
  
  mat3 nMVP = mat3(MVP[0], MVP[1], MVP[2]);
  mat3 it = inverse(transpose(nMVP)); // construct normal matrix
  normal = it * vec3(vertex_normal);
  eye = -vec3(MVP * vec4(vertex_position, 1.0));
}