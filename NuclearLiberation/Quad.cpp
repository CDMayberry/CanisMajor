#include "Quad.h"



void Quad::init(ID3D10Device* device, D3DXCOLOR color)
{
	Geometry::init(device,color);

	// Create vertex buffer

	 Vertex vertices[] =
    {
		{D3DXVECTOR3(-0.5f, -0.5f, 0.0f), color},
		{D3DXVECTOR3(0.5f, -0.5f, 0.0f), color},
		{D3DXVECTOR3(0.5f, 0.5f, 0.0f), color},
		{D3DXVECTOR3(-0.5f, 0.5f, 0.0f), color}
    };

	 numVertices = 4;
	
	 initVectorBuffer(vertices);

	//Index buffer
	DWORD indices[] = {

		// front face apparently clockwise
		0, 3, 1,
		1, 3, 2,

	};

	numIndexes = 6;

	initIndexBuffer(indices);

	
}


void Quad::init(ID3D10Device* device, D3DXCOLOR color1,D3DXCOLOR color2)
{
	Geometry::init(device,color);

	// Create vertex buffer

	 Vertex vertices[] =
    {
		{D3DXVECTOR3(-0.5f, -0.5f, 0.0f), color1},
		{D3DXVECTOR3(0.5f, -0.5f, 0.0f), color1},
		{D3DXVECTOR3(0.5f, 0.5f, 0.0f), color2},
		{D3DXVECTOR3(-0.5f, 0.5f, 0.0f), color2}
    };

	 numVertices = 4;
	
	 initVectorBuffer(vertices);

	//Index buffer
	DWORD indices[] = {

		// front face apparently clockwise
		0, 3, 1,
		1, 3, 2,

	};

	numIndexes = 6;

	initIndexBuffer(indices);

	
}