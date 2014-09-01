#ifndef VERTEXSTRUCTS_H
#define VERTEXSTRUCTS_H

#include "PolygonVertex.h"

typedef struct
{
	PolygonVertex vertexA;
	PolygonVertex vertexB;
	PolygonVertex vertexC;
} PolygonTriangle;

typedef struct
{
	PolygonVertex startVertex;
	PolygonVertex endVertex;
} PolygonDiagonal;

#endif