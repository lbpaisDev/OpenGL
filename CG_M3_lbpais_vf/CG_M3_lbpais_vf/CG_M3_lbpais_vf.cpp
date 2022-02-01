#include <windows.h>			
#include <stdio.h>				
#include <fstream>				

#include <string>
#include <fstream>

#include <errno.h>
#include <GL\glew.h>			
#include <GL\freeGlut.h>		

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glu32.lib")	
#pragma comment(lib,"opengl32.lib")

//=================== VARS AND CONSTANTS ===================//
//----TIME
GLint msec = 1;

//----SCREEN
GLint wScreen = 1920;
GLint hScreen = 1080;

//----OBSERVER
GLfloat rVision = 15, aVision = 2 * 3.1415, incVision = 0.05;
GLfloat  angZoom = 90;
GLfloat  incZoom = 3;

//Observer
GLfloat  obsP[] = { rVision * cos(aVision), 3.0, rVision * sin(aVision) };

//Limits
GLfloat xC = 10.0, yC = 10.0, zC = 10.0;

//Target
GLfloat  obsT[] = { obsP[0] - rVision * cos(aVision), obsP[1], obsP[2] - rVision * sin(aVision) };

//Scenery
GLfloat DayOfYear = 0;
GLfloat shadeAngle = 0;

//Shaders
GLint  uniOp;
GLint  uniDir;
float  Direcao[] = { 1, 0, 0 };
float  opcao = -45;

//Shader files
char filenameV[] = "Vshader.txt";
char filenameF[] = "Fshader.txt";

//Shaders variables
char* VertexShaderSource;
char* FragmentShaderSource;
GLuint  VertexShader, FragmentShader;
GLuint  ShaderProgram;

//Read shader
char* readShaderFile(char* FileName) {

	char* DATA = NULL;
	int   flength = 0;
	FILE* filepoint;
	errno_t err;

	err = fopen_s(&filepoint, FileName, "r");
	if (!err) {
		fseek(filepoint, 0, SEEK_END);
		flength = ftell(filepoint);
		rewind(filepoint);

		DATA = (char*)malloc(sizeof(char) * (flength + 1));
		flength = fread(DATA, sizeof(char), flength, filepoint);

		DATA[flength] = '\0';
		fclose(filepoint);
		return DATA;
	}
	else {
		printf(" --------  Error while reading  %s ", FileName);
	}
}

//Create, compile and use
void BuiltShader(void) {

	GLEW_ARB_vertex_shader;
	GLEW_ARB_fragment_shader;

	//Create
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	VertexShaderSource = readShaderFile(filenameV);
	FragmentShaderSource = readShaderFile(filenameF);

	const char* VS = VertexShaderSource;
	const char* FS = FragmentShaderSource;
	glShaderSource(VertexShader, 1, &VS, NULL);
	glShaderSource(FragmentShader, 1, &FS, NULL);
	free(VertexShaderSource);
	free(FragmentShaderSource);

	//Compile
	glCompileShaderARB(VertexShader);
	glCompileShaderARB(FragmentShader);

	//Create and link
	ShaderProgram = glCreateProgramObjectARB();
	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);
	glLinkProgram(ShaderProgram);

	//Use
	glUseProgramObjectARB(ShaderProgram);
}

void InitShader(void) {

	//Create and link
	BuiltShader();

	uniDir = glGetUniformLocation(ShaderProgram, "Direcao");
	glUniform3fv(uniDir, 1, Direcao);
	uniOp = glGetUniformLocation(ShaderProgram, "opcao");
	glUniform1f(uniOp, opcao);
}

//Free shaders
void DeInitShader(void) {
	glDetachShader(ShaderProgram, VertexShader);
	glDetachShader(ShaderProgram, FragmentShader);
	glDeleteShader(ShaderProgram);
}

//=================== SUPPORT FUNCTIONS ===================//
//----Initialize world related stuff
void init(void) {
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
}

//=================== KEYBOARD ===================//
//----ASCII
void kASCII(unsigned char key, int x, int y) {
	switch (key) {
	case 'o':
	case 'O':
		opcao = opcao + 10;
		glutPostRedisplay();
		break;
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


//=================== DRAW ===================//
//----Axis
void drawAxis() {
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(xC, 0, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, yC, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, zC);
	glEnd();
}

//----Scene
void drawScene() {


	shadeAngle = 360.0 * DayOfYear / 365.0;
	DayOfYear += 0.025;

	glUniform1f(uniOp, opcao);
	glUniform3fv(uniDir, 1, Direcao);

	Direcao[0] = cos(3.14 * opcao / 180.0);
	Direcao[2] = sin(3.14 * opcao / 180.0);

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	glRotatef(shadeAngle, 0.0, 1.0, 0.0);
	glutSolidSphere(1.f, 64, 64);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);
	glRotatef(shadeAngle, 0.0, 1.0, 0.0);
	glTranslatef(4.0, 0.0, 0.0); 
	glutSolidSphere(1.f, 64, 64);	
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.2, 0.2, 1.0);
	glRotatef(shadeAngle * 3, 0.0, 1.0, 0.0);
	glTranslatef(6.0, 0.0, 0.0); 
	glutSolidSphere(1.f, 64, 64);
	glPopMatrix();

}

//=================== DISPLAY ===================//
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Main display
	glViewport(0, 0, wScreen, hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, (float)wScreen / hScreen, 0.1, 9999);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], obsT[0], obsT[1], obsT[2], 0, 1, 0);

	drawAxis();

	//Main draw
	drawScene();

	//Update
	glutSwapBuffers();
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
	glutCreateWindow(" |lbpais@student.dei.uc.pt || META 3 | ");
	
	//----INIT
	init();
	GLenum err = glewInit();
	InitShader();
	DeInitShader();

	//----ASSIGN FUNCTIONS
	glutSpecialFunc(knASCII);
	glutDisplayFunc(display);
	glutKeyboardFunc(kASCII);
	glutIdleFunc(display);

	//----TIMER
	glutTimerFunc(msec, Timer, 1);

	//----LOOP
	glutMainLoop();
	return 0;
}
