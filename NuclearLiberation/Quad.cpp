
#include "Quad.h"


void Quad::init(ID3D10Device* device)
{
	Geometry::init(device);

	// Create vertex buffer

	 Vertex vertices[] =
    {
		{D3DXVECTOR3(-1.0f, 0.0f, 0.0f), WHITE},
		{D3DXVECTOR3(1.0f, 0.0f, 0.0f), WHITE},
		{D3DXVECTOR3(0.0f, 0.0f, -1.0f), WHITE},
		{D3DXVECTOR3(0.0f, 0.0f, 1.0f), WHITE}
    };

	 numVertices = 4;
	
	 initVectorBuffer(vertices);

	//Index buffer
	DWORD indices[] = {
		// front face
		0, 1, 3,
		0, 2, 1

	};

	numIndexes = 6;

	initIndexBuffer(indices);

	
}