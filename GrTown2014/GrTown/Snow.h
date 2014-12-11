#pragma once

#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GrObject.H"
#include "DrawingState.H"
#include "Behavior.H"

using namespace glm;
using std::vector;

class Snow : public GrObject{
public:

	Snow(uint numFlakes);

	vector<vec3> snowPos;

	virtual void draw(DrawingState*);

};


class SnowFall : public Behavior{
public:
	SnowFall(GrObject* owner, float rate);

	float rate;

	virtual void simulateUntil(unsigned long);
};
