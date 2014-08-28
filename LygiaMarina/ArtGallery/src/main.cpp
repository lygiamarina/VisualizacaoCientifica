#include "DrawHandler.h"
#include "MouseHandler.h"
#include <GL/glut.h>

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	WindowGeneral window("Art Gallery Problem");
	std::vector<PolygonVertex> vertices;
	MouseHandler mouseHandler;
	DrawHandler drawHandler;

	for (int i = 0; i < 5; i++)
	{
		PolygonVertex vertex(i, 50.0+i*50.0, 25.0+i*50.0, 10.0);
		vertices.push_back(vertex);
	}
	
	drawHandler.setWindow(window);
	drawHandler.setPolygonVertexVector(vertices);
	drawHandler.initialize();
	drawHandler.draw();
	mouseHandler.handleMouse();

	glutMainLoop();
	return 0;
}
