#include "PolygonVertex.h"
#include "WindowGeneral.h"
#include <GL/glut.h>

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	WindowGeneral window("Art Gallery Problem");
	window.initialize();
	window.drawDisplay();

	glutMainLoop();
	return 0;
}
