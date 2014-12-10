#version 430 core

in vec2 uv;

out vec4 frag_colour;

uniform sampler2D tex;
uniform float count;

void main () 
{
	vec2 newUV = vec2(uv.x + count, uv.y);
	if(newUV.x > 1.0){
		newUV.x = newUV.x - 1.0;
	}
  vec4 text_color = vec4(texture(tex, newUV).rgb, 1.0);
  frag_colour = text_color;
}