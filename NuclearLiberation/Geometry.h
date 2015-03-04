#pragma once

#include "constants.h"
#include "d3dUtil.h"
#include "Vertex.h"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using std::fstream;
using std::stringstream;
using std::string;
using std::vector;

class Geometry{
public:

	Geometry();
	~Geometry();

	virtual void init(ID3D10Device* device, D3DXCOLOR color = WHITE, D3D_PRIMITIVE_TOPOLOGY topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	virtual void init(ID3D10Device* device, std::string objFile,D3DXCOLOR color = WHITE);
	virtual void draw(UINT offset = 0);

protected:

	D3DXCOLOR color;

	void initVectorBuffer(Vertex* vertices);
	void initIndexBuffer(DWORD * indices);
	void initRasterState();

	DWORD numVertices;
	DWORD numIndexes;

	D3D10_RASTERIZER_DESC rasterState;

private:
	D3D_PRIMITIVE_TOPOLOGY topology;
	ID3D10RasterizerState * g_pRasterState;
	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
	bool usesIndexBuffer;
};