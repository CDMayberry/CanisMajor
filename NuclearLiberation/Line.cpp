
#include "Line.h"

void Line::init(ID3D10Device* device)
{

	Geometry::init(device);

	// Create vertex buffer
    Vertex vertices[] =
    {
		{D3DXVECTOR3(0.0f, 0.0f, 0.0f), WHITE},
		{D3DXVECTOR3(1.0f, 0.0f, 0.0f), WHITE}
    };

	numVertices = 2;

	initVectorBuffer(vertices);

}

void Line::draw()
{
	Geometry::draw(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
}
