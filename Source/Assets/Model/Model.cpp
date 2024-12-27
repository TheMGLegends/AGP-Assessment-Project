// turn off fopen warnings
#define _CRT_SECURE_NO_WARNINGS

#include "Model.h"

using namespace DirectX;

// draw object
void Model::Draw()
{
	UINT stride = sizeof(MODEL_POS_COL_TEX_NORM_VERTEX);
	UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	pImmediateContext->Draw(numVerts, 0);
}

// load object from obj file in constructor
Model::Model(char* fname, ID3D11Device* device, ID3D11DeviceContext* context)
{
	pD3DDevice = device;
	pImmediateContext = context;

	if (LoadFile(fname) == 0)
	{
		// file not loaded, check debug output;
		filename="FILE NOT LOADED";
		return;
	}

	filename = fname;

	ParseFile();
	CreateVB();

	delete[] fbuffer; // delete file buffer created in loadfile()
}

// load wavefront object file. adds terminating \n so last line of file can be correctly parsed as a 'line' later
// basic loader - only deals with vertices v, texcoords vt, normals vn 
//              - only copes with triangular meshes (no quads)
//              - doesn't deal with textures or materials
int Model::LoadFile(char* fname)
{
	FILE* pFile;

	pFile = fopen(fname , "r"); // if changed to bin format will read carriage return \r (0d) as well as \n (0a) into fbuffer, may need to add \r checks(but seemed to work with basic test)
	
	if (pFile == NULL) 
	{ 
		OutputDebugString(L"Failed to open model file"); OutputDebugStringA(fname); 
		return 0 ;
	}

	// get file size
	fseek(pFile, 0, SEEK_END);
	fbufferSize = ftell(pFile);
	rewind(pFile);

	// allocate memory for entire file size
	fbuffer  = new char[fbufferSize+1]; // 1 added to cope with adding a \n later in case file doesn't end with \n 

	if (fbuffer == NULL) 
	{
		fclose(pFile); 
		OutputDebugString(L"Failed allocate memory for model file"); 
		OutputDebugStringA(fname); 
		return 0 ;
	}

	// copy file into memory
	actualSize = fread(fbuffer,1,fbufferSize,pFile); // actualSize may be less than fbufferSize in text mode as \r are stripped
	if (actualSize == 0) 
	{
		fclose(pFile); 
		OutputDebugString(L"Failed to read model file"); 
		OutputDebugStringA(fname); 
		return 0 ;
	}

	// add a newline at end in case file does not, so can deal with whole buffer as a set of lines of text
	fbuffer[actualSize] = '\n'; fclose(pFile);

	return 1;
}

// uses concept of getting parsable tokens separated by whitespace and '/'
// one line of file is parsed at a time, lines separated by '\n'
void Model::ParseFile()
{
	tokenPtr=0; // token pointer points to first element of buffer

	int tokenstart, tokenlength;

	xyz tempxyz{};
	xy tempxy{};

	bool success;
	int line = 0;

	do
	{	
		line++; // keep track of current line number for error reporting

		if (!GetNextToken(tokenstart, tokenlength)) continue; // get first token on line, go to next line if first token is \n

		// ADD FURTHER KEYWORDS HERE TO EXTEND CAPABILITIES
		if (strncmp(&fbuffer[tokenstart], "v ", 2) ==0 ) // VERTEX POSITION - note the space in the string is needed (see vt, etc)
		{
			success = true; // used to see if correct number of tokens left on line for this type of attribute
			success = success && GetNextToken(tokenstart, tokenlength);
			tempxyz.x = (float) atof(&fbuffer[tokenstart]);
			success = success && GetNextToken(tokenstart, tokenlength);
			tempxyz.y = (float) atof(&fbuffer[tokenstart]);
			success = success && GetNextToken(tokenstart, tokenlength);
			tempxyz.z = (float) atof(&fbuffer[tokenstart]);

			// if not correct number of tokens, display error in debug output
			if (!success) 
			{
				char s[100] = "ERROR: Badly formatted vertex, line : "; 
				_itoa(line, &s[strlen(s)], 10); 
				strcat(s, " : "); 
				strcat(s, filename.c_str());  
				OutputDebugStringA(s); 
			}

			positionList.push_back(tempxyz); // add a new element to the list
		}
		else if (strncmp(&fbuffer[tokenstart], "vt", 2) == 0) // TEXTURE COORDINATES
		{
			success = true;
			success = success && GetNextToken(tokenstart, tokenlength);
			tempxy.x = (float) atof(&fbuffer[tokenstart]);
			success = success && GetNextToken(tokenstart, tokenlength);
			tempxy.y = (float) atof(&fbuffer[tokenstart]);

			if(!success) 
			{
				char s[100] = "ERROR: Badly formatted texture coordinate, line : "; 
				_itoa(line, &s[strlen(s)], 10); 
				strcat(s, " : "); 
				strcat(s, filename.c_str());  
				OutputDebugStringA(s); 
			}

			texcoordList.push_back(tempxy);
		}
		else if (strncmp(&fbuffer[tokenstart], "vn", 2) == 0)  // NORMALS
		{
			success = true;
			success = success && GetNextToken(tokenstart, tokenlength);
			tempxyz.x = (float) atof(&fbuffer[tokenstart]);
			success = success && GetNextToken(tokenstart, tokenlength);
			tempxyz.y = (float) atof(&fbuffer[tokenstart]);
			success = success && GetNextToken(tokenstart, tokenlength);
			tempxyz.z = (float) atof(&fbuffer[tokenstart]);

			if(!success) 
			{
				char s[100] = "ERROR: Badly formatted normal, line : "; 
				_itoa(line, &s[strlen(s)], 10); 
				strcat(s, " : "); 
				strcat(s, filename.c_str());  
				OutputDebugStringA(s); 
			}

			normalList.push_back(tempxyz);
		}
		else if (strncmp(&fbuffer[tokenstart], "f ", 2)==0)  // FACE - only deals with triangles so far
		{
			int tempptr = tokenstart + 2; // skip "f "
			int forwardslashcount = 0;
			bool adjacentslash = false;

			// this works out how many elements are specified for a face, e.g.
			// f 1 2 3				-> 0 forward slashes = just position
			// f 1/1 2/2 3/3		-> 3 slashes = position and texcoords
			// f 1/1/1 2/2/2 3/3/3	-> 6 slashes = position, texcoords, normals
			// f 1//1 2//2 3//3		-> 6 slashes with adjacent = position, normals
			while (fbuffer[tempptr] != '\n')
			{
				if (fbuffer[tempptr] == '/')
				{
					forwardslashcount++;
					if (fbuffer[tempptr-1] == '/') 
						adjacentslash=true;
				}
				tempptr++;
			}

			success = true;

			// Get 3 sets of indices per face
			for (int i=0; i<3; i++)
			{
				// get vertex index
				success = success && GetNextToken(tokenstart, tokenlength);
				pIndices.push_back(atoi(&fbuffer[tokenstart]));

				if (forwardslashcount >=3 && adjacentslash == false) // get texcoord index if required 
				{
					success = success && GetNextToken(tokenstart, tokenlength);
					tIndices.push_back(atoi(&fbuffer[tokenstart]));
				}

				if (forwardslashcount == 6 || adjacentslash == true) // get normal index if required 
				{
					success = success && GetNextToken(tokenstart, tokenlength);
					nIndices.push_back(atoi(&fbuffer[tokenstart]));
				}
			}

			if(!success) 
			{
				char s[100] = "ERROR: Badly formatted face, line : "; 
				_itoa(line, &s[strlen(s)], 10); 
				strcat(s, " : "); 
				strcat(s, filename.c_str());  
				OutputDebugStringA(s); 
			}
		}
	} 
	while(GetNextLine() == true);
}

// get next token. if \n is next token do not proceed, use getnextline() to resume
bool Model::GetNextToken(int& tokenstart, int& tokenlength)
{
	tokenstart = tokenPtr; 
	tokenlength = 1; 
	int tokenend;

	while (fbuffer[tokenPtr] == ' ' || fbuffer[tokenPtr] == '\t' || fbuffer[tokenPtr] == '/') tokenPtr++; //skip whitespace and '/'

	if (fbuffer[tokenPtr] == '\n') { return false; } // keeps tokenPtr pointing to \n as a token to indicate end of line
													// doesn't point to next token, dealt with in getnextline()
	tokenend = tokenPtr+1;

	while (fbuffer[tokenend] != ' ' && fbuffer[tokenend] != '\t' && fbuffer[tokenend] != '\n' && fbuffer[tokenend] != '/') tokenend++; // find length of token by finding next whitespace or '\n' or '/'

	tokenlength = tokenend - tokenPtr;
	tokenstart = tokenPtr;
	tokenPtr += tokenlength; //ready for next token

	return true;
}

// gets next line of buffer by skipping to next element after end of current line, returns false when end of buffer exceeded
bool Model::GetNextLine()
{
	// relies on getnexttoken()leaving tokenptr pointing to \n if encountered

	while (fbuffer[tokenPtr] != '\n' && tokenPtr < actualSize) tokenPtr++; // skip to end of line

	tokenPtr++; // point to start of next line

	if (tokenPtr >= actualSize) return false;
	else return true;
}

// create Vertex buffer from parsed file data
bool Model::CreateVB()
{
	// create vertex array to pass to vertex buffer from parsed data
	numVerts = static_cast<unsigned int>(pIndices.size());

	vertices = new MODEL_POS_COL_TEX_NORM_VERTEX[numVerts]; // create big enough vertex array

	for (unsigned int i = 0; i< numVerts; i++)
	{
		int vindex = pIndices[i]-1; // use -1 for indices as .obj files indices begin at 1

		// set colour data
		vertices[i].col.x = 1;
		vertices[i].col.y = 1;
		vertices[i].col.z = 1;
		vertices[i].col.w = 1;

		// set position data
		vertices[i].pos.x = positionList[vindex].x;
		vertices[i].pos.y = positionList[vindex].y;
		vertices[i].pos.z = positionList[vindex].z;

		if (tIndices.size() > 0)
		{ 
			// if there are any, set texture coord data
			int tindex = tIndices[i]-1;
			vertices[i].texCoord.x = texcoordList[tindex].x;
			vertices[i].texCoord.y = texcoordList[tindex].y;
		}

		if (nIndices.size() > 0)
		{
			// if there are any, set normal data
			int nindex = nIndices[i]-1;
			vertices[i].normal.x = normalList[nindex].x;
			vertices[i].normal.y = normalList[nindex].y;
			vertices[i].normal.z = normalList[nindex].z;
		}
	}

	// Set up and create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;										// Used by CPU and GPU
	bufferDesc.ByteWidth = sizeof(vertices[0])*numVerts;						// Total size of buffer
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;							// Use as a vertex buffer
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							// Allow CPU access
	HRESULT hr = pD3DDevice->CreateBuffer(&bufferDesc, NULL, &pVertexBuffer);	// Create the buffer

	if (FAILED(hr))
    {
        return false;
    }

	// Copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	pImmediateContext->Map(pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);	// Lock the buffer to allow writing
	memcpy(ms.pData, vertices, sizeof(vertices[0])*numVerts);							// Copy the data
	pImmediateContext->Unmap(pVertexBuffer, NULL);										// Unlock the buffer

	return true;
}

Model::~Model()
{
	// clean up memory used by object
	if(pVertexBuffer) pVertexBuffer->Release();

	delete[] vertices;

	positionList.clear();
	normalList.clear();
	texcoordList.clear();
}
