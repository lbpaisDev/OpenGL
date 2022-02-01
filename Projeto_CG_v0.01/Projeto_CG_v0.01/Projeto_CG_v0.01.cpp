#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include "C:\Users\lbpais\source\repos\Projeto_CG\Projeto_CG\RgbImage.h"

//=================== COLOR DEFINITION ===================//

#define BLUE     0.0, 0.0, 1.0, 1.0
#define RED		 1.0, 0.0, 0.0, 1.0
#define YELLOW	 1.0, 1.0, 0.0, 1.0
#define GREEN    0.0, 1.0, 0.0, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0

//=================== MATH ===================//

#define PI		 3.14159

//=================== VARS AND CONSTANTS ===================//
//----TIME
GLint msec = 1;

//----SCREEN
GLint wScreen = 1920;
GLint hScreen = 1080;
//Limits
GLfloat xC = 10.0, yC = 10.0, zC = 10.0;

//----OBSERVER
GLfloat rVision = 10, aVision = 2 * PI, incVision = 0.05;
GLfloat  angZoom = 90;
GLfloat  incZoom = 3;
//Observer
GLfloat  obsP[] = { rVision * cos(aVision), 3.0, rVision * sin(aVision) };
//Target
GLfloat  obsT[] = { obsP[0] - rVision * cos(aVision), obsP[1], obsP[2] - rVision * sin(aVision) };

//----TEXT
char text[30];

//----CHAIR
//Chair size
GLfloat chairSize = 1.0;
//Translate on x
GLfloat chairX = 0;
//Translate on Z
GLfloat chairZ = 0;
//Rotate on Y
GLfloat chairAngle = 0;
//Inclination of the back part of the chair
GLfloat chairInclination = 0;
//Scale
GLfloat mScaleFactor = 2;

//----PROJECTION
//Ortogonal projection
float angleZ = 95;
int   projectionType = 0;

//----OFFICE
GLfloat officeSize = 25;

//----TEXTURE
RgbImage texture;

//=================== DISPLAY ===================//
//Axis
void drawAxis() {
	//X
	glColor4f(RED);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(xC, 0, 0);
	glEnd();
	//Y
	glColor4f(GREEN);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, yC, 0);
	glEnd();
	//Z
	glColor4f(BLUE);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, zC);
	glEnd();

}

//Office walls
void drawWalls() {
	glColor3f(0.67, 0.4, 0.38);

	glBegin(GL_QUADS);

	glVertex3f(-officeSize, 0, -officeSize);
	glVertex3f(-officeSize, 0, officeSize);
	glVertex3f(-officeSize, officeSize, officeSize);
	glVertex3f(-officeSize, officeSize, -officeSize);

	glEnd();

	glBegin(GL_QUADS);

	glVertex3f(-officeSize, 0, -officeSize);
	glVertex3f(officeSize, 0, -officeSize);
	glVertex3f(officeSize, officeSize, -officeSize);
	glVertex3f(-officeSize, officeSize, -officeSize);

	glEnd();

	glBegin(GL_QUADS);

	glVertex3f(officeSize, 0, -officeSize);
	glVertex3f(officeSize, 0, officeSize);
	glVertex3f(officeSize, officeSize, officeSize);
	glVertex3f(officeSize, officeSize, -officeSize);

	glEnd();

	glBegin(GL_QUADS);

	glVertex3f(officeSize, 0, officeSize);
	glVertex3f(-officeSize, 0, officeSize);
	glVertex3f(-officeSize, officeSize, officeSize);
	glVertex3f(officeSize, officeSize, officeSize);

	glEnd();

}

//Floor
void drawFloor() {
	glColor3f(0.66, 0.47, 0.35);
	glBegin(GL_QUADS);

	glVertex3f(-officeSize, 0, -officeSize);
	glVertex3f(-officeSize, 0, officeSize);
	glVertex3f(officeSize, 0, officeSize);
	glVertex3f(officeSize, 0, -officeSize);

	glEnd();
}

//Ceiling
void drawCeiling() {
	glColor3f(0.76, 0.77, 0.76);
	glBegin(GL_QUADS);

	glVertex3f(-officeSize, officeSize, -officeSize);
	glVertex3f(-officeSize, officeSize, officeSize);
	glVertex3f(officeSize, officeSize, officeSize);
	glVertex3f(officeSize, officeSize, -officeSize);

	glEnd();
}

//Draw chair base
void drawChairBase(GLfloat chairHeight, GLfloat chairWidth, GLfloat chairThick) {
	
	glColor3f(0.862, 0.705, 0.517);
	glBegin(GL_QUADS);
	//Cover	

		glVertex3f(chairWidth, chairHeight, -chairWidth);
		glVertex3f(-chairWidth, chairHeight, -chairWidth);
		glVertex3f(-chairWidth, chairHeight, chairWidth);
		glVertex3f(chairWidth, chairHeight, chairWidth);

		glVertex3f(chairWidth, chairHeight - chairThick, chairWidth);
		glVertex3f(-chairWidth, chairHeight - chairThick, chairWidth);
		glVertex3f(-chairWidth, chairHeight - chairThick, -chairWidth);
		glVertex3f(chairWidth, chairHeight - chairThick, -chairWidth);

		glVertex3f(chairWidth, chairHeight, chairWidth);
		glVertex3f(-chairWidth, chairHeight, chairWidth);
		glVertex3f(-chairWidth, chairHeight - chairThick, chairWidth);
		glVertex3f(chairWidth, chairHeight - chairThick, chairWidth);
		
		glVertex3f(chairWidth, chairHeight - chairThick, -chairWidth);
		glVertex3f(-chairWidth, chairHeight - chairThick, -chairWidth);
		glVertex3f(-chairWidth, chairHeight, -chairWidth);
		glVertex3f(chairWidth, chairHeight, -chairWidth);

		glVertex3f(-chairWidth, chairHeight, chairWidth);
		glVertex3f(-chairWidth, chairHeight, -chairWidth);
		glVertex3f(-chairWidth, chairHeight - chairThick, -chairWidth);
		glVertex3f(-chairWidth, chairHeight - chairThick, chairWidth);

		glVertex3f(chairWidth, chairHeight, -chairWidth);
		glVertex3f(chairWidth, chairHeight, chairWidth);
		glVertex3f(chairWidth, chairHeight - chairThick, chairWidth);
		glVertex3f(chairWidth, chairHeight - chairThick, -chairWidth);

	glEnd();

	glBegin(GL_QUADS);
	//Left leg front
	glColor3f(0.137, 0.090, 0.035);
		glVertex3f(chairWidth, chairHeight - chairThick, chairWidth - chairThick);
		glVertex3f(chairWidth - chairThick, chairHeight - chairThick, chairWidth - chairThick);
		glVertex3f(chairWidth - chairThick, chairHeight - chairThick, chairWidth);
		glVertex3f(chairWidth, chairHeight - chairThick, chairWidth);
	
		glVertex3f(chairWidth, -chairHeight, chairWidth);
		glVertex3f(chairWidth - chairThick, -chairHeight, chairWidth);
		glVertex3f(chairWidth - chairThick, -chairHeight, chairWidth - chairThick);
		glVertex3f(chairWidth, -chairHeight, chairWidth - chairThick);
	
		glVertex3f(chairWidth, chairHeight - chairThick, chairWidth);
		glVertex3f(chairWidth - chairThick, chairHeight - chairThick, chairWidth);
		glVertex3f(chairWidth - chairThick, -chairHeight, chairWidth);
		glVertex3f(chairWidth, -chairHeight, chairWidth);

		glVertex3f(chairWidth, -chairHeight, chairWidth - chairThick);
		glVertex3f(chairWidth - chairThick, -chairHeight, chairWidth - chairThick);
		glVertex3f(chairWidth - chairThick, chairHeight - chairThick, chairWidth - chairThick);
		glVertex3f(chairWidth, chairHeight - chairThick, chairWidth - chairThick);

		glVertex3f(chairWidth - chairThick, chairHeight - chairThick, chairWidth);
		glVertex3f(chairWidth - chairThick, chairHeight - chairThick, chairWidth - chairThick);
		glVertex3f(chairWidth - chairThick, -chairHeight, chairWidth - chairThick);
		glVertex3f(chairWidth - chairThick, -chairHeight, chairWidth);

		glVertex3f(chairWidth, chairHeight - chairThick, chairWidth - chairThick);
		glVertex3f(chairWidth, chairHeight - chairThick, chairWidth);
		glVertex3f(chairWidth, -chairHeight, chairWidth);
		glVertex3f(chairWidth, -chairHeight, chairWidth - chairThick);

	glEnd();

	glBegin(GL_QUADS);
	//Left leg back
	glColor3f(0.137, 0.090, 0.035);
		glVertex3f(-chairWidth, chairHeight - chairThick, chairWidth - chairThick);
		glVertex3f(-chairWidth + chairThick, chairHeight - chairThick, chairWidth - chairThick);
		glVertex3f(-chairWidth + chairThick, chairHeight - chairThick, chairWidth);
		glVertex3f(-chairWidth, chairHeight - chairThick, chairWidth);

		glVertex3f(-chairWidth, -chairHeight, chairWidth);
		glVertex3f(-chairWidth + chairThick, -chairHeight, chairWidth);
		glVertex3f(-chairWidth + chairThick, -chairHeight, chairWidth - chairThick);
		glVertex3f(-chairWidth, -chairHeight, chairWidth - chairThick);
	
		glVertex3f(-chairWidth, chairHeight - chairThick, chairWidth);
		glVertex3f(-chairWidth + chairThick, chairHeight - chairThick, chairWidth);
		glVertex3f(-chairWidth + chairThick, -chairHeight, chairWidth);
		glVertex3f(-chairWidth, -chairHeight, chairWidth);

		glVertex3f(-chairWidth, -chairHeight, chairWidth - chairThick);
		glVertex3f(-chairWidth + chairThick, -chairHeight, chairWidth - chairThick);
		glVertex3f(-chairWidth + chairThick, chairHeight - chairThick, chairWidth - chairThick);
		glVertex3f(-chairWidth, chairHeight - chairThick, chairWidth - chairThick);
	
		glVertex3f(-chairWidth + chairThick, chairHeight - chairThick, chairWidth);
		glVertex3f(-chairWidth + chairThick, chairHeight - chairThick, chairWidth - chairThick);
		glVertex3f(-chairWidth + chairThick, -chairHeight, chairWidth - chairThick);
		glVertex3f(-chairWidth + chairThick, -chairHeight, chairWidth);

		glVertex3f(-chairWidth, chairHeight - chairThick, chairWidth - chairThick);
		glVertex3f(-chairWidth, chairHeight - chairThick, chairWidth);
		glVertex3f(-chairWidth, -chairHeight, chairWidth);
		glVertex3f(-chairWidth, -chairHeight, chairWidth - chairThick);

	glEnd();

	glBegin(GL_QUADS);
	//Right leg back
	glColor3f(0.137, 0.090, 0.035);
		glVertex3f(-chairWidth, chairHeight - chairThick, -chairWidth + chairThick);
		glVertex3f(-chairWidth + chairThick, chairHeight - chairThick, -chairWidth + chairThick);
		glVertex3f(-chairWidth + chairThick, chairHeight - chairThick, -chairWidth);
		glVertex3f(-chairWidth, chairHeight - chairThick, -chairWidth);
	
		glVertex3f(-chairWidth, -chairHeight, -chairWidth);
		glVertex3f(-chairWidth + chairThick, -chairHeight, -chairWidth);
		glVertex3f(-chairWidth + chairThick, -chairHeight, -chairWidth + chairThick);
		glVertex3f(-chairWidth, -chairHeight, -chairWidth + chairThick);
	
		glVertex3f(-chairWidth, chairHeight - chairThick, -chairWidth);
		glVertex3f(-chairWidth + chairThick, chairHeight - chairThick, -chairWidth);
		glVertex3f(-chairWidth + chairThick, -chairHeight, -chairWidth);
		glVertex3f(-chairWidth, -chairHeight, -chairWidth);

		glVertex3f(-chairWidth, -chairHeight, -chairWidth + chairThick);
		glVertex3f(-chairWidth + chairThick, -chairHeight, -chairWidth + chairThick);
		glVertex3f(-chairWidth + chairThick, chairHeight - chairThick, -chairWidth + chairThick);
		glVertex3f(-chairWidth, chairHeight, -chairWidth + chairThick);

		glVertex3f(-chairWidth + chairThick, chairHeight - chairThick, -chairWidth);
		glVertex3f(-chairWidth + chairThick, chairHeight - chairThick, -chairWidth + chairThick);
		glVertex3f(-chairWidth + chairThick, -chairHeight,-chairWidth + chairThick);
		glVertex3f(-chairWidth + chairThick, -chairHeight, -chairWidth);

		glVertex3f(-chairWidth, chairHeight - chairThick, -chairWidth + chairThick);
		glVertex3f(-chairWidth, chairHeight - chairThick, -chairWidth);
		glVertex3f(-chairWidth, -chairHeight, -chairWidth);
		glVertex3f(-chairWidth, -chairHeight, -chairWidth + chairThick);

	glEnd();
	
	glBegin(GL_QUADS);
	//Right leg front
	glColor3f(0.137, 0.090, 0.035);
		glVertex3f(chairWidth, chairHeight - chairThick,-chairWidth + chairThick);
		glVertex3f(chairWidth - chairThick, chairHeight - chairThick, -chairWidth + chairThick);
		glVertex3f(chairWidth - chairThick, chairHeight - chairThick, -chairWidth);
		glVertex3f(chairWidth, chairHeight - chairThick, -chairWidth);

		glVertex3f(chairWidth, -chairHeight, -chairWidth);
		glVertex3f(chairWidth - chairThick, -chairHeight, -chairWidth);
		glVertex3f(chairWidth - chairThick, -chairHeight, -chairWidth + chairThick);
		glVertex3f(chairWidth, -chairHeight, -chairWidth + chairThick);

		glVertex3f(chairWidth, chairHeight - chairThick, -chairWidth);
		glVertex3f(chairWidth - chairThick, chairHeight - chairThick, -chairWidth);
		glVertex3f(chairWidth - chairThick, -chairHeight, -chairWidth);
		glVertex3f(chairWidth, -chairHeight, -chairWidth);

		glVertex3f(chairWidth, -chairHeight, -chairWidth + chairThick);
		glVertex3f(chairWidth - chairThick, -chairHeight, -chairWidth + chairThick);
		glVertex3f(chairWidth - chairThick, chairHeight - chairThick, -chairWidth + chairThick);
		glVertex3f(chairWidth, chairHeight, -chairWidth + chairThick);

		glVertex3f(chairWidth - chairThick, chairHeight - chairThick, -chairWidth);
		glVertex3f(chairWidth - chairThick, chairHeight - chairThick, -chairWidth + chairThick);
		glVertex3f(chairWidth - chairThick, -chairHeight, -chairWidth + chairThick);
		glVertex3f(chairWidth - chairThick, -chairHeight, -chairWidth);

		glVertex3f(chairWidth, chairHeight - chairThick, -chairWidth + chairThick);
		glVertex3f(chairWidth, chairHeight - chairThick, -chairWidth);
		glVertex3f(chairWidth, -chairHeight, -chairWidth);
		glVertex3f(chairWidth, -chairHeight, -chairWidth + chairThick);

	glEnd();

}

//Draw chair base
void drawChairUpper(GLfloat chairHeight, GLfloat chairWidth, GLfloat chairThick){

	glColor3f(0.4, 0.2, 0);
	glBegin(GL_QUADS);
	//Back
		glVertex3f(-chairWidth, chairHeight, -chairWidth);
		glVertex3f(-chairWidth + chairThick, chairHeight, -chairWidth);
		glVertex3f(-chairWidth + chairThick, chairHeight, chairWidth);
		glVertex3f(-chairWidth, chairHeight, chairWidth);

		glVertex3f(-chairWidth, 3*chairHeight, chairWidth);	
		glVertex3f(-chairWidth + chairThick, 3*chairHeight, chairWidth);
		glVertex3f(-chairWidth + chairThick, 3*chairHeight, -chairWidth);
		glVertex3f(-chairWidth, 3*chairHeight, -chairWidth);
	
		glVertex3f(-chairWidth, chairHeight, chairWidth);
		glVertex3f(-chairWidth + chairThick, chairHeight, chairWidth);
		glVertex3f(-chairWidth + chairThick, 3*chairHeight, chairWidth);
		glVertex3f(-chairWidth, 3*chairHeight, chairWidth);
	
		glVertex3f(-chairWidth, 3*chairHeight, -chairWidth);
		glVertex3f(-chairWidth + chairThick, 3*chairHeight, -chairWidth);
		glVertex3f(-chairWidth + chairThick, chairHeight, -chairWidth);
		glVertex3f(-chairWidth, chairHeight, -chairWidth);

		glVertex3f(-chairWidth + chairThick, chairHeight, chairWidth);
		glVertex3f(-chairWidth + chairThick, chairHeight, -chairWidth);
		glVertex3f(-chairWidth + chairThick, 3*chairHeight, -chairWidth);
		glVertex3f(-chairWidth + chairThick, 3*chairHeight, chairWidth);

		glVertex3f(-chairWidth, chairHeight, -chairWidth);
		glVertex3f(-chairWidth, chairHeight, chairWidth);
		glVertex3f(-chairWidth, 3*chairHeight, chairWidth);
		glVertex3f(-chairWidth, 3*chairHeight, -chairWidth);

	glEnd();

	glBegin(GL_QUADS);
	//Right Arm Upper
	glColor3f(0.137, 0.090, 0.035);
	glVertex3f(chairWidth, 1.25 * chairHeight, chairWidth - 2*chairThick);
	glVertex3f(-chairWidth + chairThick, 1.25 * chairHeight, chairWidth - 2*chairThick);
	glVertex3f(-chairWidth + chairThick, 1.25 * chairHeight, chairWidth);
	glVertex3f(chairWidth, 1.25 * chairHeight, chairWidth);

	glVertex3f(chairWidth, 1.5 * chairHeight, chairWidth);
	glVertex3f(-chairWidth + chairThick, 1.5 * chairHeight, chairWidth);
	glVertex3f(-chairWidth + chairThick, 1.5 * chairHeight, chairWidth - 2 * chairThick);
	glVertex3f(chairWidth, 1.5 * chairHeight, chairWidth - 2 * chairThick);

	glVertex3f(chairWidth, 1.25 * chairHeight, chairWidth);
	glVertex3f(-chairWidth + chairThick, 1.25 * chairHeight, chairWidth);
	glVertex3f(-chairWidth + chairThick, 1.5 * chairHeight, chairWidth);
	glVertex3f(chairWidth, 1.5 * chairHeight, chairWidth);

	glVertex3f(chairWidth, 1.5 * chairHeight, chairWidth - 2 * chairThick);
	glVertex3f(-chairWidth + chairThick, 1.5 * chairHeight, chairWidth - 2 * chairThick);
	glVertex3f(-chairWidth + chairThick, 1.25 * chairHeight, chairWidth - 2 * chairThick);
	glVertex3f(chairWidth, 1.25 * chairHeight, chairWidth - 2 * chairThick);

	glVertex3f(-chairWidth + chairThick, 1.25 * chairHeight, chairWidth);
	glVertex3f(-chairWidth + chairThick, 1.25 * chairHeight, chairWidth - 2 * chairThick);
	glVertex3f(-chairWidth + chairThick, 1.5 * chairHeight, chairWidth - 2 * chairThick);
	glVertex3f(-chairWidth + chairThick, 1.5 * chairHeight, chairWidth);

	glVertex3f(chairWidth, 1.25 * chairHeight, chairWidth - 2 * chairThick);
	glVertex3f(chairWidth, 1.25 * chairHeight, chairWidth);
	glVertex3f(chairWidth, 1.5 * chairHeight, chairWidth);
	glVertex3f(chairWidth, 1.5 * chairHeight, chairWidth - 2 * chairThick);

	glEnd();


	glBegin(GL_QUADS);
	//Right Arm Upper
	glColor3f(0.137, 0.090, 0.035);
	glVertex3f(chairWidth, 1.25 * chairHeight, -chairWidth + 2 * chairThick);
	glVertex3f(-chairWidth + chairThick, 1.25 * chairHeight, -chairWidth + 2 * chairThick);
	glVertex3f(-chairWidth + chairThick, 1.25 * chairHeight, -chairWidth);
	glVertex3f(chairWidth, 1.25 * chairHeight, -chairWidth);

	glVertex3f(chairWidth, 1.5 * chairHeight, -chairWidth);
	glVertex3f(-chairWidth + chairThick, 1.5 * chairHeight, -chairWidth);
	glVertex3f(-chairWidth + chairThick, 1.5 * chairHeight, -chairWidth + 2 * chairThick);
	glVertex3f(chairWidth, 1.5 * chairHeight, -chairWidth + 2 * chairThick);

	glVertex3f(chairWidth, 1.25 * chairHeight, -chairWidth);
	glVertex3f(-chairWidth + chairThick, 1.25 * chairHeight, -chairWidth);
	glVertex3f(-chairWidth + chairThick, 1.5 * chairHeight, -chairWidth);
	glVertex3f(chairWidth, 1.5 * chairHeight, -chairWidth);

	glVertex3f(chairWidth, 1.5 * chairHeight, -chairWidth + 2 * chairThick);
	glVertex3f(-chairWidth + chairThick, 1.5 * chairHeight, -chairWidth + 2 * chairThick);
	glVertex3f(-chairWidth + chairThick, 1.25 * chairHeight, -chairWidth + 2 * chairThick);
	glVertex3f(chairWidth, 1.25 * chairHeight, -chairWidth + 2 * chairThick);

	glVertex3f(-chairWidth + chairThick, 1.25 * chairHeight, -chairWidth);
	glVertex3f(-chairWidth + chairThick, 1.25 * chairHeight, -chairWidth + 2 * chairThick);
	glVertex3f(-chairWidth + chairThick, 1.5 * chairHeight, -chairWidth + 2 * chairThick);
	glVertex3f(-chairWidth + chairThick, 1.5 * chairHeight, -chairWidth);

	glVertex3f(chairWidth, 1.25 * chairHeight, -chairWidth + 2 * chairThick);
	glVertex3f(chairWidth, 1.25 * chairHeight, -chairWidth);
	glVertex3f(chairWidth, 1.5 * chairHeight, -chairWidth);
	glVertex3f(chairWidth, 1.5 * chairHeight, -chairWidth + 2 * chairThick);

	glEnd();

}


//Scene
void drawScene() {


	//World
	drawWalls();
	drawFloor();
	drawCeiling();

	//Draw chair with respective movement
	glPushMatrix();
	glTranslatef(chairX, 0, chairZ);
	glRotatef(chairAngle, 0.0, 1.0, 0.0);
	glPushMatrix();
	glScalef(2, mScaleFactor, 2);
	glTranslatef(0, 1, 0);
	glPushMatrix();
	drawChairBase(1, 1, 0.1);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glScalef(2, 2, 2);
	glTranslatef(0, mScaleFactor - 1, 0);
	glTranslatef(-1, 1, 0);
	glRotatef(chairInclination, 0.0, 0.0, 1.0);
	glTranslatef(1, -1, 0);
	drawChairUpper(1, 1, 0.1);
	glPopMatrix();
	glPopMatrix();

}

//Text
void drawText(char* string, GLfloat x, GLfloat y) {
	glPushMatrix();
	glTranslatef(x, 0, y);
	glRasterPos2f(0, 0);
	while (*string)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *string++);
	glPopMatrix();
}

//Display
void display() {
	//Clear screen and depth
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Main display
	glViewport(0, 0, wScreen, hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (projectionType == 0)
		gluPerspective(angleZ, (float)wScreen / hScreen, 0.1, 9999);
	if (projectionType == 1)
		glOrtho(-100, 100, -100, 100, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], obsT[0], obsT[1], obsT[2], 0, 1, 0);

	//Main draw
	drawAxis();
	drawScene();

	//Update
	glutSwapBuffers();
}

//=================== KEYBOARD ===================//
//----ASCII
void kASCII(unsigned char key, int x, int y) {
	switch (key) {
		//Move chair to the left
	case 'A':
	case 'a':
		if (chairZ + (chairSize + 1) >= officeSize) {
			chairZ = 25 - (chairSize + 1);
		}
		else {
			chairZ = chairZ + 0.5;
		}
		glutPostRedisplay();
		break;

		//Move chair to the right
	case 'D':
	case 'd':
		if (chairZ - (chairSize + 1) <= -officeSize) {
			chairZ = -25 + (chairSize + 1);
		}
		else {
			chairZ = chairZ - 0.5;
		}
		glutPostRedisplay();
		break;

		//Move chair forward
	case 'W':
	case 'w':
		if (chairX - (chairSize + 1.5) <= -officeSize) {
			chairX = -25 + (chairSize + 1.5);
		}
		else {
			chairX = chairX - 0.5;
		}
		glutPostRedisplay();
		break;

		//Move chair backwards
	case 'S':
	case 's':
		if (chairX + (chairSize + 1.5) >= officeSize) {
			chairX = 25 - (chairSize + 1.5);
		}
		else {
			chairX = chairX + 0.5;
		}
		glutPostRedisplay();
		break;

		//Rotate chair to the right
	case 'e':
	case 'E':
		chairAngle = chairAngle + 0.5;
		glutPostRedisplay();
		break;

		//Incline chair back
	case 'x':
	case 'X':
		if (chairInclination <= -5) {
			chairInclination = -5;
		}
		else {
			chairInclination = chairInclination - 0.5;
		}
		glutPostRedisplay();
		break;

		//Incline chair front
	case 'z':
	case 'Z':
		if (chairInclination >= 45) {
			chairInclination = 45;

		}
		else {
			chairInclination = chairInclination + 0.5;
		}
		glutPostRedisplay();
		break;

		//Rotate chair to the left
	case 'q':
	case 'Q':
		chairAngle = chairAngle - 0.5;
		glutPostRedisplay();
		break;

		//Increase height
	case 'r':
	case 'R':
		if (mScaleFactor >= 2) {
			mScaleFactor = 2;
		}
		else {
			mScaleFactor += 0.1;
		}
		glutPostRedisplay();
		break;

		//Increase decrease height
	case 't':
	case 'T':
		if (mScaleFactor <= 1) {
			mScaleFactor = 1;
		}
		else {
			mScaleFactor -= 0.1;
		}
		glutPostRedisplay();
		break;

		//Projection type
	case 'p':
	case 'P':
		projectionType = (projectionType + 1) % 2;
		glutPostRedisplay();
		break;

		//Exit
	case 27:
		exit(0);
		break;
	}

}

//----NOT ASCII
void knASCII(int key, int x, int y) {
	//Observer position
	if (key == GLUT_KEY_UP)   obsP[1] = (obsP[1] + 0.1);
	if (key == GLUT_KEY_DOWN) obsP[1] = (obsP[1] - 0.1);

	//Viewing angle
	if (key == GLUT_KEY_LEFT)  aVision = (aVision + 0.1);
	if (key == GLUT_KEY_RIGHT) aVision = (aVision - 0.1);

	//Limit height
	if (obsP[1] >= yC)   obsP[1] = yC;
	if (obsP[1] <= 0.1)  obsP[1] = 0.1;
	obsP[0] = rVision * cos(aVision);
	obsP[2] = rVision * sin(aVision);

	glutPostRedisplay();
}

//=================== INIT ===================//
void init(void) {
	glClearColor(BLACK);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}

//=================== TIMER ===================//
void Timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(msec, Timer, 1);
}

//=================== MAIN ===================//
int main(int argc, char** argv) {

	//----GLUT WINDOW
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(400, 100);
	glutCreateWindow(" |lbpais@student.dei.uc.pt || Office || Observer: 'ARROWS' || Move: 'W/S/A/D' || Rotate: 'Q/E' || Resize: 'R/T' || Incline: 'Z/X' | ");

	//----INIT
	init();

	//----ASSIGN FUNCTIONS
	glutSpecialFunc(knASCII);
	glutDisplayFunc(display);
	glutKeyboardFunc(kASCII);

	//----TIMER
	glutTimerFunc(msec, Timer, 1);

	//----LOOP
	glutMainLoop();
}
