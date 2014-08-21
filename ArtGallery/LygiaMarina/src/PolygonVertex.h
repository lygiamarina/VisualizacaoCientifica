#include <stdio.h>
#include <GL/glut.h>

class PolygonVertex
{
private:
	int id;
	float size;
	pair<float, float> position;
	tuple<float, float, float> color3Float;

public:
	PolygonVertex(int id);
	PolygonVertex(int id, pair <float, float> position, float size=1.0f);

	void drawVertexWithoutColor();
	void drawVertexWithColor();

	void setId(int id);
	int getId();

	void setSize(float size);
	float getSize();

	void setPosition(pair <float, float> position);
	pair<float, float> getPosition();

	void setColor3Float(tuple <float, float, float> color3Float);
	tuple<float, float, float> getColor3Float();
}
