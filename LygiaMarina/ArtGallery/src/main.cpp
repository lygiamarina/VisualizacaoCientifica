#include "DrawHandler.h"
#include "MouseHandler.h"
#include <GL/glut.h>

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	WindowGeneral window("Art Gallery Problem");
	std::vector<PolygonVertex> vertices;

	MouseHandler mouseHandler(vertices, window.getSize().height);
	mouseHandler.setCurrentVertexId(0);

	DrawHandler drawHandler(vertices);
	
	drawHandler.setWindow(window);
	drawHandler.initialize();
	drawHandler.draw();
	mouseHandler.handleMouse();

	glutMainLoop();
	return 0;
}
