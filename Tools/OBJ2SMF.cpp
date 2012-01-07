#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Objects/Structs.h"

int vcount;
int fcount;
int ncount;
int tcount;
int main()
{
	FILE *in, *out; // Input and output files
	char inFilename[1024], outFilename[1024]; // Input and output file names
	
	// Get filenames
	printf("Please enter only the name of the OBJ file without spacing: ");
	scanf("%s", inFilename);
	printf("Please enter a name for the output Static Mesh File: ");
	scanf("%s", outFilename);
	strcat(inFilename, ".obj");
	strcat(outFilename, ".smf");
	// Open files
	in = fopen(inFilename, "r");
	out = fopen(outFilename, "wb");
	
	if(in == NULL)
	{
		printf("The specified OBJ file does not exist.\n");
		return 1;
	}
	// Create and clear header
	int set;
	int count;
	struct SMFHeader h;
	h.vcount = 0;
	h.fcount = 0;
	h.faceMode = 0;
	set = 0;
	
	vcount = ncount = fcount = tcount = 0;
	// Begin first reading
	while(!feof(in))
	{
		if(feof(in))
		{
			break;
		}
		
		char c[1024]; // Buffer to hold starting characters of OBJ file
		fscanf(in, "%s", c); // Read in starting characters
		if(strcmp(c, "v") == 0) // Check if it is a vertex
		{
			vcount++; // Increase vertex count
		}
		if(strcmp(c, "vn") == 0) // Check if it is a normal
		{
			ncount++; // Increase normal count
		}
		if(strcmp(c, "vt") == 0) // Check if it is a texture coordinate
		{
			tcount++; // Increase texture coordinate count
		}
		if(strcmp(c, "f") == 0) // Check if it is a face
		{
			char c; // Character to hold current character
			fscanf(in, "%c", &c); // Get first character
			if(c == ' ') // Skip a space
			{
				fscanf(in, "%c", &c); // Get next character
			}
			if(c >= '0' && c <= '9') // Check if it is a number
			{
				if(set == 0)
				{
					h.faceMode = FACE_INDEX; // Set face mode to only index
				}
				fscanf(in, "%c", &c); // Get next character
			}
			while(1 == 1) // Scan characters until end of numeric characters
			{
				fscanf(in, "%c", &c);
				if(!(c >= '0' && c <='9'))
				{
					break;
				}
			}
			if(c == '/') // Check if there is a slash
			{
				if(set == 0)
				{
					h.faceMode = FACE_INDEX_TEX; // Set face mode to index and texture coordinates
				}
				fscanf(in, "%c", &c); // Get next character
				if(c >= '0' && c <= '9') // Check if it is a number
				{
					if(set == 0)
					{
						h.faceMode = FACE_INDEX_TEX; // Set face mode to index and texture coordinates
					}
					while(1 == 1) // Scan characters until end of numeric characters
					{
						fscanf(in, "%c", &c);
						if(!(c >= '0' && c <='9'))
						{
							break;
						}
					}
					if(c == '/') // Check if it is a slash
					{
						if(set == 0)
						{
							h.faceMode = FACE_INDEX_TEX_NORM; // Set face mode to index, texture coordinates and normals
						}
					}
				}
				else if(c == '/') // Check if it is a slash
				{
					if(set == 0)
					{
						h.faceMode = FACE_INDEX_NORM; // Set face mode to index and normals
					}
				}
			}
			set = 1; // Indicate that face mode has been set
			fcount++; // Increase face count
		}
	}
	
	if(h.faceMode == FACE_INDEX)
	{
		h.vcount = vcount;
	}
	h.fcount = fcount;
	h.indexCount = fcount * 3;
	
	rewind(in); // Reset file
	
	struct Face3D *f;
	struct Vertex3D *v;
	struct Normal3D *n;
	struct TexCoord3D *t;
	GLuint *indices;
	
	f = (struct Face3D*)malloc(sizeof(struct Face3D)*fcount);
	v = (struct Vertex3D*)malloc(sizeof(struct Vertex3D)*vcount);
	n = (struct Normal3D*)malloc(sizeof(struct Normal3D)*ncount);
	t = (struct TexCoord3D*)malloc(sizeof(struct TexCoord3D)*tcount);
	
	if(h.faceMode == FACE_INDEX)
	{
		indices = (GLuint*)malloc(sizeof(GLuint) * h.indexCount);
	}
	
	count = 0;
	// Begin reading vertex coordinates
	while(!feof(in))
	{
		if(feof(in))
		{
			break;
		}
		char c[1024]; // Buffer to hold characters
		fscanf(in, "%s", c);
		if(strcmp(c, "v") == 0) // Scan and print and write vertex data
		{
			//struct Vertex3D v;
			fscanf(in, "%f %f %f", &v[count].x, &v[count].y, &v[count].z);
			printf("%f %f %f\n", v[count].x, v[count].y, v[count].z);
			//fwrite(&v, sizeof(struct Vertex3D), 1, out);
			count++;
		}
	}
	
	rewind(in); // Reset file
	
	count = 0;
	// Begin reading normal vectors
	while(!feof(in))
	{
		if(feof(in))
		{
			break;
		}
		char c[1024];
		fscanf(in, "%s", c);
		if(strcmp(c, "vn") == 0) // Scan and print and write normal data
		{
			//struct Normal3D n;
			fscanf(in, "%f %f %f",& n[count].x, &n[count].y, &n[count].z);
			printf("%f %f %f\n", n[count].x, n[count].y, n[count].z);
			//fwrite(&n, sizeof(struct Normal3D), 1, out);
			count++;
		}
	}
	
	rewind(in); // Reset file
	
	// Begin reading texture coordinates
	while(!feof(in))
	{
		if(feof(in))
		{
			break;
		}
		char c[1024];
		fscanf(in, "%s", c);
		if(strcmp(c, "vt") == 0) // Scan and print and write texture coordinates
		{
			//struct TexCoord3D t;
			fscanf(in, "%f %f %f", &t[count].u, &t[count].v, &t[count].w);
			if(t[count].w == 0)
			{
				t[count].w = 1;
			}
			//fscanf(in, "%f %f", &t.u, &t.v);
			//t.w = 0;
			printf("%f %f %f\n", t[count].u, t[count].v, t[count].w);
			count++;
			//fwrite(&t, sizeof(struct TexCoord3D), 1, out);
		}
	}
	
	rewind(in); // Reset file
	if(in == NULL)
	{
		printf("SHIT");
	}
	count = 0;
	// Begin reading face information
	while(!feof(in))
	{
		if(feof(in))
		{
			break;
		}
		char c[1024]; // Buffer to hold characters
		fscanf(in, "%s", c);
		if(strcmp(c, "f") == 0) // Scan and print and write face data based on input format
		{
			if(h.faceMode == FACE_INDEX)
			{
				//fscanf(in, "%d %d %d", &f.v1, &f.v2, &f.v3); // Format (v1 v2 v3)
				fscanf(in, "%d", &indices[count]);
				count++;
				fscanf(in, "%d", &indices[count]);
				count++;
				fscanf(in, "%d", &indices[count]);
				count++;
			}
			if(h.faceMode == FACE_INDEX_TEX)
			{
				char c; // Character to hold spaces and slashes
				fscanf(in, "%d%c%d %d%c%d %d%c%d", &f[count].v1, &c, &f[count].t1, &f[count].v2, &c, &f[count].t2, &f[count].v3, &c, &f[count].t3); // Format (v1/t1 v2/t2 v3/t3)
				f[count].v1--;
				f[count].v2--;
				f[count].v3--;
				f[count].n1 = 0;
				f[count].n2 = 0;
				f[count].n3 = 0;
				f[count].t1--;
				f[count].t2--;
				f[count].t3--;
				printf("%d/%d %d/%d %d/%d\n", f[count].v1, f[count].t1, f[count].v2, f[count].t2, f[count].v3, f[count].t3);
				count++;
				//fwrite(&f, sizeof(struct Face3D), 1, out);
			}
			if(h.faceMode == FACE_INDEX_TEX_NORM)
			{
				char c;
				fscanf(in, "%d%c%d%c%d %d%c%d%c%d %d%c%d%c%d", &f[count].v1, &c, &f[count].t1, &c, &f[count].n1, &f[count].v2, &c, &f[count].t2, &c, &f[count].n2, &f[count].v3, &c, &f[count].t3, &c, &f[count].n3); //Format (v1/t1/n1 v2/t2/n2 v3/t3/n3)
				f[count].v1--;
				f[count].v2--;
				f[count].v3--;
				f[count].n1--;
				f[count].n2--;
				f[count].n3--;
				f[count].t1--;
				f[count].t2--;
				f[count].t3--;
				
				printf("%d/%d/%d %d/%d/%d %d/%d/%d\n", f[count].v1, f[count].t1, f[count].n1, f[count].v2, f[count].t2, f[count].n2, f[count].v3, f[count].t3, f[count].n3);
				count++;
				//fwrite(&f, sizeof(struct Face3D), 1, out);
			}
			if(h.faceMode == FACE_INDEX_NORM)
			{
				char c;
				fscanf(in, "%d%c%c%d %d%c%c%d %d%c%c%d", &f[count].v1, &c, &c, &f[count].n1, &f[count].v2, &c, &c, &f[count].n2, &f[count].v3, &c, &c, &f[count].n3); // Format (v1//n1 v2//n2 v3//n3)
				f[count].v1--;
				f[count].v2--;
				f[count].v3--;
				f[count].n1--;
				f[count].n2--;
				f[count].n3--;
				f[count].t1 = 0;
				f[count].t2 = 0;
				f[count].t3 = 0;
				
				printf("%d//%d %d//%d %d//%d\n", f[count].v1, f[count].n1, f[count].v2, f[count].n2, f[count].v3, f[count].n3);
				count++;
				//fwrite(&f, sizeof(struct Face3D), 1, out);
			}
		}
	}
	printf("\n\nVertices: %d Faces: %d Normals: %d Texture Coordinates: %d Face Mode: %d Indices: %d\n\n", vcount, fcount, ncount, tcount, h.faceMode, h.indexCount); // Print OBJ file specs
	
	fwrite(&h, sizeof(struct SMFHeader), 1, out); // Write header
	
	if(h.faceMode == FACE_INDEX)
	{
		fwrite(v, sizeof(struct Vertex3D) * vcount, 1, out);
		fwrite(indices, sizeof(GLuint) * h.indexCount, 1, out);
		free(v);
		free(indices);
	}
	else if(h.faceMode == FACE_INDEX_TEX)
	{
		struct InterleavedIndexTex *iT;
		int i, j;
		iT = (struct InterleavedIndexTex*)malloc(sizeof(struct InterleavedIndexTex) * h.indexCount);
		
		for(i = 0, j = 0; i < h.indexCount, j < fcount; i += 3, j++)
		{	
			iT[i].v = v[f[j].v1];
			iT[i].t = t[f[j].t1];
			printf("Vertex 1: %f %f %f ", iT[i - 2].v.x, iT[i - 2].v.y, iT[i - 2].v.z);
			printf("Texture Coordinate 1: %f %f %f\n", iT[i - 2].t.u, iT[i - 2].t.v, iT[i - 2].t.w);
			
			iT[i + 1].v = v[f[j].v2];
			iT[i + 1].t = t[f[j].t2];
			printf("Vertex 2: %f %f %f ", iT[i - 1].v.x, iT[i - 1].v.y, iT[i - 1].v.z);
			printf("Texture Coordinate 2: %f %f %f\n", iT[i - 1].t.u, iT[i - 1].t.v, iT[i - 1].t.w);
			
			iT[i + 2].v = v[f[j].v3];
			iT[i + 2].t = t[f[j].t3];
			printf("Vertex 3: %f %f %f ", iT[i].v.x, iT[i].v.y, iT[i].v.z);
			printf("Texture Coordinate 3: %f %f %f\n\n", iT[i].t.u, iT[i].t.v, iT[i].t.w);
		}
		
		fwrite(iT, sizeof(struct InterleavedIndexTex), h.indexCount, out);
		free(iT);
	}
	else if(h.faceMode == FACE_INDEX_NORM)
	{
		struct InterleavedIndexNorm *iN;
		int i;
		iN = (struct InterleavedIndexNorm*)malloc(sizeof(struct InterleavedIndexTex) * h.indexCount);
		
		for(i = 0; i < fcount; i++)
		{
			int j;
			j = 0;
			
			iN[i + j].v = v[f[i].v1];
			iN[i + j].n = n[f[i].n1];
			
			j++;
			
			iN[i + j].v = v[f[i].v2];
			iN[i + j].n = n[f[i].n2];
			
			j++;
			
			iN[i + j].v = v[f[i].v3];
			iN[i + j].n = n[f[i].n3];
			
			//printf("Vertex: %d %d %d Texture Coordinate: %d %d %d\n", iN[i].v,
		}
		
		fwrite(iN, sizeof(struct InterleavedIndexNorm) * h.indexCount, 1, out);
		free(iN);
	}
	else if(h.faceMode == FACE_INDEX_TEX_NORM)
	{
		struct InterleavedAll *iA;
		int i;
		iA = (struct InterleavedAll*)malloc(sizeof(struct InterleavedAll) * h.indexCount);
		
		for(i = 0; i < fcount; i++)
		{
			int j;
			j = 0;
			
			iA[i + j].v = v[f[i].v1];
			iA[i + j].n = n[f[i].n1];
			iA[i + j].t = t[f[i].t1];
			
			j++;
			
			iA[i + j].v = v[f[i].v2];
			iA[i + j].n = n[f[i].n2];
			iA[i + j].t = t[f[i].t2];
			
			j++;
			
			iA[i + j].v = v[f[i].v3];
			iA[i + j].n = n[f[i].n3];
			iA[i + j].t = t[f[i].t3];
		}
		
		fwrite(iA, sizeof(struct InterleavedAll) * h.indexCount, i, out);
		free(iA);
	}
	
	
	// Close files
	fclose(out);
	fclose(in);
	return 0;
}
