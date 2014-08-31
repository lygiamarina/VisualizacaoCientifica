#include <iostream>
#include <vector>

//Importing our header files
#include "polygon.h"

using namespace std;

std::vector<Vertex*> polygon;
bool canAddVertex = true;

Vertex::Vertex (int x, int y)
{
	vertX = x;
	vertY = y;
}

