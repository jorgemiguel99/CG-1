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

float camX = 0, camY, camZ = 5;
int startX, startY, tracking = 0;

int alpha = 0, beta = 0, r = 5;

int numVertices, numPatches;

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
	for (i = 1; i <= numberOfPatches; i++) {
		aux = "";
		aux = patchFile[i];
		patchesString.push_back(aux);
	}
	for (j = i + 1; j <= numberOfControlPoints + numberOfPatches + 1; j++) {
		aux = "";
		aux = patchFile[j];
		verticesString.push_back(aux);
	}
}

string parsing(string aux) {
	string auxParsed = "";
	int auxSize = aux.size();
	//cout << "Size of String input: " << auxSize << endl;
	int count = 0;
	while (count <= auxSize) {
		if (aux[count] != ',') {
			auxParsed += aux[count];
		}
		count++;
	}
	//cout << auxParsed << endl;
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
			string valuesParsed = parsing(patches[j]);
			istringstream is_Value(valuesParsed);
			values.erase(values.begin(), values.end());
			copy(istream_iterator<float>(is_Value), istream_iterator<float>(), back_inserter(values));
			adicionaValues(values, 1);
		}
	}
	else if (flag == 2) { // vertices
		for (int j = 0; j < verticesString.size(); j++) {
			string valuesParsed = parsing(controlPoints[j]);
			istringstream is_Value(valuesParsed);
			values.erase(values.begin(), values.end());
			copy(istream_iterator<float>(is_Value), istream_iterator<float>(), back_inserter(values));
			adicionaValues(values, 2);
		}
	}
}

void getBezierPoint(float u, float v, vector<float> indices, float *res[4]) {
	float U[4] = { powf(u, 3), powf(u, 2), u, 1 };
	float M[4][4] = { {-1.0, 3.0, -3.0, 1.0}, {3.0, -6.0, 3.0, 0.0}, {-3.0, 3.0, 0.0, 0.0}, {1.0, 0.0, 0.0, 0.0} };
	float b[4][1] = { {powf(1 - v, 3)}, {3 * v * powf(1 - v, 2)}, {3 * powf(v, 2) * (1 - v)}, {powf(v, 3)} }; 

	// UM = U * M
	float UM[4];
	for (int i = 0; i < 4; i++) UM[i] = U[0] * M[0][i] + U[1] * M[1][i] + U[2] * M[2][i] + U[3] * M[3][i];

	// res = UM * vertices * b
	float res[4][3];
	int k = 0;
	for (int i = 0; i < 4; i++) {
		res[i] = (float*)malloc(3 * sizeof(float));
		float UMP[3];
		for (int j = 0; j < 3; j++) {
			UMP[j] = UM[0] * vertices[indices[k]][j] + UM[1] * vertices[indices[k + 1]][j] + UM[2] * vertices[indices[k + 2]][j] + UM[3] * vertices[indices[k + 3]][j];
			res[i][j] = b[i][0] * UM[j];
		}
		k += 4;
	}
}

void renderTeapot() {
	glBegin(GL_LINE_LOOP);
	for (int patch = 0; patch < numPatches; patch++) {
		int indicesPatch[16] = patches[patch]; // Gets the indices from the patch
		float v = 0.0;
		while (v <= 1) {
			float u = 0.0;
			while (u <= 1) {
				float *res[4];
				getBezierPoint(u, v, indicesPatch, res);
				for (int i = 0; i < 4; i++) glVertex3f(res[i][0], res[i][1], res[i][2]);
				u += 0.01;
			}
			v += 0.01;
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

	// Fill the arrays with vertices
	getData();

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
