#include <stdio.h>
#include <vector>

class PolygonVertex
{
private:
	int id;
	float size;
	float *position;
	float *color3Float;

public:
	PolygonVertex(int id, float xPosition=0.0, float yPosition=0.0, float size=10.0);
	PolygonVertex(const PolygonVertex &vertex);
	~PolygonVertex();

	//Drawing methods
	void initialize();
	void drawVertex();
	
	//Get and Set
	void setId(int id);
	int getId();

	void setSize(float size);
	float getSize();

	void setPosition(float xPosition, float yPosition);
	float* getPosition();

	void setColor3Float(float R, float G, float B);
	float* getColor3Float();

	//Common methods
	void deleteObj();
};
