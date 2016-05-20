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

using namespace std;
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

// Dimensions of the figures
float length, width, height=5, radius=2, slices=30, stacks=30;

// Global variable store 3d file read
vector<string> file3dRead;

// Global variable process input
vector<string> splitted;

GLuint k, vertices;

// Global variable with 3d vertex
float* vrtx;

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

void printCone3d() {
  vrtx=(float*)malloc(sizeof(float)*100000);
	ofstream cone;
	cone.open("cone.3d");
  k=0;

	// pontos base + pontos lados + pontos do topo (slice so com triangulos)
	int coneVertex = slices * 3 + (stacks - 1)*slices * 6 + slices * 3;
	cone << coneVertex << endl; // Total number of vertices
	float ratio = height / radius;

	for (int i = 0; i < slices; i++) {

		float alpha = i * (2 * M_PI) / slices;
		float beta = (2 * M_PI) / slices;

		// Base
		float x1 = radius*sin(alpha);
		float y1 = 0;
		float z1 = radius*cos(alpha);

		float x2 = radius*sin(alpha + beta);
		float y2 = 0;
		float z2 = radius*cos(alpha + beta);



		// Base
		cone << "0.0 0.0 0.0" << endl;
		cone << x2 << " " << y2 << " " << z2 << endl;
		cone << x1 << " " << y1 << " " << z1 << endl;
    vrtx[k++]=0.0;vrtx[k++]=0.0;vrtx[k++]=0.0;
    vrtx[k++]=x2;vrtx[k++]=y2;vrtx[k++]=z2;
    vrtx[k++]=x1;vrtx[k++]=y1;vrtx[k++]=z1;

		float radiusBot = radius;
		float radiusTop;

		for (int j = 0; j < stacks; j++, radiusBot = radiusTop) {
			// Sides
			float y = j * (height / stacks);
			float yn = (j + 1) * (height / stacks);

			float h = height - yn; // altura do triangulo superior
			radiusTop = h / ratio; // radius do triangulo superior

								   /* Points

								   3____4
								   |    |
								   |____|
								   1    2
								   */

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

			if (j == stacks - 1) { // top slice
				cone << x3 << " " << y3 << " " << z3 << endl;
				cone << x1 << " " << y1 << " " << z1 << endl;
				cone << x2 << " " << y2 << " " << z2 << endl;
        vrtx[k++]=x3;vrtx[k++]=y3;vrtx[k++]=z3;
        vrtx[k++]=x1;vrtx[k++]=y1;vrtx[k++]=z1;
        vrtx[k++]=x2;vrtx[k++]=y2;vrtx[k++]=z2;
			}
			else {
				// T: 3 - 1 - 2
				cone << x3 << " " << y3 << " " << z3 << endl;
				cone << x1 << " " << y1 << " " << z1 << endl;
				cone << x2 << " " << y2 << " " << z2 << endl;
        vrtx[k++]=x3;vrtx[k++]=y3;vrtx[k++]=z3;
        vrtx[k++]=x1;vrtx[k++]=y1;vrtx[k++]=z1;
        vrtx[k++]=x2;vrtx[k++]=y2;vrtx[k++]=z2;

				// T: 3 - 2 - 4
				cone << x3 << " " << y3 << " " << z3 << endl;
				cone << x2 << " " << y2 << " " << z2 << endl;
				cone << x4 << " " << y4 << " " << z4 << endl;
        vrtx[k++]=x3;vrtx[k++]=y3;vrtx[k++]=z3;
        vrtx[k++]=x2;vrtx[k++]=y2;vrtx[k++]=z2;
        vrtx[k++]=x4;vrtx[k++]=y4;vrtx[k++]=z4;
			}
		}

	}
	cone.close();
  glGenBuffers(1, &vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * k, vrtx, GL_STATIC_DRAW);
}


void drawCone() {

	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, k);
}

// Show file.3d content in file3dRead vector
void drawRenderSceneFile3d(void) {

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the Camera
	glLoadIdentity();
	gluLookAt(px,py,pz,0,0,0,0.0f,1.0f,0.0f);

	// Geometric Transformations
	glRotatef( rotate_x, 1.0, 0.0, 0.0 );
	glRotatef( rotate_y, 0.0, 1.0, 0.0 );
	glRotatef( rotate_z, 0.0, 0.0, 1.0 );
	glTranslatef(translate_x,translate_y,translate_z);

	drawCone();

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

	printCone3d();
}

// Main function
int main(int argc, char **argv) {
  // Init
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(800, 800);
  glutCreateWindow("Cone VBO");

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
  initGL();

  // Enter GLUT's main loop
  glutMainLoop();
	return 1;
}
