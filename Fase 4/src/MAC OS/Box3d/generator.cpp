#define _USE_MATH_DEFINES

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GLUT/glut.h> //-- MAC
#include </usr/local/Cellar/tinyxml/2.6.2/include/tinyxml.h> //-- MAC
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

using namespace std;

#define _PI_ 3.14159

//Global Variables to Transformations
float px = 0, py = 0, pz = 5;

float rotate_y = 0;
float rotate_x = 0;
float rotate_z = 0;

float translate_y = 0;
float translate_x = 0;
float translate_z = 0;

float alfa = 0.0f, beta = 0.0f, radii = 5;
float camX, camY, camZ;

/* Texture related variables. */
unsigned int texID;
int cols, rows;

GLfloat amb[4]  = {0.2, 0.2, 0.2, 1.0};
GLfloat diff[4] = {1.0, 1.0, 1.0, 1.0};
GLfloat pos[4]  = {1.0, 1.0, 1.0, 0.0};

// Global variable store 3d file read
vector<string> file3dRead;

// Global variable process input
vector<string> splitted;

GLuint vertexCount, vertices;
GLuint buffers[3];

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

void prepareBox(float length, float width, float height) {
	ofstream box;
  box.open("box.3d");
  float * v;
  float * n;
  float * t;
  int vertex  = 0;
  v = (float *) malloc(sizeof(float) * 1000000);
  n = (float *) malloc(sizeof(float) * 1000000);
  t = (float *) malloc(sizeof(float) * 1000000);

  // Vertices -------------------------------------------------------------
  // Front face
  v[vertex++] = 0.0f; v[vertex++] = 0.0f; v[vertex++] = 0.0f;
  v[vertex++] = width; v[vertex++] = 0.0f; v[vertex++] = 0.0f;
  v[vertex++] = 0.0f; v[vertex++] = height; v[vertex++] = 0.0f;

  v[vertex++] = width; v[vertex++] = 0.0f; v[vertex++] = 0.0f;
  v[vertex++] = width; v[vertex++] = height; v[vertex++] = 0.0f;
  v[vertex++] = 0.0f; v[vertex++] = height; v[vertex++] = 0.0f;

  // Back face
  v[vertex++] = width; v[vertex++] = 0.0f; v[vertex++] = -length;
  v[vertex++] = 0.0f; v[vertex++] = 0.0f; v[vertex++] = -length;
  v[vertex++] = 0.0f; v[vertex++] = height; v[vertex++] = -length;

  v[vertex++] = 0.0f; v[vertex++] = height; v[vertex++] = -length;
  v[vertex++] = width; v[vertex++] = height; v[vertex++] = -length;
  v[vertex++] = width; v[vertex++] = 0.0f; v[vertex++] = -length;

  // Right face
  v[vertex++] = width; v[vertex++] = 0.0f; v[vertex++] = 0.0f;
  v[vertex++] = width; v[vertex++] = 0.0f; v[vertex++] = -length;
  v[vertex++] = width; v[vertex++] = height; v[vertex++] = 0.0f;

  v[vertex++] = width; v[vertex++] = 0.0f; v[vertex++] = -length;
  v[vertex++] = width; v[vertex++] = height; v[vertex++] = -length;
  v[vertex++] = width; v[vertex++] = height; v[vertex++] = 0.0f;

  // Left face
  v[vertex++] = 0.0f; v[vertex++] = 0.0f; v[vertex++] = 0.0f;
  v[vertex++] = 0.0f; v[vertex++] = height; v[vertex++] = 0.0f;
  v[vertex++] = 0.0f; v[vertex++] = 0.0f; v[vertex++] = -length;

  v[vertex++] = 0.0f; v[vertex++] = 0.0f; v[vertex++] = -length;
  v[vertex++] = 0.0f; v[vertex++] = height; v[vertex++] = 0.0f;
  v[vertex++] = 0.0f; v[vertex++] = height; v[vertex++] = -length;

  // Upper face
  v[vertex++] = 0.0f; v[vertex++] = height; v[vertex++] = 0.0f;
  v[vertex++] = width; v[vertex++] = height; v[vertex++] = 0.0f;
  v[vertex++] = 0.0f; v[vertex++] = height; v[vertex++] = -length;

  v[vertex++] = 0.0f; v[vertex++] = height; v[vertex++] = -length;
  v[vertex++] = width; v[vertex++] = height; v[vertex++] = 0.0f;
  v[vertex++] = width; v[vertex++] = height; v[vertex++] = -length;

  // Lower face
  v[vertex++] = 0.0f; v[vertex++] = 0.0f; v[vertex++] = 0.0f;
  v[vertex++] = 0.0f; v[vertex++] = 0.0f; v[vertex++] = -length;
  v[vertex++] = width; v[vertex++] = 0.0f; v[vertex++] = 0.0f;

  v[vertex++] = 0.0f; v[vertex++] = 0.0f; v[vertex++] = -length;
  v[vertex++] = width; v[vertex++] = 0.0f; v[vertex++] = -length;
  v[vertex++] = width; v[vertex++] = 0.0f; v[vertex++] = 0.0f;

  vertex = 0;

  // Normals --------------------------------------------------------------
  // Front face
  n[vertex++] = 0.0f; n[vertex++] = 0.0f; n[vertex++] = 1.0f;
  n[vertex++] = 0.0f; n[vertex++] = 0.0f; n[vertex++] = 1.0f;
  n[vertex++] = 0.0f; n[vertex++] = 0.0f; n[vertex++] = 1.0f;

  n[vertex++] = 0.0f; n[vertex++] = 0.0f; n[vertex++] = 1.0f;
  n[vertex++] = 0.0f; n[vertex++] = 0.0f; n[vertex++] = 1.0f;
  n[vertex++] = 0.0f; n[vertex++] = 0.0f; n[vertex++] = 1.0f;

  // Back face
  n[vertex++] = 0.0f; n[vertex++] = 0.0f; n[vertex++] = -1.0f;
  n[vertex++] = 0.0f; n[vertex++] = 0.0f; n[vertex++] = -1.0f;
  n[vertex++] = 0.0f; n[vertex++] = 0.0f; n[vertex++] = -1.0f;

  n[vertex++] = 0.0f; n[vertex++] = 0.0f; n[vertex++] = -1.0f;
  n[vertex++] = 0.0f; n[vertex++] = 0.0f; n[vertex++] = -1.0f;
  n[vertex++] = 0.0f; n[vertex++] = 0.0f; n[vertex++] = -1.0f;

  // Right face
  n[vertex++] = 1.0f; n[vertex++] = 0.0f; n[vertex++] = 0.0f;
  n[vertex++] = 1.0f; n[vertex++] = 0.0f; n[vertex++] = 0.0f;
  n[vertex++] = 1.0f; n[vertex++] = 0.0f; n[vertex++] = 0.0f;

  n[vertex++] = 1.0f; n[vertex++] = 0.0f; n[vertex++] = 0.0f;
  n[vertex++] = 1.0f; n[vertex++] = 0.0f; n[vertex++] = 0.0f;
  n[vertex++] = 1.0f; n[vertex++] = 0.0f; n[vertex++] = 0.0f;

  // Left face
  n[vertex++] = -1.0f; n[vertex++] = 0.0f; n[vertex++] = 0.0f;
  n[vertex++] = -1.0f; n[vertex++] = 0.0f; n[vertex++] = 0.0f;
  n[vertex++] = -1.0f; n[vertex++] = 0.0f; n[vertex++] = 0.0f;

  n[vertex++] = -1.0f; n[vertex++] = 0.0f; n[vertex++] = 0.0f;
  n[vertex++] = -1.0f; n[vertex++] = 0.0f; n[vertex++] = 0.0f;
  n[vertex++] = -1.0f; n[vertex++] = 0.0f; n[vertex++] = 0.0f;

  // Upper face
  n[vertex++] = 0.0f; n[vertex++] = 1.0f; n[vertex++] = 0.0f;
  n[vertex++] = 0.0f; n[vertex++] = 1.0f; n[vertex++] = 0.0f;
  n[vertex++] = 0.0f; n[vertex++] = 1.0f; n[vertex++] = 0.0f;

  n[vertex++] = 0.0f; n[vertex++] = 1.0f; n[vertex++] = 0.0f;
  n[vertex++] = 0.0f; n[vertex++] = 1.0f; n[vertex++] = 0.0f;
  n[vertex++] = 0.0f; n[vertex++] = 1.0f; n[vertex++] = 0.0f;

  // Lower face
  n[vertex++] = 0.0f; n[vertex++] = -1.0f; n[vertex++] = 0.0f;
  n[vertex++] = 0.0f; n[vertex++] = -1.0f; n[vertex++] = 0.0f;
  n[vertex++] = 0.0f; n[vertex++] = -1.0f; n[vertex++] = 0.0f;

  n[vertex++] = 0.0f; n[vertex++] = -1.0f; n[vertex++] = 0.0f;
  n[vertex++] = 0.0f; n[vertex++] = -1.0f; n[vertex++] = 0.0f;
  n[vertex++] = 0.0f; n[vertex++] = -1.0f; n[vertex++] = 0.0f;

  vertex = 0;

  // Textures -------------------------------------------------------------
  // Front face
  t[vertex++] = 0.0f; t[vertex++] = 0.0f;
  t[vertex++] = 1.0f; t[vertex++] = 0.0f;
  t[vertex++] = 0.0f; t[vertex++] = 1.0f;

  t[vertex++] = 1.0f; t[vertex++] = 0.0f;
  t[vertex++] = 1.0f; t[vertex++] = 1.0f;
  t[vertex++] = 0.0f; t[vertex++] = 1.0f;

  // Back face
  t[vertex++] = 0.0f; t[vertex++] = 0.0f;
  t[vertex++] = 1.0f; t[vertex++] = 0.0f;
  t[vertex++] = 1.0f; t[vertex++] = 1.0f;

  t[vertex++] = 1.0f; t[vertex++] = 1.0f;
  t[vertex++] = 0.0f; t[vertex++] = 1.0f;
  t[vertex++] = 0.0f; t[vertex++] = 0.0f;

  // Right face
  t[vertex++] = 0.0f; t[vertex++] = 0.0f;
  t[vertex++] = 1.0f; t[vertex++] = 0.0f;
  t[vertex++] = 0.0f; t[vertex++] = 1.0f;

  t[vertex++] = 1.0f; t[vertex++] = 0.0f;
  t[vertex++] = 1.0f; t[vertex++] = 1.0f;
  t[vertex++] = 0.0f; t[vertex++] = 1.0f;

  // Left face
  t[vertex++] = 1.0f; t[vertex++] = 0.0f;
  t[vertex++] = 1.0f; t[vertex++] = 1.0f;
  t[vertex++] = 0.0f; t[vertex++] = 0.0f;

  t[vertex++] = 0.0f; t[vertex++] = 0.0f;
  t[vertex++] = 1.0f; t[vertex++] = 1.0f;
  t[vertex++] = 0.0f; t[vertex++] = 1.0f;

  // Upper face
  t[vertex++] = 0.0f; t[vertex++] = 0.0f;
  t[vertex++] = 1.0f; t[vertex++] = 0.0f;
  t[vertex++] = 0.0f; t[vertex++] = 1.0f;

  t[vertex++] = 0.0f; t[vertex++] = 1.0f;
  t[vertex++] = 1.0f; t[vertex++] = 0.0f;
  t[vertex++] = 1.0f; t[vertex++] = 1.0f;

  // Lower face
  t[vertex++] = 0.0f; t[vertex++] = 1.0f;
  t[vertex++] = 0.0f; t[vertex++] = 0.0f;
  t[vertex++] = 1.0f; t[vertex++] = 1.0f;

  t[vertex++] = 0.0f; t[vertex++] = 0.0f;
  t[vertex++] = 1.0f; t[vertex++] = 0.0f;
  t[vertex++] = 1.0f; t[vertex++] = 1.0f;

	box.close();
  vertexCount = 36;
  glGenBuffers(3, buffers);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, v, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, n, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 2, t, GL_STATIC_DRAW);

  free(v);
  free(n);
  free(t);
}

void drawBox() {
  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  glVertexPointer(3,GL_FLOAT,0,0);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
  glNormalPointer(GL_FLOAT,0,0);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
  glTexCoordPointer(2,GL_FLOAT,0,0);

  glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

// Show file.3d content in file3dRead vector
void drawRenderSceneFile3d(void) {
  glClearColor(0.0f,0.0f,0.0f,0.0f);
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the Camera
	glLoadIdentity();

  // light position
  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  // light colors
  glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);

	gluLookAt(px,py,pz,0,0,0,0.0f,1.0f,0.0f);

	// Geometric Transformations
	glRotatef( rotate_x, 1.0, 0.0, 0.0 );
	glRotatef( rotate_y, 0.0, 1.0, 0.0 );
	glRotatef( rotate_z, 0.0, 0.0, 1.0 );
	glTranslatef(translate_x,translate_y,translate_z);

  glBindTexture(GL_TEXTURE_2D, texID);
  drawBox();
  glBindTexture(GL_TEXTURE_2D, 0);


	// End of frame
	glutSwapBuffers();
}

void sphericalToCartesian() {

	camX = radii * cos(beta) * sin(alfa);
	camY = radii * sin(beta);
	camZ = radii * cos(beta) * cos(alfa);
}

void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	if (h == 0) h = 1;

	// Compute window's aspect ratio
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);

	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// Return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

// Processing Keyboard Events
void keyboardNormal (unsigned char key, int x, int y){

    if (key == 'j' || key == 'J') translate_x+=1;
    else if (key == 'k' || key == 'K') translate_y+=1;
    else if (key == 'l' || key == 'L') translate_z+=1;
    else if (key == 'q' || key == 'Q') translate_x-=1;
    else if (key == 'w' || key == 'W') translate_y-=1;
    else if (key == 'e' || key == 'E') translate_z-=1;
    else if (key == 'z' || key == 'Z') glPolygonMode(GL_FRONT,GL_LINE); //GL_FRONT & GL_LINE
    else if (key == 'x' || key == 'X') glPolygonMode(GL_FRONT,GL_POINT); //GL_FRONT & GL_POINT
    else if (key == 'c' || key == 'C') glPolygonMode(GL_FRONT,GL_FILL); //GL_FRONT & GL_FILL
    else if (key == 'v' || key == 'V') glPolygonMode(GL_BACK,GL_LINE); //GL_BACK & GL_LINE
    else if (key == 'b' || key == 'B') glPolygonMode(GL_BACK,GL_POINT); //GL_BACK & GL_POINT
    else if (key == 'n' || key == 'N') glPolygonMode(GL_BACK,GL_FILL); //GL_BACK & GL_FILL
    else if (key == 'm' || key == 'M') glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); //GL_FRONT_AND_BACK & GL_LINE
    else if (key == 'o' || key == 'O') glPolygonMode(GL_FRONT_AND_BACK,GL_POINT); //GL_FRONT_AND_BACK & GL_POINT
    else if (key == 'p' || key == 'P') glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); //GL_FRONT_AND_BACK & GL_FILL*/
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

//Examples of key_code: GLUT_KEY_F1 ; GLUT_KEY_UP.
void keyboardExtra(int key_code, int x, int y){
		/*  Right arrow key - decrease rotate_y by 5 */
		if (key_code == GLUT_KEY_RIGHT) rotate_y -= 5;
		/*  Left arrow key - increase rotate_y by 5 */
		else if (key_code == GLUT_KEY_LEFT) rotate_y += 5;
		/*  Up arrow key - increase rotate_x by 5 */
		else if (key_code == GLUT_KEY_UP) rotate_x += 5;
		/*  Down arrow key - decrease rotate_x by 5 */
		else if (key_code == GLUT_KEY_DOWN) rotate_x -= 5;
		/*   arrow key - decrease rotate_z by 5 */
		else if (key_code == GLUT_KEY_F9) rotate_z += 5;
		/*   arrow key - decrease rotate_z by 5 */
		else if (key_code == GLUT_KEY_F7) rotate_z -= 5;
		//when camera moves

		glutPostRedisplay();
}

void initGL() {

	// OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// init
	sphericalToCartesian();
	glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  //Light
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // Material.
  float red[4] = {0.8f, 0.2f, 0.2f, 1.0f};
  glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
  glMaterialf(GL_FRONT, GL_SHININESS, 128.0);

  // Texturas.
  glEnable(GL_TEXTURE_2D);

  prepareBox(1,1,1);
}

// Main function
int main(int argc, char **argv) {
  // Init
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(800, 800);
  glutCreateWindow("Solar System - Stage 1");

  // Callback registration
  glutDisplayFunc(drawRenderSceneFile3d);
  glutReshapeFunc(changeSize);

  // Registration of the keyboard
  glutKeyboardFunc(keyboardNormal);
  glutSpecialFunc(keyboardExtra);
  // OpenGL settings
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  // init GLEW
	glewInit();

  // Textures MAC OS X
  cv::Mat img = cv::imread("sun.jpg");
  cols        = img.cols;
  rows        = img.rows;

  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cols, rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img.data);


  initGL();

  // Enter GLUT's main loop
  glutMainLoop();
	return 1;
}
