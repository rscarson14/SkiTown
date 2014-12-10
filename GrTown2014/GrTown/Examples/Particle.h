#include "../GrObject.H"
#include "../DrawingState.H"

class Particle : public GrObject {
public:

	Particle(float x, float y, float z, float s);
	virtual void draw(DrawingState*);


}