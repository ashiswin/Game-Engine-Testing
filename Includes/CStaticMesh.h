#include "Structs.h"

class CStaticMesh {
	private:
		GLuint faceMode; // Mesh face mode
		GLuint indexCount; // Integer to hold the number of elements of the respective type
		GLuint faceCount; // Integer to hold the number of polygons
		
		struct InterleavedIndexTex* iT; // Interleaved structure array for vertices and textures (FACE_INDEX_TEX)
		struct InterleavedIndexNorm* iN; // Interleaved structure array for vertices and normals (FACE_INDEX_NORM)
		struct InterleavedAll* iA; // Interleaved structure array for vertices, textures and normals (FACE_INDEX_TEX_NORM)
		
		GLuint vcount; // Integer to hold the number of vertices
		struct Vertex3D *v; // Array for vertex data only (FACE_INDEX)
		GLuint *indices; // Indices to this vertex array for drawing elements
		
		GLuint width; // Texture width
		GLuint height; // Texture height
		GLubyte *textureData; // Array of texture data
	public:
		CStaticMesh(char filename[1024]); // Constructor to initialize object from a Static Mesh File
		~CStaticMesh(void);
		bool LoadTextureFile(char filename[1024]); // Load texture file
		void RenderMesh(void); // Render mesh data
		void PrintMeshData(void);
};
