#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <windows.h>
#include <GL/glut.h>
//#include <GLUT/glut.h> -- MAC

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

//Global Variables Handling Files
string desktop = "C:\\Users\\zecar\\Desktop\\";
//string desktop = "/Users/zecarlos/Desktop/"; -- MAC
string filename, fileExtension = ".3d";

//Global Variables Glut functions
float dimension; //Box
float radius; //Sphere
float baseRadius; //Cone
int slices, stacks; //Sphere & Cone

// Dimens�es do plano e da caixa
float comprimento, largura, altura;

// Vari�veis usadas para mover a c�mara
float moveX = 0.0f, moveY = 0.0f, moveZ = 0.0f, rotateX = 0.0f, rotateY = 0.0f, rotateZ = 0.0f, angle = 0;

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
    // Clear Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the Camera
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 1.0, 0.0, 0.0f, 1.0f, 0.0f);

	// Geometric Transformations
	glTranslatef(moveX, moveY, moveZ);
	glRotatef(angle, rotateX, rotateY, rotateZ);

    // Plano
    if(checkOP(splitted[1])==1) {
		glBegin(GL_TRIANGLES);

		glColor3f(0, 1, 0);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(comprimento, 0.0f, 0.0f);
		glVertex3f(0.0f, largura, 0.0f);

		glColor3f(0, 1, 0);
		glVertex3f(comprimento, 0.0f, 0.0f);
		glVertex3f(comprimento, largura, 0.0f);
		glVertex3f(0.0f, largura, 0.0f);

		glEnd();

    ofstream plane;
  	plane.open(filename.c_str());
  	plane << "6" << endl; // total number of vertex
    plane << "0.0f, 0.0f, 0.0f" << endl;
    plane << comprimento << ".0f" <<", 0.0f, 0.0f" << endl;
    plane << "0.0f, " << largura << ".0f" << ", 0.0f" << endl;
    plane << comprimento << ".0f" << ", 0.0f, 0.0f" << endl;
    plane << comprimento << ".0f" << ", " << largura << ".0f" << ", 0.0f" << endl;
    plane << "0.0f, " << largura << ".0f" << ", 0.0f" << endl;
  	plane.close();
    }
	// Caixa
    else if(checkOP(splitted[1])==2) {
		glBegin(GL_TRIANGLES);

		// Face Inferior
		glColor3f(1, 0, 0);
		glVertex3f(0.0f, largura, 0.0f);
		glVertex3f(comprimento, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);

		glColor3f(1, 0, 0);
		glVertex3f(0.0f, largura, 0.0f);
		glVertex3f(comprimento, largura, 0.0f);
		glVertex3f(comprimento, 0.0f, 0.0f);

		// Face Superior
		glColor3f(1, 0, 0);
		glVertex3f(0.0f, 0.0f, altura);
		glVertex3f(comprimento, 0.0f, altura);
		glVertex3f(0.0f, largura, altura);

		glColor3f(1, 0, 0);
		glVertex3f(comprimento, 0.0f, altura);
		glVertex3f(comprimento, largura, altura);
		glVertex3f(0.0f, largura, altura);

		// Face Direita
		glColor3f(0, 1, 0);
		glVertex3f(comprimento, 0.0f, 0.0f);
		glVertex3f(comprimento, largura, 0.0f);
		glVertex3f(comprimento, 0.0f, altura);

		glColor3f(0, 1, 0);
		glVertex3f(comprimento, largura, 0.0f);
		glVertex3f(comprimento, largura, altura);
		glVertex3f(comprimento, 0.0f, altura);

		// Face Esquerda
		glColor3f(0, 1, 0);
		glVertex3f(0.0f, largura, 0.0f);
		glVertex3f(0.0f, 0.0f, altura);
		glVertex3f(0.0f, largura, altura);

		glColor3f(0, 1, 0);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, altura);
		glVertex3f(0.0f, largura, 0.0f);

		// Face da Frente
		glColor3f(0, 0, 1);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(comprimento, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, altura);

		glColor3f(0, 0, 1);
		glVertex3f(comprimento, 0.0f, 0.0f);
		glVertex3f(comprimento, 0.0f, altura);
		glVertex3f(0.0f, 0.0f, altura);

		// Face de Tr�s
		glColor3f(0, 0, 1);
		glVertex3f(0.0f, largura, 0.0f);
		glVertex3f(0.0f, largura, altura);
		glVertex3f(comprimento, largura, 0.0f);

		glColor3f(0, 0, 1);
		glVertex3f(comprimento, largura, 0.0f);
		glVertex3f(0.0f, largura, altura);
		glVertex3f(comprimento, largura, altura);

		glEnd();
    }
	// Sphere
    else if(checkOP(splitted[1])==3) {
        glutWireSphere(radius, slices, stacks);
        //enable growing effect
        if (radius < 1.5 | slices < 15 | stacks < 30) {
            radius += 0.01;
            slices += 1;
            stacks += 1;
        }
    }
    else if(checkOP(splitted[1])==4) { //Cone
		glutWireCone(baseRadius, altura, slices, stacks);
         //enable growing effect
		if (baseRadius < 1.5 | altura < 1.5 | slices < 15 | stacks < 30) {
			baseRadius += 0.01;
			altura += 0.01;
			slices += 1;
			stacks += 1;
		}
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
    case 27 : exit(-1); //ESC -> leaves
	}
	glutPostRedisplay();
}

// Create filename.3d in Desktop and in .exe folder
void create3dFile(string fileNm) {
	ofstream File(filename);
	ofstream desktopFile(desktop + filename);
}

// Write xml into a .xml file created before
void writeXMLFile(string fileNm) {
	string aux = "generator.xml";
	ofstream fileXML;
	fileXML.open(aux.c_str());
	fileXML << "<scene>\n\t<model file = �" << fileNm << "� />\n</scene>" << endl;
	fileXML.close();

	ofstream desktopXMLFile(desktop + aux);
	desktopXMLFile << "<scene>\n\t<model file = �" << fileNm << "� />\n</scene>" << endl;
}

int main(int argc, char **argv) {
	string operationLine, operation;

	cout << "Insert your operation:" << endl;
	getline(cin, operationLine);
	splitted = split(operationLine, ' '); // Guarda a string num vetor com as substrings separadas por um espa�o

	// A fun��o "stoff" transforma o conte�do de uma string num float

	if (splitted[0] == "Generator" || splitted[0] == "generator") {
		switch (checkOP(splitted[1])) {
			case 1:	// Plano -> recebe o comprimento e a largura, sendo os v�rtices deduzidos a partir do ponto (0, 0)
				if (splitted.size() == 5) {
					filename = splitted[4] + fileExtension;
					comprimento = stof(splitted[2]);
					largura = stof(splitted[3]);
				}
				break;
			case 2: // Caixa -> recebe o comprimento, a largura e a altura, sendo os v�rtices deduzidos a partir do ponto (0, 0)
				if (splitted.size() == 6) {
			        filename = splitted[5] + fileExtension;
					comprimento = stof(splitted[2]);
					largura = stof(splitted[3]);
					altura = stof(splitted[4]);
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
					altura = stof(splitted[3]);
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

	// Create a XML file and write on it
	create3dFile(filename);
	writeXMLFile(filename);

	return 1;
}
