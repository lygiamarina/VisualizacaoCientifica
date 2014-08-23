#include <stdio.h>

class PolygonVertex
{
private:
	int id;
	float size;
	float *position;
	float *color3Float;

public:
	PolygonVertex(int id);
	PolygonVertex(int id, float *position, float size=1.0);
	~PolygonVertex();

	void drawVertex(float *color3Float);

	//Get and Set
	void setId(int id);
	int getId();

	void setSize(float size);
	float getSize();

	void setPosition(float *position);
	float* getPosition();

	void setColor3Float(float *color3Float);
	float* getColor3Float();

	//Common methods
	void deleteObj();
};
