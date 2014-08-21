#include <stdio.h>

class PolygonVertex
{
private:
	int id;
	float size;
	float position[2];
	float color3Float[3];

public:
	PolygonVertex(int id);
	PolygonVertex(int id, float *position, float size=1.0f);

	void drawVertex(float *color3Float);

	void setId(int id);
	int getId();

	void setSize(float size);
	float getSize();

	void setPosition(float *position);
	float* getPosition();

	void setColor3Float(float *color3Float);
	float* getColor3Float();
}
