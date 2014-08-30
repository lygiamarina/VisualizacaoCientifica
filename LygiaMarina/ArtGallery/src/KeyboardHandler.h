#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

#include "Structs.h"
#include "Polygon.h"
#include <string>
#include <vector>

class KeyboardHandler
{
private:
	static KeyboardHandler* currentInstance;

	std::vector<Polygon> &polygons;

	std::string key;
	std::string command;
	std::string lastAction;

public:
	KeyboardHandler(std::vector<Polygon> &inPolygons);

	void handle(unsigned char key, int x, int y);
	static void handleCallback(unsigned char key, int x, int y);
	void handleKeyboard();

	std::string getLastAction();
};

#endif

