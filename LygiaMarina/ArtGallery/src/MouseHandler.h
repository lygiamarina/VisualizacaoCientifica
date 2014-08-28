#ifndef MOUSEHANDLER_H
#define MOUSEHANDLER_H

#include "Structs.h"
#include "PolygonVertex.h"
#include <string>
#include <vector>

class MouseHandler
{
private:
	static MouseHandler* currentInstance;

	std::vector<PolygonVertex> &vertices;
	int currentVertexId;

	IntPosition2D mousePosition;
	std::string keyName;
	std::string command;
	std::string lastAction;
	int windowHeight;

public:
	MouseHandler(std::vector<PolygonVertex> &inVertices, int height);

	void handle(int button, int state, int x, int y);
	static void handleCallback(int button, int state, int x, int y);
	void handleMouse();

	IntPosition2D getMousePosition();
	void setMousePosition(int xPosition, int yPosition);

	void setCurrentVertexId(int id);

	std::string getLastAction();
};

#endif

