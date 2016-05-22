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

// Dimensions of the figures
float length, width, height=5, radius=2, slices=30, stacks=30;

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

void cross(float *a, float *b, float *res) {
 res[0] = a[1]*b[2] - a[2]*b[1]; res[1] = a[2]*b[0] - a[0]*b[2]; res[2] = a[0]*b[1] - a[1]*b[0];}

void normalize(float *a) {
 float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]); a[0] = a[0]/l; a[1] = a[1]/l; a[2] = a[2]/l;}

void prepareCone(float height, float radius, int slices, int stacks) {
  ofstream cone;
  cone.open("cone.3d");
	int nVertices = 100000;

	float *vertexB = (float*)malloc(sizeof(float) * 3 * nVertices);
	float *normalB = (float*)malloc(sizeof(float) * 3 * nVertices);
	float *textureB = (float*)malloc(sizeof(float) * 2 * nVertices);

	float ratio = height / radius;
	int vertex = 0, normal = 0, texture = 0;

	for (int i = 0; i < slices; i++) {

		float tx = i / slices;

		float alpha = i * (2 * _PI_) / slices;
		float beta = (2 * _PI_) / slices;

		// Base
		float x1 = radius*sin(alpha);
		float y1 = 0;
		float z1 = radius*cos(alpha);

		float x2 = radius*sin(alpha + beta);
		float y2 = 0;
		float z2 = radius*cos(alpha + beta);

		// Base
		vertexB[vertex++] = 0.0f; vertexB[vertex++] = 0.0f; vertexB[vertex++] = 0.0f;
		vertexB[vertex++] = x2; vertexB[vertex++] = y2; vertexB[vertex++] = z2;
		vertexB[vertex++] = x1; vertexB[vertex++] = y1; vertexB[vertex++] = z1;

		normalB[normal++] = 0.0f; normalB[normal++] = -1.0f; normalB[normal++] = 0.0f;
		normalB[normal++] = 0.0f; normalB[normal++] = -1.0f; normalB[normal++] = 0.0f;
		normalB[normal++] = 0.0f; normalB[normal++] = -1.0f; normalB[normal++] = 0.0f;

		textureB[texture++] = 0.5f; textureB[texture++] = 0.5f;
		textureB[texture++] = 0.5f + sin(alpha + beta) * 0.5f; textureB[texture++] = 0.5f + cos(alpha + beta) * 0.5f;
		textureB[texture++] = 0.5f + sin(alpha) * 0.5f; textureB[texture++] = 0.5f + cos(alpha) * 0.5f;

    cone << "0.0 0.0 0.0" << endl; // vertex
    cone << "0.0 -1.0 0.0" << endl; // normal
    cone << "0.5 0.5" << endl; // texture

    cone << x2 << " " << y2 << " " << z2 << endl;
    cone << "0.0 -1.0 0.0" << endl;
    cone << 0.5f + sin(alpha + beta) * 0.5f << " " << 0.5f + cos(alpha + beta) * 0.5f << endl;

    cone << x1 << " " << y1 << " " << z1 << endl;
    cone << "0.0 -1.0 0.0" << endl;
    cone << 0.5f + sin(alpha) * 0.5f << " " << 0.5f + cos(alpha) * 0.5f;

		float radiusBot = radius;
		float radiusTop;

		for (int j = 0; j < stacks; j++, radiusBot = radiusTop) {

			float ty = j / stacks;

			// Sides
			float y = j * (height / stacks);
			float yn = (j + 1) * (height / stacks);

			float h = height - yn; // altura do triangulo superior
			radiusTop = h / ratio; // radius do triangulo superior

			float x1 = radiusBot*sin(alpha);
			float y1 = y;
			float z1 = radiusBot*cos(alpha);

			float x2 = radiusBot*sin(alpha + beta);
			float y2 = y;
			float z2 = radiusBot*cos(alpha + beta);

			float x3 = radiusTop*sin(alpha);
			float y3 = yn;
			float z3 = radiusTop*cos(alpha);

			float x4 = radiusTop*sin(alpha + beta);
			float y4 = yn;
			float z4 = radiusTop*cos(alpha + beta);
			float v1[3], v2[3],v[3];
			if (j == stacks - 1) { // top slice
				vertexB[vertex++] = x3; vertexB[vertex++] = y3; vertexB[vertex++] = z3;
				vertexB[vertex++] = x1; vertexB[vertex++] = y1; vertexB[vertex++] = z1;
				vertexB[vertex++] = x2; vertexB[vertex++] = y2; vertexB[vertex++] = z2;

				normalB[normal++] = sin(alpha); normalB[normal++] = 0.0f; normalB[normal++] = cos(alpha);
				normalB[normal++] = sin(alpha); normalB[normal++] = 0.0f; normalB[normal++] = cos(alpha);
				normalB[normal++] = sin(alpha + beta); normalB[normal++] = 0.0f; normalB[normal++] = cos(alpha + beta);

				textureB[texture++] = 0.5f; textureB[texture++] = 0.5f;
				textureB[texture++] = sin(alpha); textureB[texture++] = cos(alpha);
				textureB[texture++] = sin(alpha + beta); textureB[texture++] = cos(alpha + beta);

        cone << x3 << " " << y3<< " " << z3 << endl;
        cone << sin(alpha) << " 0.0 " << cos(alpha) << endl;
        cone << "0.5 0.5" << endl;

        cone << x1 << " " << y1 << " " << z1 << endl;
        cone << sin(alpha) << " 0.0 " << cos(alpha) << endl;
        cone << sin(alpha) << " " << cos(alpha) << endl;

        cone << x2 << " " << y2 << " " << z2 << endl;
        cone << sin(alpha + beta) << " 0.0 " << cos(alpha + beta) << endl;
        cone << sin(alpha + beta) << " " << cos(alpha + beta) << endl;

			}
			else {

        // 1º Triangle

				vertexB[vertex++] = x1; vertexB[vertex++] = y1; vertexB[vertex++] = z1;
				vertexB[vertex++] = x2; vertexB[vertex++] = y2; vertexB[vertex++] = z2;
				vertexB[vertex++] = x3; vertexB[vertex++] = y3; vertexB[vertex++] = z3;

        textureB[texture++] = j + 1; textureB[texture++] = i;
				textureB[texture++] = j + 1; textureB[texture++] = i + 1;
				textureB[texture++] = j; textureB[texture++] = i;

        v1[0]=x2-x1;
        v1[1]=y2-y1;
        v1[2]=z2-z1;

        v2[0]=x3-x1;
        v2[1]=y3-y1;
				v2[2]=z3-z1;

				cross(v2, v1, v);
				normalize(v);
				normalB[normal++] =v[0]; normalB[normal++] = v[1]; normalB[normal++] = v[2];

        cone << x1 << " " << y1 << " " << z1 << endl;
        cone << v[0] << " " << v[1] << " " << v[2] << endl;
        cone << j+1 << " " << i << endl;

        v1[0]=x3-x2;
        v1[1]=y3-y2;
        v1[2]=z3-z2;

        v2[0]=x1-x2;
        v2[1]=y1-y2;
				v2[2]=z1-z2;

				cross(v2, v1, v);
				normalize(v);
				normalB[normal++] =v[0]; normalB[normal++] = v[1]; normalB[normal++] = v[2];

        cone << x2 << " " << y2 << " " << z2 << endl;
        cone << v[0] << " " << v[1] << " " << v[2] << endl;
        cone << j+1 << " " << i+1 << endl;

        v1[0]=x1-x3;
        v1[1]=y1-y3;
        v1[2]=z1-z3;

        v2[0]=x2-x3;
        v2[1]=y2-y3;
				v2[2]=z2-z3;

				cross(v2, v1, v);
				normalize(v);
        normalB[normal++] =v[0]; normalB[normal++] = v[1]; normalB[normal++] = v[2];

        cone << x3 << " " << y3 << " " << z3 << endl;
        cone << v[0] << " " << v[1] << " " << v[2] << endl;
        cone << j << " " << i << endl;

        // 2º Triangle

				vertexB[vertex++] = x3; vertexB[vertex++] = y3; vertexB[vertex++] = z3;
				vertexB[vertex++] = x2; vertexB[vertex++] = y2; vertexB[vertex++] = z2;
				vertexB[vertex++] = x4; vertexB[vertex++] = y4; vertexB[vertex++] = z4;

        textureB[texture++] = j; textureB[texture++] = i + 1;
        textureB[texture++] = j + 1; textureB[texture++] = i;
        textureB[texture++] = j + 1; textureB[texture++] = i + 1;

        normalB[normal++] =v[0]; normalB[normal++] = v[1]; normalB[normal++] = v[2];

        cone << x3 << " " << y3 << " " << z3 << endl;
        cone << v[0] << " " << v[1] << " " << v[2] << endl;
        cone << j << " " << i+1 << endl;

        v1[0]=x3-x2;
        v1[1]=y3-y2;
        v1[2]=z3-z2;

        v2[0]=x4-x2;
        v2[1]=y4-y2;
        v2[2]=z4-z2;

				cross(v1, v2, v);
				normalize(v);
        normalB[normal++] =v[0]; normalB[normal++] = v[1]; normalB[normal++] = v[2];

        cone << x2 << " " << y2 << " " << z2 << endl;
        cone << v[0] << " " << v[1] << " " << v[2] << endl;
        cone << j+1 << " " << i << endl;

        v1[0]=x4-x3;
        v1[1]=y4-y3;
        v1[2]=z4-z3;

        v2[0]=x2-x3;
        v2[1]=y2-y3;
        v2[2]=z2-z3;

				cross(v1, v2, v);
				normalize(v);
        normalB[normal++] =v[0]; normalB[normal++] = v[1]; normalB[normal++] = v[2];

        cone << x4 << " " << y4 << " " << z4 << endl;
        cone << v[0] << " " << v[1] << " " << v[2] << endl;
        cone << j+1 << " " << i+1 << endl;

        v1[0]=x2-x4;
        v1[1]=y2-y4;
        v1[2]=z2-z4;

        v2[0]=x3-x4;
        v2[1]=y3-y4;
        v2[2]=z3-z4;

				cross(v1, v2, v);
				normalize(v);


			}
		}
	}

	vertexCount = vertex;

  cone << "End" << endl;
  cone.close();
	glGenBuffers(3, buffers);

	// Save data on the array of vertices
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, vertexB, GL_STATIC_DRAW);

	// Save data on the array of vertex
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, normalB, GL_STATIC_DRAW);

	// Sava data on the texture array
	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * vertexCount, textureB, GL_STATIC_DRAW);

	free(normalB);
	free(vertexB);
	free(textureB);
}

void drawCone() {
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
  drawCone();
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

  prepareCone(2, 1, 30, 30);
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
