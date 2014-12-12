#version 330

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv;

out vec2 uv;
out vec3 normal;
out vec3 sunPos;
out vec3 fragPos;
out mat4 mMat;
out mat4 vMat;
out mat4 pMat;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 V;
uniform mat4 P;
uniform vec4 sunLookFrom;

void main () 
{
  uv = vertex_uv;
  gl_Position = MVP * vec4(vertex_position, 1.0);
  
  fragPos = normalize(gl_Position.xyz);
  sunPos = vec3(sunLookFrom);
  
  vMat = V;
  pMat = P;
  mat4 mm = mat4(inverse(V) * (MV));
  mMat = mm;
  mat3 it = mat3(transpose(inverse((mm)))); // construct normal matrix
  normal = vec3(it * vec3(vertex_normal));

}