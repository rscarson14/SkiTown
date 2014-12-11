#version 330

struct Directional{
	vec4 lookFrom;
	vec4 lookTowards;
	vec4 ambientColor;
};

in vec2 uv;
in vec4 normal;
in vec4 eye;

out vec4 frag_colour;

uniform sampler2D tex;
uniform sampler2D normals;
uniform Directional sun;

void main () 
{
  vec4 text_color = vec4(texture(tex, uv).rgba);
  vec3 normal_vals = vec3(texture(normals, uv).rgb);
  
  vec3 l_dir = vec3(sun.lookFrom) - vec3(sun.lookTowards); 
  
  vec3 n = normalize(vec3(normal.x*normal_vals.x, normal.y*normal_vals.y, normal.z*normal_vals.z));
  vec3 e = normalize(eye.xyz);
  vec4 spec = vec4(vec3(0.0), 1.0);
  float intensity = max(dot(n, l_dir), 0.0) + 0.25;
  if(intensity > 0.0){
	vec3 h = normalize(l_dir + e);
	
	//float intSpec = max(dot(h,n), 0.0);
	
  }
  if(intensity < 0.35){
	intensity = 0.35;
  }
  
  frag_colour = text_color * intensity + spec;
  frag_colour.a = text_color.a;
  //frag_colour = text_color;
}