//windows
// #include <GL/glut.h>
// #include <GL/glu.h>
// #include <GL/gl.h>

// MAC_OS_X
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string>
#include <math.h>
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

// Global variable that allows to know if XML file was found and read
int read=0;

// Declare radii of all 8 planets & Sun
float SunRadius         = 8.0;
float MercuryRadius     = 2.0;
float VenusRadius       = 3.0;
float TerraRadius       = 3.0;
float MarteRadius       = 4.0;
float JupiterRadius     = 8.0;
float UranoRadius       = 7.0;
float SaturnoRadius     = 9.0;
float NeptunoRadius     = 7.0;


// Declare radii of all 8 planets orbits
float MercuryRadiusOrbit     = 20.0;
float VenusRadiusOrbit       = 30.0;
float TerraRadiusOrbit       = 40.0;
float MarteRadiusOrbit       = 50.0;
float JupiterRadiusOrbit     = 65.0;
float UranoRadiusOrbit       = 80.0;
float SaturnoRadiusOrbit     = 95.0;
float NeptunoRadiusOrbit     = 100.0;

// Global variables that allows drawing circles os planets around the sun
float alphaM            = 0.0;
float alphaIncM         = (2 * M_PI)/10.0;

float alphaV            = 0.0;
float alphaIncV         = (2 * M_PI)/10.0;

float alphaT            = 0.0;
float alphaIncT         = (2 * M_PI)/10.0;

float alphaMT           = 0.0;
float alphaIncMT        = (2 * M_PI)/10.0;

float alphaJ            = 0.0;
float alphaIncJ         = (2 * M_PI)/10.0;

float alphaS            = 0.0;
float alphaIncS         = (2 * M_PI)/10.0;

float alphaU            = 0.0;
float alphaIncU         = (2 * M_PI)/10.0;

float alphaN            = 0.0;
float alphaIncN         = (2 * M_PI)/10.0;

// Global variables that allows controlling the xml content
int num_planets=0,num_moons=0;

// Global variables that have the xml content of the sun
int sun_X,sun_Y,sun_Z,sun_axisX,sun_axisY,sun_axisZ;
float sun_angle;
string sun_filename;

// Global variables that have the xml content of Mercury
int mercury_X,mercury_Y,mercury_Z,mercury_axisX,mercury_axisY,mercury_axisZ;
float mercury_angle;
string mercury_filename;

// Global variables that have the xml content of Venus
int venus_X,venus_Y,venus_Z,venus_axisX,venus_axisY,venus_axisZ;
float venus_angle;
string venus_filename;

// Global variables that have the xml content of Earth
int earth_X,earth_Y,earth_Z,earth_axisX,earth_axisY,earth_axisZ;
float earth_angle;
string earth_filename,earth_moon_filename;

// Global variables that have the xml content of Mars
int mars_X,mars_Y,mars_Z,mars_axisX,mars_axisY,mars_axisZ;
float mars_angle;
string mars_filename,mars_moon1_filename,mars_moon2_filename;

// Global variables that have the xml content of Jupiter
int jupiter_X,jupiter_Y,jupiter_Z,jupiter_axisX,jupiter_axisY,jupiter_axisZ;
float jupiter_angle;
string jupiter_filename,jupiter_moon1_filename,jupiter_moon2_filename,jupiter_moon3_filename,jupiter_moon4_filename;

// Global variables that have the xml content of Saturn
int saturn_X,saturn_Y,saturn_Z,saturn_axisX,saturn_axisY,saturn_axisZ;
float saturn_angle;
string saturn_filename,saturn_moon1_filename,saturn_moon2_filename;

// Global variables that have the xml content of Uranus
int uranus_X,uranus_Y,uranus_Z,uranus_axisX,uranus_axisY,uranus_axisZ;
float uranus_angle;
string uranus_filename,uranus_moon1_filename,uranus_moon2_filename,uranus_moon3_filename,uranus_moon4_filename,uranus_moon5_filename;

// Global variables that have the xml content of Neptune
int neptune_X,neptune_Y,neptune_Z,neptune_axisX,neptune_axisY,neptune_axisZ;
float neptune_angle;
string neptune_filename,neptune_moon1_filename,neptune_moon2_filename;

void fillTranslates(int x, int y, int z, int count){
  switch (count) {
    case 1:
           sun_X=x;
           sun_Y=y;
           sun_Z=z;
           break;
    case 2:
           mercury_X=x;
           mercury_Y=y;
           mercury_Z=z;
           break;
    case 3:
           venus_X=x;
           venus_Y=y;
           venus_Z=z;
           break;
    case 4:
           earth_X=x;
           earth_Y=y;
           earth_Z=z;
           break;
    case 5:
           mars_X=x;
           mars_Y=y;
           mars_Z=z;
           break;
    case 6:
           jupiter_X=x;
           jupiter_Y=y;
           jupiter_Z=z;
           break;
    case 7:
           saturn_X=x;
           saturn_Y=y;
           saturn_Z=z;
           break;
    case 8:
           uranus_X=x;
           uranus_Y=y;
           uranus_Z=z;
           break;
    case 9:
           neptune_X=x;
           neptune_Y=y;
           neptune_Z=z;
           break;

    default:
           break;
  }

}

void printTranslates(){
    printf("%d\n", sun_X);
    printf("%d\n", sun_Y);
    printf("%d\n", sun_Z);
    printf("%d\n", mercury_X);
    printf("%d\n", mercury_Y);
    printf("%d\n", mercury_Z);
    printf("%d\n", venus_X);
    printf("%d\n", venus_Y);
    printf("%d\n", venus_Z);
    printf("%d\n", earth_X);
    printf("%d\n", earth_Y);
    printf("%d\n", earth_Z);
    printf("%d\n", mars_X);
    printf("%d\n", mars_Y);
    printf("%d\n", mars_Z);
    printf("%d\n", jupiter_X);
    printf("%d\n", jupiter_Y);
    printf("%d\n", jupiter_Z);
    printf("%d\n", saturn_X);
    printf("%d\n", saturn_Y);
    printf("%d\n", saturn_Z);
    printf("%d\n", uranus_X);
    printf("%d\n", uranus_Y);
    printf("%d\n", uranus_Z);
    printf("%d\n", neptune_X);
    printf("%d\n", neptune_Y);
    printf("%d\n", neptune_Z);
}

void fillRotates(float ang, int x, int y, int z, int count){
  switch (count) {
    case 1:
           sun_angle=ang;
           sun_axisX=x;
           sun_axisY=y;
           sun_axisZ=z;
           break;
    case 2:
           mercury_angle=ang;
           mercury_axisX=x;
           mercury_axisY=y;
           mercury_axisZ=z;
           break;
    case 3:
           venus_angle=ang;
           venus_axisX=x;
           venus_axisY=y;
           venus_axisZ=z;
           break;
    case 4:
           earth_angle=ang;
           earth_axisX=x;
           earth_axisY=y;
           earth_axisZ=z;
           break;
    case 5:
           mars_angle=ang;
           mars_axisX=x;
           mars_axisY=y;
           mars_axisZ=z;
           break;
    case 6:
           jupiter_angle=ang;
           jupiter_axisX=x;
           jupiter_axisY=y;
           jupiter_axisZ=z;
           break;
    case 7:
           saturn_angle=ang;
           saturn_axisX=x;
           saturn_axisY=y;
           saturn_axisZ=z;
           break;
    case 8:
           uranus_angle=ang;
           uranus_axisX=x;
           uranus_axisY=y;
           uranus_axisZ=z;
           break;
    case 9:
           neptune_angle=ang;
           neptune_axisX=x;
           neptune_axisY=y;
           neptune_axisZ=z;
           break;

    default:
           break;
  }
}

void printRotates(){
    printf("%f\n", sun_angle);
    printf("%d\n", sun_axisX);
    printf("%d\n", sun_axisY);
    printf("%d\n", sun_axisZ);
    printf("%f\n", mercury_angle);
    printf("%d\n", mercury_axisX);
    printf("%d\n", mercury_axisY);
    printf("%d\n", mercury_axisZ);
    printf("%f\n", venus_angle);
    printf("%d\n", venus_axisX);
    printf("%d\n", venus_axisY);
    printf("%d\n", venus_axisZ);
    printf("%f\n", earth_angle);
    printf("%d\n", earth_axisX);
    printf("%d\n", earth_axisY);
    printf("%d\n", earth_axisZ);
    printf("%f\n", mars_angle);
    printf("%d\n", mars_axisX);
    printf("%d\n", mars_axisY);
    printf("%d\n", mars_axisZ);
    printf("%f\n", jupiter_angle);
    printf("%d\n", jupiter_axisX);
    printf("%d\n", jupiter_axisY);
    printf("%d\n", jupiter_axisZ);
    printf("%f\n", saturn_angle);
    printf("%d\n", saturn_axisX);
    printf("%d\n", saturn_axisY);
    printf("%d\n", saturn_axisZ);
    printf("%f\n", uranus_angle);
    printf("%d\n", uranus_axisX);
    printf("%d\n", uranus_axisY);
    printf("%d\n", uranus_axisZ);
    printf("%f\n", neptune_angle);
    printf("%d\n", neptune_axisX);
    printf("%d\n", neptune_axisY);
    printf("%d\n", neptune_axisZ);
}

void fillPlanetsFilenames(string name, int count){
  switch (count) {
    case 1:
           sun_filename=name;
           break;
    case 2:
           mercury_filename=name;
           break;
    case 3:
           venus_filename=name;
           break;
    case 4:
           earth_filename=name;
           break;
    case 5:
           mars_filename=name;
           break;
    case 6:
           jupiter_filename=name;
           break;
    case 7:
           saturn_filename=name;
           break;
    case 8:
           uranus_filename=name;
           break;
    case 9:
           neptune_filename=name;
           break;

    default:
           break;
  }
}

void printPlanetsFilenames(){
    printf("%s\n", sun_filename.c_str());
    printf("%s\n", mercury_filename.c_str());
    printf("%s\n", venus_filename.c_str());
    printf("%s\n", earth_filename.c_str());
    printf("%s\n", mars_filename.c_str());
    printf("%s\n", jupiter_filename.c_str());
    printf("%s\n", saturn_filename.c_str());
    printf("%s\n", uranus_filename.c_str());
    printf("%s\n", neptune_filename.c_str());
}

void fillMoonsFilenames(string name, int count){
  switch (count) {
    case 1:
           earth_moon_filename=name;
           break;
    case 2:
           mars_moon1_filename=name;
           break;
    case 3:
           mars_moon2_filename=name;
           break;
    case 4:
           jupiter_moon1_filename=name;
           break;
    case 5:
           jupiter_moon2_filename=name;
           break;
    case 6:
           jupiter_moon3_filename=name;
           break;
    case 7:
           jupiter_moon4_filename=name;
           break;
    case 8:
           saturn_moon1_filename=name;
           break;
    case 9:
           saturn_moon2_filename=name;
           break;
    case 10:
           uranus_moon1_filename=name;
           break;
    case 11:
           uranus_moon2_filename=name;
           break;
    case 12:
           uranus_moon3_filename=name;
           break;
    case 13:
           uranus_moon4_filename=name;
           break;
    case 14:
           uranus_moon5_filename=name;
           break;
    case 15:
           neptune_moon1_filename=name;
           break;
    case 16:
           neptune_moon2_filename=name;
           break;
    default:
           break;
  }
}

void printMoonsFilenames(){
    printf("%s\n", earth_moon_filename.c_str());
    printf("%s\n", mars_moon1_filename.c_str());
    printf("%s\n", mars_moon2_filename.c_str());
    printf("%s\n", jupiter_moon1_filename.c_str());
    printf("%s\n", jupiter_moon2_filename.c_str());
    printf("%s\n", jupiter_moon3_filename.c_str());
    printf("%s\n", jupiter_moon4_filename.c_str());
    printf("%s\n", saturn_moon1_filename.c_str());
    printf("%s\n", saturn_moon2_filename.c_str());
    printf("%s\n", uranus_moon1_filename.c_str());
    printf("%s\n", uranus_moon2_filename.c_str());
    printf("%s\n", uranus_moon3_filename.c_str());
    printf("%s\n", uranus_moon4_filename.c_str());
    printf("%s\n", uranus_moon5_filename.c_str());
    printf("%s\n", neptune_moon1_filename.c_str());
    printf("%s\n", neptune_moon2_filename.c_str());
}

void readXML(){

// Load the xml file, I put your XML in a file named test.xml
TiXmlDocument XMLdoc("SolarSystemXML.xml");
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
            num_planets++;
            pTranslate = pGroup->FirstChildElement("translate" );
            while ( pTranslate )
            {
                //cout << "Translate X: " << pTranslate->Attribute("X") << endl;
                //cout << "Translate Y: " << pTranslate->Attribute("Y") << endl;
                //cout << "Translate Z: " << pTranslate->Attribute("Z") << endl;

                fillTranslates(atoi(pTranslate->Attribute("X")),atoi(pTranslate->Attribute("Y")),atoi(pTranslate->Attribute("Z")),num_planets);

                pTranslate = pTranslate->NextSiblingElement("translate");
            }

            pRotate = pGroup->FirstChildElement("rotate" );
            while ( pRotate )
            {
                //cout << "Angle: " << pRotate->Attribute("angle") << endl;
                //cout << "Axis X: " << pRotate->Attribute("axisX") << endl;
                //cout << "Axis Y: " << pRotate->Attribute("axisY") << endl;
                //cout << "Axis Z: " << pRotate->Attribute("axisZ") << endl;

                fillRotates(atof(pRotate->Attribute("angle")),atoi(pRotate->Attribute("axisX")),atoi(pRotate->Attribute("axisY")),atoi(pRotate->Attribute("axisZ")),num_planets);

                pRotate = pRotate->NextSiblingElement("rotate");
            }
            pModels = pGroup->FirstChildElement("models");
            while ( pModels )
            {
                //cout << "models: planets & sun" << endl;

                pModelFile = pModels->FirstChildElement("model");
                while( pModelFile )
                {
                    //cout << "Planet File: " << pModelFile->Attribute("file") << endl;

                    fillPlanetsFilenames(pModelFile->Attribute("file"),num_planets);

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
                //cout << "models: moons" << endl;
                num_moons++;

                pModelFileMoons = pModelsMoons->FirstChildElement("model");
                while( pModelFileMoons )
                {
                    //cout << "Moon File: " << pModelFileMoons->Attribute("file") << endl;

                    fillMoonsFilenames(pModelFileMoons->Attribute("file"),num_moons);

                    pModelFileMoons = pModelFileMoons->NextSiblingElement("model");
                }

                pModelsMoons = pModelsMoons->NextSiblingElement("models");
            }

              pMoons = pMoons->NextSiblingElement("group");
            }

            pGroup = pGroup->NextSiblingElement("group");
        }
    }
    else
    {
        cout << "Cannot find 'scene' node" << endl;
    }
}
  else
  {
        cout << "Cannot find XML file" << endl;
        read=0;
  }
}

int main(int argc, char **argv) {
  readXML();
  if(read!=0){
    printf("Translates\n");
    printTranslates();
    printf("\n\nRotates\n");
    printRotates();
    printf("\n\nPlanets\n");
    printPlanetsFilenames();
    printf("\n\nMoons\n");
    printMoonsFilenames();

  /*
   * Init GLUT and the window.
   */
  glutInit(&argc, argv);

  // Sets up a double buffer with RGBA components and a depth component
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

  // Sets the window size to 512*512 square pixels
  glutInitWindowSize(800, 800);

  // Sets the window position to the upper left
  glutInitWindowPosition(0, 0);

  // Creates a window using internal glut functionality
  glutCreateWindow("Solar System - Stage 2");

  /*
   * Register functions that will process events.
   */

  // Callback function responsible for the window's contents.
  glutDisplayFunc(renderScene);
  // Function to be callend when the window is created or resized.
  glutReshapeFunc(changeSize);
  // Function called when the event queue is empty.
  glutIdleFunc(renderScene);
  // Function called when a key is pressed.
  glutKeyboardFunc(keyPressed);

  // put init here
  init();

  // enter GLUT's main cycle.
	glutMainLoop();
  }
	return 0;
}

void init () {
	glClearColor(0.0,0.0,0.0,0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
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
	gluLookAt(0.0,200.0,300.0,0.0,0.0,-1.0,0.0f,1.0f,0.0f);

  // Draw plane.
  glBegin(GL_TRIANGLES);
    glColor3f(0.9,0.9,0.75);

    glVertex3f(-100, 0, -100);
    glVertex3f(-100, 0, 100);
    glVertex3f(100, 0, 100);

    glColor3f(0,1,1);

    glVertex3f(100, 0, -100);
    glVertex3f(-100, 0, -100);
    glVertex3f(100, 0, 100);
  glEnd();

  // Sun.
  glColor3f(1.0, 1.0, 0.0);
  glTranslatef(0.0, 10.0, 0.0);
  glutWireSphere(SunRadius, 30, 30);

  // Mercury
  glColor3f(0.0, 0.5, 1);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(MercuryRadiusOrbit * cos(alphaM), 1.0, MercuryRadiusOrbit * sin(alphaM)); //proxima posicao no eixo XoZ
    glRotatef(10.0, 0.0, 1.0, 0.0);
    glutWireSphere(MercuryRadius, 30, 30);
    glPopMatrix();
    alphaM -= alphaIncM; // andar em sentido contrario, em circulos
  }
  alphaM -= 100.0;

  // Venus
  glColor3f(1.0, 0.2, 0.2);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(VenusRadiusOrbit * cos(alphaV), 1.0, VenusRadiusOrbit * sin(alphaV));
    glutWireSphere(VenusRadius, 30, 30);
    glPopMatrix();
    alphaV += alphaIncV;
  }
  alphaV += 100.0;

  // Terra
  glColor3f(1.0, 0.5, 1.0);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(TerraRadiusOrbit * cos(alphaT), 1.0, TerraRadiusOrbit * sin(alphaT));
    glutWireSphere(TerraRadius, 30, 30);
    glPopMatrix();
    alphaT -= alphaIncT;
  }
  alphaT -= 100.0;

  // Marte
  glColor3f(0.0, 0.5, 1.0);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(MarteRadiusOrbit * cos(alphaMT), 1.0, MarteRadiusOrbit * sin(alphaMT));
    glutWireSphere(MarteRadius, 30, 30);
    glPopMatrix();
    alphaMT += alphaIncMT;
  }
  alphaMT += 100.0;

  // Jupiter
  glColor3f(0.5, 0.5, 0.5);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(JupiterRadiusOrbit * cos(alphaJ), 1.0, JupiterRadiusOrbit * sin(alphaJ));
    glutWireSphere(JupiterRadius, 30, 30);
    glPopMatrix();
    alphaJ += alphaIncJ;
  }
  alphaJ += 100.0;

  // Saturno
  glColor3f(0.0, 0.0, 0.5);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(SaturnoRadiusOrbit * cos(alphaS), 1.0, SaturnoRadiusOrbit * sin(alphaS));
    glutWireSphere(SaturnoRadius, 30, 30);
    glPopMatrix();
    alphaS -= alphaIncS;
  }
  alphaS -= 100.0;

  // Urano
  glColor3f(1.0, 0.5, 0.0);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(UranoRadiusOrbit * cos(alphaU), 1.0, UranoRadiusOrbit * sin(alphaU));
    glutWireSphere(UranoRadius, 30, 30);
    glPopMatrix();
    alphaU -= alphaIncU;
  }
  alphaU -= 100.0;

  // Neptuno
  glColor3f(0.0, 1.0, 0.5);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(NeptunoRadiusOrbit * cos(alphaN), 1.0, NeptunoRadiusOrbit * sin(alphaN));
    glutWireSphere(NeptunoRadius, 30, 30);
    glPopMatrix();
    alphaN += alphaIncN;
  }
  alphaN += 100.0;

	// End of frame.
	glutSwapBuffers();
}

void keyPressed(unsigned char k, int x, int y) {
  if (k == 27) { exit(0); }
}
