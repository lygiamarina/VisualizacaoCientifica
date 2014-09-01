#include "Polygon.h"
#include "CommonFunctions.h"
#include <GL/glut.h>
#include <iostream>
#include <iterator>
#include <algorithm>

Polygon::Polygon()
{
	this->closedPolygon = false;
	this->triangulatedPolygon = false;
	this->verticesPaintedPolygon = false;
}

void Polygon::triangulate()
{
	if (this->closedPolygon && !this->triangulatedPolygon)
	{
		this->setCounterClockwise();
		std::vector<PolygonVertex> inVertices(this->vertices);
		inVertices.erase(inVertices.end());
		this->diagonals = this->triangulateByDiagonals(inVertices);
		this->triangulatedPolygon = true;
		std::cout << "Triangles: " << this->triangles.size() << std::endl;
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
	probDiagonal.startVertex = rightAdjacent;
	probDiagonal.endVertex = leftAdjacent;

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

			probDiagonal.startVertex = currentVertex;
			probDiagonal.endVertex = orderedXVertices[i];
			diagonals.push_back(probDiagonal);

			it = std::find(vertices.begin(), vertices.end(), orderedXVertices[i]);
			posInVector = std::distance(vertices.begin(), it);

			std::vector<PolygonVertex> leftVertices(std::min(it, itCurrentVertex), std::max(it, itCurrentVertex)+1);

			std::vector<PolygonVertex> rightVertices;
			rightVertices.insert(rightVertices.end(), vertices.begin(), std::min(it, itCurrentVertex)+1);
			rightVertices.insert(rightVertices.end(), std::max(it, itCurrentVertex), vertices.end());

			if (rightVertices.size() > 3)
			{
				std::vector<PolygonDiagonal> second = this->triangulateByDiagonals(rightVertices);
				diagonals.insert(diagonals.end(), second.begin(), second.end());
			}
			else
			{
				PolygonTriangle newTriangle;
				newTriangle.vertexA = rightVertices[0];
				newTriangle.vertexB = rightVertices[1];
				newTriangle.vertexC = rightVertices[2];
				this->triangles.push_back(newTriangle);
			}

			if (leftVertices.size() > 3)
			{
				std::vector<PolygonDiagonal> first = this->triangulateByDiagonals(leftVertices);
				diagonals.insert(diagonals.end(), first.begin(), first.end());
			}
			else
			{
				PolygonTriangle newTriangle;
				newTriangle.vertexA = leftVertices[0];
				newTriangle.vertexB = leftVertices[1];
				newTriangle.vertexC = leftVertices[2];
				this->triangles.push_back(newTriangle);
			}
			
			return diagonals;
		}
	}

	std::cout << "Proper diagonal" << std::endl;
	diagonals.push_back(probDiagonal);

	std::cout << "Erase currentVertex" << std::endl;
	it = std::find(vertices.begin(), vertices.end(), currentVertex);
	posInVector = std::distance(vertices.begin(), it);

	PolygonTriangle newTriangle;
	newTriangle.vertexA = vertices[posInVector];
	vertices.erase(it);

	it = std::find(vertices.begin(), vertices.end(), rightAdjacent);
	posInVector = std::distance(vertices.begin(), it);

	newTriangle.vertexB = vertices[posInVector];

	it = std::find(vertices.begin(), vertices.end(), leftAdjacent);
	posInVector = std::distance(vertices.begin(), it);

	newTriangle.vertexC = vertices[posInVector];

	this->triangles.push_back(newTriangle);

	if (vertices.size() > 3)
	{
		std::vector<PolygonDiagonal> resultPolygon = this->triangulateByDiagonals(vertices);
		diagonals.insert(diagonals.end(), resultPolygon.begin(), resultPolygon.end());
	}
	else
	{
		PolygonTriangle newTriangle;
		newTriangle.vertexA = vertices[0];
		newTriangle.vertexB = vertices[1];
		newTriangle.vertexC = vertices[2];
		this->triangles.push_back(newTriangle);
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

void Polygon::colorVertices()
{
	if(this->triangulatedPolygon && !this->verticesPaintedPolygon)
	{
		this->paintVertices();
		this->verticesPaintedPolygon = true;
	}
}

void Polygon::paintVertices()
{
	std::vector<PolygonVertex>::iterator it;
	int posInVector;

	FloatColor3D colorToPaint;
	std::string flagColor;

	FloatColor3D green;
	green.R = 0.0;
	green.G = 1.0;
	green.B = 0.0;

	FloatColor3D yellow;
	yellow.R = 1.0;
	yellow.G = 1.0;
	yellow.B = 0.0;

	FloatColor3D pink;
	pink.R = 1.0;
	pink.G = 0.0;
	pink.B = 1.0;
		
	std::cout << "Coloring initial vertices" << std::endl;	

	it = std::find(this->vertices.begin(), this->vertices.end()-1, this->triangles[0].vertexA);
	posInVector = std::distance(this->vertices.begin(), it);

	std::cout << "Coloring VertexA" << std::endl;
	this->vertices[posInVector].setColor3Float(green);
	this->vertices[posInVector].setFlagColor("green");

	it = std::find(this->vertices.begin(), this->vertices.end()-1, this->triangles[0].vertexB);
	posInVector = std::distance(this->vertices.begin(), it);
	this->vertices[posInVector].setColor3Float(yellow);
	this->vertices[posInVector].setFlagColor("yellow");

	it = std::find(this->vertices.begin(), this->vertices.end()-1, this->triangles[0].vertexC);
	posInVector = std::distance(this->vertices.begin(), it);
	this->vertices[posInVector].setColor3Float(pink);
	this->vertices[posInVector].setFlagColor("pink");

	std::cout << "Coloring rest of polygon" << std::endl;
	for(int i = 1; i < this->triangles.size(); i++)
	{
		std::cout << "Coloring a triangle" << std::endl;

		bool hasGreen = false;
		bool hasYellow = false;
		bool hasPink = false;
		int itHasDefault = 0;

		std::string colors[3];

		it = std::find(this->vertices.begin(), this->vertices.end()-1, this->triangles[i].vertexA);
		int posInVectorA = std::distance(this->vertices.begin(), it);

		colors[0] = this->vertices[posInVectorA].getFlagColor();
		std::cout << colors[0] << std::endl;

		it = std::find(this->vertices.begin(), this->vertices.end()-1, this->triangles[i].vertexB);
		int posInVectorB = std::distance(this->vertices.begin(), it);

		colors[1] = this->vertices[posInVectorB].getFlagColor();
		std::cout << colors[1] << std::endl;

		it = std::find(this->vertices.begin(), this->vertices.end()-1, this->triangles[i].vertexC);
		int posInVectorC = std::distance(this->vertices.begin(), it);

		colors[2] = this->vertices[posInVectorC].getFlagColor();
		std::cout << colors[2] << std::endl;

		for (int j = 0; j < 3; j++)
		{
			if (colors[j] == "green")
			{
				hasGreen = true;
				if(hasYellow) 
				{
					colorToPaint = pink; 
					flagColor = "pink";
				}
				else if(hasPink) 
				{
					colorToPaint = yellow;
					flagColor = "yellow";
				}
			}	
			else if (colors[j] == "yellow")
			{
				hasYellow = true;
				if(hasGreen) 
				{
					colorToPaint = pink;
					flagColor = "pink";
				}
				else if(hasPink) 
				{ 
					colorToPaint = green;
					flagColor = "green"; 
				}	
			}
			else if (colors[j] == "pink")
			{
				hasPink = true;
				if(hasYellow) 
				{
					colorToPaint = green; 
					flagColor = "green";
				}
				else if(hasGreen) 
				{ 
					colorToPaint = yellow; 
					flagColor = "yellow";
				}
			}
			else if (colors[j] == "default")
			{
				itHasDefault = j;
			}
		}

		switch(itHasDefault)
		{
			case 0:
				this->vertices[posInVectorA].setFlagColor(flagColor);
				this->vertices[posInVectorA].setColor3Float(colorToPaint);
				break;
			case 1:
				this->vertices[posInVectorB].setFlagColor(flagColor);
				this->vertices[posInVectorB].setColor3Float(colorToPaint);
				break;
			case 2:
				this->vertices[posInVectorC].setFlagColor(flagColor);
				this->vertices[posInVectorC].setColor3Float(colorToPaint);
				break;
		}
	}

	this->vertices[this->vertices.size()-1].setColor3Float(this->vertices[0].getColor3Float());
	this->vertices[this->vertices.size()-1].setFlagColor(this->vertices[0].getFlagColor());
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
		glVertex2f(this->diagonals[i].startVertex.getPosition().xPosition, 
						this->diagonals[i].startVertex.getPosition().yPosition);
		glVertex2f(this->diagonals[i].endVertex.getPosition().xPosition, 
						this->diagonals[i].endVertex.getPosition().yPosition);
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

void Polygon::setTriangulatedPolygon(bool state)
{
	this->triangulatedPolygon = state;
}

void Polygon::setVerticesPaintedPolygon(bool state)
{
	this->verticesPaintedPolygon = state;
}