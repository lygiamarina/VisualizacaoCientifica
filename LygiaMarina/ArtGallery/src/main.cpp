#include "DrawHandler.h"
#include <GL/glut.h>

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	WindowGeneral window("Art Gallery Problem");
	//std::vector<PolygonVertex> vertices;
	DrawHandler drawHandler;

	/*for (int i = 0; i < 5; i++)
	{
		float *position = new float[2];
		position[0] = 50.0+i;
		position[1] = 40.0+i;
		PolygonVertex vertex(i, position);
		vertices.push_back(vertex);
	}*/
	
	drawHandler.setWindow(window);
	//drawHandler.setPolygonVertexVector(vertices);
	drawHandler.initialize();
	drawHandler.draw();

	glutMainLoop();
	return 0;
}
