#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL\glut.h>
#include "Headers/RgbImage.h"

/*
 *
 *=========================@DEFINE VARS AND CONSTANTS
 *
 */
//Screen
#define SCREEN_FPS    60
#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

//Colors
#define COLOR_BLUE     0.0, 0.0, 1.0, 1.0
#define COLOR_RED	   1.0, 0.0, 0.0, 1.0
#define COLOR_YELLOW   1.0, 1.0, 0.0, 1.0
#define COLOR_GREEN    0.0, 1.0, 0.0, 1.0
#define COLOR_WHITE    1.0, 1.0, 1.0, 1.0
#define COLOR_BLACK    0.0, 0.0, 0.0, 1.0

//Math
#define PI             3.14159

//Textures
RgbImage imag;
GLuint   texture[9];

//Materials
void initMaterials(int material);
char Materiais[19][30] = {
    "Esmerald",  "Jade",  "obsidian",    "Pearl",        "Ruby",
    "Turquoise", "Brass", "Bronze",      "Chrome",       "Copper",
    "Gold",      "Silver","blackPlastic","cyankPlastic", "greenPlastic",
    "redPlastic", "whitePlastic","yellowPlastic", "default"
};
GLint tablemat = 0;

//Observer
GLfloat  rVision = 10, aVision = 2 * PI, incVision = 0.05;
GLfloat  angZoom = 90;
GLfloat  incZoom = 3;
GLfloat  obsP[] = { rVision * cos(aVision), 3.0, rVision * sin(aVision) };
GLfloat  obsT[] = { obsP[0] - rVision * cos(aVision), obsP[1], obsP[2] - rVision * sin(aVision) };

//Chair
GLfloat chairSize = 1.0;
GLfloat chairX = 0;
GLfloat chairZ = 0;
GLfloat chairAngle = 0;
GLfloat chairInclination = 0;
GLfloat mScaleFactor = 2;

//Office
GLfloat officeSize = 25;

//Light
//Ambient light
GLint ambState = 1;
GLint ambIntensity = 0.3;
GLfloat ambColor[4] = { ambIntensity, ambIntensity, ambIntensity, 1.0 };

//Ceiling lamp
GLint cLampState = 1;
GLfloat cLampIntensity = 0.3;
GLint cLampR = 1;
GLint cLampG = 1;
GLint cLampB = 1;
GLfloat cLampPos[4] = { 0, officeSize - 2, 1.0 };
GLfloat cLampColorAmb[4] = { cLampR, cLampG, cLampB, 1.0 };
GLfloat cLampColorDif[4] = { cLampR, cLampG, cLampB, 1.0 };
GLfloat cLampColorSpc[4] = { cLampR, cLampG, cLampB, 1.0 };
GLfloat cLampAttCon = 0.1;
GLfloat cLampAttLin = 0.1;
GLfloat cLampAttQua = 0.1;

//Desk Lamp
GLint dLampState = 1;
GLfloat dLampIntensity = 0.3;
GLint dLampR = 1;
GLint dLampG = 1;
GLint dLampB = 1;
GLfloat dLampPos[4] = { officeSize, officeSize-3, 0 };
GLfloat dLampColorAmb[4] = { dLampR, dLampG, dLampB, 1.0 };
GLfloat dLampColorDif[4] = { dLampR, dLampG, dLampB, 1.0 };
GLfloat dLampColorSpc[4] = { dLampR, dLampG, dLampB, 1.0 };
GLfloat dLampAttCon = 0.1;
GLfloat dLampAttLin = 0.1;
GLfloat dLampAttQua = 0.1;


/*
 *
 *=========================@Support Functions
 *
 */
//Load textures
void loadTexture(const char *textName, GLfloat param, GLuint *textArr, int ntext) {
    glGenTextures(1, &textArr[ntext]);
    glBindTexture(GL_TEXTURE_2D, textArr[ntext]);
    imag.LoadBmpFile(textName);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, param);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
        imag.GetNumCols(),
        imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        imag.ImageData());
    printf("%s loaded!\n", textName);
}

//Alternative solid cube draw to apply materials/textures
static void drawBox(GLfloat size, GLenum type) {
    static GLfloat n[6][3] =
    {
      {-1.0, 0.0, 0.0},
      {0.0, 1.0, 0.0},
      {1.0, 0.0, 0.0},
      {0.0, -1.0, 0.0},
      {0.0, 0.0, 1.0},
      {0.0, 0.0, -1.0}
    };
    static GLint faces[6][4] =
    {
      {0, 1, 2, 3},
      {3, 2, 6, 7},
      {7, 6, 5, 4},
      {4, 5, 1, 0},
      {5, 6, 2, 1},
      {7, 4, 0, 3}
    };
    GLfloat v[8][3];
    GLint i;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    for (i = 5; i >= 0; i--) {
        glBegin(type);
        glNormal3fv(&n[i][0]);
        glVertex3fv(&v[faces[i][0]][0]);
        glVertex3fv(&v[faces[i][1]][0]);
        glVertex3fv(&v[faces[i][2]][0]);
        glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
    }
}
void drawCube(GLdouble size) {
    drawBox(size, GL_QUADS);
}

//Light switches
//Ceiling
void ceilingLighting() {
    if (cLampState)  glEnable(GL_LIGHT0);
    else   		  glDisable(GL_LIGHT0);
}
//Desk
void deskLighting() {
    if (dLampState)  glEnable(GL_LIGHT0);
    else   		  glDisable(GL_LIGHT0);
}
//Ceiling light updater
void cUpdateLight() {
    cLampColorAmb[0] = cLampR * cLampIntensity;
    cLampColorAmb[1] = cLampG * cLampIntensity;
    cLampColorAmb[2] = cLampB * cLampIntensity;
    cLampColorDif[0] = cLampR * cLampIntensity;
    cLampColorDif[1] = cLampG * cLampIntensity;
    cLampColorDif[2] = cLampB * cLampIntensity;
    cLampColorSpc[0] = cLampR * cLampIntensity;
    cLampColorSpc[1] = cLampG * cLampIntensity;
    cLampColorSpc[2] = cLampB * cLampIntensity;
    glLightfv(GL_LIGHT0, GL_AMBIENT, cLampColorAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cLampColorDif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, cLampColorSpc);
}
//Desk light updater
void dUpdateLight() {
    dLampColorAmb[0] = dLampR * dLampIntensity;
    dLampColorAmb[1] = dLampG * dLampIntensity;
    dLampColorAmb[2] = dLampB * dLampIntensity;
    dLampColorDif[0] = dLampR * dLampIntensity;
    dLampColorDif[1] = dLampG * dLampIntensity;
    dLampColorDif[2] = dLampB * dLampIntensity;
    dLampColorSpc[0] = dLampR * dLampIntensity;
    dLampColorSpc[1] = dLampG * dLampIntensity;
    dLampColorSpc[2] = dLampB * dLampIntensity;
    glLightfv(GL_LIGHT0, GL_AMBIENT, dLampColorAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dLampColorDif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, dLampColorSpc);
}

 /*
  *
  *=========================@WORLD INITIALIZER - initialize stuff such as texture, lighting, etc ...
  *
  */
//Textures
void texturesInit() {
    loadTexture("front.bmp", GL_MODULATE, texture, 0);
    loadTexture("back.bmp", GL_MODULATE, texture, 1);
    loadTexture("right.bmp", GL_MODULATE, texture, 2);
    loadTexture("left.bmp", GL_MODULATE, texture, 3);
    loadTexture("bottom.bmp", GL_MODULATE, texture, 4);
    loadTexture("top.bmp", GL_MODULATE, texture, 5);
    loadTexture("wall.bmp", GL_MODULATE, texture, 6);
    loadTexture("floor.bmp", GL_MODULATE, texture, 7);
    loadTexture("glass.bmp", GL_MODULATE, texture, 8);

}

void lightsInit() {
    //Enable lights and materials 
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    //Ambient light
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambColor);

    //Ceiling lamp
    glLightfv(GL_LIGHT0, GL_POSITION, cLampPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, cLampColorAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cLampColorDif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, cLampColorSpc);
    
    //Desk lamp
    glLightfv(GL_LIGHT1, GL_POSITION, dLampPos);
    glLightfv(GL_LIGHT1, GL_AMBIENT, dLampColorAmb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, dLampColorDif);
    glLightfv(GL_LIGHT1, GL_SPECULAR, dLampColorSpc);
    
    printf("Lights inicialized!\n");
}

void materialsInit() {
    initMaterials(22);

    printf("Materials inicialized!\n");
}

void skyboxInit() {
    glEnable(GL_DEPTH_TEST);

    printf("Skybox inicialized!\n");
}

void worldInit() {
    //Clear screen
    glClearColor(COLOR_BLACK);

    //Enable world stuff
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    //Initialize world stuff
    texturesInit();
    lightsInit();
    materialsInit();
    skyboxInit();

    printf("World successfully inicialized!\n");
}

/*
 *
 *=========================@KEYBOARD HANDLER - Handler of keyboards events
 *
 */
void keyboardASCII(unsigned char key, int x, int y) {
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
        case 'f':
        case 'F':
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
        case 'l':
        case 'L':
            cLampState = !cLampState;
            cUpdateLight();
            glutPostRedisplay();
            break;
        case 'n':
        case 'N':
            ambState = !ambState;
            if (ambState) { ambColor[0] = 0.7;   ambColor[1] = 0.7;   ambColor[2] = 0.7; }
            else { ambColor[0] = 0.2;   ambColor[1] = 0.2;   ambColor[2] = 0.2; }
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambColor);
            glutPostRedisplay();
            break;

        case 'm':
        case 'M':
            tablemat = (tablemat + 1) % 18;
            initMaterials(tablemat);
            glutPostRedisplay();
            break;
        case 'i': 
        case 'I':
            cLampIntensity = cLampIntensity + 0.1;
            if (cLampIntensity > 1.1) cLampIntensity = 0;
            cUpdateLight();
            glutPostRedisplay();
            break;
        case 'r':
        case 'R':
            cLampR = (cLampR + 1) % 2;
            cUpdateLight();
            glutPostRedisplay();
            break;
        case 'g':
        case 'G':
            cLampG = (cLampG + 1) % 2;
            cUpdateLight();
            glutPostRedisplay();
            break;
        case 'b':
        case 'B':
            cLampB = (cLampB + 1) % 2;
            cUpdateLight();
            glutPostRedisplay();
            break;
            //Exit
        case 27:
            exit(0);
            break;
        }
}

void keyboardNotASCII(int key, int x, int y) {
    //Observer position
    if (key == GLUT_KEY_UP)   obsP[1] = (obsP[1] + 0.2);
    if (key == GLUT_KEY_DOWN) obsP[1] = (obsP[1] - 0.2);

    //Viewing angle
    if (key == GLUT_KEY_LEFT)  aVision = (aVision + 0.2);
    if (key == GLUT_KEY_RIGHT) aVision = (aVision - 0.2);

    //Viewing radius
    if (key == GLUT_KEY_PAGE_DOWN) rVision = (rVision - 1);
    if (key == GLUT_KEY_PAGE_UP) rVision = (rVision + 1);

    //Limit height
    if (obsP[1] >= 50)   obsP[1] = 50;
    if (obsP[1] <= 0.1)  obsP[1] = 0.1;
    obsP[0] = rVision * cos(aVision);
    obsP[2] = rVision * sin(aVision);

    glutPostRedisplay();
}

/*
 *
 *=========================@GAME DRAWS - Draw game scenerios
 *
 */
 //----Axis
void drawAxis() {
    glBegin(GL_LINES);
    glVertex3i(0, 0, 0);
    glVertex3i(10, 0, 0);
    glEnd();
    glBegin(GL_LINES);
    glVertex3i(0, 0, 0);
    glVertex3i(0, 10, 0);
    glEnd();
    glBegin(GL_LINES);
    glVertex3i(0, 0, 0);
    glVertex3i(0, 0, 10);
    glEnd();
}

void drawSkyBox(int skyboxSize) {
    initMaterials(18);

    //Back 
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);  		glVertex3f(-skyboxSize, -1, -skyboxSize);
    glTexCoord2f(1.0f, 0.0f); 		glVertex3f(-skyboxSize, -1, skyboxSize);
    glTexCoord2f(1.0f, 1.0f);   	glVertex3f(-skyboxSize, skyboxSize, skyboxSize);
    glTexCoord2f(0.0f, 1.0f);		glVertex3f(-skyboxSize, skyboxSize, -skyboxSize);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //Left  
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);  		glVertex3f(-skyboxSize, -1, -skyboxSize);
    glTexCoord2f(1.0f, 0.0f); 		glVertex3f(skyboxSize, -1, -skyboxSize);
    glTexCoord2f(1.0f, 1.0f);   	glVertex3f(skyboxSize, skyboxSize, -skyboxSize);
    glTexCoord2f(0.0f, 1.0f);		glVertex3f(-skyboxSize, skyboxSize, -skyboxSize);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //Front
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);  		glVertex3f(skyboxSize, -1, -skyboxSize);
    glTexCoord2f(1.0f, 0.0f); 		glVertex3f(skyboxSize, -1, skyboxSize);
    glTexCoord2f(1.0f, 1.0f);   	glVertex3f(skyboxSize, skyboxSize, skyboxSize);
    glTexCoord2f(0.0f, 1.0f);		glVertex3f(skyboxSize, skyboxSize, -skyboxSize);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //Right 
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);  		glVertex3f(skyboxSize, -1, skyboxSize);
    glTexCoord2f(1.0f, 0.0f); 		glVertex3f(-skyboxSize, -1, skyboxSize);
    glTexCoord2f(1.0f, 1.0f);   	glVertex3f(-skyboxSize, skyboxSize, skyboxSize);
    glTexCoord2f(0.0f, 1.0f);		glVertex3f(skyboxSize, skyboxSize, skyboxSize);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //Bottom
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);  	glVertex3f(-skyboxSize, -1, -skyboxSize);
    glTexCoord2f(1.0f, 0.0f); 	glVertex3f(-skyboxSize, -1, skyboxSize);
    glTexCoord2f(1.0f, 1.0f);   glVertex3f(skyboxSize, -1, skyboxSize);
    glTexCoord2f(0.0f, 1.0f);	glVertex3f(skyboxSize, -1, -skyboxSize);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //Top
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glBegin(GL_QUADS);
    glVertex3f(-skyboxSize, skyboxSize, -skyboxSize);
    glVertex3f(-skyboxSize, skyboxSize, skyboxSize);
    glVertex3f(skyboxSize, skyboxSize, skyboxSize);
    glVertex3f(skyboxSize, skyboxSize, -skyboxSize);
    glEnd();
    glDisable(GL_TEXTURE_2D);

}

//Draw office
void drawOffice() {
    initMaterials(18);

    //Back Wall
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);  		glVertex3f(-officeSize, 0, -officeSize);
    glTexCoord2f(1.0f, 0.0f); 		glVertex3f(-officeSize, 0, officeSize);
    glTexCoord2f(1.0f, 1.0f);   	glVertex3f(-officeSize, officeSize, officeSize);
    glTexCoord2f(0.0f, 1.0f);		glVertex3f(-officeSize, officeSize, -officeSize);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //Left Wall 
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);  		glVertex3f(-officeSize, 0, -officeSize);
    glTexCoord2f(1.0f, 0.0f); 		glVertex3f(officeSize, 0, -officeSize);
    glTexCoord2f(1.0f, 1.0f);   	glVertex3f(officeSize, officeSize, -officeSize);
    glTexCoord2f(0.0f, 1.0f);		glVertex3f(-officeSize, officeSize, -officeSize);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //Front Wall
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    /*
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);  		glVertex3f(officeSize,10, -officeSize + 10);
    glTexCoord2f(1.0f, 0.0f); 		glVertex3f(officeSize, 10, officeSize - 10);
    glTexCoord2f(1.0f, 1.0f);   	glVertex3f(officeSize, officeSize - 10, officeSize - 10);
    glTexCoord2f(0.0f, 1.0f);		glVertex3f(officeSize, officeSize - 10, -officeSize + 10);
    glEnd();
   */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);  		glVertex3f(officeSize, 0, -officeSize);
    glTexCoord2f(1.0f, 0.0f); 		glVertex3f(officeSize , 0, officeSize );
    glTexCoord2f(1.0f, 1.0f);   	glVertex3f(officeSize , officeSize, officeSize);
    glTexCoord2f(0.0f, 1.0f);		glVertex3f(officeSize , officeSize , -officeSize);
    glEnd();
    glDisable(GL_BLEND);


    //Right Wall
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);  		glVertex3f(officeSize, 0, officeSize);
    glTexCoord2f(1.0f, 0.0f); 		glVertex3f(-officeSize, 0, officeSize);
    glTexCoord2f(1.0f, 1.0f);   	glVertex3f(-officeSize, officeSize, officeSize);
    glTexCoord2f(0.0f, 1.0f);		glVertex3f(officeSize, officeSize, officeSize);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //Floor
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);  	glVertex3f(-officeSize, 0, -officeSize);
    glTexCoord2f(1.0f, 0.0f); 	glVertex3f(-officeSize, 0, officeSize);
    glTexCoord2f(1.0f, 1.0f);   glVertex3f(officeSize, 0, officeSize);
    glTexCoord2f(0.0f, 1.0f);	glVertex3f(officeSize, 0, -officeSize);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //Ceiling
    glBegin(GL_QUADS);
    glVertex3f(-officeSize, officeSize, -officeSize);
    glVertex3f(-officeSize, officeSize, officeSize);
    glVertex3f(officeSize, officeSize, officeSize);
    glVertex3f(officeSize, officeSize, -officeSize);
    glEnd();
}

//Chair
//Draw chair base
void drawChairBase(GLfloat chairHeight, GLfloat chairWidth, GLfloat chairThick) {
    initMaterials(5);

    //Cover	
    glBegin(GL_QUADS);
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

    //Left leg front
    glBegin(GL_QUADS);
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

    //Left leg back
    glBegin(GL_QUADS);
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

    //Right leg back
    glBegin(GL_QUADS);
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
    glVertex3f(-chairWidth + chairThick, -chairHeight, -chairWidth + chairThick);
    glVertex3f(-chairWidth + chairThick, -chairHeight, -chairWidth);
    glVertex3f(-chairWidth, chairHeight - chairThick, -chairWidth + chairThick);
    glVertex3f(-chairWidth, chairHeight - chairThick, -chairWidth);
    glVertex3f(-chairWidth, -chairHeight, -chairWidth);
    glVertex3f(-chairWidth, -chairHeight, -chairWidth + chairThick);
    glEnd();

    //Right leg front
    glBegin(GL_QUADS);
    glVertex3f(chairWidth, chairHeight - chairThick, -chairWidth + chairThick);
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

//Draw chair upper
void drawChairUpper(GLfloat chairHeight, GLfloat chairWidth, GLfloat chairThick) {
    initMaterials(2);

    //Back
    glBegin(GL_QUADS);
    glVertex3f(-chairWidth, chairHeight, -chairWidth);
    glVertex3f(-chairWidth + chairThick, chairHeight, -chairWidth);
    glVertex3f(-chairWidth + chairThick, chairHeight, chairWidth);
    glVertex3f(-chairWidth, chairHeight, chairWidth);
    glVertex3f(-chairWidth, 3 * chairHeight, chairWidth);
    glVertex3f(-chairWidth + chairThick, 3 * chairHeight, chairWidth);
    glVertex3f(-chairWidth + chairThick, 3 * chairHeight, -chairWidth);
    glVertex3f(-chairWidth, 3 * chairHeight, -chairWidth);
    glVertex3f(-chairWidth, chairHeight, chairWidth);
    glVertex3f(-chairWidth + chairThick, chairHeight, chairWidth);
    glVertex3f(-chairWidth + chairThick, 3 * chairHeight, chairWidth);
    glVertex3f(-chairWidth, 3 * chairHeight, chairWidth);
    glVertex3f(-chairWidth, 3 * chairHeight, -chairWidth);
    glVertex3f(-chairWidth + chairThick, 3 * chairHeight, -chairWidth);
    glVertex3f(-chairWidth + chairThick, chairHeight, -chairWidth);
    glVertex3f(-chairWidth, chairHeight, -chairWidth);
    glVertex3f(-chairWidth + chairThick, chairHeight, chairWidth);
    glVertex3f(-chairWidth + chairThick, chairHeight, -chairWidth);
    glVertex3f(-chairWidth + chairThick, 3 * chairHeight, -chairWidth);
    glVertex3f(-chairWidth + chairThick, 3 * chairHeight, chairWidth);
    glVertex3f(-chairWidth, chairHeight, -chairWidth);
    glVertex3f(-chairWidth, chairHeight, chairWidth);
    glVertex3f(-chairWidth, 3 * chairHeight, chairWidth);
    glVertex3f(-chairWidth, 3 * chairHeight, -chairWidth);
    glEnd();

    //Right Arm Upper
    glBegin(GL_QUADS);
    glColor3f(0.137, 0.090, 0.035);
    glVertex3f(chairWidth, 1.25 * chairHeight, chairWidth - 2 * chairThick);
    glVertex3f(-chairWidth + chairThick, 1.25 * chairHeight, chairWidth - 2 * chairThick);
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

    //Right Arm Upper
    glBegin(GL_QUADS);
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

//Table
void drawLeg(float xt, float yt, float zt, int nmat) {
    initMaterials(nmat);

    glPushMatrix();
    glTranslatef(xt, yt, zt);
    glScalef(0.2, 8, 0.2);
    glTranslatef(0, 0.3, 0);
    drawCube(1.0);
    glPopMatrix();
}
//Table top
void drawTable(int nmat) {
    initMaterials(nmat);

    glPushMatrix();
    glTranslatef(20, 0, 0);
    glPushMatrix();
    glScalef(10, 0.1, 15);
    glTranslatef(0, 60, 0);
    drawCube(1.0);
    glPopMatrix();

    drawLeg(4, -0.5, -4, nmat);
    drawLeg(4, -0.5, 4, nmat);
    drawLeg(-4, -0.5, -4, nmat);
    drawLeg(-4, -0.5, 4, nmat);

    glPopMatrix();
}

//Lamp
void drawLamp() {
    initMaterials(17);

    glPushMatrix();
    glTranslatef(0, officeSize, 0);
    drawCube(3);
    glRotatef(45, 0, 1, 0);
    drawCube(3);
    glPopMatrix();
}

void drawMainScenery() {
    //Draw office
    drawOffice();
    
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
    
    //Table
    drawTable(tablemat);

    //Lamp
    drawLamp();

}

/*
 *
 *=========================@MAIN DISPLAY - choose what can be displayed
 *
 */
void mainDisplay() {
    //Clear window and buffers
    glClearColor(COLOR_BLACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Main display
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1, 9999);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(obsP[0], obsP[1], obsP[2], obsT[0], obsT[1], obsT[2], 0, 1, 0);

    //Lights
    ceilingLighting();

    //Draws
    //drawAxis();
    drawSkyBox(200);
    drawMainScenery();

    //Updates
    glutSwapBuffers();
}

/*
 *
 *=========================@GAME LOOP - game loop called 1000 / SCREEN_FPS ms
 *
 */
void gameLoop(int value) {
    glutPostRedisplay();
    glutTimerFunc(1000 / SCREEN_FPS, gameLoop, 0);
}

/*
 *
 *=========================@MAIN CALL - Call main function to get started
 *
 */
int main(int argc, char** argv) {
    /*
     *Glut window
     */
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
   glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - SCREEN_WIDTH) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - SCREEN_HEIGHT) / 2);
   glutCreateWindow("@lbpaisDev || 2017251509 || lbpais@student.dei.uc.pt");
   printf("Window created!\n");
    /*
     *Initialize pre-world stuff
     */
   worldInit();
    /*
     *Assign glut functions
     */
   glutKeyboardFunc(keyboardASCII);
   glutSpecialFunc(keyboardNotASCII);
   glutDisplayFunc(mainDisplay);
    /*
     *Game loop call
     */
   gameLoop(0);
     /*
      *Main glut loop unique call
      */
    glutMainLoop();
    return 0;
}