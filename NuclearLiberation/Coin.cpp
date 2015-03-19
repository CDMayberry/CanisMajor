//=======================================================================================
// Coin.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#include "Coin.h"

void Coin::init(ID3D10Device* device, D3DXCOLOR color)
{
	Geometry::init(device);
	 

	// Create vertex buffer
    Vertex vertices[] =
    {
		{D3DXVECTOR3(0.0f, 0.0f, 0.0f), color},
		{D3DXVECTOR3(0.0f, 0.0f, 0.0f), color},
		{D3DXVECTOR3(0.0f, 0.0f, 0.0f), color},
		{D3DXVECTOR3(0.0f, 0.0f, 0.0f), color},
		{D3DXVECTOR3(0.0f, 0.0f, 0.0f), color},
		{D3DXVECTOR3(0.0f, 0.0f, 0.0f), color},
		{D3DXVECTOR3(0.0f, 0.0f, 0.0f), color},
		{D3DXVECTOR3(0.0f, 0.0f, 0.0f), color},
		{D3DXVECTOR3(0.0f, 0.0f, 0.0f), color},
		{D3DXVECTOR3(0.0f, 0.0f, 0.0f), color},
		{D3DXVECTOR3(0.0f, 0.0f, 0.0f), color},
		{D3DXVECTOR3(0.0f, 0.0f, 0.0f), color},
		{D3DXVECTOR3(0.0f, 0.0f, 0.0f), color},
		{D3DXVECTOR3(0.0f, 0.0f, 0.0f), color},
		{D3DXVECTOR3(0.0f, 0.0f, 0.0f), color},
		{D3DXVECTOR3(0.0f, 0.0f, 0.0f), color},
		{D3DXVECTOR3(0.0f, 0.0f, 0.0f), color},
    };

	for (int i=1;i<=16;i++){
		vertices[i].pos.x = cos((2*PI/16) * i);
		vertices[i].pos.y = sin((2*PI/16)* i);
	}

	numVertices = 17;

	initVectorBuffer(vertices);

	// Create the index buffer
	DWORD indices[] = {
		0,  2,1,
		0, 3, 2,
		0,  4,3,
		0, 5, 4,
		0,  6,5,
		0,  7,6,
		0,  8,7,
		0, 9, 8,
		0,  10,9,
		0,  11,10,
		0,  12,11,
		0,  13,12,
		0,  14,13,
		0,  15,14,
		0,  16,15,
		0, 1, 16,
	};

	numIndexes = 48;

	initIndexBuffer(indices);

}

