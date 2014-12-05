#pragma once
// R Scott Carson

#include "GL/glew.h"
#include "GrObject.H"
#include "DrawingState.H"

class Texture;

class Cabin : public GrObject{
public:
	
	Texture* day_side;
	Texture* night_side;
	Texture* roof;

	Cabin();

	virtual void draw(DrawingState*);
	virtual void drawAfter(DrawingState*);

private:

};