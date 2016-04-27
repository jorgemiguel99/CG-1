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

using namespace std;

float camX = 0, camY, camZ = 5;
int startX, startY, tracking = 0;

int alpha = 0, beta = 0, r = 5;

int numVertices, numPatches;

// Global variable to save patch file content
vector<string> patchFile;

// Global variable to save separated content patches
vector<string> patchesString;

// Global variable to save separated content control points
vector<string> verticesString;

// Global variable to save patches floats
vector<vector<float>> patches;

// Global variable to save control points floats
vector<vector<float>> vertices;

vector<float> values;

// Reads the patch and saves on a vector
vector<string> readPatchFile(string figure) {
	vector<string> vecx;
	ifstream fileTXT(figure);
	string line;
	while (getline(fileTXT, line)) {
		vecx.push_back(line); // Adding the line at the end of vector
	}
	numPatches = stoi(vecx[0]); // Number of total patches
	numVertices = stoi(vecx[numPatches + 1]); // Number of total control points
	return vecx;
}

void separeContent() {
	string aux;
	int i, j;
	for (i = 1; i <= numPatches; i++) {
		aux = "";
		aux = patchFile[i];
		patchesString.push_back(aux);
	}
	for (j = i + 1; j <= numVertices + numPatches+ 1; j++) {
		aux = "";
		aux = patchFile[j];
		verticesString.push_back(aux);
	}
}

string parsing(string aux) {
	string auxParsed = "";
	int auxSize = aux.size();
	int count = 0;
	while (count <= auxSize) {
		if (aux[count] != ',') {
			auxParsed += aux[count];
		}
		count++;
	}
	return auxParsed;
}

void adicionaValues(vector<float> aux, int flag) {
	switch (flag) {
	case 1:
		patches.push_back(aux);
		break;
	case 2:
		vertices.push_back(aux);
		break;
	}
}

void preencheVectors(int flag) {
	if (flag == 1) { // patches
		for (int j = 0; j < patchesString.size(); j++) {
			string valuesParsed = parsing(patchesString[j]);
			istringstream is_Value(valuesParsed);
			values.erase(values.begin(), values.end());
			copy(istream_iterator<float>(is_Value), istream_iterator<float>(), back_inserter(values));
			adicionaValues(values, 1);
		}
	}
	else if (flag == 2) { // vertices
		for (int j = 0; j < verticesString.size(); j++) {
			string valuesParsed = parsing(verticesString[j]);
			istringstream is_Value(valuesParsed);
			values.erase(values.begin(), values.end());
			copy(istream_iterator<float>(is_Value), istream_iterator<float>(), back_inserter(values));
			adicionaValues(values, 2);
		}
	}
}

float getBezierPoint(float u, float v, vector<float> indices, int coord) {
	float pointValue = 0;

	float bu[4][1] = { {powf(1 - u, 3)}, {3 * u * powf(1 - u, 2)}, {3 * powf(u, 2) * (1 - u)}, {powf(u, 3)} };
	float bv[4][1] = { {powf(1 - v, 3)}, {3 * v * powf(1 - v, 2)}, {3 * powf(v, 2) * (1 - v)}, {powf(v, 3)} };

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			pointValue += vertices[indices[j + 4 * i]][coord] * bu[i][0] * bv[j][0];
		}
	}

	return pointValue;
}

void renderTeapot() {
	glBegin(GL_LINE_LOOP);
	for (int patch = 0; patch < numPatches; patch++) {
		vector<float> indicesPatch = patches[patch];
		float v = 0.0;
		while (v <= 1) {
			float u = 0.0;
			while (u <= 1) {
				float res[4][3];
				for (int i = 0; i < 4; i++) {
					res[i][0] = getBezierPoint(u, v, indicesPatch, 0);
					res[i][1] = getBezierPoint(u, v, indicesPatch, 1);
					res[i][2] = getBezierPoint(u, v, indicesPatch, 2);
					glVertex3f(res[i][0], res[i][1], res[i][2]);
				}
				u += 0.05;
			}
			v += 0.05;
		}
	}
	glEnd();
}

void renderScene(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

	// Prints the teapot using bezier curves
	renderTeapot();

	glutSwapBuffers();
}

void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0) h = 1;

	// Compute window's aspect ratio
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45, ratio, 1, 1000);

	// Return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void processMouseButtons(int button, int state, int xx, int yy) {
	if (state == GLUT_DOWN) {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON) tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON) tracking = 2;
		else tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			r -= yy - startY;
			if (r < 3) r = 3.0;
		}
		tracking = 0;
	}
}

void processMouseMotion(int xx, int yy) {
	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking) return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {
		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0) betaAux = 85.0;
		else if (betaAux < -85.0) betaAux = -85.0;
		rAux = r;
	}
	else if (tracking == 2) {
		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3) rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux * sin(betaAux * 3.14 / 180.0);
}


int main(int argc, char **argv) {
	// Inicialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 320);
	glutCreateWindow("CG@DI-UM");

	patchFile=readPatchFile("teapot.patch");
	separeContent();
	preencheVectors(1);
	preencheVectors(2);

	// Callback registration
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	// Mouse callbacks
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	// OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Enter GLUT's main cycle
	glutMainLoop();

	return 0;
}
