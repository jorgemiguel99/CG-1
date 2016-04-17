#define _USE_MATH_DEFINES

//#include <GLUT/glut.h>      //-- MAC
//#include <OpenGL/glu.h>    //-- MAC
//#include <OpenGL/gl.h>    //-- MAC
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
#include <windows.h>
#include <Glew/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "/Users/Tiago/Desktop/VS/Fase 2/tinyxml/tinyxml.h"	// The place where the file "tinyxml.h" is located
//#include </usr/local/Cellar/tinyxml/2.6.2/include/tinyxml.h> //-- MAC

#pragma comment(lib,"glew32.lib")

using namespace std;

// Declare radius of all 8 planets orbits
float MercuryRadiusOrbit = 20.0;
float VenusRadiusOrbit = 40.0;
float TerraRadiusOrbit = 55.0;
float MarteRadiusOrbit = 80.0;
float JupiterRadiusOrbit = 110.0;
float UranoRadiusOrbit = 150.0;
float SaturnoRadiusOrbit = 200.0;
float NeptunoRadiusOrbit = 250.0;

float earthMoonRadiusOrbit = 20.0;
float PhobosMoonRadiusOrbit	= 20.0;
float CallistoMoonRadiusOrbit = 20.0;
float TitanMoonRadiusOrbit = 20.0;
float OberonMoonRadiusOrbit	= 20.0;
float TritonMoonRadiusOrbit	= 20.0;

// Length of the VBO array (starts with 18 because the plane has 6 vertices with 3 coordinates each)
int totalSize = 18;

// Plane size
int planeSize = 600;

// Length of the buffer
const int n = 1;

// Buffers for the VBO's
GLuint buffers[n];

// Array for the VBO's
float *vertexB = new float[totalSize];

// Size of the VBO array (in bytes)
int arraySize = sizeof(float) * totalSize;

// Global variable that allows to know if XML file was found and read
int read = 0;

// Global variable to activate movement around the Sun as defined previously
int movingON = 1;

// Global Variables to Transformations
float px = 0.0, py = 100.0, pz = 100.0;

float rotate_y = 0;
float rotate_x = 0;
float rotate_z = 0;

float translate_y = 0;
float translate_x = 0;
float translate_z = 0;

// Global variable process input
vector<string> splitted;
vector<vector<float>> verticesP;
vector<vector<string>> filesP;

vector<vector<float>> verticesM;
vector<vector<string>> filesM;

// Auxiliary global variable process input
vector<string> file3dRead;
vector<float> vrtx;

// Global variables that have the xml content of the sun
int file_X, file_Y, file_Z, file_axisX, file_axisY, file_axisZ;
float angleFile;
string file3dPlanet,file3dMoon;

vector<string> planets;		// use adicionaName flag 1
vector<string> moons;		// use adicionaName flag 2
vector<float> angles;		// use adicionaAngles
vector<int> translatesX;	// use adicionaTransformations flag 1
vector<int> translatesY;	// use adicionaTransformations flag 2
vector<int> translatesZ;	// use adicionaTransformations flag 3
vector<int> rotatesX;		// use adicionaTransformations flag 4
vector<int> rotatesY;		// use adicionaTransformations flag 5
vector<int> rotatesZ;		// use adicionaTransformations flag 6

// Global variables that allows drawing circles
vector<float> alphaSP;
vector<float> alphaIncSP;

vector<float> alphaSM;
vector<float> alphaIncSM;

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_LINE);
}

void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0) h = 1;

	// compute window's aspect ratio
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);

	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

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

void limpaFile3dRead(vector<string> aux) {
	for (int j = 0; j < aux.size(); j++) aux[j]="";
}

void adicionaVrtx(vector<float> aux, int flag) {
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
	for (int j = 0; j < aux.size(); j++) aux[j]=0.0;
}

void adicionaTransformations(int aux, int flag) {
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
	for (int j = 0; j < aux.size(); j++) aux[j]=0;
}

string reading(vector<string> aux){
	int size = stoi(aux[0]); // Number of vertex
	int conta = 0;
	string line; // String processed of all file3dRead strings concatenated into one of plane.3d or box.3d

	for (int i = 1; i <= size; i++) {
		if (conta != 0) line = line + " " + aux[i];
		else {
			line += aux[i];
			conta++;
		}
	}
	return line;
}

void preencheVrtx(int flag) {
	if(flag == 1) {
		vector<string> aux2;
		for (int j = 0; j < filesP.size(); j++) {
			aux2 = filesP[j];
			string vrtxFile = reading(aux2);
			istringstream is_Vrtx(vrtxFile);
			vrtx.erase (vrtx.begin(),vrtx.end());
			copy(istream_iterator<float>(is_Vrtx), istream_iterator<float>(), back_inserter(vrtx));
			adicionaVrtx(vrtx, 1);
			totalSize++;
		}
	} else if(flag == 2) {
		vector<string> aux2;
		for (int j = 0; j < filesM.size(); j++) {
			aux2 = filesM[0];
			string vrtxFile = reading(aux2);
			istringstream is_Vrtx(vrtxFile);
			vrtx.erase (vrtx.begin(),vrtx.end());
			copy(istream_iterator<float>(is_Vrtx), istream_iterator<float>(), back_inserter(vrtx));
			adicionaVrtx(vrtx, 2);
			totalSize++;
		}
	}
}

void readXML(char *f){
	// Load the xml file, I put your XML in a file named test.xml
	TiXmlDocument XMLdoc(f);
	bool loadOkay = XMLdoc.LoadFile();
	if(loadOkay) {
		cout << "SolarSystemXML.xml loaded" << endl;
		read = 1;
		TiXmlElement *pRoot, *pGroup, *pTranslate, *pRotate, *pModels, *pModelFile, *pMoons, *pModelsMoons, *pModelFileMoons;
		pRoot = XMLdoc.FirstChildElement("scene");
		if (pRoot) {
			pGroup = pRoot->FirstChildElement("group");
			while (pGroup) {
				pTranslate = pGroup->FirstChildElement("translate");
				while (pTranslate) {
					adicionaTransformations(atoi(pTranslate->Attribute("X")), 1);
					adicionaTransformations(atoi(pTranslate->Attribute("Y")), 2);
					adicionaTransformations(atoi(pTranslate->Attribute("Z")), 3);
					pTranslate = pTranslate->NextSiblingElement("translate");
				}
				pRotate = pGroup->FirstChildElement("rotate" );
				while (pRotate) {
					adicionaAngles(atof(pRotate->Attribute("angle")));
					adicionaTransformations(atoi(pRotate->Attribute("axisX")), 4);
					adicionaTransformations(atoi(pRotate->Attribute("axisY")), 5);
					adicionaTransformations(atoi(pRotate->Attribute("axisZ")), 6);
					pRotate = pRotate->NextSiblingElement("rotate");
				}
				pModels = pGroup->FirstChildElement("models");
				while (pModels) {
					pModelFile = pModels->FirstChildElement("model");
					while(pModelFile) {
						limpaFile3dRead(file3dRead);
						file3dRead = read3d((string) pModelFile->Attribute("file"));
						adicionaFile(file3dRead, 1);
						adicionaName((string) pModelFile->Attribute("file"), 1);
						pModelFile = pModelFile->NextSiblingElement("model");
					}
					pModels = pModels->NextSiblingElement("models");
				}
				pMoons = pGroup->FirstChildElement("group");
				while (pMoons) {
					pModelsMoons = pMoons->FirstChildElement("models");
					while (pModelsMoons) {
						pModelFileMoons = pModelsMoons->FirstChildElement("model");
						while(pModelFileMoons) {
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
		else cout << "Cannot find 'scene' node" << endl;
	}
	else {
		cout << "Cannot find XML file" << endl;
		read = 0; 
	}
}

// Draw the solar system, using the pre-saved VBO's
void drawSolarSystem() {
	// Bind and semantics
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	// Draw the VBO's
	glDrawArrays(GL_TRIANGLES, 0, totalSize);
}

void renderScene(void) {
	// Clear buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the camera.
	glLoadIdentity();
	gluLookAt(px, py, pz, 0.0, 0.0, -1.0, 0.0f, 1.0f, 0.0f);

	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);
	glRotatef(rotate_z, 0.0, 0.0, 1.0);
	glTranslatef(translate_x, translate_y, translate_z);

	// Draw the solar system
	drawSolarSystem();

	/**Draw plane.
	glBegin(GL_TRIANGLES);
	glVertex3f(-600, 0, -600);
	glVertex3f(-600, 0, 600);
	glVertex3f(600, 0, 600);

	glVertex3f(600, 0, -600);
	glVertex3f(-600, 0, -600);
	glVertex3f(600, 0, 600);
	glEnd();

	vector<float> vrtx_aux, vrtx_auxM;
	int vrt = 0, vrtM = 0;
	vrtx_auxM = verticesM[vrtM];

	// Only executes for the exact number of planets read, vrt = 0 is the Sun
	vrtx_aux = verticesP[vrt]; // input read from XML
	glRotatef(angles[vrt], rotatesX[vrt], rotatesY[vrt], rotatesZ[vrt]); // input read from XML
	glTranslatef(translatesX[vrt], translatesY[vrt], translatesZ[vrt]); // input read from XML

	glBegin(GL_TRIANGLES);
	for (int j = 0; j < vrtx_aux.size();) glVertex3f(vrtx_aux[j++], vrtx_aux[j++], vrtx_aux[j++]);
	glEnd();

	vrt++;
	while (vrt < planets.size()) {
		vrtx_aux = verticesP[vrt];
		for (int i = 0; i < 1; i++) {
			glPushMatrix();
			switch (vrt) {
			case 1:
				glTranslatef(MercuryRadiusOrbit * cos(alphaSP[vrt]), 1.0, MercuryRadiusOrbit * sin(alphaSP[vrt])); //proxima posicao no eixo XoZ
				break;
			case 2:
				glTranslatef(VenusRadiusOrbit * cos(alphaSP[vrt]), 1.0, VenusRadiusOrbit * sin(alphaSP[vrt])); //proxima posicao no eixo XoZ
				break;
			case 3:
				glTranslatef(TerraRadiusOrbit * cos(alphaSP[vrt]), 1.0, TerraRadiusOrbit * sin(alphaSP[vrt])); //proxima posicao no eixo XoZ
				break;
			case 4:
				glTranslatef(MarteRadiusOrbit * cos(alphaSP[vrt]), 1.0, MarteRadiusOrbit * sin(alphaSP[vrt])); //proxima posicao no eixo XoZ
				break;
			case 5:
				glTranslatef(JupiterRadiusOrbit * cos(alphaSP[vrt]), 1.0, JupiterRadiusOrbit * sin(alphaSP[vrt])); //proxima posicao no eixo XoZ
				break;
			case 6:
				glTranslatef(SaturnoRadiusOrbit * cos(alphaSP[vrt]), 1.0, SaturnoRadiusOrbit * sin(alphaSP[vrt])); //proxima posicao no eixo XoZ
				break;
			case 7:
				glTranslatef(UranoRadiusOrbit * cos(alphaSP[vrt]), 1.0, UranoRadiusOrbit * sin(alphaSP[vrt])); //proxima posicao no eixo XoZ
				break;
			case 8:
				glTranslatef(NeptunoRadiusOrbit * cos(alphaSP[vrt]), 1.0, NeptunoRadiusOrbit * sin(alphaSP[vrt])); //proxima posicao no eixo XoZ
				break;
			}

			glRotatef(angles[vrt], rotatesX[vrt], rotatesY[vrt], rotatesZ[vrt]);
			glTranslatef(translatesX[vrt], translatesY[vrt], translatesZ[vrt]);

			glBegin(GL_TRIANGLES);
			for (int j = 0; j < vrtx_aux.size();) glVertex3f(vrtx_aux[j++], vrtx_aux[j++], vrtx_aux[j++]);
			glEnd();

			// Drawing Rings
			switch (vrt) {
			case 6:
				glutWireTorus(1.5, 10, 20, 20);
				break;
			case 7:
				glutWireTorus(1.5, 10, 20, 20);
				break;
			case 8:
				glutWireTorus(1.5, 15, 20, 20);
				break;
			}

			for (int m = 0; m < 1; m++) {
				if (vrt == 3) {
					glPushMatrix();
					glTranslatef(earthMoonRadiusOrbit * cos(alphaSM[vrtM]), 1.0, earthMoonRadiusOrbit * sin(alphaSM[vrtM])); //proxima posicao no eixo XoZ
					glBegin(GL_TRIANGLES);
					for (int j = 0; j < vrtx_auxM.size();) glVertex3f(vrtx_auxM[j++], vrtx_auxM[j++], vrtx_auxM[j++]);
					glEnd();
					glPopMatrix();
					if (movingON == 1) alphaSM[vrtM] += alphaIncSM[vrtM];
				}
				else if (vrt == 4) {
					glPushMatrix();
					glTranslatef(PhobosMoonRadiusOrbit * cos(alphaSM[vrtM]), 1.0, PhobosMoonRadiusOrbit * sin(alphaSM[vrtM])); //proxima posicao no eixo XoZ
					glBegin(GL_TRIANGLES);
					for (int j = 0; j < vrtx_auxM.size();) glVertex3f(vrtx_auxM[j++], vrtx_auxM[j++], vrtx_auxM[j++]);
					glEnd();
					glPopMatrix();
					if (movingON == 1) alphaSM[vrtM] += alphaIncSM[vrtM];
				}
				else if (vrt == 5) {
					glPushMatrix();
					glTranslatef(CallistoMoonRadiusOrbit * cos(alphaSM[vrtM]), 1.0, CallistoMoonRadiusOrbit * sin(alphaSM[vrtM])); //proxima posicao no eixo XoZ
					glBegin(GL_TRIANGLES);
					for (int j = 0; j < vrtx_auxM.size();) glVertex3f(vrtx_auxM[j++], vrtx_auxM[j++], vrtx_auxM[j++]);
					glEnd();
					glPopMatrix();
					if (movingON == 1) alphaSM[vrtM] += alphaIncSM[vrtM];
				}
				else if (vrt == 6) {
					glPushMatrix();
					glTranslatef(TitanMoonRadiusOrbit * cos(alphaSM[vrtM]), 1.0, TitanMoonRadiusOrbit * sin(alphaSM[vrtM])); //proxima posicao no eixo XoZ
					glBegin(GL_TRIANGLES);
					for (int j = 0; j < vrtx_auxM.size();) glVertex3f(vrtx_auxM[j++], vrtx_auxM[j++], vrtx_auxM[j++]);
					glEnd();
					glPopMatrix();
					if (movingON == 1) alphaSM[vrtM] += alphaIncSM[vrtM];
				}
				else if (vrt == 7) {
					glPushMatrix();
					glTranslatef(OberonMoonRadiusOrbit * cos(alphaSM[vrtM]), 1.0, OberonMoonRadiusOrbit * sin(alphaSM[vrtM])); //proxima posicao no eixo XoZ
					glBegin(GL_TRIANGLES);
					for (int j = 0; j < vrtx_auxM.size();) glVertex3f(vrtx_auxM[j++], vrtx_auxM[j++], vrtx_auxM[j++]);
					glEnd();
					glPopMatrix();
					if (movingON == 1) alphaSM[vrtM] += alphaIncSM[vrtM];
				}
				else if (vrt == 8) {
					glPushMatrix();
					glTranslatef(TritonMoonRadiusOrbit * cos(alphaSM[vrtM]), 1.0, TritonMoonRadiusOrbit * sin(alphaSM[vrtM])); //proxima posicao no eixo XoZ
					glBegin(GL_TRIANGLES);
					for (int j = 0; j < vrtx_auxM.size();) glVertex3f(vrtx_auxM[j++], vrtx_auxM[j++], vrtx_auxM[j++]);
					glEnd();
					glPopMatrix();
					if (movingON == 1) alphaSM[vrtM] += alphaIncSM[vrtM];
				}
				if (movingON == 1) alphaSM[vrtM] += 10.0;
				vrtM++;
				if (vrtM<verticesM.size()) vrtx_auxM = verticesM[vrtM];
			}
			glPopMatrix();
			if (movingON == 1) {
				if (vrt == 2 || vrt == 7) alphaSP[vrt] -= alphaIncSP[vrt];
				else alphaSP[vrt] += alphaIncSP[vrt];
			}
		}
		if (movingON == 1) {
			if (vrt == 2 || vrt == 7) alphaSP[vrt] -= 10.0;
			else alphaSP[vrt] += 10.0;
		}
		vrt++;
	}*/

	// End of frame.
	glutSwapBuffers();
}

void keyPressed(unsigned char key, int x, int y) {
	if (key == 'j' || key == 'J') translate_x += 1;
	else if (key == 'k' || key == 'K') translate_y += 1;
	else if (key == 'l' || key == 'L') translate_z += 1;
	else if (key == 'q' || key == 'Q') translate_x -= 1;
	else if (key == 'w' || key == 'W') translate_y -= 1;
	else if (key == 'e' || key == 'E') translate_z -= 1;
	else if (key == 'z' || key == 'Z') glPolygonMode(GL_FRONT, GL_FILL);
	else if (key == 'x' || key == 'X') glPolygonMode(GL_FRONT, GL_POINT);
	else if (key == 'c' || key == 'C') glPolygonMode(GL_FRONT, GL_FILL);
	else if (key == 'v' || key == 'V') glPolygonMode(GL_BACK, GL_LINE);
	else if (key == 'b' || key == 'B') glPolygonMode(GL_BACK, GL_POINT);
	else if (key == 'n' || key == 'N') glPolygonMode(GL_BACK, GL_FILL);
	else if (key == 'm' || key == 'M') glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (key == 'o' || key == 'O') glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else if (key == 'p' || key == 'P') glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (key == 's' || key == 'S') movingON = 0; // Parar rotação circular
	else if (key == 'r' || key == 'R') movingON = 1; // retomar rotação circular

	// Move only camera
	else if (key == 'g') py += 50;
	else if (key == 'h') py -= 50;
	else if (key == 'i') pz += 50;
	else if (key == 'u') pz += 50;
	else if (key == '0') glColor3f(1.0f, 1.0f, 1.0f); // WHITE 
	else if (key == '1') glColor3f(1.0f, 0.0f, 0.0f); // RED
	else if (key == '2') glColor3f(0.0f, 0.0f, 1.0f); // BLUE
	else if (key == '3') glColor3f(0.0f, 1.0f, 0.0f); // GREEN
	else if (key == '4') glColor3f(1.0f, 0.5f, 0.0f); // ORANGE
	else if (key == '5') glColor3f(1.0f, 0.5f, 0.5f); // PINK
	else if (key == '6') glColor3f(1.0f, 1.0f, 0.0f); // YELLOW
	else if (key == '7') glColor3f(0.0f, 1.0f, 1.0f); // BRIGHT BLUE
	else if (key == 27) exit(-1);

	// When camera moves
	glutPostRedisplay();
}

// Function to process menu events
void newMenu(int id_op) {
	keyPressed((unsigned char)id_op, 0, 0);
}

void keyboardExtra(int key_code, int x, int y) {
	if (key_code == GLUT_KEY_RIGHT) rotate_y -= 5;
	else if (key_code == GLUT_KEY_LEFT) rotate_y += 5;
	else if (key_code == GLUT_KEY_UP) rotate_x += 5;
	else if (key_code == GLUT_KEY_DOWN) rotate_x -= 5;
	else if (key_code == GLUT_KEY_F9) rotate_z += 5;
	else if (key_code == GLUT_KEY_F7) rotate_z -= 5;

	// When camera moves
	glutPostRedisplay();
}

void printFilenames() {
	cout << "\nPlanets:" << endl;
	for (int j = 0; j < planets.size(); j++) cout << planets[j] << endl;
	cout << "\nMoons:" << endl;
	for (int j = 0; j < moons.size(); j++) cout << moons[j] << endl;
}

void printTransformations() {
	cout << "\nTranslate X=" << endl;
	for (int j = 0; j < translatesX.size(); j++) cout << translatesX[j] << endl;
	cout << "\nTranslate Y=" << endl;
	for (int j = 0; j < translatesY.size(); j++) cout << translatesY[j] << endl;
	cout << "\nTranslate Z=" << endl;
	for (int j = 0; j < translatesZ.size(); j++) cout << translatesZ[j] << endl;
	cout << "\nAngles:" << endl;
	for (int j = 0; j < angles.size(); j++) cout << angles[j] << endl;
	cout << "\nRotate X=" << endl;
	for (int j = 0; j < rotatesX.size(); j++) cout << rotatesX[j] << endl;
	cout << "\nRotate Y=" << endl;
	for (int j = 0; j < rotatesY.size(); j++) cout << rotatesY[j] << endl;
	cout << "\nRotate Z=" << endl;
	for (int j = 0; j < rotatesZ.size(); j++) cout << rotatesZ[j] << endl;
}

void preencheAlphaS() {
	float aux4, aux5;
	for (int j = 0; j < planets.size(); j++) {
		aux4 = 0.0;
		alphaSP.push_back(aux4);
		switch (j) {
		case 1:
			aux5 = (2 * M_PI) / MercuryRadiusOrbit;
			break;
		case 2:
			aux5 = (2 * M_PI) / VenusRadiusOrbit;
			break;
		case 3:
			aux5 = (2 * M_PI) / TerraRadiusOrbit;
			break;
		case 4:
			aux5 = (2 * M_PI) / MarteRadiusOrbit;
			break;
		case 5:
			aux5 = (2 * M_PI) / JupiterRadiusOrbit;
			break;
		case 6:
			aux5 = (2 * M_PI) / SaturnoRadiusOrbit;
			break;
		case 7:
			aux5 = (2 * M_PI) / UranoRadiusOrbit;
			break;
		case 8:
			aux5 = (2 * M_PI) / NeptunoRadiusOrbit;
			break;
		}
		alphaIncSP.push_back(aux5);
	}
	for (int j = 0; j < moons.size(); j++) {
		aux4 = 0.0;
		alphaSM.push_back(aux4);
		switch (j) {
		case 0:
			aux5 = (2 * M_PI) / earthMoonRadiusOrbit;
			break;
		case 1:
			aux5 = (2 * M_PI) / PhobosMoonRadiusOrbit;
			break;
		case 2:
			aux5 = (2 * M_PI) / CallistoMoonRadiusOrbit;
			break;
		case 3:
			aux5 = (2 * M_PI) / TitanMoonRadiusOrbit;
			break;
		case 4:
			aux5 = (2 * M_PI) / OberonMoonRadiusOrbit;
			break;
		case 5:
			aux5 = (2 * M_PI) / TritonMoonRadiusOrbit;
			break;
		}
		alphaIncSM.push_back(aux5);
	}
}

void printAlphaS() {
	cout << "\nAlphaSP" << endl;
	for (int j = 0; j < alphaSP.size(); j++) cout << alphaSP[j] << endl;
	cout << "\nAlphaIncSP" << endl;
	for (int j = 0; j < alphaIncSP.size(); j++) cout << alphaIncSP[j] << endl;
	cout << "\nAlphaSM" << endl;
	for (int j = 0; j < alphaSM.size(); j++) cout << alphaSM[j] << endl;
	cout << "\nAlphaIncSM" << endl;
	for (int j = 0; j < alphaIncSM.size(); j++) cout << alphaIncSM[j] << endl;
}

void prepareSolarSystem() {
	// Enable buffer
	glEnableClientState(GL_VERTEX_ARRAY);

	int k = 0;

	// Save the plane's vertices
	vertexB[k++] = -planeSize; vertexB[k++] = 0; vertexB[k++] = -planeSize;
	vertexB[k++] = -planeSize; vertexB[k++] = 0; vertexB[k++] = planeSize;
	vertexB[k++] = planeSize; vertexB[k++] = 0; vertexB[k++] = planeSize;

	vertexB[k++] = planeSize; vertexB[k++] = 0; vertexB[k++] = -planeSize;
	vertexB[k++] = -planeSize; vertexB[k++] = 0; vertexB[k++] = -planeSize;
	vertexB[k++] = planeSize; vertexB[k++] = 0; vertexB[k++] = planeSize;

	// Only executes for the exact number of planets read, vrt = 0 is the Sun
	vector<float> vrtx_aux, vrtx_auxM;
	int vrt = 0, vrtM = 0;
	vrtx_auxM = verticesM[vrtM];

	vrtx_aux = verticesP[vrt]; // input read from XML

	// Sun
	for (int j = 0; j < vrtx_aux.size();) {
		vertexB[k++] = vrtx_aux[j++]; vertexB[k++] = vrtx_aux[j++]; vertexB[k++] = vrtx_aux[j++];
	}

	// Saves the planets and moons VBO's
	vrt++;
	while (vrt < planets.size()) {
		vrtx_aux = verticesP[vrt];

		// Planet
		for (int j = 0; j < vrtx_aux.size();) {
			vertexB[k++] = vrtx_aux[j++]; vertexB[k++] = vrtx_aux[j++]; vertexB[k++] = vrtx_aux[j++];
		}

		// Saves the moons
		for (int m = 0; m < 1; m++) {
			for (int j = 0; j < vrtx_auxM.size();) {
				vertexB[k++] = vrtx_auxM[j++]; vertexB[k++] = vrtx_auxM[j++]; vertexB[k++] = vrtx_auxM[j++];
			}
			vrtM++;
			if (vrtM < verticesM.size()) vrtx_auxM = verticesM[vrtM];
		}
		vrt++;
	}

	// Generate VBO's
	glGenBuffers(n, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, arraySize, vertexB, GL_STATIC_DRAW);
}

int main(int argc, char **argv) {
	string operationLine, operation;
	cout << "Insert your operation:" << endl;
	getline(cin, operationLine);
	splitted = split(operationLine, ' '); // Save a string on a vector with the substrings separated by a space

	// The function "stoff" transforms the content of a string in a float
	if (splitted[0] == "Draw" || splitted[0] == "draw") {	// Draw -> receives the name of the .3d file
		if (splitted.size() == 1) {
			readXML("SolarSystemXML.xml");
			if(read != 0) {
				preencheAlphaS();
				preencheVrtx(1);
				preencheVrtx(2);

				// Glut init
				glutInit(&argc, argv);
				glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
				glutInitWindowSize(1000, 1000);
				glutInitWindowPosition(0, 0);
				glutCreateWindow("Solar System - Stage 2");

				// Callback functions
				glutDisplayFunc(renderScene);
				glutReshapeFunc(changeSize);
				glutIdleFunc(renderScene);
				glutKeyboardFunc(keyPressed);
				glutSpecialFunc(keyboardExtra);

				// Init glew
				GLenum err = glewInit();
				if (err != GLEW_OK) printf("Error initializing GLEW!\n");
				else printf("Initializing GLEW succeeded!\n");

				// Menus
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
				glutAttachMenu(GLUT_RIGHT_BUTTON);

				// Put init here
				init();

				// Prepare the VBO's of the solar system
				prepareSolarSystem();
			
				// Enter GLUT's main cycle.
				glutMainLoop();
			}
		}
		else cout << "Try again! Amount of arguments incorrect" << endl;
	}
	else cout << "Try again! Use: draw <filename.3d>" << endl;
	return 1;
}