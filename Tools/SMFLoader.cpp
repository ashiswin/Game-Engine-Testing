#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include "Objects/CStaticMesh.h"

CStaticMesh *Model;

void init (void) 
{
/*  select clearing (background) color       */
    glClearColor (0.0, 0.0, 0.0, 0.0);

/*  initialize viewing values  */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 0.0001, 1000.0);
    
    Model = new CStaticMesh((char*)"icos.smf");
    Model->PrintMeshData();
}

void display(void)
{
   	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, -10.0);
	Model->RenderMesh();
	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition (100, 100);
	glutCreateWindow("OBJ Loader");
	init();
	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
