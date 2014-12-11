#pragma once

/// GraphicsTown2006 - CS559 Sample Code
// written by Michael Gleicher - originally written Fall, 2000
// updated Fall, 2005
// updated Fall, 2006 - new texture manager, improvements for behaviors

//
// A set of relatively simple objects to get the ideas across
// these are mainly here as examples - real objects should be more
// interesting 

#include "../GrObject.H"
#include "../DrawingState.H"
#include "glm/glm.hpp"

using std::vector;
using namespace std;
using namespace glm;

// mainly for debugging
class Cube : public GrObject {
public:
  Cube(float x, float y, float z, float s, float r=1, float g=.2, float b=.2);
  Color color;
  float size;
  virtual void draw(DrawingState*);
};

class Hill : public GrObject {
public:
	Hill(float a, float l, float d);
	float angle, length, div;
	bool firstDraw;
	vector<float> hillPointsX;
	vector<float> hillPointsY;
	virtual void draw(DrawingState*);
};

class Tree : public GrObject {
public:
	Tree(float px, float py, float pz);
	float x, y, z;
	virtual void draw(DrawingState*);
};

class Lift : public GrObject{
public:
	Lift(float z);
	GrObject*sup1;
	GrObject*sup2;
};

class Chair : public GrObject {
public:
	Chair(float u);
	//float x, y, z;
	virtual void draw(DrawingState*);
};

class Support : public GrObject {
public:
	Support(float px, float py, float pz, float h, float rp, float rw);
	float x, y, z;
	float supHeight, poleRad, wheelRad;
	std::vector<vec3> va_vertices;
	std::vector<ivec3> va_indices;
	std::vector<vec3> va_normals;
	std::vector<vec2> va_tex_coords;
	virtual void draw(DrawingState*);
};

class Cable : public GrObject {
public:
	Cable(float h, float z, float x1, float x2, float y1, float y2, float wr, float t);
	virtual void draw(DrawingState*);
	vector<vec3> cntlPnts;
	float tension;
};

class TestCubes : public GrObject {
public:
  TestCubes();
  virtual void draw(DrawingState*);
};

// this object uses a shader!
class ShadedCube : public GrObject {
public:
  ShadedCube(float x, float y, float z, float s, float r=1, float g=.2, float b=.2);
  Color color;
  float size;
  virtual void draw(DrawingState*);
};


// $Header: /p/course/cs559-gleicher/private/CVS/GrTown/Examples/Objects.H,v 1.3 2008/11/11 04:18:40 gleicher Exp $


