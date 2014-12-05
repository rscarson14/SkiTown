#pragma once
// R Scott Carson

#include "GrObject.H"
#include "DrawingState.H"
#include "FlyCamera.H"

class Texture;

class Skycube : public GrObject{
public:
	Texture* top_tex;
	Texture* bottom_tex;
	Texture* left_tex;
	Texture* right_tex;
	Texture* front_tex;
	Texture* back_tex;

	Skycube();

	virtual void draw(DrawingState*, GrObject *camera);
	virtual void drawAfter(DrawingState*);

private:

};