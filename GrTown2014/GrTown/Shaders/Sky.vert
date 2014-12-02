#version 430 core

void main()
{
	//gl_TexCoord[0].stp = gl_Vertex.xyz;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

