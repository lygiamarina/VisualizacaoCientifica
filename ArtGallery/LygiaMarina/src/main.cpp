#include <GL/glut.h>


void draw()
{
	//Background color
	glClearColor(0.95,0.95,0.95,0.95);
	glClear(GL_COLOR_BUFFER_BIT);

	//Draw order
	glFlush();
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	//Simple buffer
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
	glutInitWindowPosition(50,25);
	glutInitWindowSize(600,480);
	glutCreateWindow("Art gallery problem");

	//Call to the drawing function
	glutDisplayFunc(draw);
	glutMainLoop();
	return 0;
}
