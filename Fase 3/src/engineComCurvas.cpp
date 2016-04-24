#include <GL/glew.h>
#include <GLUT/glut.h>      //-- MAC
#include <OpenGL/glu.h>    //-- MAC
#include <OpenGL/gl.h>    //-- MAC
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
#include <vector>
#include <sstream>
#include <string>
#include <math.h>
#include <algorithm>
//windows include
//  #include <windows.h>
// #include<Glew\glew.h>
//  #include <GL/glut.h>
// #include <GL/glu.h>
//  #include <GL/gl.h>

//#include "tinyxml\tinyxml.h"	// The place where the file "tinyxml.h" is located
#include </usr/local/Cellar/tinyxml/2.6.2/include/tinyxml.h> //-- MAC

#define _USE_MATH_DEFINES

using namespace std;

#define _M_PI_ 3.14159

#pragma comment(lib,"glew32.lib")

// camera variables
float alfa = 0.0f, beta = 0.0f, radius = 500.0f;
float camX, camY=15, camZ = 15;
int startX, startY, tracking = 0;
int alphaMouse = 0, betaMouse = 0, rMouse = 5;

// Declared here and implemented after main in order to better organization of the code
void initGl();
void initVBOsPlanets();
void initVBOsMoons();
void sphericalToCartesian();
void getGlobalCatmullRomPoint(float gt, float *res);
void renderCatmullRomCurve();
void drawPlanetVBO(int);
void drawMoonVBO(int);
void changeSize(int, int);
void renderScene(void);
void keyPressed(unsigned char, int, int);
void newMenu (int id_op);
void keyboardExtra(int key_code, int x, int y);
void processMouseButtons(int button, int state, int xx, int yy);
void processMouseMotion(int xx, int yy);
void printFilenames();
void printTransformations();

// Declare radii of all 8 planets orbits
float planetsRadiusOrbits[9] = {0,30,50,90,110,120,140,160,180};
float moonRadiusOrbit[6] = { 20,20,20,20,20,20 };
int planetMoonsQuantity[9] = {0,0,0,1,1,1,1,1,1 };
// boolean, check if planet has rings
int planetRings[9] = {0,0,0,0,0,0,1,1,1 };

//VBO
#define MAX_STACKS 100
#define MAX_SLICES 100
#define VERTEX_MAX_SIZE (((MAX_STACKS - 2)*MAX_SLICES * 6 + 2 * MAX_SLICES * 3) * 3)

GLuint vertexCountP, vertexCountM, *planetas, *luas;

#define POINT_COUNT 8
// Points that make up the loop for catmull-rom interpolation
float p[POINT_COUNT][3] = { { 0, 0 , 0 }, { 0, 0 , 0 },{ 0, 0 , 0 },{ 0, 0 , 0 },{ 0, 0 , 0 },{ 0, 0 , 0 },{ 0, 0 , 0 },{ 0, 0 , 0 }};

// Global variable that allows to know if XML file was found and read
int read=0;

float rotate_y = 0;
float rotate_x = 0;
float rotate_z = 0;

float translate_y = 0;
float translate_x = 0;
float translate_z = 0;

// Global variable process input
vector<string> splitted;
vector< vector<float> > verticesP;
vector< vector<string> > filesP;

vector< vector<float> > verticesM;
vector< vector<string> > filesM;
// Auxiliary global variable process input
vector<string> file3dRead;
vector<float> vrtx;

// Global variables that have the xml content of the sun
int file_X,file_Y,file_Z,file_axisX,file_axisY,file_axisZ;
float angleFile;
string file3dPlanet,file3dMoon;

vector<string> planets; // use adicionaName flag 1
vector<string> moons;   // use adicionaName flag 2
vector<float> angles;  //  use adicionaAngles
vector<int> translatesX; // use adicionaTransformations flag 1
vector<int> translatesY; // use adicionaTransformations flag 2
vector<int> translatesZ; // use adicionaTransformations flag 3
vector<int> rotatesX; // use adicionaTransformations flag 4
vector<int> rotatesY; // use adicionaTransformations flag 5
vector<int> rotatesZ; // use adicionaTransformations flag 6

// Global variables that allows drawing circles
vector<float> alphaSP;
vector<float> alphaIncSP;

vector<float> alphaSM;
vector<float> alphaIncSM;

// Splits a string into its substrings accordingly to its delimiter that must be provided
vector<string> split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}
	return internal;
}

// Reads the vertices of figure.3d files into a vector
vector<string> read3d(string figure) {
	vector<string> vecx;
	ifstream fileTXT(figure);
	string line;
	while (getline(fileTXT, line)) {
		vecx.push_back(line); // Adding the line at the end of vector
	}
	int size = stoi(vecx[0]); // Number of total vertices. It's the first line in the .3d file
	return vecx;
}

void adicionaFile(vector<string> aux, int flag){
	switch (flag) {
		case 1:
					 filesP.push_back(aux);
					 break;
		case 2:
					 filesM.push_back(aux);
					 break;
	}
}

void adicionaName(string aux, int flag){
	switch (flag) {
		case 1:
					 planets.push_back(aux);
					 break;
		case 2:
					 moons.push_back(aux);
					 break;
	}
}

void limpaFile3dRead(vector<string> aux){
	for (int j = 0; j < aux.size();j++){
			aux[j]="";
	}
}

void adicionaVrtx(vector<float> aux, int flag){
	switch (flag) {
		case 1:
					 verticesP.push_back(aux);
					 break;
		case 2:
					 verticesM.push_back(aux);
					 break;
	}
}

void adicionaAngles(float aux){
	angles.push_back(aux);
}

void limpaAngles(vector<float> aux){
	for (int j = 0; j < aux.size();j++){
			aux[j]=0.0;
	}
}

void adicionaTransformations(int aux, int flag){
	switch (flag) {
		case 1:
						translatesX.push_back(aux);
						break;
		case 2:
						translatesY.push_back(aux);
						break;
		case 3:
						translatesZ.push_back(aux);
						break;
		case 4:
						rotatesX.push_back(aux);
						break;
		case 5:
						rotatesY.push_back(aux);
						break;
		case 6:
						rotatesZ.push_back(aux);
						break;
	}
}

void limpaTransformations(vector<int> aux){
	for (int j = 0; j < aux.size();j++){
			aux[j]=0;
	}
}

string reading(vector<string> aux){
	int size = stoi(aux[0]); // number of vertex
	int conta = 0;
	string line; // String processed of all file3dRead strings concatenated into one of plane.3d or box.3d

	for (int i = 1; i <= size; i++) {
		if (conta != 0) { // Adding ',' at the end of each line except the first
			line = line + " " + aux[i];
		}
		else {
			line += aux[i];
			conta++;
		}
	}
	return line;
}

void preencheVrtx(int flag){
	if(flag==1){
		vector<string> aux2;
		for (int j = 0; j < filesP.size();j++){
			aux2 = filesP[j];
			string vrtxFile = reading(aux2);
			istringstream is_Vrtx(vrtxFile);
			vrtx.erase (vrtx.begin(),vrtx.end());
			copy(istream_iterator<float>(is_Vrtx), istream_iterator<float>(), back_inserter(vrtx));
			adicionaVrtx(vrtx, 1);
		}
	}else if(flag==2){
		vector<string> aux2;
		for (int j = 0; j < filesM.size();j++){
			aux2 = filesM[0];
			string vrtxFile = reading(aux2);
			istringstream is_Vrtx(vrtxFile);
			vrtx.erase (vrtx.begin(),vrtx.end());
			copy(istream_iterator<float>(is_Vrtx), istream_iterator<float>(), back_inserter(vrtx));
			adicionaVrtx(vrtx, 2);
		}
	}
}

void readXML(const char* f){
// Load the xml file, I put your XML in a file named test.xml
TiXmlDocument XMLdoc(f);
bool loadOkay = XMLdoc.LoadFile();
if (loadOkay)
{
    cout << "SolarSystemXML.xml loaded" << endl;
    read=1;
    TiXmlElement *pRoot, *pGroup, *pTranslate, *pRotate, *pModels, *pModelFile, *pMoons, *pModelsMoons, *pModelFileMoons;
    pRoot = XMLdoc.FirstChildElement( "scene" );
    if ( pRoot )
    {
        pGroup = pRoot->FirstChildElement("group" );
        while ( pGroup )
        {
            pTranslate = pGroup->FirstChildElement("translate" );
            while ( pTranslate )
            {
								adicionaTransformations(atoi(pTranslate->Attribute("X")), 1);
								adicionaTransformations(atoi(pTranslate->Attribute("Y")), 2);
								adicionaTransformations(atoi(pTranslate->Attribute("Z")), 3);
                pTranslate = pTranslate->NextSiblingElement("translate");
            }

            pRotate = pGroup->FirstChildElement("rotate" );
            while ( pRotate )
            {
							  adicionaAngles(atof(pRotate->Attribute("angle")));
								adicionaTransformations(atoi(pRotate->Attribute("axisX")), 4);
								adicionaTransformations(atoi(pRotate->Attribute("axisY")), 5);
								adicionaTransformations(atoi(pRotate->Attribute("axisZ")), 6);
                pRotate = pRotate->NextSiblingElement("rotate");
            }
            pModels = pGroup->FirstChildElement("models");
            while ( pModels )
            {
                pModelFile = pModels->FirstChildElement("model");
                while( pModelFile )
                {
										limpaFile3dRead(file3dRead);
										file3dRead = read3d((string) pModelFile->Attribute("file"));
										adicionaFile(file3dRead, 1);
										adicionaName((string) pModelFile->Attribute("file"), 1);
                    pModelFile = pModelFile->NextSiblingElement("model");
                }
                pModels = pModels->NextSiblingElement("models");
            }
            pMoons = pGroup->FirstChildElement("group" );
            while ( pMoons )
            {
            pModelsMoons = pMoons->FirstChildElement("models");
            while ( pModelsMoons )
            {
                pModelFileMoons = pModelsMoons->FirstChildElement("model");
                while( pModelFileMoons )
                {
										limpaFile3dRead(file3dRead);
										file3dRead = read3d((string) pModelFileMoons->Attribute("file"));
										adicionaFile(file3dRead, 2);
										adicionaName((string) pModelFileMoons->Attribute("file"), 2);
                    pModelFileMoons = pModelFileMoons->NextSiblingElement("model");
                }
                pModelsMoons = pModelsMoons->NextSiblingElement("models");
            }
              pMoons = pMoons->NextSiblingElement("group");
            }
            pGroup = pGroup->NextSiblingElement("group");
        }
    }
    else{ cout << "Cannot find 'scene' node" << endl; }
}
else{ cout << "Cannot find XML file" << endl;read=0; }
}

int main(int argc, char **argv) {
  string operationLine, operation;

	cout << "Insert your operation:" << endl;
	getline(cin, operationLine);
	splitted = split(operationLine, ' '); // Save a string on a vector with the substrings separated by a space

	// The function "stoff" transforms the content of a string in a float

  if (splitted[0] == "engine" || splitted[0] == "Engine") {	// Draw -> receives the name of the .3d file
    if (splitted.size() == 1){
      readXML("SolarSystemXML.xml");
      if(read!=0){
				/* Verifying that vector < vector<string> > is OK!!
				for (int j = 0; j < filesP.size();j++){
						printf("Size of each position: %lu\n",filesP[j].size());
						vector<string> aux2;
						aux2 = filesP[j];
						for (int k = 0; k < aux2.size();k++){
								cout << aux2[k] << endl;
						}
				}
				for (int j = 0; j < filesM.size();j++){
						printf("Size of each position: %lu\n",filesM[j].size());
						vector<string> aux2;
						aux2 = filesM[j];
						for (int k = 0; k < aux2.size();k++){
								cout << aux2[k] << endl;
						}
				}*/
				//printFilenames();
        preencheVrtx(1);
				preencheVrtx(2);
				/* Verifying that vector < vector<string> > is OK!!
				for (int j = 0; j < verticesP.size();j++){
						printf("Size of each position: %lu\n",verticesP[j].size()/3);
						vector<float> aux3;
						aux3 = verticesP[j];
						for (int k = 0; k < aux3.size();k++){
								cout << aux3[k] << endl;
						}
				}
				for (int j = 0; j < verticesM.size();j++){
						printf("Size of each position: %lu\n",verticesM[j].size());
						vector<float> aux3;
						aux3 = verticesM[j];
						for (int k = 0; k < aux3.size();k++){
								cout << aux3[k] << endl;
						}
				}*/

        //printTransformations();
        //printFilenames();

  /*
   * Init GLUT and the window.
   */
  glutInit(&argc, argv);

  // Sets up a double buffer with RGBA components and a depth component
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

  // Sets the window size to 512*512 square pixels
  glutInitWindowSize(1000, 1000);

  // Sets the window position to the upper left
  glutInitWindowPosition(0, 0);

  // Creates a window using internal glut functionality
  glutCreateWindow("Solar System - Stage 3");

  // Callback function responsible for the window's contents.
  glutDisplayFunc(renderScene);
  // Function to be callend when the window is created or resized.
  glutReshapeFunc(changeSize);
  // Function called when the event queue is empty.
  glutIdleFunc(renderScene);
  // Function called when a key is pressed.
  glutKeyboardFunc(keyPressed);
  glutSpecialFunc(keyboardExtra);
  // mouse callbacks
  glutMouseFunc(processMouseButtons);
  glutMotionFunc(processMouseMotion);
  //menu
  glutCreateMenu(newMenu);
  glutAddMenuEntry("Increase Translate x value",'j');
  glutAddMenuEntry("Increase Translate y value",'k');
  glutAddMenuEntry("Increase Translate z value",'l');
  glutAddMenuEntry("Decrease Translate x value",'q');
  glutAddMenuEntry("Decrease Translate y value",'w');
  glutAddMenuEntry("Decrease Translate z value",'e');
  glutAddMenuEntry("GL_FRONT & GL_FILL",'z');
  glutAddMenuEntry("GL_FRONT & GL_POINT",'x');
  glutAddMenuEntry("GL_FRONT & GL_FILL",'c');
  glutAddMenuEntry("GL_BACK & GL_LINE",'v');
  glutAddMenuEntry("GL_BACK & GL_POINT",'b');
  glutAddMenuEntry("GL_BACK & GL_FILL",'n');
  glutAddMenuEntry("GL_FRONT_AND_BACK & GL_LINE",'m');
  glutAddMenuEntry("GL_FRONT_AND_BACK & GL_POINT",'o');
  glutAddMenuEntry("GL_FRONT_AND_BACK & GL_FILL",'p');
  glutAddMenuEntry("Change Colour to Default",'0');
  glutAddMenuEntry("Change Colour to Red",'1');
  glutAddMenuEntry("Change Colour to Blue",'2');
  glutAddMenuEntry("Change Colour to Green",'3');
  glutAddMenuEntry("Change Colour to Orange",'4');
  glutAddMenuEntry("Change Colour to Pink",'5');
  glutAddMenuEntry("Change Colour to Yellow",'6');
  glutAddMenuEntry("Change Colour to Bright Blue",'7');
	glutAddMenuEntry("Stop Translation around Sun",'s');
	glutAddMenuEntry("Restart Translation around Sun",'r');
  //button= GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, or GLUT MIDDLE_BUTTON
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glewInit();
  initGl();
  initVBOsPlanets();

  // enter GLUT's main cycle.
	glutMainLoop();
  }
 }
 else cout << "Try again! Amount of arguments incorrect" << endl;
}
else cout << "Try again! Use: draw <filename.3d>" << endl;
return 1;
}
void initGl () {
	glClearColor(0.0,0.0,0.0,0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
  //default
	glPolygonMode(GL_FRONT,GL_LINE);
  // init
  sphericalToCartesian();
	glEnableClientState(GL_VERTEX_ARRAY);
}

void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0) h = 1;

	// compute window's aspect ratio
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
  glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {
  static float t[9] = {0,0,0,0,0,0,0,0,0};
  float pos[4] = {1.0, 1.0, 1.0, 0.0}, res[3],res2[3];
  int numberCurves=(int) verticesP.size();
  int position;

	// Clear buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set the camera.
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,0.0,0.0,-1.0,0.0f,1.0f,0.0f);
  glRotatef( rotate_x, 1.0, 0.0, 0.0 );
  glRotatef( rotate_y, 0.0, 1.0, 0.0 );
  glRotatef( rotate_z, 0.0, 0.0, 1.0 );
  glTranslatef(translate_x,translate_y,translate_z);

  //PREENCHER CURVAS
	for(position=0;position<numberCurves;position++){
		p[0][0] = (3.0/2)*planetsRadiusOrbits[position];
		p[0][1] = 0;
		p[0][2] = 0;

		p[1][0] = (2.2/2)*planetsRadiusOrbits[position];
		p[1][1] = 0;
		p[1][2] = (-1.5/2)*planetsRadiusOrbits[position];

		p[2][0] = 0;
		p[2][1] = 0;
		p[2][2] = (-2.0/2)*planetsRadiusOrbits[position];

		p[3][0] = (-2.2/2)*planetsRadiusOrbits[position];
		p[3][1] = 0;
		p[3][2] = (-1.5/2)*planetsRadiusOrbits[position];

		p[4][0] = (-3.0/2)*planetsRadiusOrbits[position];
		p[4][1] = 0;
		p[4][2] = 0;

		p[5][0] = (-2.2/2)*planetsRadiusOrbits[position];
		p[5][1] = 0;
		p[5][2] = (1.5/2)*planetsRadiusOrbits[position];

		p[6][0] = 0;
		p[6][1] = 0;
		p[6][2] = (2.0/2)*planetsRadiusOrbits[position];

		p[7][0] = (2.2/2)*planetsRadiusOrbits[position];
		p[7][1] = 0;
		p[7][2] = (1.5/2)*planetsRadiusOrbits[position];

		getGlobalCatmullRomPoint(t[position], res);

		renderCatmullRomCurve();

		glPushMatrix();
		glTranslatef(res[0], res[1], res[2]);

		glRotatef(angles[position], rotatesX[position], rotatesY[position], rotatesZ[position]);
	  glTranslatef(translatesX[position], translatesY[position], translatesZ[position]);

		drawPlanetVBO(position);
		glPopMatrix();
		switch (position) {
			case 0:
						t[position] += 0.000001;
						break;
			case 1:
						t[position] += 0.05;
						break;
			case 2:
						t[position] += 0.03;
						break;
			case 3:
						t[position] += 0.02;
						break;
			case 4:
						t[position] += 0.01;
						break;
			case 5:
						t[position] += 0.005;
						break;
			case 6:
						t[position] += 0.001;
						break;
			case 7:
						t[position] += 0.0005;
						break;
			case 8:
						t[position] += 0.0001;
						break;

		}
	}

	// End of frame.
	glutSwapBuffers();
}

// write function to process menu events
void newMenu (int id_op){
    keyPressed((unsigned char)id_op, 0, 0);
}

void keyPressed(unsigned char key, int x, int y) {
      if (key == 'j' || key == 'J') translate_x+=1;
      else if (key == 'k' || key == 'K') translate_y+=1;
      else if (key == 'l' || key == 'L') translate_z+=1;
      else if (key == 'q' || key == 'Q') translate_x-=1;
      else if (key == 'w' || key == 'W') translate_y-=1;
      else if (key == 'e' || key == 'E') translate_z-=1;
      else if (key == 'z' || key == 'Z') glPolygonMode(GL_FRONT,GL_FILL); //GL_FRONT & GL_FILL
      else if (key == 'x' || key == 'X') glPolygonMode(GL_FRONT,GL_POINT); //GL_FRONT & GL_POINT
      else if (key == 'c' || key == 'C') glPolygonMode(GL_FRONT,GL_FILL); //GL_FRONT & GL_FILL
      else if (key == 'v' || key == 'V') glPolygonMode(GL_BACK,GL_LINE); //GL_BACK & GL_LINE
      else if (key == 'b' || key == 'B') glPolygonMode(GL_BACK,GL_POINT); //GL_BACK & GL_POINT
      else if (key == 'n' || key == 'N') glPolygonMode(GL_BACK,GL_FILL); //GL_BACK & GL_FILL
      else if (key == 'm' || key == 'M') glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); //GL_FRONT_AND_BACK & GL_LINE
      else if (key == 'o' || key == 'O') glPolygonMode(GL_FRONT_AND_BACK,GL_POINT); //GL_FRONT_AND_BACK & GL_POINT
      else if (key == 'p' || key == 'P') glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); //GL_FRONT_AND_BACK & GL_FILL*/
			// Move only camera
			else if (key == 'g') camY += 50;
			else if (key == 'h') camY -= 50;
			else if (key == 'i') camZ += 50;
			else if (key == 'u') camZ += 50;
			else if (key == '0'){ // DEFAULT IS WHITE
          glColor3f(1.0f,1.0f,1.0f);
      }
      else if (key == '1'){ // RED
          glColor3f(1.0f,0.0f,0.0f);
      }
      else if (key == '2'){ // BLUE
          glColor3f(0.0f,0.0f,1.0f);
      }
      else if (key == '3'){ // GREEN
          glColor3f(0.0f,1.0f,0.0f);
      }
      else if (key == '4'){ // ORANGE
          glColor3f(1.0f,0.5f,0.0f);
      }
      else if (key == '5'){ // PINK
          glColor3f(1.0f,0.5f,0.5f);
      }
      else if (key == '6'){ // YELLOW
          glColor3f(1.0f,1.0f,0.0f);
      }
      else if (key == '7'){ // BRIGHT BLUE
          glColor3f(0.0f,1.0f,1.0f);
      }
      else if (key==27) exit(-1);

      //when camera moves
      glutPostRedisplay();
}

void keyboardExtra(int key_code, int x, int y){
		// Move only rotate and translate parameters
		/*  Right arrow key - decrease rotate_y by 5*/
		if (key_code == GLUT_KEY_RIGHT) rotate_y -= 5;
		/*  Left arrow key - increase rotate_y by 5*/
		else if (key_code == GLUT_KEY_LEFT) rotate_y += 5;
		/*  Up arrow key - increase rotate_x by 5*/
		else if (key_code == GLUT_KEY_UP) rotate_x += 5;
		/*  Down arrow key - decrease rotate_x by 5*/
		else if (key_code == GLUT_KEY_DOWN) rotate_x -= 5;
		/*   arrow key - decrease rotate_z by 5*/
		else if (key_code == GLUT_KEY_F9) rotate_z += 5;
		/*   arrow key - decrease rotate_z by 5*/
		else if (key_code == GLUT_KEY_F7) rotate_z -= 5;
		//when camera moves

		glutPostRedisplay();
}

void processMouseButtons(int button, int state, int xx, int yy) {
	if (state == GLUT_DOWN) {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON) tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON) tracking = 2;
		else tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alphaMouse += (xx - startX);
			betaMouse += (yy - startY);
		}
		else if (tracking == 2) {
			rMouse -= yy - startY;
			if (rMouse < 3) rMouse = 3.0;
		}
		tracking = 0;
	}
}

void processMouseMotion(int xx, int yy) {
	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking) return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {
		alphaAux = alphaMouse + deltaX;
		betaAux = betaMouse + deltaY;

		if (betaAux > 85.0) betaAux = 85.0;
		else if (betaAux < -85.0) betaAux = -85.0;
        rAux = rMouse;
        if (rAux < 500) rAux = 500;
	}
	else if (tracking == 2) {
		alphaAux = alphaMouse;
		betaAux = betaMouse;
		rAux = rMouse - deltaY;
		if (rAux < 500) rAux = 500;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux * sin(betaAux * 3.14 / 180.0);
}

void printFilenames(){
	cout << "\nPlanets:" << endl;
	for (int j = 0; j < planets.size();j++){
			cout << planets[j] << endl;
	}
	cout << "\nMoons:" << endl;
	for (int j = 0; j < moons.size();j++){
			cout << moons[j] << endl;
	}
}

void printTransformations(){
	cout << "\nTranslate X=" << endl;
	for (int j = 0; j < translatesX.size();j++){
		cout << translatesX[j] << endl;
	}
	cout << "\nTranslate Y=" << endl;
	for (int j = 0; j < translatesY.size();j++){
		cout << translatesY[j] << endl;
	}
	cout << "\nTranslate Z=" << endl;
	for (int j = 0; j < translatesZ.size();j++){
		cout << translatesZ[j] << endl;
	}
	cout << "\nAngles:" << endl;
	for (int j = 0; j < angles.size();j++){
		cout << angles[j] << endl;
	}
	cout << "\nRotate X=" << endl;
	for (int j = 0; j < rotatesX.size();j++){
		cout << rotatesX[j] << endl;
	}
	cout << "\nRotate Y=" << endl;
	for (int j = 0; j < rotatesY.size();j++){
		cout << rotatesY[j] << endl;
	}
	cout << "\nRotate Z=" << endl;
	for (int j = 0; j < rotatesZ.size();j++){
		cout << rotatesZ[j] << endl;
	}
}

void sphericalToCartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}

void getCatmullRomPoint(float t, int *indices, float *res) {
	// catmull-rom matrix
	float m[4][4] = { { -0.5f, 1.5f, -1.5f, 0.5f },{ 1.0f, -2.5f, 2.0f, -0.5f },{ -0.5f, 0.0f, 0.5f, 0.0f },{ 0.0f, 1.0f, 0.0f, 0.0f } };

	res[0] = 0.0; res[1] = 0.0; res[2] = 0.0;

	float T[4] = { powf(t, 3), powf(t, 2), t, 1 }, mult[4];

	// mult = T * M
	for (int i = 0; i < 4; i++) {
		mult[i] = T[0] * m[0][i] + T[1] * m[1][i] + T[2] * m[2][i] + T[3] * m[3][i];
	}

	// res = mult * p
	for (int i = 0; i < 3; i++) {
		res[i] = mult[0] * p[indices[0]][i] + mult[1] * p[indices[1]][i] + mult[2] * p[indices[2]][i] + mult[3] * p[indices[3]][i];
	}
}

// given global t, returns the point in the curve
void getGlobalCatmullRomPoint(float gt, float *res) {
	float t = gt * POINT_COUNT; // this is the real global t
	int index = floor(t); // which segment
	t = t - index; // where within the segment

	// indices store the points
	int indices[4];
	indices[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
	indices[1] = (indices[0] + 1) % POINT_COUNT;
	indices[2] = (indices[1] + 1) % POINT_COUNT;
	indices[3] = (indices[2] + 1) % POINT_COUNT;

	getCatmullRomPoint(t, indices, res);
}

void renderCatmullRomCurve() {
	float gt = 0.0;

	glBegin(GL_LINE_LOOP);
	while (gt < 1) {
		float res[3];
		getGlobalCatmullRomPoint(gt, res);
		glVertex3f(res[0], res[1], res[2]);
		gt += 0.0001;
	}
	glEnd();
}

void drawPlanetVBO(int position){
	switch (position) {
		case 0:
						glBindBuffer(GL_ARRAY_BUFFER, planetas[0]);
						glVertexPointer(3, GL_FLOAT, 0, 0);
						glColor3f(1.0f, 0.0f, 0.0f);
						glDrawArrays(GL_TRIANGLES, 0, vertexCountP);
						break;
		case 1:
						glBindBuffer(GL_ARRAY_BUFFER, planetas[1]);
						glVertexPointer(3, GL_FLOAT, 0, 0);
						glColor3f(1.0f, 0.0f, 0.0f);
						glDrawArrays(GL_TRIANGLES, 0, vertexCountP);
						break;
		case 2:
						glBindBuffer(GL_ARRAY_BUFFER, planetas[2]);
						glVertexPointer(3, GL_FLOAT, 0, 0);
						glColor3f(1.0f, 0.0f, 0.0f);
						glDrawArrays(GL_TRIANGLES, 0, vertexCountP);
						break;
		case 3:
						glBindBuffer(GL_ARRAY_BUFFER, planetas[3]);
						glVertexPointer(3, GL_FLOAT, 0, 0);
						glColor3f(1.0f, 0.0f, 0.0f);
						glDrawArrays(GL_TRIANGLES, 0, vertexCountP);
						break;
		case 4:
						glBindBuffer(GL_ARRAY_BUFFER, planetas[4]);
						glVertexPointer(3, GL_FLOAT, 0, 0);
						glColor3f(1.0f, 0.0f, 0.0f);
						glDrawArrays(GL_TRIANGLES, 0, vertexCountP);
						break;
		case 5:
						glBindBuffer(GL_ARRAY_BUFFER, planetas[5]);
						glVertexPointer(3, GL_FLOAT, 0, 0);
						glColor3f(1.0f, 0.0f, 0.0f);
						glDrawArrays(GL_TRIANGLES, 0, vertexCountP);
						break;
		case 6:
						glBindBuffer(GL_ARRAY_BUFFER, planetas[6]);
						glVertexPointer(3, GL_FLOAT, 0, 0);
						glColor3f(1.0f, 0.0f, 0.0f);
						glDrawArrays(GL_TRIANGLES, 0, vertexCountP);
						glutWireTorus(1.5, 10, 20, 20);
						break;
		case 7:
						glBindBuffer(GL_ARRAY_BUFFER, planetas[7]);
						glVertexPointer(3, GL_FLOAT, 0, 0);
						glColor3f(1.0f, 0.0f, 0.0f);
						glDrawArrays(GL_TRIANGLES, 0, vertexCountP);
						glutWireTorus(1.5, 10, 20, 20);
						break;
		case 8:
						glBindBuffer(GL_ARRAY_BUFFER, planetas[8]);
						glVertexPointer(3, GL_FLOAT, 0, 0);
						glColor3f(1.0f, 0.0f, 0.0f);
						glDrawArrays(GL_TRIANGLES, 0, vertexCountP);
						glutWireTorus(1.5, 10, 20, 20);
						break;
	}
}

	void initVBOsPlanets() {
		int vertex = 0,i,j,points,numberOfPlanets=verticesP.size();
		planetas=(GLuint *)malloc(sizeof(GLuint) * numberOfPlanets);
		glGenBuffers(numberOfPlanets, planetas);
		vector<float> auxiliar;
		for(i=0;i<numberOfPlanets;i++){
			float *vertexPlanets=(float *)malloc(sizeof(float) * VERTEX_MAX_SIZE);
			//Each i stands for one planet
			auxiliar=verticesP[i];

			for (points = 0; points < auxiliar.size();points++){
					vertexPlanets[points] = auxiliar[points];
			}
			vertexCountP = points;

			glBindBuffer(GL_ARRAY_BUFFER, planetas[i]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCountP, vertexPlanets, GL_STATIC_DRAW);

			free(vertexPlanets);
		}
	}

	void drawMoonVBO(int position){
		switch (position) {
			case 3:
							glBindBuffer(GL_ARRAY_BUFFER, luas[0]);
							glVertexPointer(3, GL_FLOAT, 0, 0);
							glColor3f(1.0f, 0.0f, 0.0f);
							glDrawArrays(GL_TRIANGLES, 0, vertexCountM);
							break;
			case 4:
							glBindBuffer(GL_ARRAY_BUFFER, luas[4]);
							glVertexPointer(3, GL_FLOAT, 0, 0);
							glColor3f(1.0f, 0.0f, 0.0f);
							glDrawArrays(GL_TRIANGLES, 0, vertexCountM);
							break;
			case 5:
							glBindBuffer(GL_ARRAY_BUFFER, luas[5]);
							glVertexPointer(3, GL_FLOAT, 0, 0);
							glColor3f(1.0f, 0.0f, 0.0f);
							glDrawArrays(GL_TRIANGLES, 0, vertexCountM);
							break;
			case 6:
							glBindBuffer(GL_ARRAY_BUFFER, luas[6]);
							glVertexPointer(3, GL_FLOAT, 0, 0);
							glColor3f(1.0f, 0.0f, 0.0f);
							glDrawArrays(GL_TRIANGLES, 0, vertexCountM);
							break;
			case 7:
							glBindBuffer(GL_ARRAY_BUFFER, luas[7]);
							glVertexPointer(3, GL_FLOAT, 0, 0);
							glColor3f(1.0f, 0.0f, 0.0f);
							glDrawArrays(GL_TRIANGLES, 0, vertexCountM);
							break;
			case 8:
							glBindBuffer(GL_ARRAY_BUFFER, luas[8]);
							glVertexPointer(3, GL_FLOAT, 0, 0);
							glColor3f(1.0f, 0.0f, 0.0f);
							glDrawArrays(GL_TRIANGLES, 0, vertexCountM);
							break;
		}
	}

	void initVBOsMoons() {
		int vertex = 0,i,j,points,numberOfMoons=verticesM.size();
		luas=(GLuint *)malloc(sizeof(GLuint) * numberOfMoons);
		glGenBuffers(numberOfMoons, luas);
		vector<float> auxiliar;
		for(i=0;i<numberOfMoons;i++){
			float *vertexMoons=(float *)malloc(sizeof(float) * VERTEX_MAX_SIZE);
			//Each i stands for one planet
			auxiliar=verticesM[i];

			for (points = 0; points < auxiliar.size();points++){
					vertexMoons[points] = auxiliar[points];
			}
			vertexCountM = points;

			glBindBuffer(GL_ARRAY_BUFFER, luas[i]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCountM, vertexMoons, GL_STATIC_DRAW);

			free(vertexMoons);
		}
	}
