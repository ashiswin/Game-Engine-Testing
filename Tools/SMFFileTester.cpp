#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Structs.h"

int main(int argc, char* argv[])
{
	FILE *smf;
	SMFHeader h;
	InterleavedIndexNorm *iN;
	InterleavedIndexTex *iT;
	InterleavedAll *iA;
	Vertex3D *v;
	GLuint *indices;
	int i;
	
	if(argv[1] == NULL)
	{
		printf("Invalid input!\nUsage: SMFFileTester smffile.smf\n");
		return -2;
	}
	
	smf = fopen(argv[1], "rb");
	
	if(smf == NULL)
	{
		printf("The specified OBJ file, %s, does not exist!\n", argv[1]);
		return -1;
	}
	
	fread(&h, sizeof(SMFHeader), 1, smf);
	
	if(h.faceMode == FACE_INDEX)
	{
		v = (Vertex3D*)malloc(sizeof(Vertex3D) * h.vcount);
		indices = (GLuint*)malloc(sizeof(GLuint) * h.indexCount);
		
		fread(v, sizeof(Vertex3D) * h.vcount, 1, smf);
		fread(indices, sizeof(GLuint) * h.indexCount, 1, smf);
		
		for(i = 0; i < h.vcount; i++)
		{
			printf("v %f %f %f\n", v[i].x, v[i].y, v[i].z);
		}
		printf("\n");
		for(i = 0; i < h.indexCount; i += 3)
		{
			printf("f %d %d %d\n", indices[i], indices[i + 1], indices[i + 2]);
		}
	}
	else if(h.faceMode == FACE_INDEX_TEX)
	{
		iT = (InterleavedIndexTex*)malloc(sizeof(InterleavedIndexTex) * h.indexCount);
		
		fread(iT, sizeof(InterleavedIndexTex) * h.indexCount, 1, smf);
	}
	else if(h.faceMode == FACE_INDEX_NORM)
	{
		iN = (InterleavedIndexNorm*)malloc(sizeof(InterleavedIndexNorm) * h.indexCount);
		
		fread(iN, sizeof(InterleavedIndexNorm) * h.indexCount, 1, smf);
	}
	else if(h.faceMode == FACE_INDEX_TEX_NORM)
	{
		iA = (InterleavedAll*)malloc(sizeof(InterleavedAll) * h.indexCount);
		
		fread(iA, sizeof(InterleavedAll) * h.indexCount, 1, smf);
	}
	
	fclose(smf);
	
	return 0;
}
