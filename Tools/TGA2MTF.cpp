#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Structs.h"

struct TGAHeader
{
    GLubyte Header[12];         // File Header To Determine File Type
};
 
struct TGA
{
    GLubyte header[6];          // Holds The First 6 Useful Bytes Of The File
    GLuint bytesPerPixel;           // Number Of BYTES Per Pixel (3 Or 4)
    GLuint imageSize;           // Amount Of Memory Needed To Hold The Image
    GLuint type;                // The Type Of Image, GL_RGB Or GL_RGBA
    GLuint Height;              // Height Of Image                 
    GLuint Width;               // Width Of Image              
    GLuint Bpp;             // Number Of BITS Per Pixel (24 Or 32)
};

// Uncompressed TGA Header
GLubyte uTGAcompare[12] = {0,0, 2,0,0,0,0,0,0,0,0,0};
// Compressed TGA Header
GLubyte cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};

int main(int argc, char* argv[])
{
	MTFHeader h; // Structure to store information about Mesh Texture File
	TGAHeader tgaheader;                // Used To Store Our File Header
	TGA tg;                    // Used To Store File Information
	FILE *tga, *mtf; // Pointers to input and output files
	GLuint bytesPerPixel; // Unsigned integer to hold bytes per pixel
	GLubyte *imageData; // Byte array to hold image data
	
	if(argv[1] == NULL || argv[2] == NULL) // Check if program start arguments are present
	{
		printf("Invalid input!\nUsage: TGA2MTF tgafile.tga mtffile.mtf\n"); // Print error
		return -2; // Exit with error
	}
	
	tga = fopen(argv[1], "rb"); // Open input file
	
	if(tga == NULL) // Report error if input file does not exist
	{
		printf("The specified TGA file, %s, does not exist!\n", argv[1]); // Print error
		return -1; // Exit with error
	}
	
	mtf = fopen(argv[2], "rb"); // Open output file
	
	fread(&tgaheader, sizeof(TGAHeader), 1, tga); // Read in TGA file header
	
	if(memcmp(uTGAcompare, &tgaheader, sizeof(tgaheader)) != 0) // Check if TGA file is uncompressed
	{
		printf("The specified TGA file is not in an uncompressed format!\n"); // Print error
		return -3; // Exit with error
	}
	
	fread(tg.header, sizeof(tg.header), 1, tga); // Read in useful bytes of TGA file
	
	h.width = tg.header[1] * 256 + tg.header[0]; // Calculate width
	h.height = tg.header[3] * 256 + tg.header[2]; // Calculate height
	h.bpp = tg.header[4]; // Get bits per pixel
	
	if(h.bpp == 24) // Set bitness of image
	{
		h.type = GL_RGB;
	}
	else
	{
		h.type = GL_RGBA;
	}
	
	bytesPerPixel = h.bpp / 8; // Calculate bytes per pixel
	h.imageSize = bytesPerPixel * h.width * h.height; // Calculate amount of space needed to store image
	
	imageData = (GLubyte*)malloc(h.imageSize); // Allocate memory for image data
	
	fread(imageData, h.imageSize, 1, tga); // Read in image data
	
	// Swap BGR to RGB for OpenGL to render
	for(GLuint cswap = 0; cswap < (int)h.imageSize; cswap += bytesPerPixel)
    	{
        	// 1st Byte XOR 3rd Byte XOR 1st Byte XOR 3rd Byte
        	imageData[cswap] ^= imageData[cswap+2] ^=
        	imageData[cswap] ^= imageData[cswap+2];
    	}
	
	printf("Height: %d Width: %d\n", h.height, h.width); // Print file specs
	
	fclose(tga); // Close input file
	
	return 0; // Exit program
}
