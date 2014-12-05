#version 430 core

layout(location = 0) in vec3 vertex_position;
//layout(location = 1) in vec2 vertex_colour;

//out vec2 uv;

void main () 
{
  //uv = vertex_colour;
  gl_Position = vec4(vertex_position, 1.0);
  //gl_Position = ftransform();
}