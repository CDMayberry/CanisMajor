#include"Geometry.h"

Geometry::Geometry()
: numVertices(0), numIndexes(0), md3dDevice(0), mVB(0), mIB(0), usesIndexBuffer(false)
{
}
 
Geometry::~Geometry()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

void Geometry::init(ID3D10Device* device)
{
	md3dDevice = device;
}

void Geometry::draw(D3D_PRIMITIVE_TOPOLOGY topology, UINT offset)
{
	UINT stride = sizeof(Vertex);

	md3dDevice->IASetPrimitiveTopology(topology);
    md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);

	if(usesIndexBuffer)
	{
		md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
		md3dDevice->DrawIndexed(numIndexes, 0, 0);
	}
	else
		md3dDevice->Draw(numVertices,0);
}

void Geometry::initVectorBuffer(Vertex* vertices)
{
	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * numVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));
}

void Geometry::initIndexBuffer(DWORD * indices)
{
	usesIndexBuffer = true;
	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * numIndexes;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}