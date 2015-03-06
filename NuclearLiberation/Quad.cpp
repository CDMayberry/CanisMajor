#include "Quad.h"


void Quad::init(ID3D10Device* device, D3DXCOLOR color1, D3DXCOLOR color2)
{
	Geometry::init(device);

	// Create vertex buffer

	 Vertex vertices[] =
    {
		//{D3DXVECTOR3(-20, -20, 11), color1},
		//{D3DXVECTOR3(-20, 10, 11), color2},//top of quad
		//{D3DXVECTOR3(10, 10, 11), color2},//top of quad
		//{D3DXVECTOR3(10, -20, 11), color1}
		{D3DXVECTOR3(0, 0, 11), color1},
		{D3DXVECTOR3(0, 10, 11), color2},//top of quad
		{D3DXVECTOR3(10, 10, 11), color2},//top of quad
		{D3DXVECTOR3(10, 0, 11), color1}
    };

	 numVertices = 4;
	
	 initVectorBuffer(vertices);

	//Index buffer
	DWORD indices[] = {
		// front face
		0, 1, 2,
		0,2,3

	};

	numIndexes = 6;

	initIndexBuffer(indices);

	
}