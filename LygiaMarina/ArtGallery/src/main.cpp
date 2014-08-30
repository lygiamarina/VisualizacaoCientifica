#include "DrawHandler.h"
#include "MouseHandler.h"
#include "KeyboardHandler.h"
#include <GL/glut.h>
#include <iostream>

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	WindowGeneral window("Art Gallery Problem");

	std::vector<Polygon> polygons;

	Polygon firstPolygon;
	polygons.push_back(firstPolygon);

	MouseHandler mouseHandler(polygons, window.getSize().height);
	mouseHandler.setCurrentVertexId(0);

	KeyboardHandler keyboardHandler(polygons);

	DrawHandler drawHandler(polygons);
	
	drawHandler.setWindow(window);
	drawHandler.initialize();
	drawHandler.draw();
	mouseHandler.handleMouse();
	keyboardHandler.handleKeyboard();

	glutMainLoop();
	return 0;
}
