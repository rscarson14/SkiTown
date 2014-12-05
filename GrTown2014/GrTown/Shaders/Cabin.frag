#version 430 core

//in vec2 uv;

out vec4 frag_colour;

//uniform sampler2D tex;

void main () 
{
  //frag_colour = texture2D(tex, uv, 0.0);
  frag_colour = vec4(1.0, 0.1, 0.1, 1.0);
}