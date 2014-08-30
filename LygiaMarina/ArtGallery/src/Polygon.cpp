#include "Polygon.h"
#include <GL/glut.h>
#include <iostream>
#include <iterator>
#include <algorithm>

Polygon::Polygon()
{
	this->closedPolygon = false;
}

void Polygon::triangulate()
{
	if (this->closedPolygon)
	{
		std::vector<PolygonVertex> inVertices(this->vertices);
		inVertices.erase(inVertices.end());
		this->diagonals = Polygon::triangulateByDiagonals(inVertices);
	}
}

std::vector<PolygonDiagonal> Polygon::triangulateByDiagonals(std::vector<PolygonVertex> inVertices)
{
	std::cout << "Begin of Triangulate" << std::endl;

	std::vector<PolygonVertex> vertices(inVertices);
	std::vector<PolygonVertex> orderedXVertices(inVertices);
	std::sort(orderedXVertices.begin(), orderedXVertices.end());

	std::cout << "Ordered size: " << orderedXVertices.size() << std::endl;
	std::cout << "InVertices size: " << vertices.size() << std::endl;

	std::vector<PolygonDiagonal> diagonals;

	std::cout << "Get currentVertex" << std::endl;
	PolygonVertex currentVertex = orderedXVertices.front();
	std::cout << "currentVertex x: " << currentVertex.getPosition().xPosition << std::endl;
	std::cout << "currentVertex y: " << currentVertex.getPosition().yPosition << std::endl;

	std::cout << "Find currentVertex position" << std::endl;
	std::vector<PolygonVertex>::iterator it;
	it = std::find(vertices.begin(), vertices.end(), currentVertex);
	int posInVector = std::distance(vertices.begin(), it);

	std::cout << "Find adjacent vertex" << std::endl;
	PolygonVertex leftAdjacent;
	PolygonVertex rightAdjacent;

	if (posInVector+1 > vertices.size()-1)
	{
		rightAdjacent = vertices.front();
	}
	else
	{
		rightAdjacent = vertices[posInVector+1];
	}

	if (posInVector - 1 < 0)
	{
		leftAdjacent = vertices.back();
	}
	else
	{
		leftAdjacent = vertices[posInVector - 1];
	}

	std::cout << "leftAdjacent x: " << leftAdjacent.getPosition().xPosition << std::endl;
	std::cout << "leftAdjacent y: " << leftAdjacent.getPosition().yPosition << std::endl;

	std::cout << "rightAdjacent x: " << rightAdjacent.getPosition().xPosition << std::endl;
	std::cout << "rightAdjacent y: " << rightAdjacent.getPosition().yPosition << std::endl;

	PolygonDiagonal probDiagonal;
	probDiagonal.startVertex = rightAdjacent.getPosition();
	probDiagonal.endVertex = leftAdjacent.getPosition();

	it = std::find(orderedXVertices.begin(), orderedXVertices.end(), rightAdjacent);
	int nTilRightAdjacent = std::distance(orderedXVertices.begin(), it);

	it = std::find(orderedXVertices.begin(), orderedXVertices.end(), leftAdjacent);
	int nTillLeftAdjacent = std::distance(orderedXVertices.begin(), it);

	int nTroubbleVertices = std::max(nTillLeftAdjacent, nTilRightAdjacent);

	std::cout << "Begin of Find Diagonals" << std::endl;

	for(int i = 0; i < nTroubbleVertices; i++)
	{
		bool notInterestingVertex = 
			(orderedXVertices[i] != leftAdjacent) && (orderedXVertices[i] != currentVertex) &&
			(orderedXVertices[i] != rightAdjacent);

		bool hasPriorityVertex = 
			orderedXVertices[i].getPosition().xPosition < rightAdjacent.getPosition().xPosition;

		hasPriorityVertex = hasPriorityVertex &&
			orderedXVertices[i].getPosition().xPosition > leftAdjacent.getPosition().xPosition;

		if(notInterestingVertex && hasPriorityVertex)
		{
			std::cout << "Not a proper diagonal; other vertex has priority" << std::endl;

			probDiagonal.startVertex = currentVertex.getPosition();
			probDiagonal.endVertex = orderedXVertices[i].getPosition();
			diagonals.push_back(probDiagonal);

			it = std::find(vertices.begin(), vertices.end(), orderedXVertices[i]);
			posInVector = std::distance(vertices.begin(), it);

			std::cout << "Divided polygon" << std::endl;
			std::vector<PolygonVertex> leftVertices(vertices.begin(), it);
			std::vector<PolygonVertex> rightVertices(it, vertices.end());

			std::cout << "leftVertices size: " << leftVertices.size() << std::endl;
			std::cout << "rightVertices size: " << rightVertices.size() << std::endl;

			rightVertices.push_back(currentVertex);

			if (leftVertices.size() > 3)
			{
				std::vector<PolygonDiagonal> first = Polygon::triangulateByDiagonals(leftVertices);
				diagonals.insert(diagonals.end(), first.begin(), first.end());
			}
			if (rightVertices.size() > 3)
			{
				std::vector<PolygonDiagonal> second = Polygon::triangulateByDiagonals(rightVertices);
				diagonals.insert(diagonals.end(), second.begin(), second.end());
			}
			return diagonals;
		}
	}

	std::cout << "Proper diagonal" << std::endl;
	diagonals.push_back(probDiagonal);

	std::cout << "Erase currentVertex" << std::endl;
	it = std::find(vertices.begin(), vertices.end(), currentVertex);
	posInVector = std::distance(vertices.begin(), it);
	vertices.erase(it);

	if (vertices.size() > 3)
	{
		std::vector<PolygonDiagonal> resultPolygon = Polygon::triangulateByDiagonals(vertices);
		diagonals.insert(diagonals.end(), resultPolygon.begin(), resultPolygon.end());
	}
	return diagonals;
}

void Polygon::initialize()
{
	this->vertices[0].initialize();
}

void Polygon::drawPolygon()
{
	std::cout << "Vertices: " << this->vertices.size() << std::endl;
	for(int i = 0; i < this->vertices.size(); i++)
	{
		if (i > 0)
		{
			glLineWidth(2.5);
			glColor3f(1.0, 0.0, 0.0);

			glBegin(GL_LINES);
			glVertex2f(this->vertices[i-1].getPosition().xPosition, this->vertices[i-1].getPosition().yPosition);
			glVertex2f(this->vertices[i].getPosition().xPosition, this->vertices[i].getPosition().yPosition);
			glEnd();
		}

		this->vertices[i].drawVertex();
	}
}

void Polygon::drawDiagonals()
{
	for(int i = 0; i < this->diagonals.size(); i++)
	{
		glLineWidth(2.5);
		glColor3f(0.0, 0.0, 1.0);

		glBegin(GL_LINES);
		glVertex2f(this->diagonals[i].startVertex.xPosition, this->diagonals[i].startVertex.yPosition);
		glVertex2f(this->diagonals[i].endVertex.xPosition, this->diagonals[i].endVertex.yPosition);
		glEnd();
	}
}

std::vector<PolygonVertex> &Polygon::getVertices()
{
	return this->vertices;
}

std::vector<PolygonDiagonal> Polygon::getDiagonals()
{
	return this->diagonals;
}

void Polygon::setClosedPolygon(bool state)
{
	this->closedPolygon = state;
}