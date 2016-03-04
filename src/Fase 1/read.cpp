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
#include "Read\tinyxml\tinyxml.h"	// The place where the file "tinyxml.h" is located
#include <math.h>
//#include <GLUT/glut.h> //-- MAC
//#include </usr/local/Cellar/tinyxml/2.6.2/include/tinyxml.h> //-- MAC


#define _USE_MATH_DEFINES
#define Cos(th) cos(3.1415/180*(th))
#define Sin(th) sin(3.1415/180*(th))

using namespace std;

// Global variables handling files
string desktop = "C:\\Users\\Tiago\\Desktop\\Universidade\\3º Ano\\CG\\Projeto\\Fase 1\\generatorXML.xml";
//string desktop = "/Users/zecarlos/Desktop/";// -- MAC


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

// Show file.3d content in file3dRead vector
void drawRenderSceneFile3d(void) {
	vector<float> side_vrtx, circle_vrtx, rotate_vrtx;
	int size = stoi(file3dRead[0]); // number of vertex
	int conta = 0;
    if(splitted[1] == "plane.3d" || splitted[1] == "box.3d" ){
        string line; // String processed of all file3dRead strings concatenated into one of plane.3d or box.3d
        
        for (int i = 1; i <= size; i++) {
            if (conta != 0) { // Adding ',' at the end of each line except the first
                line = line + " " + file3dRead[i];
            }
            else {
                line += file3dRead[i];
                conta++;
            }
        }
        
        // Build an istream that holds the input string
        istringstream iss(line);
        
        // Iterates over the istream, using >> to grab floats and push_back to store them in the vector
        copy(istream_iterator<float>(iss), istream_iterator<float>(), back_inserter(vrtx));
        
    }
    else if(splitted[1] == "cone.3d"){
        string coneSide,coneCircle,coneRotate;
        
        //save only side vertex of file cone.3d
        for (int i = 1; i <= size && file3dRead[i]!="rotate"; i++) {
            if (conta != 0) { // Adding ',' at the end of each line except the first
                coneSide = coneSide + " " + file3dRead[i];
            }
            else {
                coneSide += file3dRead[i];
                conta++;
            }
        }
        
        //save only rotate values of file cone.3d
        coneRotate =  file3dRead[i+1];
        i+=2;
        
        //save only circle vertex of file cone.3d
        for (;i <= size; i++) {
            if (conta != 0) { // Adding ',' at the end of each line except the first
                coneCircle = coneCircle + " " + file3dRead[i];
            }
            else {
                coneCircle += file3dRead[i];
                conta++;
            }
        }
        
        // Build an istream that holds the input string
        istringstream iss(coneSide);
        
        // Iterates over the istream, using >> to grab floats and push_back to store them in the vector
        copy(istream_iterator<float>(iss), istream_iterator<float>(), back_inserter(side_vrtx));
        
        // Build an istream that holds the input string
        istringstream iss(coneRotate);
        
        // Iterates over the istream, using >> to grab floats and push_back to store them in the vector
        copy(istream_iterator<float>(iss), istream_iterator<float>(), back_inserter(rotate_vrtx));
        
        // Build an istream that holds the input string
        istringstream iss(coneCircle);
        
        // Iterates over the istream, using >> to grab floats and push_back to store them in the vector
        copy(istream_iterator<float>(iss), istream_iterator<float>(), back_inserter(circle_vrtx));
        
    }

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the Camera
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 1.0, 0.0, 0.0f, 1.0f, 0.0f);

	// Geometric Transformations
	glTranslatef(moveX, moveY, moveZ);
	glRotatef(angle, rotateX, rotateY, rotateZ);

	// Plane needs 6 vertices -- vrtx[0..17]
	if (splitted[1] == "plane.3d") {
		glBegin(GL_TRIANGLES);
		for (int j = 0; j<vrtx.size(); j += 3) {
			glColor3f(0, 1, 0);
			glVertex3f(vrtx[j], vrtx[j + 1], vrtx[j + 2]);
		}
		glEnd();
	}
	// Box needs 36 vertices  -- vrtx[0..107]
	else if (splitted[1] == "box.3d") {
		int colour = 0;
		glBegin(GL_TRIANGLES);

		for (int j = 0; j<vrtx.size(); j += 3) {
			if (j <= 35) glColor3f(1, 0, 0); // Lower face && Upper face
			else if (j >= 36 && j <= 72) glColor3f(0, 1, 0); // Left face && Right face
			else glColor3f(0, 0, 1);  // Front face && Back face
			glVertex3f(vrtx[j], vrtx[j + 1], vrtx[j + 2]);
		}
		glEnd();
	}
	// Sphere needs 60 vertices -- CHANGE TO REAL NUMBER
	else if (splitted[1] == "sphere.3d") {
		// Under construction...
        drawEsferaAndre();
	}
	//Cone needs 80 vertices -- CHANGE TO REAL NUMBER
	else if (splitted[1] == "cone.3d") {
		// Under construction...
	}

	// End of frame
	glutSwapBuffers();
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
	case 27: exit(-1); // Esc -> leaves
	}
	glutPostRedisplay();
}

void searchXMLData(TiXmlElement* pElem, string name3d) {
	TiXmlHandle hRoot(0);
	TiXmlElement* pSubElem = pElem;
	TiXmlAttribute* pAttrib = NULL;

	// Set current node to root node and determine if child node exists or not  
	hRoot = TiXmlHandle(pSubElem);
	pSubElem = hRoot.FirstChildElement().Element();
	if (!pSubElem) return;

	char* pszText = NULL;

	while (pSubElem) {
		// Find attribute  
		pAttrib = pSubElem->FirstAttribute();
		while (pAttrib) {
			char* pszText = (char*)pAttrib->Value();
			if (pszText == name3d) {
				printf("Found %s!\n", pszText);
				read = true;
				file3dRead = read3d((string)pszText); // Save the content of the .3d file in a vector
				return;
			}
			pAttrib = pAttrib->Next();
		}

		// Recursive call for searching child node based on the current node  
		searchXMLData(pSubElem, name3d);
		pSubElem = pSubElem->NextSiblingElement();
	}
	printf("File not found!\n");
}

// Load the named file and dump its structure to stdout
void readXML(const char* file, string name3d) {
	TiXmlDocument doc;
	doc.LoadFile(file);
	TiXmlHandle hDoc(&doc);

	// Access root node  
	TiXmlElement* pRoot = hDoc.FirstChildElement().Element();
	if (!pRoot) return;

	// Search child node step by step from the starting root node  
	searchXMLData(pRoot, name3d);
}

// Main function
int main(int argc, char **argv) {
	string operationLine, operation;

	cout << "Insert your operation:" << endl;
	getline(cin, operationLine);
	splitted = split(operationLine, ' '); // Save a string on a vector with the substrings separated by a space

	// The function "stoff" transforms the content of a string in a float

	if (splitted[0] == "Read" || splitted[0] == "read") {	// Read -> receives the name of the .3d file
		if (splitted.size() == 2) readXML("generatorXML.xml", splitted[1]);
	}
	else cout << "Try again with this usage: generator operationName inputs filename" << endl;

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
	glutSpecialFunc(translation);
	glutKeyboardFunc(rotation);

	// OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Enter GLUT's main loop
	glutMainLoop();

	return 1;
}
