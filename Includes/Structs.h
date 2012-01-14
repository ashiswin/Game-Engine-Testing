#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <OpenGL/glut.h>
#endif

#define FACE_INDEX 0 // Face Format (v1 v2 v3)
#define FACE_INDEX_TEX 1 // Face Format (v1/t1 v2/t2 v3/t3)
#define FACE_INDEX_TEX_NORM 2 // Face Format (v1/t1/n1 v2/t2/n2 v3/t3/n3)
#define FACE_INDEX_NORM 3 // Face Format (v1//n1 v2//n2 v3//n3)

struct SMFHeader
{
	GLuint indexCount;
	GLuint vcount;
	GLuint fcount;
	GLuint faceMode;
}; // Struct to hold header information about SMF file

struct Vertex3D
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
}; // Vertex struct

struct Normal3D
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
}; // Normal struct

struct TexCoord3D
{
	GLfloat u;
	GLfloat v;
	GLfloat w;
}; // Texture Coordinate struct

struct Face3D
{
	int v1;
	int v2;
	int v3;
	
	int n1;
	int n2;
	int n3;
	
	int t1;
	int t2;
	int t3;
}; // Face structure

struct InterleavedIndexTex
{
	struct Vertex3D v;
	struct TexCoord3D t;
};

struct InterleavedIndexNorm
{
	struct Vertex3D v;
	struct Normal3D n;
};

struct InterleavedAll
{
	struct Vertex3D v;
	struct Normal3D n;
	struct TexCoord3D t;
};

struct MTFHeader
{
	GLuint width;
	GLuint height;
	GLuint bpp;
	GLuint imageSize;
	GLuint type;
};

typedef GLfloat Matrix3D[16];
