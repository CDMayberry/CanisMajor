
#include "Line.h"

void Line::init(ID3D10Device* device, D3DXCOLOR color)
{

	Geometry::init(device,color,D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	// Create vertex buffer
    Vertex vertices[] =
    {
		{D3DXVECTOR3(0.0f, 0.0f, 0.0f), color},
		{D3DXVECTOR3(1.0f, 0.0f, 0.0f), color}
    };

	numVertices = 2;

	initVectorBuffer(vertices);

}

void Line::draw()
{
	Geometry::draw(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
}
