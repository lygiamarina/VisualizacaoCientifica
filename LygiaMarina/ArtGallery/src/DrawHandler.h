#include "PolygonVertex.h"
#include "WindowGeneral.h"
#include <vector>
#include <stdio.h>

class DrawHandler
{
private:
	static DrawHandler* currentInstance;

	WindowGeneral window;
	std::vector<PolygonVertex> vertices;

	char* windowTitle;

public:
	DrawHandler();

	void initialize();
	void drawOrder();
	static void drawCallback();
	void draw();

	//Set and Get
	void setWindow(WindowGeneral window);
	WindowGeneral getWindow();

	void setPolygonVertexVector(std::vector<PolygonVertex> vertices);
	std::vector<PolygonVertex> getPolygonVertexVector();
};