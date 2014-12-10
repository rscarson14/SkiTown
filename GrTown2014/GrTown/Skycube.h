#pragma once
// R Scott Carson

#include "GrObject.H"
#include "DrawingState.H"
#include "FlyCamera.H"

class Texture;

class Skycube : public GrObject{
public:

	Skycube();

	virtual void draw(DrawingState*, GrObject *camera);
	virtual void drawAfter(DrawingState*);

private:

};