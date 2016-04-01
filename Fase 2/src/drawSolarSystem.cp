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
// #include <windows.h>
// #include <GL/glut.h>
// #include <GL/glu.h>
// #include <GL/gl.h>
//#include "tinyxml\tinyxml.h"	// The place where the file "tinyxml.h" is located
#include </usr/local/Cellar/tinyxml/2.6.2/include/tinyxml.h> //-- MAC

#define _USE_MATH_DEFINES

using namespace std;


// Declared here and implemented after main in order to better organization of the code
void init();
void changeSize(int, int);
void renderScene(void);
void idleScene(void);
void keyPressed(unsigned char, int, int);
void newMenu (int id_op);
void keyboardExtra(int key_code, int x, int y);
void printFilenames();
void printTransformations();
void preencheAlphaS();
void printAlphaS();

// Declare radii of all 8 planets orbits
float MercuryRadiusOrbit     = 30.0;
float VenusRadiusOrbit       = 50.0;
float TerraRadiusOrbit       = 70.0;
float MarteRadiusOrbit       = 95.0;
float JupiterRadiusOrbit     = 110.0;
float UranoRadiusOrbit       = 150.0;
float SaturnoRadiusOrbit     = 200.0;
float NeptunoRadiusOrbit     = 250.0;

// Global variable that allows to know if XML file was found and read
int read=0;

// Global variable to activate Colour as defined previously
int colourON=0;

// Global variable to activate movement around the Sun as defined previously
int movingON=1;

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
vector< vector<float> > vertices;
vector< vector<string> > files;
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
vector<float> alphaS;
vector<float> alphaIncS;

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

void adicionaFile(vector<string> aux){
	files.push_back(aux);
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

void adicionaVrtx(vector<float> aux){
	vertices.push_back(aux);
}

void limpaVertices(vector<float> aux){
	for (int j = 0; j < aux.size();j++){
			aux[j]=0.0;
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

void preencheVrtx(){
	for (int j = 0; j < files.size();j++){
		vector<string> aux2;
		aux2 = files[7];
		string vrtxFile = reading(aux2);
		istringstream is_Vrtx(vrtxFile);
		copy(istream_iterator<float>(is_Vrtx), istream_iterator<float>(), back_inserter(vrtx));
		adicionaVrtx(vrtx);
		limpaVertices(vrtx);
	}
}

void readXML(string f){
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
										adicionaFile(file3dRead);
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
										adicionaFile(file3dRead);
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

  if (splitted[0] == "Draw" || splitted[0] == "draw") {	// Draw -> receives the name of the .3d file
    if (splitted.size() == 1){
      readXML("SolarSystemXML.xml");
      if(read!=0){
				/* Verifying that vector < vector<string> > is OK!!
				for (int j = 0; j < files.size();j++){
						printf("Size of each position: %lu\n",files[j].size());
						vector<string> aux2;
						aux2 = files[j];
						for (int k = 0; k < aux2.size();k++){
								cout << aux2[k] << endl;
						}
				}*/
				preencheAlphaS();
				//printAlphaS();
        preencheVrtx();
				/* Verifying that vector < vector<string> > is OK!!
				for (int j = 0; j < vertices.size();j++){
						printf("Size of each position: %lu\n",files[j].size());
						vector<float> aux3;
						aux3 = vertices[j];
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
  glutCreateWindow("Solar System - Stage 2");

  // Callback function responsible for the window's contents.
  glutDisplayFunc(renderScene);
  // Function to be callend when the window is created or resized.
  glutReshapeFunc(changeSize);
  // Function called when the event queue is empty.
  glutIdleFunc(renderScene);
  // Function called when a key is pressed.
  glutKeyboardFunc(keyPressed);
  glutSpecialFunc(keyboardExtra);

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
	glutAddMenuEntry("Enable Colour Similar Reality",'8');
	glutAddMenuEntry("Disable Colour Similar Reality",'9');
	glutAddMenuEntry("Stop Translation around Sun",'s');
	glutAddMenuEntry("Restart Translation around Sun",'r');
	glutAddMenuEntry("Use Translation & Rotation from 3d file",'t');
  //button= GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, or GLUT MIDDLE_BUTTON
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  // put init here
  init();

  // enter GLUT's main cycle.
	glutMainLoop();
  }
 }
 else cout << "Try again! Amount of arguments incorrect" << endl;
}
else cout << "Try again! Use: draw <filename.3d>" << endl;
return 1;
}
void init () {
	glClearColor(0.0,0.0,0.0,0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
  //default
	glPolygonMode(GL_FRONT,GL_LINE);
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
	// Clear buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the camera.
	glLoadIdentity();
	gluLookAt(px, py, pz,0.0,0.0,-1.0,0.0f,1.0f,0.0f);

	glRotatef( rotate_x, 1.0, 0.0, 0.0 );
	glRotatef( rotate_y, 0.0, 1.0, 0.0 );
	glRotatef( rotate_z, 0.0, 0.0, 1.0 );
	glTranslatef(translate_x,translate_y,translate_z);
  // Draw plane.
  glBegin(GL_TRIANGLES);
    if(colourON==1) glColor3f(0.5,0.5,0.5);
    glVertex3f(-600, 0, -600);
    glVertex3f(-600, 0, 600);
    glVertex3f(600, 0, 600);

    glVertex3f(600, 0, -600);
    glVertex3f(-600, 0, -600);
    glVertex3f(600, 0, 600);
  glEnd();

  // Sun.
	vector<float> vrtx_sun;
	vrtx_sun = vertices[0];
  if(colourON==1) glColor3f(1.0, 1.0, 0.0);
    glTranslatef(0.0, 10.0, 0.0);
	  if(movingON==2) {
		glRotatef(angles[0],rotatesX[0],rotatesY[0],rotatesZ[0]);
		glTranslatef(translatesX[0],translatesY[0],translatesZ[0]);
	}
  glBegin(GL_TRIANGLES);
    for (int j = 0; j < vrtx_sun.size();){
        glVertex3f(vrtx_sun[j++], vrtx_sun[j++], vrtx_sun[j++]);
    }
  glEnd();

  // Mercury
	vector<float> vrtx_Mercury;
	vrtx_Mercury = vertices[1];
  if(colourON==1) glColor3f(0.0, 0.5, 1);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
			glTranslatef(MercuryRadiusOrbit * cos(alphaS[1]), 1.0, MercuryRadiusOrbit * sin(alphaS[1])); //proxima posicao no eixo XoZ
    	glRotatef(10.0, 0.0, 1.0, 0.0);
		  if(movingON==2) {
			glRotatef(angles[1],rotatesX[1],rotatesY[1],rotatesZ[1]);
			glTranslatef(translatesX[1],translatesY[1],translatesZ[1]);
		}
    glBegin(GL_TRIANGLES);
      for (int j = 0; j < vrtx_Mercury.size();){
          glVertex3f(vrtx_Mercury[j++], vrtx_Mercury[j++], vrtx_Mercury[j++]);
      }
    glEnd();
    glPopMatrix();
		if(movingON==1) alphaS[1] += alphaIncS[1]; // andar em sentido contrario, em circulos
  }
  if(movingON==1) alphaS[1] += 10.0;

  // Venus
	vector<float> vrtx_Venus;
	vrtx_Venus = vertices[2];
  if(colourON==1) glColor3f(1.0, 0.2, 0.2);
  for (int i = 0; i < 1; i++) {
      glPushMatrix();
      glTranslatef(VenusRadiusOrbit * cos(alphaS[2]), 1.0, VenusRadiusOrbit * sin(alphaS[2]));
		  if(movingON==2) {
			glRotatef(angles[2],rotatesX[2],rotatesY[2],rotatesZ[2]);
			glTranslatef(translatesX[2],translatesY[2],translatesZ[2]);
		}
		glBegin(GL_TRIANGLES);
      for (int j = 0; j < vrtx_Venus.size();){
          glVertex3f(vrtx_Venus[j++], vrtx_Venus[j++], vrtx_Venus[j++]);
      }
    glEnd();
    glPopMatrix();
		if(movingON==1) alphaS[2] -= alphaIncS[2]; // andar em sentido contrario, em circulos
  }
  if(movingON==1) alphaS[2] -= 10.0;

  // Terra
	vector<float> vrtx_Earth;
	vrtx_Earth = vertices[3];
  if(colourON==1) glColor3f(1.0, 0.5, 1.0);
  for (int i = 0; i < 1; i++) {
      glPushMatrix();
      glTranslatef(TerraRadiusOrbit * cos(alphaS[3]), 1.0, TerraRadiusOrbit * sin(alphaS[3]));
		  if(movingON==2) {
			glRotatef(angles[3],rotatesX[3],rotatesY[3],rotatesZ[3]);
			glTranslatef(translatesX[3],translatesY[3],translatesZ[3]);
		}
		glBegin(GL_TRIANGLES);
      for (int j = 0; j < vrtx_Earth.size();){
          glVertex3f(vrtx_Earth[j++], vrtx_Earth[j++], vrtx_Earth[j++]);
      }
    glEnd();
    glPopMatrix();
		if(movingON==1) alphaS[3] += alphaIncS[3]; // andar em sentido contrario, em circulos
  }
  if(movingON==1) alphaS[3] += 10.0;

  // Marte
	vector<float> vrtx_Mars;
	vrtx_Mars = vertices[4];
  if(colourON==1) glColor3f(0.0, 0.5, 1.0);
  for (int i = 0; i < 1; i++) {
      glPushMatrix();
      glTranslatef(MarteRadiusOrbit * cos(alphaS[4]), 1.0, MarteRadiusOrbit * sin(alphaS[4]));
		  if(movingON==2) {
			glRotatef(angles[4],rotatesX[4],rotatesY[4],rotatesZ[4]);
			glTranslatef(translatesX[4],translatesY[4],translatesZ[4]);
		}
		glBegin(GL_TRIANGLES);
      for (int j = 0; j < vrtx_Mars.size();){
          glVertex3f(vrtx_Mars[j++], vrtx_Mars[j++], vrtx_Mars[j++]);
      }
    glEnd();
    glPopMatrix();
		if(movingON==1) alphaS[4] += alphaIncS[4]; // andar em sentido contrario, em circulos
  }
  if(movingON==1) alphaS[4] += 10.0;

  // Jupiter
	vector<float> vrtx_Jupiter;
	vrtx_Jupiter = vertices[5];
  if(colourON==1) glColor3f(0.5, 0.5, 0.5);
  for (int i = 0; i < 1; i++) {
      glPushMatrix();
      glTranslatef(JupiterRadiusOrbit * cos(alphaS[5]), 1.0, JupiterRadiusOrbit * sin(alphaS[5]));
		  if(movingON==2) {
			glRotatef(angles[5],rotatesX[5],rotatesY[5],rotatesZ[5]);
			glTranslatef(translatesX[5],translatesY[5],translatesZ[5]);
		}
		glBegin(GL_TRIANGLES);
      for (int j = 0; j < vrtx_Jupiter.size();){
          glVertex3f(vrtx_Jupiter[j++], vrtx_Jupiter[j++], vrtx_Jupiter[j++]);
      }
    glEnd();
    glPopMatrix();
		if(movingON==1) alphaS[5] += alphaIncS[5]; // andar em sentido contrario, em circulos
  }
  if(movingON==1) alphaS[5] += 10.0;

  // Saturno
	vector<float> vrtx_Saturn;
	vrtx_Saturn = vertices[6];
  if(colourON==1) glColor3f(0.0, 0.0, 0.5);
  for (int i = 0; i < 1; i++) {
     glPushMatrix();
     glTranslatef(SaturnoRadiusOrbit * cos(alphaS[6]), 1.0, SaturnoRadiusOrbit * sin(alphaS[6]));
		 if(movingON==2) {
		 glRotatef(angles[6],rotatesX[6],rotatesY[6],rotatesZ[6]);
		 glTranslatef(translatesX[6],translatesY[6],translatesZ[6]);
		}
		glBegin(GL_TRIANGLES);
      for (int j = 0; j < vrtx_Saturn.size();){
          glVertex3f(vrtx_Saturn[j++], vrtx_Saturn[j++], vrtx_Saturn[j++]);
      }
    glEnd();
		if(colourON==1) glColor3f(0.0, 1.0, 0.5);
		glutWireTorus(1.5,10,20,20);
    glPopMatrix();
		if(movingON==1) alphaS[6] += alphaIncS[6]; // andar em sentido contrario, em circulos
  }
  if(movingON==1) alphaS[6] += 10.0;

  // Urano
	vector<float> vrtx_Uranus;
	vrtx_Uranus = vertices[7];
  if(colourON==1) glColor3f(1.0, 0.5, 0.0);
  for (int i = 0; i < 1; i++) {
      glPushMatrix();
      glTranslatef(UranoRadiusOrbit * cos(alphaS[7]), 1.0, UranoRadiusOrbit * sin(alphaS[7]));
		  if(movingON==2) {
			glRotatef(angles[7],rotatesX[7],rotatesY[7],rotatesZ[7]);
			glTranslatef(translatesX[7],translatesY[7],translatesZ[7]);
		}
		glBegin(GL_TRIANGLES);
      for (int j = 0; j < vrtx_Uranus.size();){
          glVertex3f(vrtx_Uranus[j++], vrtx_Uranus[j++], vrtx_Uranus[j++]);
      }
    glEnd();
		if(colourON==1) glColor3f(0.0, 0.0, 0.5);
		glutWireTorus(1.5,10,20,20);
    glPopMatrix();
		if(movingON==1) alphaS[7] -= alphaIncS[7]; // andar em sentido contrario, em circulos
  }
  if(movingON==1) alphaS[7] -= 10.0;

  // Neptuno
	vector<float> vrtx_Neptune;
	vrtx_Neptune = vertices[8];
  if(colourON==1) glColor3f(0.0, 1.0, 0.5);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(NeptunoRadiusOrbit * cos(alphaS[8]), 1.0, NeptunoRadiusOrbit * sin(alphaS[8]));
		if(movingON==2) {
			glRotatef(angles[8],rotatesX[8],rotatesY[8],rotatesZ[8]);
			glTranslatef(translatesX[8],translatesY[8],translatesZ[8]);
		}
		glBegin(GL_TRIANGLES);
      for (int j = 0; j < vrtx_Neptune.size();){
          glVertex3f(vrtx_Neptune[j++], vrtx_Neptune[j++], vrtx_Neptune[j++]);
      }
    glEnd();
		if(colourON==1) glColor3f(0.5, 1.0, 0.5);
		glutWireTorus(1.5,15,20,20);
    glPopMatrix();
		if(movingON==1) alphaS[8] += alphaIncS[8]; // andar em sentido contrario, em circulos
  }
  if(movingON==1) alphaS[8] += 10.0;

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
			else if (key == 's' || key == 'S') movingON=0;
			else if (key == 'r' || key == 'R') movingON=1;
			else if (key == 't' || key == 'T') movingON=2;
			// Move only camera
			else if (key == 'g') py += 50;
			else if (key == 'h') py -= 50;
			else if (key == 'i') pz += 50;
			else if (key == 'u') pz += 50;
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
			else if (key == '8'){
        colourON=1;
      }
			else if (key == '9'){
        colourON=0;
				glColor3f(1.0f,1.0f,1.0f);
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

void preencheAlphaS(){
 float aux4,aux5;
 for (int j = 0; j < planets.size();j++){
	 aux4 = 0.0;
	 alphaS.push_back(aux4);
	 aux5 = (2 * M_PI)/10.0;
	 alphaIncS.push_back(aux4);
 }
}

void printAlphaS(){
 cout << "\nAlphaS" << endl;
 for (int j = 0; j < alphaS.size();j++){
	 cout << alphaS[j] << endl;
 }
 cout << "\nAlphaIncS" << endl;
 for (int j = 0; j < alphaIncS.size();j++){
	 cout <<  alphaIncS[j] << endl;
 }
}
