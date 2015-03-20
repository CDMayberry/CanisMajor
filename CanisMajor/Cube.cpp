//=======================================================================================
// Box.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#include "Cube.h"

void Cube::init(ID3D10Device* device, D3DXCOLOR color)
{
	Geometry::init(device);
	 

	// Create vertex buffer
    Vertex vertices[] =
    {
		{D3DXVECTOR3(-1.0f, -1.0f, -1.0f), color},
		{D3DXVECTOR3(-1.0f, +1.0f, -1.0f), color},
		{D3DXVECTOR3(+1.0f, +1.0f, -1.0f), color},
		{D3DXVECTOR3(+1.0f, -1.0f, -1.0f), color},
		{D3DXVECTOR3(-1.0f, -1.0f, +1.0f), color},
		{D3DXVECTOR3(-1.0f, +1.0f, +1.0f), color},
		{D3DXVECTOR3(+1.0f, +1.0f, +1.0f), color},
		{D3DXVECTOR3(+1.0f, -1.0f, +1.0f), color},
    };

	numVertices = 8;

	initVectorBuffer(vertices);

	// Create the index buffer
	DWORD indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3, 
		4, 3, 7
	};
	
	numIndexes = 36;

	initIndexBuffer(indices);

}

