#include <stdio.h>
#include <stdlib.h>

#include "CStaticMesh.h" // Include interface for class

// Constructor to initialize object to static mesh file
CStaticMesh::CStaticMesh(char filename[1024])
{
	FILE *smf; // Pointer to SMF file
	SMFHeader *h; // Pointer to structure to hold file header
	
	smf = fopen(filename, "rb"); // Open SMF file
	h = (SMFHeader*)malloc(sizeof(SMFHeader)); // Allocate memory for header structure
	
	if(smf == NULL) // Check if SMF file exists
	{
		printf("The specified OBJ file, %s, does not exist!\n", filename); // Print error
		return; // Exit
	}
	
	fread(h, sizeof(SMFHeader), 1, smf); // Read in file header
	
	// Save data in class variables
	faceMode = h->faceMode;
	indexCount = h->indexCount;
	faceCount = h->fcount;
	
	if(faceMode == FACE_INDEX) // If face mode is only indices
	{
		vcount = h->vcount; // Store vertex count in local variable
		
		v = (Vertex3D*)malloc(sizeof(Vertex3D) * vcount); // Allocate memory for vertex array
		indices = (GLuint*)malloc(sizeof(GLuint) * indexCount); // Allocate memory for index array
		
		fread(v, sizeof(Vertex3D) * vcount, 1, smf); // Read in vertex data
		fread(indices, sizeof(GLuint) * indexCount, 1, smf); // Read in indices
	}
	else if(faceMode == FACE_INDEX_TEX) // If face mode is indices and texture coordinates
	{
		iT = (InterleavedIndexTex*)malloc(sizeof(InterleavedIndexTex) * indexCount); // Allocate memory for interleaved array
		
		fread(iT, sizeof(InterleavedIndexTex) * indexCount, 1, smf); // Read in interleaved data
	}
	else if(faceMode == FACE_INDEX_TEX_NORM) // If face mode is indices, texture coordinates and normals
	{
		iA = (InterleavedAll*)malloc(sizeof(InterleavedAll) * indexCount); // Allocate memory for interleaved array
		
		fread(iA, sizeof(InterleavedAll) * indexCount, 1, smf); // Read in interleaved data
	}
	else if(faceMode == FACE_INDEX_NORM) // If face mode is indices and normals
	{
		iN = (InterleavedIndexNorm*)malloc(sizeof(InterleavedIndexNorm) * indexCount); // Allocate memory for interleaved array
		
		fread(iN, sizeof(InterleavedIndexNorm) * indexCount, 1, smf); // Read in interleaved data
	}
	free(h); // Free memory held by header structure
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
		glVertexPointer(3, GL_FLOAT, 0, v);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, indices);
	}
	else if(faceMode == FACE_INDEX_TEX) // Execute if mode is FACE_INDEX_TEX
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY); // Enable texture coord array processing
		glVertexPointer(3, GL_FLOAT, sizeof(struct InterleavedIndexTex), &iT[0].v); // Load vertex array into video memory
		glTexCoordPointer(3, GL_FLOAT, sizeof(struct InterleavedIndexTex), &iT[0].t); // Load texture coord array into video memory
		glDrawArrays(GL_TRIANGLES, 0, indexCount); // Draw data in provided arrays as a number of triangles represented by faceCount
		glDisableClientState(GL_TEXTURE_COORD_ARRAY); // Disable texture coord array processing
	}
	else if(faceMode == FACE_INDEX_NORM) // Execute if mode is FACE_INDEX_NORM
	{
		glEnableClientState(GL_NORMAL_ARRAY); // Enable normal array processing
		glVertexPointer(3, GL_FLOAT, sizeof(struct InterleavedIndexNorm), &iN[0].v); // Load vertex array into video memory
		glNormalPointer(GL_FLOAT, sizeof(struct InterleavedIndexNorm), &iN[0].n); // Load normal array into video memory
		glDrawArrays(GL_TRIANGLES, 0, indexCount); // Draw data in provided arrays as a number of triangles represented by faceCount
		glDisableClientState(GL_NORMAL_ARRAY); // Disable normal array processing
	}
	else if(faceMode == FACE_INDEX_TEX_NORM) // Execute if mode is FACE_INDEX_TEX_NORM
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY); // Enable texture coord array processing
		glEnableClientState(GL_NORMAL_ARRAY); // Enable normal array processing
		glVertexPointer(3, GL_FLOAT, sizeof(struct InterleavedAll), &iA[0].v); // Load vertex array into video memory
		glTexCoordPointer(3, GL_FLOAT, sizeof(struct InterleavedAll), &iA[0].t); // Load texture coord array into video memory
		glNormalPointer(GL_FLOAT, sizeof(struct InterleavedAll), &iA[0].n); // Load normal array into video memory
		glDrawArrays(GL_TRIANGLES, 0, indexCount); // Draw data in provided arrays as a number of triangles represented by faceCount
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
