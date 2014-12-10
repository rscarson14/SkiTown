#include "GrObject.H"


class Terrain : public GrObject{
public:
	Terrain(float a, float d, float l);
	float angle, div, length;
	Color color;
	virtual void draw(DrawingState*);
};