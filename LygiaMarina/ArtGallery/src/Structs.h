#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct
{
	float xPosition;
	float yPosition;
} FloatPosition2D;

typedef struct
{
	float xPosition;
	float yPosition;
	float zPosition;
} FloatPosition3D;

typedef struct
{
	int xPosition;
	int yPosition;
} IntPosition2D;

typedef struct
{
	float R;
	float G;
	float B;
} FloatColor3D;

typedef struct
{
	float R;
	float G;
	float B;
	float alpha;
} FloatColor4D;

#endif