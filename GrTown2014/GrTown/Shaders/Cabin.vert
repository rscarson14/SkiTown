#version 430 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec3 vertex_normal;

out vec2 uv;
out vec4 normal;
out vec4 eye;

uniform mat4 MVP;

void main () 
{
  uv = vertex_uv;
  gl_Position = MVP * vec4(vertex_position, 1.0);
  normal = vec4(vertex_normal, 1.0);
  //normal = normalize(MVP * vec4(vertex_normal, 1.0));
  eye = -gl_Position;
  
}