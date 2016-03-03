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

#define _USE_MATH_DEFINES
#define Cos(th) cos(3.1415/180*(th))
#define Sin(th) sin(3.1415/180*(th))

using namespace std;

// Global variables handling files
string desktop = "C:\\Users\\Tiago\\Desktop\\Universidade\\3º Ano\\CG\\Projeto\\Fase 1\\generatorXML.xml";
//string desktop = "/Users/zecarlos/Desktop/";// -- MAC
string filename;

// Number of cone vertices
int coneVertex = 0;

// Dimensions of the figures
float length, width, height, radius, slices, stacks;

// Variables used to move the camera 
float moveX = 0.0f, moveY = 0.0f, moveZ = 0.0f, rotateX = 0.0f, rotateY = 0.0f, rotateZ = 0.0f, angle = 0;

// Global variable store 3d file read
vector<string> file3dRead;

// Global variable process input
vector<string> splitted;

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

// Prints the figures on the .3d files
void print3d(string figure) {
	if (figure == "plane") {
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
	else if (figure == "box") {
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
	else if (figure == "cone") {
		ofstream cone;
		cone.open(filename.c_str());
		cone << coneVertex << endl; // Total number of vertices

		// Sides
		float k = 0;
		while (k <= 360) {
			cone << "0.0 0.0 " << height << endl;
			cone << radius*Cos(k) << " " << radius*Sin(k) << " 0.0" << endl;
			cone << radius*Cos(k + stacks) << " " << radius*Sin(k + stacks) << " 0.0" << endl;
			k += slices;
		}

		// Bottom circle
		k = 0;
		while (k <= 360) {
			cone << "0.0 0.0 0.0" << endl;
			cone << radius*Cos(k) << " 0.0 " << radius*Sin(k) << endl;
			cone << radius*Cos(k + stacks) << " 0.0 " << radius*Sin(k + stacks) << endl;
			k += slices;
		}
	}
}

void drawEsferaAndre(){
    float v1x, v1y, v1z, v2x, v2y, v2z, v3x, v3y, v3z, v4x, v4y, v4z;
    
    float thetaAmt = 360/slices/2, phiAmt = 180/stacks/2, radius = raio;
    float RADIANS = 3.14159f / 180.0f;
    
    for (float phi = 0; phi <= (180 - phiAmt); phi += 2*(int)phiAmt)
    {
        for (float theta = 0; theta <= (360 - thetaAmt); theta += 2 * thetaAmt)
        {
            v1x = radius*sin(phi*RADIANS)*cos(theta*RADIANS);
            v1y = radius*sin(phi*RADIANS)*sin(theta*RADIANS);
            v1z = radius*cos(phi*RADIANS);
            
            
            v2x = radius*sin((phi + phiAmt)*RADIANS)*cos(theta*RADIANS);
            v2y = radius*sin((phi + phiAmt)*RADIANS)*sin(theta*RADIANS);
            v2z = radius*cos((phi + phiAmt)*RADIANS);
            
            
            v3x = radius*sin(RADIANS*phi)*cos(RADIANS*(theta + thetaAmt));
            v3y = radius*sin(RADIANS*phi)*sin(RADIANS*(theta + thetaAmt));
            v3z = radius*cosf(RADIANS*phi);
            
            v4x = radius*sin(RADIANS*(phi + phiAmt))*cos(RADIANS*(theta + thetaAmt));
            v4y = radius*sin(RADIANS*(phi + phiAmt))*sin(RADIANS*(theta + thetaAmt));
            v4z = radius*cosf(RADIANS*(phi + phiAmt));
            
            
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
            
            glEnd();
        }
        
        
    }

}

void drawSphere(float r, float slices, float stacks) {

	int i, j;

	const double pi = 3.1415926535897;

	float longitude = 0, polar = 0;
	// 0 < longitude < 2 * PI , 0 < polar < PI 
	float slices_step = (2 * pi) / slices;
	float stacks_step = pi / stacks;


	glBegin(GL_TRIANGLES);

	for (i = 0; i < stacks; i++) {

		float polar2 = polar + stacks_step;

		for (j = 0; j < slices; j++) {

			float longitude2 = longitude + slices_step;


			/*
			Points

			4 ___2
			|	|
			|___|
			3	1

			*/

			float x1 = r * cos(polar) * cos(longitude);
			float y1 = r * cos(polar) * sin(longitude);
			float z1 = r * sin(polar);

			float x2 = r * cos(polar2) * cos(longitude);
			float y2 = r * cos(polar2) * sin(longitude);
			float z2 = r * sin(polar2);

			float x3 = r * cos(polar) * cos(longitude2);
			float y3 = r * cos(polar) * sin(longitude2);
			float z3 = r * sin(polar);

			float x4 = r * cos(polar2) * cos(longitude2);
			float y4 = r * cos(polar2) * sin(longitude2);
			float z4 = r * sin(polar2);


			// TRIANGLE 2-3-1
			glVertex3f(x2, y2, z2);
			glVertex3f(x3, y3, z3);
			glVertex3f(x1, y1, z1);

			// TRIANGLE 2-4-3
			glVertex3f(x2, y2, z2);
			glVertex3f(x4, y4, z4);
			glVertex3f(x3, y3, z3);



			longitude += slices_step;

		}

		polar += stacks_step;
	}

	glEnd();
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
    gluLookAt(0.0, 0.0, 5.0, 0.0, 1.0, 0.0, 0.0f, 1.0f, 0.0f);

	// Geometric Transformations
	glTranslatef(moveX, moveY, moveZ);
	glRotatef(angle, rotateX, rotateY, rotateZ);

    // Plane
    if(checkOP(splitted[1])==1) {
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

		print3d("plane");
    }
	// Box
    else if(checkOP(splitted[1])==2) {
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

		print3d("box");
    }
	// Sphere
    else if(checkOP(splitted[1])==3) {
		drawSphere(radius,slices,stacks);
		print3d("sphere");
    }
	// Cone
    else if(checkOP(splitted[1])==4) {
		// Sides
		glBegin(GL_TRIANGLES);
		float k = 0;
		while (k <= 360) {
			glColor3f(1, 0, 0);
			glVertex3f(0.0f, 0.0f, height);
			glVertex3f(radius*Cos(k), radius*Sin(k), 0);
			glVertex3f(radius*Cos(k + stacks), radius*Sin(k + stacks), 0.0f);
			k += slices;
			coneVertex += 3;
		}
		glEnd();

		// Bottom circle
		glRotated(90, 1, 0, 0);	// Rotate back
		glBegin(GL_TRIANGLES);
		k = 0;
		while(k <= 360) {
			glColor3f(0, 1, 0);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(radius*Cos(k), 0.0f, radius*Sin(k));
			glVertex3f(radius*Cos(k + stacks), 0.0f, radius*Sin(k + stacks));
			k += slices;
			coneVertex += 3;
		}
		glEnd();

		print3d("cone");
	}

    // End of frame
    glutSwapBuffers();
}



// Processing Keyboard Events
void translation(int key_code, int x, int y) {
	switch (key_code) {
		case GLUT_KEY_UP: moveY++; break;
		case GLUT_KEY_DOWN: moveY--; break;
		case GLUT_KEY_LEFT: moveX--; break;
		case GLUT_KEY_RIGHT: moveX++; break;
	}
	glutPostRedisplay();
}

void rotation(unsigned char key, int x, int y) {
	switch (key) {
		case '+': moveZ++; break;
		case '-': moveZ--; break;
		case 'd': angle -= 2; rotateZ = 1; rotateX = 0; rotateY = 0; break;
		case 'a': angle += 2; rotateZ = 1; rotateX = 0; rotateY = 0; break;
		case 'w': angle -= 2; rotateX = 1; rotateZ = 0; rotateY = 0; break;
		case 's': angle += 2; rotateX = 1; rotateZ = 0; rotateY = 0; break;
		case 'e': angle += 2; rotateY = 1; rotateZ = 0; rotateX = 0; break;
		case 'q': angle -= 2; rotateY = 1; rotateZ = 0; rotateX = 0; break;
		case 27 : exit(-1); // Esc -> leaves
	}
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
	}
	else cout << "Try again with this usage: generator operationName inputs filename" << endl;

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
	glutSpecialFunc(translation);
	glutKeyboardFunc(rotation);

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Enter GLUT's main loop
    glutMainLoop();

	return 1;
}
