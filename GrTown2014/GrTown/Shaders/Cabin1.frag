#version 330

struct Directional{
	vec4 lookFrom;
	vec4 lookTowards;
	vec4 ambientColor;
};

in vec2 uv;
in vec3 normal;
in vec3 sunPos;
in vec3 fragPos;
in mat4 mMat;
in mat4 vMat;
in mat4 pMat;

out vec4 frag_colour;

uniform sampler2D tex;
uniform sampler2D normals;
uniform Directional sun;

void main () 
{
  vec4 text_color = vec4(texture(tex, uv).rgba);
  vec3 normal_vals = vec3(texture(normals, uv).rgb);
  
  vec3 n = normalize(vec3(normal.x*normal_vals.x, normal.y*normal_vals.y, normal.z*normal_vals.z));

  float brightness = max(dot(n, sunPos) + 0.2, 0.0);
  brightness = min(brightness, 1.0);
  
  vec3 pColor = text_color.xyz * brightness;
  
  frag_colour = vec4(pColor, text_color.a);
}