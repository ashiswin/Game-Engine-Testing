/* Copyright (C) 2012 Isaac Ashwin Ravindran
 *
 * Written By: Isaac Ashwin Ravindran
 *
 * Description: This program allows you to load and view a Model Texture File with its accompanying
 * Mesh Texture File applied onto it.
 * 
 * Usage: Change loaded model file path and texture file path and recompile
 */

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include "CStaticMesh.h"

CStaticMesh *Model; // Pointer to CStaticMesh object to hold model information
int rotx, roty, rotz; // Variables to hold model rotation

void handleKeypress(unsigned char key, int x, int y) // Function to handle key presses
{
	switch(key) // Switch based on pressed key
	{
		case 'd': // If key pressed is d
			roty += 10.0f; // Increase rotation on y-axis by 10 degrees
			glutPostRedisplay(); // Call a redraw
			break; // Break
		case 'a': // If key pressed is a
			roty -= 10.0f; // Decrease rotation on y-axis by 10 degrees
			glutPostRedisplay(); // Call a redraw
			break; // Break
		case 'w': // If key pressed is w
			rotx -= 10.0f; // Decrease rotation on x-axis by 10 degrees
			glutPostRedisplay(); // Call a redraw
			break; // Break
		case 's': // If key pressed is s
			rotx += 10.0f; // Increase rotation on x-axis by 10 degrees
			glutPostRedisplay(); // Call a redraw
			break; // Break
		case 'q': // If key pressed is q
			rotz += 10.0f; // Increase rotation on z-axis by 10 degrees
			glutPostRedisplay(); // Call a redraw
			break; // Break
		case 'e': // If key pressed is e
			rotz -= 10.0f; // Decrease rotation on z-axis by 10 degrees
			glutPostRedisplay(); // Call a redraw
			break; // Break
	}
}

void init(void) // Function to initialize program
{
    	glClearColor (0.0, 0.0, 0.0, 0.0); // Set color to clear background to
	
	glMatrixMode(GL_PROJECTION); // Set matrix to projection matrix
	glLoadIdentity(); // Reset projection matrix
	gluPerspective(60.0, 1.0, 0.0001, 1000.0); // Initialize perspective and viewing frustum
	
	Model = new CStaticMesh((char*)"normalpaintball.smf"); // Load Static Mesh File
	Model->LoadTextureFile((char*)"lolpaint.mtf"); // Load Mesh Texture File
	Model->PrintMeshData(); // Print info about mesh
	Model->PrintTextureData(); // Print info about texture
	    
	rotx = roty = rotz = 0.0f; // Reset rotation variables
}

void display(void) // Function used to render scene
{
	glEnable(GL_DEPTH_TEST); // Enable depth testing
	glEnable(GL_CULL_FACE); // Enable back face culling
   	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	
	glPushMatrix(); // Push current matrix onto stack
	Model->TranslateModel(0.0f, 0.0f, -3.0f); // Translate model 3 units down the z-axis
	Model->RotateModel(rotx, roty, rotz); // Rotate model by value specified in variables
	Model->RenderMesh(); // Render model
	glPopMatrix(); // Pop current matrix off stack
	
	glFlush(); // Force completion of all drawing
	glutSwapBuffers(); // Swap buffers
	
	glDisable(GL_CULL_FACE); // Disable back face culling
	glDisable(GL_DEPTH_TEST); // Disable depth testing
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv); // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Set display mode to RGB and double buffered
	glutInitWindowSize(600, 600); // Set window size
	glutInitWindowPosition (100, 100); // Set window starting position
	glutCreateWindow("OBJ Loader"); // Create window with title OBJ Loader
	init(); // Call init function
	glutDisplayFunc(display); // Set display as drawing function
	glutKeyboardFunc(handleKeypress); // Set handle keypress as function to handle keyboard input
	//glutReshapeFunc(reshape);
	glutMainLoop(); // Start render loop
	return 0; // Exit program
}
