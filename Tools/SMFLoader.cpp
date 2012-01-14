#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include "CStaticMesh.h"

CStaticMesh *Model;
int rotx, roty, rotz;

void handleKeypress(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'd':
			roty += 10.0f;
			glutPostRedisplay();
			break;
		case 'a':
			roty -= 10.0f;
			glutPostRedisplay();
			break;
		case 'w':
			rotx -= 10.0f;
			glutPostRedisplay();
			break;
		case 's':
			rotx += 10.0f;
			glutPostRedisplay();
			break;
	}
}

void init (void) 
{
/*  select clearing (background) color       */
    glClearColor (0.0, 0.0, 0.0, 0.0);

/*  initialize viewing values  */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 0.0001, 1000.0);
    
    Model = new CStaticMesh((char*)"normalpaintball.smf");
    Model->LoadTextureFile((char*)"lolpaint.mtf");	
    Model->PrintMeshData();
    Model->PrintTextureData();
    
    rotx = roty = rotz = 0.0f;
}

void display(void)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
   	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	Model->TranslateModel(0.0f, 0.0f, -3.0f);
	Model->RotateModel(rotx, roty, rotz);
	Model->RenderMesh();
	glPopMatrix();
	glFlush();
	glutSwapBuffers();
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
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
	glutKeyboardFunc(handleKeypress);
	//glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
