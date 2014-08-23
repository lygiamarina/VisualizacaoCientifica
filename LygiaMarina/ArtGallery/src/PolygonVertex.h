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
	PolygonVertex(int id, float *position, float size=5.0);
	~PolygonVertex();

	//Drawing methods
	void initialize();
	void drawVertex();
	
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
