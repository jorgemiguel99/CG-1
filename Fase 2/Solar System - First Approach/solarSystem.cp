//windows
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

// MAC_OS_X
//#include <GLUT/glut.h>
//#include <OpenGL/glu.h>
//#include <OpenGL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define _USE_MATH_DEFINES
#include <math.h>

void init();
void changeSize(int, int);
void renderScene(void);
void idleScene(void);
void keyPressed(unsigned char, int, int);

float MercuryRadius     = 20.0;
float VenusRadius       = 30.0;
float TerraRadius       = 40.0;
float MarteRadius       = 50.0;
float JupiterRadius     = 65.0;
float UranoRadius       = 80.0;
float SaturnoRadius     = 95.0;
float NeptunoRadius     = 100.0;


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


int main(int argc, char **argv) {
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
  glutWireSphere(8, 30, 30);

  // Mercury
  glColor3f(0.0, 0.5, 1);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(MercuryRadius * cos(alphaM), 1.0, MercuryRadius * sin(alphaM)); //proxima posicao no eixo XoZ
    glRotatef(10.0, 0.0, 1.0, 0.0);
    glutWireSphere(2, 30, 30);
    glPopMatrix();
    alphaM -= alphaIncM; // andar em sentido contrario, em circulos
  }
  alphaM -= 100.0;

  // Venus
  glColor3f(1.0, 0.2, 0.2);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(VenusRadius * cos(alphaV), 1.0, VenusRadius * sin(alphaV));
    glutWireSphere(3, 30, 30);
    glPopMatrix();
    alphaV += alphaIncV;
  }
  alphaV += 100.0;

  // Terra
  glColor3f(1.0, 0.5, 1.0);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(TerraRadius * cos(alphaT), 1.0, TerraRadius * sin(alphaT));
    glutWireSphere(3, 30, 30);
    glPopMatrix();
    alphaT -= alphaIncT;
  }
  alphaT -= 100.0;

  // Marte
  glColor3f(0.0, 0.5, 1.0);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(MarteRadius * cos(alphaMT), 1.0, MarteRadius * sin(alphaMT));
    glutWireSphere(4, 30, 30);
    glPopMatrix();
    alphaMT += alphaIncMT;
  }
  alphaMT += 100.0;

  // Jupiter
  glColor3f(0.5, 0.5, 0.5);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(JupiterRadius * cos(alphaJ), 1.0, JupiterRadius * sin(alphaJ));
    glutWireSphere(8, 30, 30);
    glPopMatrix();
    alphaJ += alphaIncJ;
  }
  alphaJ += 100.0;

  // Saturno
  glColor3f(0.0, 0.0, 0.5);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(SaturnoRadius * cos(alphaS), 1.0, SaturnoRadius * sin(alphaS));
    glutWireSphere(7, 30, 30);
    glPopMatrix();
    alphaS -= alphaIncS;
  }
  alphaS -= 100.0;

  // Urano
  glColor3f(1.0, 0.5, 0.0);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(UranoRadius * cos(alphaU), 1.0, UranoRadius * sin(alphaU));
    glutWireSphere(9, 30, 30);
    glPopMatrix();
    alphaU -= alphaIncU;
  }
  alphaU -= 100.0;

  // Neptuno
  glColor3f(0.0, 1.0, 0.5);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(NeptunoRadius * cos(alphaN), 1.0, NeptunoRadius * sin(alphaN));
    glutWireSphere(7, 30, 30);
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
