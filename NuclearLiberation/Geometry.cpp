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

void Geometry::init(ID3D10Device* device, D3DXCOLOR color, D3D_PRIMITIVE_TOPOLOGY top)
{
	md3dDevice = device;
	initRasterState();
	this->color = color;
	topology = top;
}

void Geometry::init(ID3D10Device* device, std::string objFile,D3DXCOLOR color)
{
	md3dDevice = device;
	initRasterState();
	this->color = color;

	fstream fin(objFile,std::ios::in);

	vector<Vector3> vertices, faces;

	string l,temp;
	float tx,ty,tz;
	while(getline(fin,l))
	{
		stringstream line;
		line<<l;
		line>>temp;
		if(temp == "v")
		{
			line>>tx>>ty>>tz;
			vertices.push_back(Vector3(tx,ty,tz));
		}
		if(temp == "f")
		{
			line>>tx>>ty>>tz;
			faces.push_back(Vector3(tx-1,ty-1,tz-1)); //obj file has 1 based indexes
		}
	}
	fin.close();

	if(vertices.size()>0)
	{
		numVertices = vertices.size();
		Vertex * verts = new Vertex[numVertices];

		for(int i = 0 ; i < numVertices; i++)
		{
			verts[i].pos = vertices[i];
			verts[i].color = color;
		}

		initVectorBuffer(verts);
		delete verts;
	}
	if(faces.size()>0)
	{
		numIndexes = faces.size()*3;
		DWORD * indexes = new DWORD[numIndexes];

		for(int i = 0 ; i < faces.size(); i++)
		{
			indexes[3*i] = faces[i].x;
			indexes[3*i+1] = faces[i].y;
			indexes[3*i+2] = faces[i].z;
		}

		initIndexBuffer(indexes);
		delete indexes;
	}
}


void Geometry::draw(UINT offset)
{
	UINT stride = sizeof(Vertex);

	md3dDevice->IASetPrimitiveTopology(topology);
    md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	
	md3dDevice->RSSetState(g_pRasterState);

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
    ibd.ByteWidth = sizeof(DWORD)* numIndexes;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}

void Geometry::initRasterState()
{
	rasterState.FrontCounterClockwise = true;
	md3dDevice->CreateRasterizerState(&rasterState,&g_pRasterState);
}