#include"Geometry.h"

Geometry::Geometry()
: numVertices(0), numIndexes(0), md3dDevice(0), mVB(0), mIB(0), usesIndexBuffer(false)
{
	indices = nullptr;
	verts = nullptr;
}
 
Geometry::~Geometry()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);

	delete [] verts;
	delete [] indices;
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
	topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	fstream fin(objFile,std::ios::in);
	//string test;

	if(!fin) throw "THERE WASNT A FILE THERE";

	vector<Vector3> vertices, faces, normals;
	vector<Vector2> combos;

	string l,temp;
	float tx,ty,tz;
	float nx,ny,nz;
	while(getline(fin,l))
	{
		stringstream line;
		line<<l;
		line>>temp;
		if(temp == "vn"){
			line >>tx>>ty>>tz;
			normals.push_back(Vector3(tx,ty,tz));
		}
		else if(temp == "v")
		{
			line>>tx>>ty>>tz;
			//_RPT1(0, "position y %f \n", tz);
			vertices.push_back(Vector3(tx,ty,tz));
		}

		else if(temp == "f")
		{
			bool foundInd = false;
			Vector3 index;
			char * token;
			char * dup = strdup(l.c_str());
			strtok(dup,"// ");

			token = strtok(NULL,"// ");//first face value index
			tx = atoi(token);
			token = strtok(NULL,"// ");//first normal index
			nx = atoi(token);

			//Check if the Vertex already exists
			for(int i = 0; i < combos.size(); i++) {
				if(tx == combos.at(i).x && nx == combos.at(i).y) {
					index.x = i; //If it does set the face location to it.
					foundInd = true;
					break;
				}
			}
			if(foundInd == false) { //If it didn't find one at it to the list.
				combos.push_back(Vector2(tx,nx));
				index.x = combos.size()-1;
			}

			foundInd = false;
			token = strtok(NULL,"// ");//second face value index
			ty = atoi(token);
			token = strtok(NULL,"// ");//second normal index
			ny = atoi(token);

			//Check if the Vertex already exists
			for(int i = 0; i < combos.size(); i++) {
				if(ty == combos.at(i).x && ny == combos.at(i).y) {
					index.y = i; //If it does set the face location to it.
					foundInd = true;
					break;
				}
			}
			if(foundInd == false) {
				combos.push_back(Vector2(ty,ny));
				index.y = combos.size()-1;
			}

			foundInd = false;
			token = strtok(NULL,"// ");//thrid face value index
			tz = atoi(token);
			token = strtok(NULL,"// ");//third normal index
			nz = atoi(token);

			//Check if the Vertex already exists
			for(int i = 0; i < combos.size(); i++) {
				if(tz == combos.at(i).x && nz == combos.at(i).y) {
					index.z = i; //If it does set the face location to it.
					foundInd = true;
					break;
				}
			}
			if(foundInd == false) {
				combos.push_back(Vector2(tz,nz));
				index.z = combos.size()-1;
			}

			faces.push_back(index); //obj file has 1 based indexes
		}
	}
	fin.close();

	if(combos.size()>0)
	{
		numVertices = combos.size();
		verts = new Vertex[numVertices];

		for(int i = 0 ; i < numVertices; i++)
		{
			verts[i].pos = vertices.at(combos.at(i).x-1);
			verts[i].normal = normals.at(combos.at(i).y-1);

			//normals must be added in according to faces, down below
			verts[i].diffuse = this->color;
			verts[i].spec = D3DXCOLOR(0.2f, 0.2f, 0.2f, 32.0f);
		}

		initVectorBuffer(verts);
	}
	if(faces.size()>0)
	{
		numIndexes = faces.size()*3;
		indices = new DWORD[numIndexes];

		for(int i = 0 ; i < faces.size(); i++)
		{
			indices[3*i] = faces[i].x;
			indices[3*i+1] = faces[i].y;
			indices[3*i+2] = faces[i].z;
		}

		initIndexBuffer(indices);
	}


	calculateDefaultAABB(vertices);
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

void Geometry::calculateDefaultAABB(vector<Vector3> & verts)
{
	max = min = verts[0];

	for(Vector3& v : verts)
	{
		if(min.x>v.x)min.x=v.x;
		if(min.y>v.y)min.y=v.y;
		if(min.z>v.z)min.z=v.z;

		if(max.x<v.x)max.x=v.x;
		if(max.y<v.y)max.y=v.y;
		if(max.z<v.z)max.z=v.z;
	}
}