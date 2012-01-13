/* Copyright (C) 2012 Isaac Ashwin Ravindran
 *
 * Written By: Isaac Ashwin Ravindran
 *
 * Description: This program is used to convert static Wavefront OBJ (.obj) files into quickly loadable Static Mesh
 * Files (.smf) for use in real time 3D programs. Where applicable, vertex data with indices are provided. Otherwise,
 * interleaved per vertex data arrays are provided.
 * 
 * Usage: OBJ2SMF objfile.obj smffile.smf
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Structs.h"

Vertex3D *v; // Vertex array
Normal3D *n; // Normal array
TexCoord3D *t; // Texture Coordinate array
Face3D *f; // Face array
GLuint *indices; // Index array

int vcount, fcount, ncount, tcount; // Variables to hold number of each element
int vcounting, fcounting, ncounting, tcounting; // Variables to hold current position in arrays
int set, faceMode; // Variables to handle face modes

int main(int argc, char* argv[])
{
	FILE *obj, *smf; // Necessary Files
	char objName[1024], smfName[1024]; // Filename arrays
	
	if(argv[1] == NULL || argv[2] == NULL) // Check if program start arguments are present
	{
		printf("Invalid input!\nUsage: OBJ2SMF objfile.obj smffile.smf\n"); // Print error
		return -2; // Exit with error
	}
	
	strcpy(objName, argv[1]); // Copy input file name
	strcpy(smfName, argv[2]); // Copy output file name
	
	obj = fopen(objName, "r"); // Open input file
	
	if(obj == NULL) // Report error if input file does not exist
	{
		printf("The specified OBJ file, %s, does not exist!\n", objName); // Print error
		return -1; // Exit with error
	}
	
	smf = fopen(smfName, "wb"); // Open output file
	
	// Reset variables
	vcount = fcount = ncount = tcount = 0;
	vcounting = fcounting = ncounting = tcounting = 0;
	set = 0;
	faceMode = 0;
	
	// Begin first reading
	while(!feof(obj))
	{
		if(feof(obj)) // Break if end of file reached
		{
			break;
		}
		
		char prefix[5]; // Array to hold prefix of line
		fscanf(obj, "%s", prefix); // Scan in prefix
		
		if(strcmp(prefix, "v") == 0) // If prefix denotes vertex
		{
			vcount++; // Increase vertex count
		}
		else if(strcmp(prefix, "vn") == 0) // If prefix denotes normal
		{
			ncount++; // Increase normal count
		}
		else if(strcmp(prefix, "vt") == 0) // If prefix denotes texture coordinate
		{
			tcount++; // Increase texture coordinate count
		}
		else if(strcmp(prefix, "f") == 0) // If prefix denotes face description
		{
			char c; // Variable to hold current working character
			
			fscanf(obj, "%c", &c); // Scan in first character
			if(c == ' ') // If current character is a space
			{
				fscanf(obj, "%c", &c); // Get next character
			}
			if(c >= '0' && c <= '9') // If current character is a number
			{
				if(set == 0) // If face mode is not already set
				{
					faceMode = FACE_INDEX; // Set face mode to only indices
				}
				while(1 == 1) // Skip characters until next non-numeric character
				{
					if(!(c >= '0' && c <= '9')) // Check if current character is non-numeric
					{
						break; // Exit loop
					}
					fscanf(obj, "%c", &c); // Get next character
				}
			}
			if(c == '/') // If current character is a slash
			{
				if(set == 0) // If face mode is not already set
				{
					faceMode = FACE_INDEX_TEX; // Set face mode to indices and texture coordinates
				}
				fscanf(obj, "%c", &c); // Get next character
			}
			if(c == '/') // If current character is a slash
			{
				if(set == 0) // If face mode is not already set
				{
					faceMode = FACE_INDEX_NORM; // Set face mode to indices and normals
				}
			}
			else if(c >= '0' && c <= '9') // Else if current character is a number
			{
				if(set == 0) // If face mode is not already set
				{
					faceMode = FACE_INDEX_TEX; // Set face mode to indices and texture coordinates
				}
				while(1 == 1) // Skip characters until next non-numeric character
				{
					if(!(c >= '0' && c <= '9')) // Check if current character is non-numeric
					{
						break; // Exit loop
					}
					fscanf(obj, "%c", &c); // Get next character
				}
				if(c == '/') // If current character is a slash
				{
					if(set == 0) // If face mode is not already set
					{
						faceMode = FACE_INDEX_TEX_NORM; // Set face mode to indices, texture coordinates and normals
					}
				}
			}
			set = 1; // Indicate that face mode is set
			fcount++; // Increase face count
		}
	}
	
	rewind(obj); // Reset input file
	
	// Allocate memory for all arrays
	v = (Vertex3D*)malloc(sizeof(Vertex3D) * vcount);
	n = (Normal3D*)malloc(sizeof(Normal3D) * ncount);
	t = (TexCoord3D*)malloc(sizeof(TexCoord3D) * tcount);
	f = (Face3D*)malloc(sizeof(Face3D) * fcount);
	indices = (GLuint*)malloc(sizeof(GLuint) * fcount * 3);
	
	// Begin second reading
	while(!feof(obj))
	{
		if(feof(obj)) // Check if end of file has been reached
		{
			break; // Exit loop
		}
		
		char prefix[5]; // Array to hold prefix of line
		fscanf(obj, "%s", prefix); // Scan in prefix
		
		if(strcmp(prefix, "v") == 0) // If prefix denotes vertex
		{
			fscanf(obj, "%f %f %f", &v[vcounting].x, &v[vcounting].y, &v[vcounting].z); // Scan in vertex data
			vcounting++; // Increase index of vertex array
		}
		else if(strcmp(prefix, "vt") == 0) // If prefix denotes texture coordinate
		{
			fscanf(obj, "%f %f %f", &t[tcounting].u, &t[tcounting].v, &t[tcounting].w); // Scan in texture coordinates
			tcounting++; // Increase index of texture coordinate array
		}
		else if(strcmp(prefix, "vn") == 0) // If prefix denotes normals
		{
			fscanf(obj, "%f %f %f", &n[ncounting].x, &n[ncounting].y, &n[ncounting].z); // Scan in normal data
			ncounting++; // Increase index of normal array
		}
		else if(strcmp(prefix, "f") == 0) // If prefix denotes face description
		{
			if(faceMode == FACE_INDEX) // If face mode is only indices
			{
				fscanf(obj, "%d %d %d", &indices[fcounting], &indices[fcounting + 1], &indices[fcounting + 2]); // Scan vertex indices into indices array
				indices[fcounting]--;
				indices[fcounting + 1]--;
				indices[fcounting + 2]--;
				fcounting += 3; // Increase index of indices array
			}
			else if(faceMode == FACE_INDEX_TEX) // If face mode is indices and texture coordinates
			{
				fscanf(obj, "%d/%d %d/%d %d/%d", &f[fcounting].v1, &f[fcounting].t1, &f[fcounting].v2, &f[fcounting].t2, &f[fcounting].v3, &f[fcounting].t3); // Scan in vertex and texture coordinate indices into face array
				f[fcounting].v1--;
				f[fcounting].v2--;
				f[fcounting].v3--;
				f[fcounting].t1--;
				f[fcounting].t2--;
				f[fcounting].t3--;
				fcounting++; // Increase index of face array
			}
			else if(faceMode == FACE_INDEX_TEX_NORM) // If face mode is indices, texture coordinates and normals
			{
				fscanf(obj, "%d/%d/%d %d/%d/%d %d/%d/%d", &f[fcounting].v1, &f[fcounting].t1, &f[fcounting].n1, &f[fcounting].v2, &f[fcounting].t2, &f[fcounting].n2, &f[fcounting].v3, &f[fcounting].t3, &f[fcounting].n3); // Scan in vertex, texture coordinate and normal indices
				f[fcounting].v1--;
				f[fcounting].v2--;
				f[fcounting].v3--;
				f[fcounting].t1--;
				f[fcounting].t2--;
				f[fcounting].t3--;
				f[fcounting].n1--;
				f[fcounting].n2--;
				f[fcounting].n3--;
				fcounting++; // Increase index of face array
			}
			else if(faceMode == FACE_INDEX_NORM) // If face mode is indices and normals
			{
				fscanf(obj, "%d//%d %d//%d %d//%d", &f[fcounting].v1, &f[fcounting].n1, &f[fcounting].v2, &f[fcounting].n2, &f[fcounting].v3, &f[fcounting].n3); // Scan in vertex and normal indices
				f[fcounting].v1--;
				f[fcounting].v2--;
				f[fcounting].v3--;
				f[fcounting].n1--;
				f[fcounting].n2--;
				f[fcounting].n3--;
				fcounting++; // Increase index of face array
			}
		}
	}
	
	// Interleaving of Per Vertex Data
	InterleavedIndexNorm *iN; // Array to hold interleaved vertex and normal data
	InterleavedIndexTex *iT; // Array to hold interleaved vertex and texture coordinate information
	InterleavedAll *iA; // Array to hold interleaved vertex, texture coordinate and normal data
	
	if(faceMode == FACE_INDEX_TEX) // If face mode is indices and texture coordinates
	{
		int i, j; // Indices to arrays
		
		iT = (InterleavedIndexTex*)malloc(sizeof(InterleavedIndexTex) * fcount * 3); // Allocate memory for array
		
		for(i = 0, j = 0; i < fcount; i++, j += 3) // Loop through arrays
		{
			iT[j].v = v[f[i].v1]; // Copy vertex data into interleaved array based on index provided in face description
			iT[j].t = t[f[i].t1]; // Copy texture coordinates into interleaved array based on index provided in face description
			
			iT[j + 1].v = v[f[i].v2];
			iT[j + 1].t = t[f[i].t2];
			
			iT[j + 2].v = v[f[i].v3];
			iT[j + 2].t = t[f[i].t3];
		}
	}
	else if(faceMode == FACE_INDEX_TEX_NORM) // If face mode is indices, texture coordinates and normals
	{
		int i, j; // Indices to arrays
		
		iA = (InterleavedAll*)malloc(sizeof(InterleavedAll) * fcount * 3); // Allocate memory for array
		
		for(i = 0, j = 0; i < fcount; i++, j += 3) // Loop through arrays
		{
			iA[j].v = v[f[i].v1]; // Copy vertex data into interleaved array based on index provided in face description
			iA[j].t = t[f[i].t1]; // Copy texture coordinates into interleaved array based on index provided in face description
			iA[j].n = n[f[i].n1]; // Copy normal data into interleaved array based on index provided in face description
			
			iA[j + 1].v = v[f[i].v2];
			iA[j + 1].t = t[f[i].t2];
			iA[j + 1].n = n[f[i].n2];
			
			iA[j + 2].v = v[f[i].v3];
			iA[j + 2].t = t[f[i].t3];
			iA[j + 2].n = n[f[i].n3];
		}
	}
	else if(faceMode == FACE_INDEX_NORM) // If face mode is indices and normals
	{
		int i, j; // Indices to arrays
		
		iN = (InterleavedIndexNorm*)malloc(sizeof(InterleavedIndexNorm) * fcount * 3); // Allocate memory for array
		
		for(i = 0, j = 0; i < fcount; i++, j += 3) // Loop through arrays
		{
			iN[j].v = v[f[i].v1]; // Copy vertex data into interleaved array based on index provided in face description
			iN[j].n = n[f[i].n1]; // Copy normal data into interleaved array based on index provided in face description
			
			iN[j + 1].v = v[f[i].v2];
			iN[j + 1].n = n[f[i].n2];
			
			iN[j + 2].v = v[f[i].v3];
			iN[j + 2].n = n[f[i].n3];
		}
	}
	
	SMFHeader h; // Structure to store information about file
	h.indexCount = fcount * 3; // Store index count
	h.vcount = vcount; // Store number of vertices
	h.fcount = fcount; // Store number of faces
	h.faceMode = faceMode; // Store face mode
	
	if(faceMode == FACE_INDEX) // If face mode is only index
	{
		fwrite(&h, sizeof(SMFHeader), 1, smf); // Write header
		fwrite(v, sizeof(Vertex3D) * vcount, 1, smf); // Write vertex array into SMF file
		fwrite(indices, sizeof(GLuint) * fcount * 3, 1, smf); // Write index array into SMF file
		printf("OBJ file successfully compiled!\n\nVertices: %d Faces: %d Indices: %d Face Mode: INDEX\n", vcount, fcount, (fcount * 3)); // Output file specs
		free(v);
		free(indices);
	}
	else if(faceMode == FACE_INDEX_TEX) // If face mode is indices and texture coordinates
	{
		fwrite(&h, sizeof(SMFHeader), 1, smf); // Write header
		fwrite(iT, sizeof(InterleavedIndexTex) * fcount * 3, 1, smf); // Write interleaved index and texture coordinate array into file
		printf("OBJ file successfully compiled!\n\nVertices: %d Texture Coordinates: %d Faces: %d Indices: %d Face Mode: INDEX_TEX\n", vcount, tcount, fcount, (fcount * 3)); // Output file specs
		free(iT);
	}
	else if(faceMode == FACE_INDEX_TEX_NORM) // If face mode is indices, texture coordinates and normals
	{
		fwrite(&h, sizeof(SMFHeader), 1, smf); // Write header
		fwrite(iA, sizeof(InterleavedAll) * fcount * 3, 1, smf); // Write interleaved index, texture coordinate and normal array into file
		printf("OBJ file successfully compiled!\n\nVertices: %d Texture Coordinates: %d Normals: %d Faces %d Indices: %d Face Mode: INDEX_TEX_NORM\n", vcount, tcount, ncount, fcount, (fcount * 3)); // Output file specs
		free(iA);
	}
	else if(faceMode == FACE_INDEX_NORM) // If face mode is indices and normals
	{
		fwrite(&h, sizeof(SMFHeader), 1, smf); // Write header
		fwrite(iN, sizeof(InterleavedIndexNorm) * fcount * 3, 1, smf); // Write interleaved index and normal array into file
		printf("OBJ file successfully compiled!\n\nVertices: %d Normals: %d Faces: %d Indices: %d Face Mode: INDEX_NORM\n", vcount, ncount, fcount, (fcount * 3)); // Output file specs
		free(iN);
	}
	
	fclose(obj); // Close input file
	fclose(smf); // Close output file
	
	return 0; // Exit program
}
