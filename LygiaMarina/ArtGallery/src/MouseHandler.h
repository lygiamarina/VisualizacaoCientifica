#include "Structs.h"
#include <string>

class MouseHandler
{
private:
	static MouseHandler* currentInstance;

	IntPosition2D mousePosition;
	std::string keyName;
	std::string command;
	std::string lastAction;

public:
	MouseHandler();

	IntPosition2D getMousePosition();
	void setMousePosition(int xPosition, int yPosition);
	std::string getLastAction();
	void handle(int button, int state, int x, int y);
	static void handleCallback(int button, int state, int x, int y);
	void handleMouse();

};

