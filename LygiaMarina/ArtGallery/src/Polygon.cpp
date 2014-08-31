#include "Polygon.h"
#include "CommonFunctions.h"
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
		Polygon::setCounterClockwise();
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

	std::vector<PolygonDiagonal> diagonals;

	PolygonVertex currentVertex = orderedXVertices.front();

	std::vector<PolygonVertex>::iterator itCurrentVertex;
	std::vector<PolygonVertex>::iterator it;
	itCurrentVertex = std::find(vertices.begin(), vertices.end(), currentVertex);
	int posInVector = std::distance(vertices.begin(), itCurrentVertex);

	std::cout << "Find adjacent vertex" << std::endl;
	PolygonVertex leftAdjacent;
	PolygonVertex rightAdjacent;

	if(posInVector+1 > vertices.size()-1) { rightAdjacent = vertices.front(); }
	else { rightAdjacent = vertices[posInVector+1]; }

	if(posInVector - 1 < 0) { leftAdjacent = vertices.back(); }
	else { leftAdjacent = vertices[posInVector - 1]; }

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

		bool hasPriorityVertex = hasVertexPriority(leftAdjacent, 
													rightAdjacent, 
													orderedXVertices[i]);

		if(notInterestingVertex && hasPriorityVertex)
		{
			std::cout << "Not a proper diagonal; other vertex has priority" << std::endl;

			probDiagonal.startVertex = currentVertex.getPosition();
			probDiagonal.endVertex = orderedXVertices[i].getPosition();
			diagonals.push_back(probDiagonal);

			it = std::find(vertices.begin(), vertices.end(), orderedXVertices[i]);
			posInVector = std::distance(vertices.begin(), it);

			std::vector<PolygonVertex> leftVertices(std::min(it, itCurrentVertex), std::max(it, itCurrentVertex)+1);

			std::vector<PolygonVertex> rightVertices;
			rightVertices.insert(rightVertices.end(), vertices.begin(), std::min(it, itCurrentVertex)+1);
			rightVertices.insert(rightVertices.end(), std::max(it, itCurrentVertex), vertices.end());

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

void Polygon::setCounterClockwise()
{
	int sumOverEdges = 0;
	for(int i = 1; i < this->vertices.size(); i++)
	{
		int xComponent = vertices[i].getPosition().xPosition - vertices[i-1].getPosition().xPosition;
		int yComponent = vertices[i].getPosition().yPosition + vertices[i-1].getPosition().yPosition;
		sumOverEdges += xComponent * yComponent;
	}

	if(sumOverEdges > 0)
	{
		std::reverse(vertices.begin(), vertices.end());
	}
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