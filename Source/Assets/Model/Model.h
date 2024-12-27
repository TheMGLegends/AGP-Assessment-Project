#pragma once

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <d3d11.h>
#include <DirectXMath.h>
#include <stdio.h>
#include <string>
#include <vector>

/// @brief Model class handles loading of OBJ files and rendering them
class Model
{
private:
	ID3D11Device*           pD3DDevice;
	ID3D11DeviceContext*    pImmediateContext;

//////////////////////////////////////////////////

	int LoadFile(char* fname);

	char* fbuffer;
	long fbufferSize; // file size
	size_t actualSize; // actual size of loaded data (can be less if loading as text files as ASCII CR (0d) are stripped out)

//////////////////////////////////////////////////

	void ParseFile();
	bool GetNextLine() ;
	bool GetNextToken(int& tokenStart, int& tokenLength);

	unsigned int tokenPtr;

//////////////////////////////////////////////////

	bool CreateVB();

	ID3D11Buffer* pVertexBuffer; 

public:
	struct xyz { float x, y, z; };	//used for vertices and normals during file parse
	struct xy { float x, y; };		//used for texture coordinates during file parse

	// Define model vertex structure
	struct MODEL_POS_COL_TEX_NORM_VERTEX
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 col;
		DirectX::XMFLOAT2 texCoord;
		DirectX::XMFLOAT3 normal;
	};

	std::string filename;

	Model(char* filename, ID3D11Device* device, ID3D11DeviceContext* context);
	~Model();

	void Draw();

	std::vector<xyz> positionList;		// list of parsed positions
	std::vector<xyz> normalList;		// list of parsed normals
	std::vector<xy> texcoordList;		// list of parsed texture coordinates
	std::vector<int> pIndices, tIndices, nIndices; // lists of indices into above lists derived from faces

	MODEL_POS_COL_TEX_NORM_VERTEX* vertices;
	unsigned int numVerts;
};

