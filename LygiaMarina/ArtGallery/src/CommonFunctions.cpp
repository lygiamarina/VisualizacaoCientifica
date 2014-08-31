#include "CommonFunctions.h"
#include "Structs.h"
#include <algorithm>

float vertexCrossProduct(PolygonVertex leftVertex, PolygonVertex rightVertex, PolygonVertex testVertex)
{
	FloatPosition2D diagonalVector2D;
	diagonalVector2D.xPosition = leftVertex.getPosition().xPosition - 
									rightVertex.getPosition().xPosition;
	diagonalVector2D.yPosition = leftVertex.getPosition().yPosition - 
									rightVertex.getPosition().yPosition;

	FloatPosition2D vectorOfInterest;
	vectorOfInterest.xPosition = testVertex.getPosition().xPosition - 
									rightVertex.getPosition().xPosition;
	vectorOfInterest.yPosition = testVertex.getPosition().yPosition - 
									rightVertex.getPosition().yPosition;

	float crossProduct2D = (vectorOfInterest.xPosition*diagonalVector2D.yPosition) - 
							(vectorOfInterest.yPosition*diagonalVector2D.xPosition);

	return crossProduct2D;
}

bool hasVertexPriority(PolygonVertex leftVertex, PolygonVertex rightVertex, PolygonVertex testVertex)
{
	float crossProduct = vertexCrossProduct(leftVertex, rightVertex, testVertex);

	bool hasPriority = crossProduct < 0;

	hasPriority = hasPriority && 
				testVertex.getPosition().xPosition < std::max(rightVertex.getPosition().xPosition, 
																leftVertex.getPosition().xPosition);

	hasPriority = hasPriority && 
				testVertex.getPosition().xPosition > std::min(rightVertex.getPosition().xPosition, 
																leftVertex.getPosition().xPosition);

	return hasPriority;
}