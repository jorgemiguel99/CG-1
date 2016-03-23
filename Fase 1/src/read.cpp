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
#include "tinyxml\tinyxml.h"	// The place where the file "tinyxml.h" is located
#include <math.h>
//#include <GLUT/glut.h> //-- MAC
//#include </usr/local/Cellar/tinyxml/2.6.2/include/tinyxml.h> //-- MAC

using namespace std;

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

// Global variable with 3d vertex
vector<float> vrtx;

// Operation name has 100 char at max
#define MAX 100

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
	int size = stoi(file3dRead[0]); // number of vertex
	int conta = 0;
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

	glBegin(GL_TRIANGLES);

    for (int j = 0; j < vrtx.size();) 
            glVertex3f(vrtx[j++], vrtx[j++], vrtx[j++]);

	glEnd();

	// End of frame
	glutSwapBuffers();
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

// write function to process menu events
void newMenu (int id_op){
    keyboardNormal((unsigned char)id_op, 0, 0);
}

// Main function
int main(int argc, char **argv) {
	string operationLine, operation;

	cout << "Insert your operation:" << endl;
	getline(cin, operationLine);
	splitted = split(operationLine, ' '); // Save a string on a vector with the substrings separated by a space

	// The function "stoff" transforms the content of a string in a float

	if (splitted[0] == "Read" || splitted[0] == "read") {	// Read -> receives the name of the .3d file
		if (splitted.size() == 2){
			readXML("generatorXML.xml", splitted[1]);

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
            
            
            //menu
            glutCreateMenu(newMenu);
            glutAddMenuEntry("Increase Translate x value",'j');
            glutAddMenuEntry("Increase Translate y value",'k');
            glutAddMenuEntry("Increase Translate z value",'l');
            glutAddMenuEntry("Decrease Translate x value",'q');
            glutAddMenuEntry("Decrease Translate y value",'w');
            glutAddMenuEntry("Decrease Translate z value",'e');
            glutAddMenuEntry("GL_FRONT & GL_LINE",'z');
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
            //button= GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, or GLUT MIDDLE_BUTTON
            glutAttachMenu(GLUT_RIGHT_BUTTON);
            
			// OpenGL settings
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
            
            //default
            glPolygonMode(GL_FRONT,GL_FILL);

			// Enter GLUT's main loop
			glutMainLoop();
		}
		else cout << "Try again with this usage: generator operationName inputs filename" << endl;
	}
	else cout << "Try again with this usage: generator operationName inputs filename" << endl;

	return 1;
}
