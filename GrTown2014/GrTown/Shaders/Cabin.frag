#version 430 core

in vec2 uv;

out vec4 frag_colour;

uniform sampler2D tex;
uniform sampler2D normals;

void main () 
{
  vec4 text_color = vec4(texture(tex, uv).rgb, 1.0);
  vec4 normal_vals = vec4(texture(normals, uv).rgb, 1.0);
  frag_colour = text_color;
  //frag_colour = normal_vals;
  //frag_colour = vec4(uv, 0.1, 1.0);
}