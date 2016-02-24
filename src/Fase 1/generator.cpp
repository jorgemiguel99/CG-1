#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <sstream>
#include <windows.h>
#include <GL/glut.h>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

//Global Variables Handling Files
string desktop = "C:\\Users\\zecar\\Desktop\\";
string filename, fileExtension = ".3d";

//Global Variables Glut functions
float dimension; //Box
float radius; //Sphere
float baseRadius, height; //Cone
int slices, stacks; //Sphere & Cone

//Plane needs 4 points(X,Y), points A,B,C & D ????
float coordenateXA,coordenateYA,coordenateXB,coordenateYB,coordenateXC,coordenateYC,coordenateXD,coordenateYD; //Plane

//Global Variable Process Input
vector<string> splitted;

//operation name has 100 char at max
#define MAX 100

//Return the number to be used on the switch in main function; splitted[1]<=>operation name
int checkOP(string str){
    if (str == "Plane" || str == "plane") return 1;
    else if (str == "Box" || str == "box") return 2;
    else if (str == "Sphere" || str == "sphere") return 3;
    else if (str == "Cone" || str == "cone") return 4;
    else return -1;
}

//function that splits a string into its substrings accordingly to its delimiter that must be provided
vector<string> split(string str, char delimiter) {
    vector<string> internal;
    stringstream ss(str); // Turn the string into a stream.
    string tok;
    
    while (getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }
    
    return internal;
}

void changeSize(int w, int h) {
    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if (h == 0) h = 1;
    
    // compute window's aspect ratio
    float ratio = w * 1.0 / h;
    
    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    
    // Load Identity Matrix
    glLoadIdentity();
    
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    
    // Set perspective
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
    
    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // set the camera
    glLoadIdentity();
    gluLookAt(2.0, 0.0, 5.0, 0.0, 0.0, -1.0, 0.0f, 1.0f, 0.0f);
    
    // put drawing instructions here
    //use values of global variable splitted
    
    if(checkOP(splitted[1])==1) { //Plane
        //Glut functions missing!!!!
        cout << "Plane Operation is not finished!!!" << endl;
        //enable growing effect
    }
    else if(checkOP(splitted[1])==2) { //Box
        glutWireCube (dimension);
        //enable growing effect
        if(dimension < 1.5) dimension+=0.001;
    }
    else if(checkOP(splitted[1])==3) { //Sphere
        glutWireSphere(radius, slices, stacks);
        //enable growing effect
        if (radius < 1.5 | slices < 15 | stacks < 30) {
            radius += 0.01;
            slices += 1;
            stacks += 1;
        }
    }
    else if(checkOP(splitted[1])==4) { //Cone
		glutWireCone(baseRadius, height, slices, stacks);
         //enable growing effect
		if (baseRadius < 1.5 | height < 1.5 | slices < 15 | stacks < 30) {
			baseRadius += 0.01;
			height += 0.01;
			slices += 1;
			stacks += 1;
		}
	}

    // End of frame
    glutSwapBuffers();
}

//create filename.3d in Desktop and in .exe folder
void create3dFile(string fileNm) {
	ofstream File(filename);
	ofstream desktopFile(desktop + filename);
}

//write xml into .xml file create before
void writeXMLFile(string fileNm) {
	string aux = "generator.xml";
	ofstream fileXML;
	fileXML.open(aux.c_str());
	fileXML << "<scene>\n\t<model file = ”" << fileNm << "” />\n</scene>" << endl;
	fileXML.close();

	ofstream desktopXMLFile(desktop + aux);
	desktopXMLFile << "<scene>\n\t<model file = ”" << fileNm << "” />\n</scene>" << endl;
}

int main(int argc, char **argv) {
	string operationLine, operation;

	cout << "Insert your operation:" << endl;
	getline(cin, operationLine);
	splitted = split(operationLine, ' ');	//guarda a string no vector com as substrings separadas por um espaco

	//stof is a function that transforms the content of string into float
	if (splitted[0] == "Generator" || splitted[0] == "generator") {
		switch (checkOP(splitted[1])) {
			case 1:		  
				if (splitted.size() == 11) { //Plane
					filename = splitted[10] + fileExtension;
                    //update Glut Functions args used in renderScene
                    coordenateXA=stof(splitted[2]);
                    coordenateYA=stof(splitted[3]);
                    coordenateXB=stof(splitted[4]);
                    coordenateYB=stof(splitted[5]);
                    coordenateXC=stof(splitted[6]);
                    coordenateYC=stof(splitted[7]);
                    coordenateXD=stof(splitted[8]);
                    coordenateYD=stof(splitted[9]);
				}
				break;
			case 2:
				if (splitted.size() == 4) { //Box
			        filename = splitted[3] + fileExtension;
                    //update Glut Functions args used in renderScene
					dimension=stof(splitted[2]);
				}
				break;
			case 3:       
				if (splitted.size() == 6) { //Sphere
			        filename = splitted[5] + fileExtension;
                    //update Glut Functions args used in renderScene
					radius = stof(splitted[2]);
					slices = stof(splitted[3]);
					stacks = stof(splitted[4]);
				}
				break;
			case 4:       
				if (splitted.size() == 7) { //Cone
			        filename = splitted[6] + fileExtension;
                    filename = splitted[5] + fileExtension;
                    //update Glut Functions args used in renderScene
					baseRadius = stof(splitted[2]);
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
    
	// put init here
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Solar System - Stage 1");
    
    // put callback registration here
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    
    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    // enter GLUT's main loop
    glutMainLoop();
    
	create3dFile(filename);
	writeXMLFile(filename);
	return 1;
}
