/*
*** FILE NAME   : animation_ex.c
*** DESCRIPTION : This program is an animation example program using OpenGL
*** DATE        : Sep. 2017
*** WRITTEN By  : JKL
*/

#include <stdio.h>               // standard C libraries
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <GL/glut.h>
#include <iostream>
#include <string>// GLUT library
#include <time.h>
using namespace std;

//@@***********************************************************************************@@
// Constants
#define WINDOW_XS 300							// Window size
#define WINDOW_YS 400
#define WINDOW_NAME "8 Puzzle"			// Window name

#define ANI_MSEC 10						// gap between frames

//@@***********************************************************************************@@

class Tile {
public:
	Tile::Tile(int, int, char*);
	char* label;
	int x, y;
	// Overload= operator to compare 2 tiles
	bool operator==(const Tile& CompareTile) {
		if (this->label == CompareTile.label && this->x == CompareTile.x && this->y == CompareTile.y) {
			return true;
		}
		else {
			return false;
		}
	}
};
Tile::Tile(int a, int b, char* SetLabel) {
	x = a;
	y = b;
	label = SetLabel;
}

//@@***********************************************************************************@@
// Global Variables

Tile One(0, 200, "1");
Tile Two(100, 200, "2");
Tile Three(200, 200, "3");
Tile Four(0, 100, "4");
Tile Five(100, 100, "5");
Tile Six(200, 100, "6");
Tile Seven(0, 0, "7");
Tile Eight(100, 0, "8");
Tile Blank(200, 0, "-1");

Tile GamBoard[3][3] = { { Seven, Four , One },{ Eight , Five , Two },{ Blank , Six, Three } };
Tile GoalState[3][3] = { { Seven, Four , One },{ Eight , Five , Two },{ Blank , Six, Three } };
Tile StartingGamBoard[3][3] = { { Seven, Four , One },{ Eight , Five , Two },{ Blank , Six, Three } };

int recLength = 90;		// size of puzzle
int recHeight = 90;

int offSetX = 5;
int offSetY = 5;

int doAmination = 0;	// =0 no animation, 1= animation
int gap = 10;			// step for animation

int MoveRow, MoveColumn;
bool UserMode = false;
int UserMoves = 0;

//@@***********************************************************************************@@
// Function prototypes
void reshape_handler(int width, int height);
void init_setup(int width, int height, char *windowName);
void display_func(void);
void keyboard_func(unsigned char c, int x, int y);
void mouse_func(int button, int state, int x, int y);
void animation_func(int val);
void output(int x, int y, int mode, char *string);
void DrawTile(Tile ToDraw);
string FindEmptySpace(int row, int column);
void randomize(Tile GamBoard[3][3]);
void MoveTile(int row, int column);
void SetTileLocations(Tile GamBoard[3][3]);
int NumTilesOutOfPlace(Tile GamBoard[3][3]);
int DistanceToGoal(Tile GamBoard[3][3]);
int SingleTileDistance(int Row, int Column, char* Label);

//@@***********************************************************************************@@
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	init_setup(WINDOW_XS, WINDOW_YS, WINDOW_NAME);

	glutDisplayFunc(display_func);
	glutKeyboardFunc(keyboard_func);
	glutMouseFunc(mouse_func);
	glutTimerFunc(ANI_MSEC, animation_func, 0);

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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  // double buffer, rgb color
	glutInitWindowSize(width, height);			  // init. window size
	glutInitWindowPosition(5, 5);				  // init. window position
	glutCreateWindow(windowName);				  // window name
	glutReshapeFunc(reshape_handler);		      // sets the reshape call back
	randomize(GamBoard);
}	// end of init_setup()

	//@@***********************************************************************************@@
void display_func(void)
{
	glClearColor(1.000, 0.961, 0.933, 1.0);   // background color (light-grey)
	glClear(GL_COLOR_BUFFER_BIT);         // clearing the buffer not to keep the color

										  // draw top of the puzzle board
	glColor3f(0.863, 0.863, 0.863);				// setting pen color
	glBegin(GL_POLYGON);
	glVertex2i(0, 300);
	glVertex2i(300, 300);
	glVertex2i(300, 400);
	glVertex2i(0, 400);
	glEnd();

	// banner
	glColor3f(0.255, 0.412, 0.882);					// setting pen color
	output(70, 360, 1, "CS4200/5200: 8 Puzzle");	// puzzle number
	output(150, 320, 2, "Ben Thornton");	// puzzle number

											// draw the grids for the puzzle board
	glColor3f(0.412, 0.412, 0.412);			// setting pen color
	glBegin(GL_LINES);
	for (int i = 0; i < WINDOW_XS; i += 100)
	{
		glVertex2i(i, 0);		// vertical grid lines
		glVertex2i(i, 300);

		glVertex2i(0, i);		// horizontal grid lines
		glVertex2i(300, i);
	}
	glEnd();

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (GamBoard[i][j].label != "-1") {
				DrawTile(GamBoard[i][j]);
			}
		}
	}
	glFlush();

	glutSwapBuffers();	// double buffering
}	// end of display_func()

void DrawTile(Tile ToDraw) {
	// draw a puzzle piece
	glColor3f(0.753, 0.753, 0.753);			// setting pen color
	glBegin(GL_POLYGON);
	glVertex2i(ToDraw.x + offSetX, ToDraw.y + offSetY);
	glVertex2i(ToDraw.x + offSetX + recLength, ToDraw.y + offSetY);
	glVertex2i(ToDraw.x + offSetX + recLength, ToDraw.y + offSetY + recHeight);
	glVertex2i(ToDraw.x + offSetX, +ToDraw.y + offSetY + recHeight);
	glEnd();
	glColor3f(0.502, 0.000, 0.000);
	glBegin(GL_LINE_LOOP);
	glVertex2i(ToDraw.x + offSetX, ToDraw.y + offSetY);
	glVertex2i(ToDraw.x + offSetX + recLength, ToDraw.y + offSetY);
	glVertex2i(ToDraw.x + offSetX + recLength, ToDraw.y + offSetY + recHeight);
	glVertex2i(ToDraw.x + offSetX, +ToDraw.y + offSetY + recHeight);
	glEnd();
	glColor3f(0.502, 0.000, 0.000);			// setting pen color
	output(ToDraw.x + offSetX + 40, ToDraw.y + offSetY + 40, 1, ToDraw.label);	// puzzle number
	glFlush();
}
//@@***********************************************************************************@@
void animation_func(int val)
{
	if (doAmination == 1)
	{
		GamBoard[MoveRow][MoveColumn].x += gap;
		if (GamBoard[MoveRow][MoveColumn].x % 100 == 0) {
			doAmination = 0;
		}
	}
	else if (doAmination == 2) {
		GamBoard[MoveRow][MoveColumn].y += gap;
		if (GamBoard[MoveRow][MoveColumn].y % 100 == 0) {
			doAmination = 0;
		}
	}
	if (doAmination == 3)
	{
		GamBoard[MoveRow][MoveColumn].x -= gap;
		if (GamBoard[MoveRow][MoveColumn].x % 100 == 0) {
			doAmination = 0;
		}
	}
	else if (doAmination == 4) {
		GamBoard[MoveRow][MoveColumn].y -= gap;
		if (GamBoard[MoveRow][MoveColumn].y % 100 == 0) {
			doAmination = 0;
		}
	}

	glutPostRedisplay();

	glutTimerFunc(ANI_MSEC, animation_func, 0);
}
//@@***********************************************************************************@@
void output(int x, int y, int mode, char* string)
{
	int len, i;
	glRasterPos2i(x, y);
	len = (int)strlen(string);
	if (mode == 1)
	{
		for (i = 0; i < len; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
		}
	}
	else if (mode == 2)
	{
		for (i = 0; i < len; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
		}
	}
}
//@@***********************************************************************************@@
void keyboard_func(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 'S':
	case 's':
		UserMoves = 0;
		UserMode=true;
		break;
	case 'I':
	case 'i':
		randomize(GamBoard);
		UserMode = false;
		break;
	case 'K':
	case 'k':
		DistanceToGoal(GamBoard);
		UserMode = false;
		break;
	case 'J':
	case 'j':
		NumTilesOutOfPlace(GamBoard);
		UserMode = false;
		break;
	case 'Q':
	case 'q':
		printf("Good Bye !\n");
		exit(0);				 // terminates the program
	}
}
//@@***********************************************************************************@@
void mouse_func(int button, int state, int x, int y)
{
	if (UserMode) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if (WINDOW_YS - y < 301) {
				MoveTile((int)x / 100, (int)(WINDOW_YS - y) / 100);
			}
			glutPostRedisplay();
		}
	}
}
//@@***********************************************************************************@@
void randomize(Tile GamBoard[3][3])
{
	srand(time(NULL));

	// Start from the last element and swap one by one. We don't
	// need to run for the first element that's why i > 0
	for (int i = 2; i >= 0; i--)
	{
		for (int j = 2; j >= 0; j--) {
			// Pick a random index from 0 to i
			int RandRow = rand() % (3);
			int RandCol = rand() % (3);
			Tile temp = GamBoard[i][j];
			GamBoard[i][j] = GamBoard[RandRow][RandCol];
			GamBoard[RandRow][RandCol] = temp;
		}
	}

	SetTileLocations(GamBoard);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++) {
			StartingGamBoard[i][j] = GamBoard[i][j];
		}
	}
}
//@@***********************************************************************************@@
void SetTileLocations(Tile GamBoard[3][3]) {
	// loop through tile and set the tiles x and y cords in respect to their location in the 2d arrat
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++) {
			GamBoard[i][j].x = i * 100;
			GamBoard[i][j].y = (j * 100);
		}
	}
}
//@@***********************************************************************************@@
void MoveTile(int row, int column) {
	string MoveCommand = "";
	MoveCommand = FindEmptySpace(row, column);
	UserMoves++;
	if (MoveCommand != "locked") {
		if (MoveCommand == "right") {
			GamBoard[row + 1][column] = GamBoard[row][column];
			GamBoard[row][column] = Blank;
			doAmination = 1;
			MoveRow = row + 1;
			MoveColumn = column;
		}
		else if (MoveCommand == "left") {
			GamBoard[row - 1][column] = GamBoard[row][column];
			GamBoard[row][column] = Blank;
			doAmination = 3;
			MoveRow = row - 1;
			MoveColumn = column;
		}
		else if (MoveCommand == "up") {
			GamBoard[row][column + 1] = GamBoard[row][column];
			GamBoard[row][column] = Blank;
			doAmination = 2;
			MoveRow = row;
			MoveColumn = column + 1;
		}
		else if (MoveCommand == "down") {
			GamBoard[row][column - 1] = GamBoard[row][column];
			GamBoard[row][column] = Blank;
			doAmination = 4;
			MoveRow = row;
			MoveColumn = column - 1;
		}
	}
	else {
		UserMoves--;
	}
}
//@@***********************************************************************************@@
string FindEmptySpace(int row, int column) {
	//returns where adjacent to the tile the empty space is located
	//left
	if (row > 0) {
		if (GamBoard[row - 1][column].label == "-1") {
			return"left";
		}
	}
	//right
	if (row < 2) {
		if (GamBoard[row + 1][column].label == "-1") {
			return"right";
		}
	}
	//down
	if (column > 0) {
		if (GamBoard[row][column - 1].label == "-1") {
			return"down";
		}
	}
	//up
	if (column < 2) {
		if (GamBoard[row][column + 1].label == "-1") {
			return"up";
		}
	}
	//locked
	return "locked";
}
//@@***********************************************************************************@@
int NumTilesOutOfPlace(Tile GamBoard[3][3]) {
	int NumMissPlaced = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++) {
			if (GamBoard[i][j] == GoalState[i][j]) {
			}
			else {
				NumMissPlaced++;
			}
		}
	}
	cout << "Misplaced tiles : " << NumMissPlaced << endl;
	return NumMissPlaced;
}
int DistanceToGoal(Tile GamBoard[3][3]) {
	int TotalDistance = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++) {
			TotalDistance += SingleTileDistance(i, j, GamBoard[i][j].label);
		}
	}
	cout << "Total Distance = " << TotalDistance << endl;
	return TotalDistance;
}
//@@***********************************************************************************@@
int SingleTileDistance(int Row, int Column, char* Label) {
	if (Label == "1") {
		cout << "distance for " << Label << " = " << abs(Row - 0) + abs(Column - 2) << endl;
		return abs(Row - 0) + abs(Column - 2);
	}
	else if (Label == "2") {
		cout << "distance for " << Label << " = " << abs(Row - 1) + abs(Column - 2) << endl;
		return abs(Row - 1) + abs(Column - 2);
	}
	else if (Label == "3") {
		cout << "distance for " << Label << " = " << abs(Row - 2) + abs(Column - 2) << endl;
		return abs(Row - 2) + abs(Column - 2);
	}
	else if (Label == "4") {
		cout << "distance for " << Label << " = " << abs(Row - 0) + abs(Column - 1) << endl;
		return abs(Row - 0) + abs(Column - 1);
	}
	else if (Label == "5") {
		cout << "distance for " << Label << " = " << abs(Row - 1) + abs(Column - 1) << endl;
		return abs(Row - 1) + abs(Column - 1);
	}
	else if (Label == "6") {
		cout << "distance for " << Label << " = " << abs(Row - 2) + abs(Column - 1) << endl;
		return abs(Row - 2) + abs(Column - 1);
	}
	else if (Label == "7") {
		cout << "distance for " << Label << " = " << abs(Row - 0) + abs(Column - 0) << endl;
		return abs(Row - 0) + abs(Column - 0);
	}
	else if (Label == "8") {
		cout << "distance for " << Label << " = " << abs(Row - 1) + abs(Column - 0) << endl;
		return abs(Row - 1) + abs(Column - 0);
	}
	else if (Label == "-1") {
		cout << "distance for " << Label << " = " << abs(Row - 2) + abs(Column - 0) << endl;
		return abs(Row - 2) + abs(Column - 0);
	}
}//@@***********************************************************************************@@