#include <stdio.h>
#include <stdlib.h>

#include "CStaticMesh.h" // Include interface for class

// Constructor to initialize object to static mesh file
CStaticMesh::CStaticMesh(char filename[1024])
{
	FILE *SMFFile; // Pointer to input file
	SMFHeader h; // Static Mesh File header structure to extract data
	
	SMFFile = fopen(filename, "rb"); // Open Static Mesh File based on given filename
	
	fread(&h, sizeof(struct SMFHeader), 1, SMFFile); // Read in header structure
	
	faceMode = h.faceMode; // Get facemode of current object
	indexCount = h.indexCount; // Get number of elements in respective array
	faceCount = h.fcount;
	if(faceMode == FACE_INDEX) // Execute if mode is FACE_INDEX
	{
		vcount = h.vcount; // Get number of vertices
		v = (struct Vertex3D*)malloc(sizeof(struct Vertex3D) * vcount); // Allocate memory for vertices
		indices = (GLuint*)malloc(sizeof(GLuint) * indexCount); // Allocate memory for indices
		
		fread(v, sizeof(struct Vertex3D), vcount, SMFFile); // Read in vertices into array
		fread(indices, sizeof(GLuint), indexCount, SMFFile); // Read in indices into array
	}
	else if(faceMode == FACE_INDEX_TEX) // Execute if mode is FACE_INDEX_TEX
	{
		iT = (struct InterleavedIndexTex*)malloc(sizeof(struct InterleavedIndexTex) * indexCount); // Allocate memory for interleaved data structure
		
		fread(iT, sizeof(struct InterleavedIndexTex), indexCount, SMFFile); // Read in array of vertices and texture coords
	}
	else if(faceMode == FACE_INDEX_NORM) // Execute if mode is FACE_INDEX_NORM
	{
		iN = (struct InterleavedIndexNorm*)malloc(sizeof(struct InterleavedIndexNorm) * indexCount); // Allocate memory for interleaved data structure
		
		fread(iN, sizeof(struct InterleavedIndexNorm), indexCount, SMFFile); // Read in array of vertices and normals
	}
	else if(faceMode == FACE_INDEX_TEX_NORM) // Execute if mode is FACE_INDEX_TEX_NORM
	{
		iA = (struct InterleavedAll*)malloc(sizeof(struct InterleavedAll) * indexCount); // Allocate memory for interleaved data structure
		
		fread(iA, sizeof(struct InterleavedAll), indexCount, SMFFile); // Read in array of vertices, texture coords and normals
	}
}

// Destructor to free all memory
CStaticMesh::~CStaticMesh(void)
{
	if(faceMode == FACE_INDEX)
	{
		free(v);
		free(indices);
	}
	else if(faceMode == FACE_INDEX_TEX)
	{
		free(iT);
	}
	else if(faceMode == FACE_INDEX_NORM)
	{
		free(iN);
	}
	else if(faceMode == FACE_INDEX_TEX_NORM)
	{
		free(iA);
	}
}
// Function to load texture data
bool CStaticMesh::LoadTextureFile(char filename[1024])
{
	return false;
}

// Function to render stored mesh data
void CStaticMesh::RenderMesh(void)
{
	glEnableClientState(GL_VERTEX_ARRAY); // Enable vertex array processing
	if(faceMode == FACE_INDEX) // Execute if mode is FACE_INDEX
	{
		glEnableClientState(GL_INDEX_ARRAY); // Enable index array processing
		glVertexPointer(3, GL_FLOAT, sizeof(struct Vertex3D), v); // Load vertex array into video memory
		glDrawElements(GL_TRIANGLES, faceCount, GL_UNSIGNED_INT, indices); // Draw vertices based on provided index array
		glDisableClientState(GL_INDEX_ARRAY); // Disable index array processing
	}
	else if(faceMode == FACE_INDEX_TEX) // Execute if mode is FACE_INDEX_TEX
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY); // Enable texture coord array processing
		glVertexPointer(3, GL_FLOAT, sizeof(struct InterleavedIndexTex), &iT[0].v); // Load vertex array into video memory
		glTexCoordPointer(3, GL_FLOAT, sizeof(struct InterleavedIndexTex), &iT[0].t); // Load texture coord array into video memory
		glDrawArrays(GL_TRIANGLES, 0, faceCount); // Draw data in provided arrays as a number of triangles represented by faceCount
		glDisableClientState(GL_TEXTURE_COORD_ARRAY); // Disable texture coord array processing
	}
	else if(faceMode == FACE_INDEX_NORM) // Execute if mode is FACE_INDEX_NORM
	{
		glEnableClientState(GL_NORMAL_ARRAY); // Enable normal array processing
		glVertexPointer(3, GL_FLOAT, sizeof(struct InterleavedIndexNorm), &iN[0].v); // Load vertex array into video memory
		glNormalPointer(GL_FLOAT, sizeof(struct InterleavedIndexNorm), &iN[0].n); // Load normal array into video memory
		glDrawArrays(GL_TRIANGLES, 0, faceCount); // Draw data in provided arrays as a number of triangles represented by faceCount
		glDisableClientState(GL_NORMAL_ARRAY); // Disable normal array processing
	}
	else if(faceMode == FACE_INDEX_TEX_NORM) // Execute if mode is FACE_INDEX_TEX_NORM
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY); // Enable texture coord array processing
		glEnableClientState(GL_NORMAL_ARRAY); // Enable normal array processing
		glVertexPointer(3, GL_FLOAT, sizeof(struct InterleavedAll), &iA[0].v); // Load vertex array into video memory
		glTexCoordPointer(2, GL_FLOAT, sizeof(struct InterleavedAll), &iA[0].t); // Load texture coord array into video memory
		glNormalPointer(GL_FLOAT, sizeof(struct InterleavedAll), &iA[0].n); // Load normal array into video memory
		glDrawArrays(GL_TRIANGLES, 0, faceCount); // Draw data in provided arrays as a number of triangles represented by faceCount
		glDisableClientState(GL_NORMAL_ARRAY); // Disable normal array processing
		glDisableClientState(GL_TEXTURE_COORD_ARRAY); // Disable texture coord array processing
	}
	glDisableClientState(GL_VERTEX_ARRAY); // Disable vertex array processing
}

void CStaticMesh::PrintMeshData(void)
{
	printf("Mesh Info:\nFaces: %d Indices: %d Face Mode: %d\n\n", faceCount, indexCount, faceMode);
	int i;
	for(i = 0; i < indexCount; i++)
	{
		if(faceMode == FACE_INDEX_TEX)
		{
			printf("Vertex: %f %f %f ", iT[i].v.x, iT[i].v.y, iT[i].v.z);
			printf("Texture Coordinate: %f %f %f\n", iT[i].t.u, iT[i].t.v, iT[i].t.w);
		}
	}
}
