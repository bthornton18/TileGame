/*
*** FILE NAME   : opengl_ex.c
*** DESCRIPTION : This program is an example program using OpenGL
*** DATE        : July 2013
*** WRITTEN By  : JKL
*/

#include <stdio.h>               // standard C libraries
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <GL/glut.h>             // GLUT library


//@@***********************************************************************************@@
// Constants
#define WINDOW_XS 300							// Window size
#define WINDOW_YS 403
#define WINDOW_NAME "OpenGL Example"			// Window name

//@@***********************************************************************************@@
// Structures
typedef struct pt
{
   GLfloat x, y;
}MyPoint;

//@@***********************************************************************************@@
// Global Variables
MyPoint bottomLeftPt;

int recLength = 90;
int recHeight = 90;

//@@***********************************************************************************@@
// Function prototypes
void reshape_handler(int width, int height);
void init_setup(int width, int height, char *windowName);
void display_func(void);
void keyboard_func(unsigned char c, int x, int y);
void mouse_func(int button, int state, int x, int y);

//@@***********************************************************************************@@
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	init_setup(WINDOW_XS, WINDOW_YS, WINDOW_NAME);
	
	// initial position of bottom left corner of rectangle
	bottomLeftPt.x = 5;
	bottomLeftPt.y = 5;

	glutDisplayFunc(display_func);
	glutKeyboardFunc(keyboard_func);
	glutMouseFunc(mouse_func);	
	glutMainLoop();

	return 1;
}	// end of main()

//@@***********************************************************************************@@
void reshape_handler(int width, int height)
{
    glViewport(0, 0, width, height);							// sets the viewport
    glMatrixMode(GL_PROJECTION);								// projection matrix
    glLoadIdentity();											// loads identity matrix
    gluOrtho2D(0.0, (GLdouble)width, 0.0, (GLdouble)height);	// 2D orthographic projection
}	// end of reshape_handler()

//@@***********************************************************************************@@
void init_setup(int width, int height, char *windowName)
{
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  // single buffer, rgb color
    //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  // double buffer, rgb color
    glutInitWindowSize(width, height);			  // init. window size
    glutInitWindowPosition(5, 5);				  // init. window position
    glutCreateWindow(windowName);				  // window name
    glutReshapeFunc(reshape_handler);		      // sets the reshape call back
}	// end of init_setup()

//@@***********************************************************************************@@
void display_func(void)
{
	glClearColor(0.827, 0.827, 0.827, 1.0);   // background color (light-grey)
	glClear(GL_COLOR_BUFFER_BIT);       // clearing the buffer not to keep the color

	// draw barrier between top and game board
	glColor3f(0.439, 0.502, 0.565);			// setting pen color (grey-blue)
	glBegin(GL_LINES);
	for (int i = 0; i < 3; i++) {
		glVertex2i(0, 301 + i);
		glVertex2i(300, 301 + i);
	}
	glEnd();

	// draw game baord grid
	glColor3f(0.439, 0.502, 0.565);			// setting pen color (grey-blue)
	glBegin(GL_LINES);
	for (int i = 1; i <= 2; i++) {
		glVertex2i(300 / 3 * i, 0); 
		glVertex2i(300 / 3 * i, 300);
		glVertex2i(0,300 / 3 * i);
		glVertex2i(300,300 / 3 * i);
	}

	glEnd();
	// draw a rectangle
	glColor3f(0.824, 0.706, 0.549);			// setting pen color (tan)
	glBegin(GL_POLYGON);
		glVertex2i(bottomLeftPt.x,           bottomLeftPt.y);
		glVertex2i(bottomLeftPt.x+recLength, bottomLeftPt.y);
		glVertex2i(bottomLeftPt.x+recLength, bottomLeftPt.y+recHeight);
		glVertex2i(bottomLeftPt.x,           bottomLeftPt.y+recHeight);
	glEnd();
		
	// draw the outline of rectangle
	glColor3f(0.502, 0.000, 0.000);			// setting pen color (maroon)
	glBegin(GL_LINES);
		glVertex2i(bottomLeftPt.x,           bottomLeftPt.y);
		glVertex2i(bottomLeftPt.x+recLength, bottomLeftPt.y);

		glVertex2i(bottomLeftPt.x+recLength, bottomLeftPt.y);
		glVertex2i(bottomLeftPt.x+recLength, bottomLeftPt.y+recHeight);

		glVertex2i(bottomLeftPt.x+recLength, bottomLeftPt.y+recHeight);
		glVertex2i(bottomLeftPt.x,           bottomLeftPt.y+recHeight);

		glVertex2i(bottomLeftPt.x,           bottomLeftPt.y+recHeight);
		glVertex2i(bottomLeftPt.x,           bottomLeftPt.y);
	glEnd();
	
	glFlush();
}	// end of display_func()


//@@***********************************************************************************@@
void keyboard_func(unsigned char c, int x, int y)
{
	switch(c)
	{
		case '1' : //bottom left
			bottomLeftPt.x = 5;
			bottomLeftPt.y = 5;
			glutPostRedisplay();
			break;

		case '2' : // bottom middle
			bottomLeftPt.x = 105;
			bottomLeftPt.y = 5;

			glutPostRedisplay();
			break;
		case '3': //bottom right
			bottomLeftPt.x = 205;
			bottomLeftPt.y = 5;

			glutPostRedisplay();
			break;

		case '4': //middle left
			bottomLeftPt.x = 5;
			bottomLeftPt.y = 105;

			glutPostRedisplay();
			break;
		case '5': //middle middle
			bottomLeftPt.x = 105;
			bottomLeftPt.y = 105;

			glutPostRedisplay();
			break;

		case '6': // middle right
			bottomLeftPt.x = 205;
			bottomLeftPt.y = 105;

			glutPostRedisplay();
			break;
		case '7': // top left
			bottomLeftPt.x = 5;
			bottomLeftPt.y = 205;

			glutPostRedisplay();
			break;

		case '8': // top middle
			bottomLeftPt.x = 105;
			bottomLeftPt.y = 205;

			glutPostRedisplay();
			break;
		case '9': // top right
			bottomLeftPt.x = 205;
			bottomLeftPt.y = 205;

			glutPostRedisplay();
			break;


		case 'Q' :
		case 'q' :	
			printf("Good Bye !\n");
			exit(0);				 // terminates the program
	}  // end of switch
}	// end of keyboard_func()

//@@***********************************************************************************@@
void mouse_func(int button, int state, int x, int y)
{	
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		recHeight += 10;

		if((bottomLeftPt.y+recHeight) >= WINDOW_YS)
			recHeight -= 10;

		glutPostRedisplay();
	}
	else if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		recHeight -= 10;

		if(recHeight < 30)
			recHeight = 30;

		glutPostRedisplay();
	}

}
