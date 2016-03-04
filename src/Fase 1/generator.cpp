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
#include <windows.h>
#include <GL/glut.h>
#include <math.h>
//#include <GLUT/glut.h> //-- MAC

using namespace std;

// Global variable for handling files
string filename;

// Number of cone vertices
int coneVertex = 0;

// Number of sphere vertices
int sphereVertex = 0;

// Dimensions of the figures
float length, width, height, radius, slices, stacks;

//Global Variables to Transformations
float px = 0, py = 0, pz = 5;

float rotate_y = 0;
float rotate_x = 0;
float rotate_z = 0;

float translate_y = 0;
float translate_x = 0;
float translate_z = 0;

// Global variable store 3d file read
vector<string> file3dRead;

// Global variable process input
vector<string> splitted;

// Global variable to draw sphere
float v1x, v1y, v1z, v2x, v2y, v2z, v3x, v3y, v3z, v4x, v4y, v4z;
float thetaAux = 360 / slices / 2, phiAux = 180 / stacks / 2;
float rad = 3.14159f / 180.0f;

// Operation name has 100 char at max
#define MAX 100

// Returns the number to be used on the switch in main function; splitted[1] = operation name
int checkOP(string str) {
    if (str == "Plane" || str == "plane") return 1;
    else if (str == "Box" || str == "box") return 2;
    else if (str == "Sphere" || str == "sphere") return 3;
    else if (str == "Cone" || str == "cone") return 4;
    else return -1;
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

// Prints the plane on the .3d file
void printPlane3d() {
	ofstream plane;
	plane.open(filename.c_str());
	plane << "6" << endl; // Total number of vertices

	plane << "0.0 0.0 0.0" << endl;
	plane << length << ".0" << " 0.0 0.0" << endl;
	plane << "0.0 " << width << ".0" << " 0.0" << endl;

	plane << length << ".0" << " 0.0 0.0" << endl;
	plane << length << ".0 " << width << ".0" << " 0.0" << endl;
	plane << "0.0 " << width << ".0" << " 0.0" << endl;

	plane.close();
}

// Prints the box on the .3d file
void printBox3d() {
	ofstream box;
	box.open(filename.c_str());
	box << "36" << endl; // Total number of vertex

	// Lower face
	box << "0.0 " << width << ".0" << " 0.0" << endl;
	box << length << ".0" << " 0.0 0.0" << endl;
	box << "0.0 0.0 0.0" << endl;

	box << "0.0 " << width << ".0" << " 0.0" << endl;
	box << length << ".0 " << width << ".0" << " 0.0" << endl;
	box << length << ".0" << " 0.0 0.0" << endl;

	// Upper face
	box << "0.0 0.0 " << height << ".0" << endl;
	box << length << ".0" << " 0.0 " << height << ".0" << endl;
	box << "0.0 " << width << ".0 " << height << ".0" << endl;

	box << length << ".0" << " 0.0 " << height << ".0" << endl;
	box << length << ".0 " << width << ".0 " << height << ".0" << endl;
	box << "0.0 " << width << ".0 " << height << ".0" << endl;

	// Right face
	box << length << ".0" << " 0.0 0.0" << endl;
	box << length << ".0 " << width << ".0" << " 0.0" << endl;
	box << length << ".0" << " 0.0 " << height << ".0" << endl;

	box << length << ".0 " << width << ".0 0.0" << endl;
	box << length << ".0 " << width << ".0 " << height << ".0" << endl;
	box << length << ".0" << " 0.0 " << height << ".0" << endl;

	// Left Face
	box << "0.0 " << width << ".0" << " 0.0" << endl;
	box << "0.0 0.0 " << height << ".0" << endl;
	box << "0.0 " << width << ".0 " << height << ".0" << endl;

	box << "0.0 0.0 0.0" << endl;
	box << "0.0 0.0 " << height << ".0" << endl;
	box << "0.0 " << width << ".0" << " 0.0" << endl;

	// Front face
	box << "0.0 0.0 0.0" << endl;
	box << length << ".0" << " 0.0 0.0" << endl;
	box << "0.0 0.0 " << height << ".0" << endl;

	box << length << ".0" << " 0.0 0.0" << endl;
	box << length << ".0" << " 0.0 " << height << ".0" << endl;
	box << "0.0 0.0 " << height << ".0" << endl;

	// Back face
	box << "0.0 " << width << ".0" << " 0.0" << endl;
	box << "0.0 " << width << ".0 " << height << ".0" << endl;
	box << length << ".0 " << width << ".0 0.0" << endl;

	box << length << ".0 " << width << ".0 0.0" << endl;
	box << "0.0 " << width << ".0 " << height << ".0" << endl;
	box << length << ".0 " << width << ".0 " << height << ".0" << endl;

	box.close();
}

// Prints the sphere on the .3d file
void printSphere3d() {
    ofstream sphere;
    sphere.open(filename.c_str());
    sphere << sphereVertex << endl; // Total number of vertices

    int sliceVar,stackVar;

    if(slices<=1) sliceVar=1;
    else sliceVar=2;

    if(stacks<=1) stackVar=1;
    else stackVar=2;

    if (slices<=1) {
        thetaAux=1;
    }
    else thetaAux=360/slices/2;


    if (stacks<=1) {
        phiAux=1;
    }
    else phiAux=360/slices/2;

    for (float phi = 0; phi <= (180 - phiAux); phi += stackVar * phiAux) {
        for (float theta = 0; theta <= (360 - thetaAux); theta += sliceVar * thetaAux) {
            sphere << v1x << " " << v1y << " " << v1z << endl;
            sphere << v2x << " " << v2y << " " << v2z << endl;
            sphere << v3x << " " << v3y << " " << v3z << endl;
            sphere << v3x << " " << v3y << " " << v3z << endl;
            sphere << v2x << " " << v2y << " " << v2z << endl;
            sphere << v4x << " " << v4y << " " << v4z << endl;
        }
    }
}

// Prints the cone on the .3d file
void printCone3d() {
	ofstream cone;
	cone.open(filename.c_str());
	cone << coneVertex << endl; // Total number of vertices

	for (int i = 0; i < slices; i++) {
		float alpha = i * (2 * M_PI) / slices;
		float beta = (2 * M_PI) / slices;

		// Base
		cone << "0.0 0.0 0.0" << endl;
		cone << radius*sin(alpha + beta) << " 0.0 " << radius*cos(alpha + beta) << endl;
		cone << radius*sin(alpha) << " 0.0 " << radius*cos(alpha) << endl;

		// Sides
		cone << "0.0 " << height << " 0.0" << endl;
		cone << radius*sin(alpha) << " 0.0 " << radius*cos(alpha) << endl;
		cone << radius*sin(alpha + beta) << " 0.0 " << radius*cos(alpha + beta) << endl;
	}
}

// Draws the plane
void drawPlane() {
	glBegin(GL_TRIANGLES);

	glColor3f(0, 1, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(length, 0.0f, 0.0f);
	glVertex3f(0.0f, width, 0.0f);

	glColor3f(0, 1, 0);
	glVertex3f(length, 0.0f, 0.0f);
	glVertex3f(length, width, 0.0f);
	glVertex3f(0.0f, width, 0.0f);

	glEnd();

	// Saves the vertices of the plane on the .3d file
	printPlane3d();
}

// Draws the box
void drawBox() {
	glBegin(GL_TRIANGLES);

	// Lower face
	glColor3f(1, 0, 0);
	glVertex3f(0.0f, width, 0.0f);
	glVertex3f(length, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glColor3f(1, 0, 0);
	glVertex3f(0.0f, width, 0.0f);
	glVertex3f(length, width, 0.0f);
	glVertex3f(length, 0.0f, 0.0f);

	// Upper face
	glColor3f(1, 0, 0);
	glVertex3f(0.0f, 0.0f, height);
	glVertex3f(length, 0.0f, height);
	glVertex3f(0.0f, width, height);

	glColor3f(1, 0, 0);
	glVertex3f(length, 0.0f, height);
	glVertex3f(length, width, height);
	glVertex3f(0.0f, width, height);

	// Right face
	glColor3f(0, 1, 0);
	glVertex3f(length, 0.0f, 0.0f);
	glVertex3f(length, width, 0.0f);
	glVertex3f(length, 0.0f, height);

	glColor3f(0, 1, 0);
	glVertex3f(length, width, 0.0f);
	glVertex3f(length, width, height);
	glVertex3f(length, 0.0f, height);

	// Left face
	glColor3f(0, 1, 0);
	glVertex3f(0.0f, width, 0.0f);
	glVertex3f(0.0f, 0.0f, height);
	glVertex3f(0.0f, width, height);

	glColor3f(0, 1, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, height);
	glVertex3f(0.0f, width, 0.0f);

	// Front face
	glColor3f(0, 0, 1);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(length, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, height);

	glColor3f(0, 0, 1);
	glVertex3f(length, 0.0f, 0.0f);
	glVertex3f(length, 0.0f, height);
	glVertex3f(0.0f, 0.0f, height);

	// Back face
	glColor3f(0, 0, 1);
	glVertex3f(0.0f, width, 0.0f);
	glVertex3f(0.0f, width, height);
	glVertex3f(length, width, 0.0f);

	glColor3f(0, 0, 1);
	glVertex3f(length, width, 0.0f);
	glVertex3f(0.0f, width, height);
	glVertex3f(length, width, height);

	glEnd();

	// Saves the vertices of the box on the .3d file
	printBox3d();
}

// Draws the sphere
void drawSphere() {
    int sliceVar,stackVar;

    if(slices<=1) sliceVar=1;
    else sliceVar=2;

    if(stacks<=1) stackVar=1;
    else stackVar=2;

    if (slices<=1) {
        thetaAux=1;
    }
    else thetaAux=360/slices/2;


    if (stacks<=1) {
        phiAux=1;
    }
    else phiAux=360/slices/2;

	for (float phi = 0; phi <= (180 - phiAux); phi += stackVar * (int)phiAux) {
		for (float theta = 0; theta <= (360 - thetaAux); theta += sliceVar * thetaAux) {
			v1x = radius*sin(phi*rad)*cos(theta*rad);
			v1y = radius*sin(phi*rad)*sin(theta*rad);
			v1z = radius*cos(phi*rad);

			v2x = radius*sin((phi + phiAux)*rad)*cos(theta*rad);
			v2y = radius*sin((phi + phiAux)*rad)*sin(theta*rad);
			v2z = radius*cos((phi + phiAux)*rad);

			v3x = radius*sin(rad*phi)*cos(rad*(theta + thetaAux));
			v3y = radius*sin(rad*phi)*sin(rad*(theta + thetaAux));
			v3z = radius*cosf(rad*phi);

			v4x = radius*sin(rad*(phi + phiAux))*cos(rad*(theta + thetaAux));
			v4y = radius*sin(rad*(phi + phiAux))*sin(rad*(theta + thetaAux));
			v4z = radius*cosf(rad*(phi + phiAux));

			glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(v1x, v1y, v1z);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(v2x, v2y, v2z);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(v3x, v3y, v3z);

			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(v3x, v3y, v3z);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(v2x, v2y, v2z);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(v4x, v4y, v4z);

      sphereVertex+=6;

			glEnd();
		}
	}

	// Saves the vertices of the sphere on the .3d file
	printSphere3d();
}

// Draws the cone
void drawCone() {
	// Sides
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < slices; i++) {
		float alpha = i * (2 * M_PI) / slices;
		float beta = (2 * M_PI) / slices;

		// Base
		glColor3f(0, 1, 0);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(radius*sin(alpha + beta), 0.0f, radius*cos(alpha + beta));
		glVertex3f(radius*sin(alpha), 0.0f, radius*cos(alpha));

		// Sides
		glColor3f(1, 0, 0);
		glVertex3f(0.0f, height, 0.0f);
		glVertex3f(radius*sin(alpha), 0.0f, radius*cos(alpha));
		glVertex3f(radius*sin(alpha + beta), 0.0f, radius*cos(alpha + beta));

		coneVertex += 6;
	}

	glEnd();

	// Saves the vertices of the cone on the .3d file
	printCone3d();
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

void renderScene(void) {
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

    // Draws the figures
	switch (checkOP(splitted[1])) {
		case 1: drawPlane(); break;
		case 2: drawBox(); break;
		case 3: drawSphere(); break;
		case 4: drawCone(); break;
	}

    // End of frame
    glutSwapBuffers();
}

// Processing Keyboard Events
void keyboardNormal (unsigned char key, int x, int y){

    if (key == 's' || key == 'S') {length-=1;stacks--;}
    else if (key == 'w' || key == 'W') {length+=1;stacks--;}
		else if (key == 's' || key == 'S') {height-=1;stacks++;}
		else if (key == 'a' || key == 'A') {height+=1;stacks++;}
    else if (key == 'q' || key == 'Q') width-=1;
    else if (key == 'x' || key == 'X') width+=1;
		else if (key == 'd' || key == 'D') translate_x+=1;
		else if (key == 'f' || key == 'F') translate_y+=1;
		else if (key == 'g' || key == 'G') translate_z+=1;
		else if (key == 'h' || key == 'H') translate_x-=1;
		else if (key == 'i' || key == 'I') translate_y-=1;
		else if (key == 'j' || key == 'J') translate_z-=1;
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

// Main function
int main(int argc, char **argv) {
	string operationLine, operation;

	cout << "Insert your operation:" << endl;
	getline(cin, operationLine);
	splitted = split(operationLine, ' '); // Save a string on a vector with the substrings separated by a space

	// The function "stoff" transforms the content of a string in a float

	if (splitted[0] == "Generator" || splitted[0] == "generator") {
		switch (checkOP(splitted[1])) {
			case 1:	// Plane -> receives the length and the width
				if (splitted.size() == 5) {
					filename = splitted[4];
					length = stof(splitted[2]);
					width = stof(splitted[3]);
				}
				break;
			case 2: // Box -> receives the length, the width and the height
				if (splitted.size() == 6) {
			        filename = splitted[5];
					length = stof(splitted[2]);
					width = stof(splitted[3]);
					height = stof(splitted[4]);
				}
				break;
			case 3: // Sphere -> receives the radius, the slices and the stacks
				if (splitted.size() == 6) {
			        filename = splitted[5];
					radius = stof(splitted[2]);
					slices = stof(splitted[3]);
					stacks = stof(splitted[4]);
				}
				break;
			case 4: // Cone -> receives the base radius, the height, the slices and the stacks
				if (splitted.size() == 7) {
			        filename = splitted[6];
					radius = stof(splitted[2]);
					height = stof(splitted[3]);
					slices = stof(splitted[4]);
					stacks = stof(splitted[5]);
				}
				break;
			default:
				cout << "Not an operation!\n" << endl;
				filename = "invalid.3d";
				break;
		}

	// Init
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Solar System - Stage 1");

    // Callback registration
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

	  // Registration of the keyboard
    glutKeyboardFunc(keyboardNormal);
    glutSpecialFunc(keyboardExtra);

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Enter GLUT's main loop
    glutMainLoop();
  }
	else cout << "Try again with this usage: generator operationName inputs filename" << endl;

	return 1;
}
