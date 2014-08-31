#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H

#include "PolygonVertex.h"

float vertexCrossProduct(PolygonVertex leftVertex, PolygonVertex rightVertex, PolygonVertex testVertex);
bool hasVertexPriority(PolygonVertex leftVertex, PolygonVertex rightVertex, PolygonVertex testVertex);

#endif