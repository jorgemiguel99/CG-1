#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLUT/glut.h>
//Colocar aqui includes DevIL WIMDOWS!!!
// MAC OS X
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <math.h>

#define _PI_ 3.14159
using namespace std;

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

GLfloat amb[4]  = {0.2, 0.2, 0.2, 1.0};
GLfloat diff[4] = {1.0, 1.0, 1.0, 1.0};
GLfloat pos[4]  = {1.0, 1.0, 1.0, 0.0};

int timebase = 0, frame = 0;

/* Texture related variables. */
unsigned int texID;
int cols, rows;

const int number = 3;
GLuint vertexCount, normalCount, textureCount, verticesGL[number];
float *vrt = new float[1000000];
float *n = new float[1000000];
float *t = new float[1000000];
void initVBO() {
	for (int i = 0;i<1000000;i++) {
		vrt[i]=0;
		n[i]=0;
		t[i]=0;
	}
}

void sphericalToCartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

int numVertices, numPatches;

// Global variable to save patch file content
vector<string> patchFile;

// Global variable to save separated content patches
vector<string> patchesString;

// Global variable to save separated content control points
vector<string> verticesString;

// Global variable to save patches floats
vector< vector<float> > patches;

// Global variable to save control points floats
vector< vector<float> > vertices;

vector<float> values;

// Reads the teapot.patch into a vector
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
	for (j = i + 1; j <= numVertices + numPatches + 1; j++) {
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
	if (flag == 1) { //patches
		for (int j = 0; j < patchesString.size(); j++) {
			string valuesParsed = parsing(patchesString[j]);
			istringstream is_Value(valuesParsed);
			values.erase(values.begin(), values.end());
			copy(istream_iterator<float>(is_Value), istream_iterator<float>(), back_inserter(values));
			adicionaValues(values, 1);
		}
	}
	else if (flag == 2) { // verticesString
		for (int j = 0; j < verticesString.size(); j++) {
			string valuesParsed = parsing(verticesString[j]);
			istringstream is_Value(valuesParsed);
			values.erase(values.begin(), values.end());
			copy(istream_iterator<float>(is_Value), istream_iterator<float>(), back_inserter(values));
			adicionaValues(values, 2);
		}
	}
}

void cross(float *a, float *b, float *res) {
 res[0] = a[1]*b[2] - a[2]*b[1]; res[1] = a[2]*b[0] - a[0]*b[2]; res[2] = a[0]*b[1] - a[1]*b[0];}

void normalize(float *a) {
 float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]); a[0] = a[0]/l; a[1] = a[1]/l; a[2] = a[2]/l;}

// Generates the points for the bezier curve
float getBezierPoint(float u, float v, vector<float> indices, int coord) {
	float pointValue = 0;

	float bu[4][1] = { { powf(1 - u, 3) },{ 3 * u * powf(1 - u, 2) },{ 3 * powf(u, 2) * (1 - u) },{ powf(u, 3) } };
	float bv[4][1] = { { powf(1 - v, 3) },{ 3 * v * powf(1 - v, 2) },{ 3 * powf(v, 2) * (1 - v) },{ powf(v, 3) } };

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			pointValue += vertices[indices[j + 4 * i]][coord] * bu[i][0] * bv[j][0];
		}
	}

	return pointValue;
}

// Generates the points for the tangent bezier curve
// Tangent is a boolean
// 1 indicates that is dbu
// 0 indicates that is dbv
float getBezierPointTangents(float u, float v, vector<float> indices, int coord, int tangent) {
	float bu[4]  = { powf(u,3), powf(u,2), u, 1 };
	float dbu[4] = { 3*powf(u,2), 2*u, 1, 0 };
	float bv[4]  = { powf(v,3), powf(v,2), v, 1 };
	float dbv[4] = { 3*powf(v,2), 2*v, 1, 0 };

	float m[4][4] = {
		{ -1.0,3.0f,-3.0f,1.0f },
		{ 3.0f,-6.0f,3.0f,0.0f },
		{ -3.0f,3.0f,0.0f,0.0f },
		{ 1.0f,0.0f,0.0f,0.0f }
	};

	float p[4][4] = {
		{ vertices[indices[0]][coord],vertices[indices[1]][coord],vertices[indices[2]][coord],vertices[indices[3]][coord] },
		{ vertices[indices[4]][coord],vertices[indices[5]][coord],vertices[indices[6]][coord],vertices[indices[7]][coord] },
		{ vertices[indices[8]][coord],vertices[indices[9]][coord],vertices[indices[10]][coord],vertices[indices[11]][coord] },
		{ vertices[indices[12]][coord],vertices[indices[13]][coord],vertices[indices[14]][coord],vertices[indices[15]][coord] }
	};

	float aux1[4] = { 0.0f,0.0f,0.0f,0.0f };
	float aux2[4] = { 0.0f,0.0f,0.0f,0.0f };
	float aux3[4] = { 0.0f,0.0f,0.0f,0.0f };

	if (tangent == 1) {
		for (int i = 0; i < 4; i++) {
			aux1[i] = (dbu[0] * m[0][i]) + (dbu[1] * m[1][i]) + (dbu[2] * m[2][i]) + (dbu[3] * m[3][i]);
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			aux1[i] = (bu[0] * m[0][i]) + (bu[1] * m[1][i]) + (bu[2] * m[2][i]) + (bu[3] * m[3][i]);
		}
	}

	for (int i = 0; i < 4; i++) {
		aux2[i] = (aux1[0] * p[0][i]) + (aux1[1] * p[1][i]) + (aux1[2] * p[2][i]) + (aux1[3] * p[3][i]);
	}

	for (int i = 0; i < 4; i++) {
		aux3[i] = (aux2[0] * m[i][0]) + (aux2[1] * m[i][1]) + (aux2[2] * m[i][2]) + (aux2[3] * m[i][3]);
	}

	if(tangent==1){
		return ((aux3[0] * bv[0]) + (aux3[1] * bv[1]) + (aux3[2] * bv[2]) + (aux3[3] * bv[3]));
	}
	else{
		return ((aux3[0] * dbv[0]) + (aux3[1] * dbv[1]) + (aux3[2] * dbv[2]) + (aux3[3] * dbv[3]));
  }
}


void prepareTeapot() {
	initVBO();
	float res[3], normU[3], normV[3], norm[3]; //X,Y,Z
	float tesselation = 0.1;

	for (int patch = 0; patch < numPatches; patch++) {
		vector<float> indicesPatch = patches[patch];
		float v = 0.0;
		int countv=0;
		while (v <= 1) {
			float u = 0.0;
			int countu=0;
			while (u <= 1) {
				//Vertex
				res[0] = getBezierPoint(u, v, indicesPatch, 0);
				res[1] = getBezierPoint(u, v, indicesPatch, 1);
				res[2] = getBezierPoint(u, v, indicesPatch, 2);
				vrt[vertexCount] = res[0];
				vertexCount++;
				vrt[vertexCount] = res[1];
				vertexCount++;
				vrt[vertexCount] = res[2];
				vertexCount++;

				//Normals
				normU[0] = getBezierPointTangents(u, v, indicesPatch, 0, 1);
				normU[1] = getBezierPointTangents(u, v, indicesPatch, 1, 1);
				normU[2] = getBezierPointTangents(u, v, indicesPatch, 2, 1);
				normalize(normU);
				normV[0] = getBezierPointTangents(u, v, indicesPatch, 0, 0);
				normV[1] = getBezierPointTangents(u, v, indicesPatch, 1, 0);
				normV[2] = getBezierPointTangents(u, v, indicesPatch, 2, 0);
				normalize(normV);
				cross(normV,normU,norm);
				normalize(norm);
				n[normalCount] = norm[0];
				normalCount++;
				n[normalCount] = norm[1];
				normalCount++;
				n[normalCount] = norm[2];
				normalCount++;

				//Textures
				t[textureCount] = countu;
				textureCount++;
				t[textureCount] = countv;
				textureCount++;


				//Vertex
				res[0] = getBezierPoint(u + tesselation, v, indicesPatch, 0);
				res[1] = getBezierPoint(u + tesselation, v, indicesPatch, 1);
				res[2] = getBezierPoint(u + tesselation, v, indicesPatch, 2);
				vrt[vertexCount] = res[0];
				vertexCount++;
				vrt[vertexCount] = res[1];
				vertexCount++;
				vrt[vertexCount] = res[2];
				vertexCount++;

				//Normals
				normU[0] = getBezierPointTangents(u + tesselation, v, indicesPatch, 0, 1);
				normU[1] = getBezierPointTangents(u + tesselation, v, indicesPatch, 1, 1);
				normU[2] = getBezierPointTangents(u + tesselation, v, indicesPatch, 2, 1);
				normalize(normU);
				normV[0] = getBezierPointTangents(u + tesselation, v, indicesPatch, 0, 0);
				normV[1] = getBezierPointTangents(u + tesselation, v, indicesPatch, 1, 0);
				normV[2] = getBezierPointTangents(u + tesselation, v, indicesPatch, 2, 0);
				normalize(normV);
				cross(normV,normU,norm);
				normalize(norm);
				n[normalCount] = norm[0];
				normalCount++;
				n[normalCount] = norm[1];
				normalCount++;
				n[normalCount] = norm[2];
				normalCount++;

				//Textures
				t[textureCount] = countu+1;
				textureCount++;
				t[textureCount] = countv;
				textureCount++;

				//Vertex
				res[0] = getBezierPoint(u, v + tesselation, indicesPatch, 0);
				res[1] = getBezierPoint(u, v + tesselation, indicesPatch, 1);
				res[2] = getBezierPoint(u, v + tesselation, indicesPatch, 2);
				vrt[vertexCount] = res[0];
				vertexCount++;
				vrt[vertexCount] = res[1];
				vertexCount++;
				vrt[vertexCount] = res[2];
				vertexCount++;

				//Normals
				normU[0] = getBezierPointTangents(u, v + tesselation, indicesPatch, 0, 1);
				normU[1] = getBezierPointTangents(u, v + tesselation, indicesPatch, 1, 1);
				normU[2] = getBezierPointTangents(u, v + tesselation, indicesPatch, 2, 1);
				normalize(normU);
				normV[0] = getBezierPointTangents(u, v + tesselation, indicesPatch, 0, 0);
				normV[1] = getBezierPointTangents(u, v + tesselation, indicesPatch, 1, 0);
				normV[2] = getBezierPointTangents(u, v + tesselation, indicesPatch, 2, 0);
				normalize(normV);
				cross(normV,normU,norm);
				normalize(norm);
				n[normalCount] = norm[0];
				normalCount++;
				n[normalCount] = norm[1];
				normalCount++;
				n[normalCount] = norm[2];
				normalCount++;

				//Textures
				t[textureCount] = countu;
				textureCount++;
				t[textureCount] = countv+1;
				textureCount++;

				//Vertex
				res[0] = getBezierPoint(u, v + tesselation, indicesPatch, 0);
				res[1] = getBezierPoint(u, v + tesselation, indicesPatch, 1);
				res[2] = getBezierPoint(u, v + tesselation, indicesPatch, 2);
				vrt[vertexCount] = res[0];
				vertexCount++;
				vrt[vertexCount] = res[1];
				vertexCount++;
				vrt[vertexCount] = res[2];
				vertexCount++;

				//Normals
				normU[0] = getBezierPointTangents(u, v + tesselation, indicesPatch, 0, 1);
				normU[1] = getBezierPointTangents(u, v + tesselation, indicesPatch, 1, 1);
				normU[2] = getBezierPointTangents(u, v + tesselation, indicesPatch, 2, 1);
				normalize(normU);
				normV[0] = getBezierPointTangents(u, v + tesselation, indicesPatch, 0, 0);
				normV[1] = getBezierPointTangents(u, v + tesselation, indicesPatch, 1, 0);
				normV[2] = getBezierPointTangents(u, v + tesselation, indicesPatch, 2, 0);
				normalize(normV);
				cross(normV,normU,norm);
				normalize(norm);
				n[normalCount] = norm[0];
				normalCount++;
				n[normalCount] = norm[1];
				normalCount++;
				n[normalCount] = norm[2];
				normalCount++;

				//Textures
				t[textureCount] = countu;
				textureCount++;
				t[textureCount] = countv+1;
				textureCount++;

				//Vertex
				res[0] = getBezierPoint(u + tesselation, v, indicesPatch, 0);
				res[1] = getBezierPoint(u + tesselation, v, indicesPatch, 1);
				res[2] = getBezierPoint(u + tesselation, v, indicesPatch, 2);
				vrt[vertexCount] = res[0];
				vertexCount++;
				vrt[vertexCount] = res[1];
				vertexCount++;
				vrt[vertexCount] = res[2];
				vertexCount++;

				//Normals
				normU[0] = getBezierPointTangents(u + tesselation, v, indicesPatch, 0, 1);
				normU[1] = getBezierPointTangents(u + tesselation, v, indicesPatch, 1, 1);
				normU[2] = getBezierPointTangents(u + tesselation, v, indicesPatch, 2, 1);
				normalize(normU);
				normV[0] = getBezierPointTangents(u + tesselation, v, indicesPatch, 0, 0);
				normV[1] = getBezierPointTangents(u + tesselation, v, indicesPatch, 1, 0);
				normV[2] = getBezierPointTangents(u + tesselation, v, indicesPatch, 2, 0);
				normalize(normV);
				cross(normV,normU,norm);
				normalize(norm);
				n[normalCount] = norm[0];
				normalCount++;
				n[normalCount] = norm[1];
				normalCount++;
				n[normalCount] = norm[2];
				normalCount++;

				//Textures
				t[textureCount] = countu+1;
				textureCount++;
				t[textureCount] = countv;
				textureCount++;

				//Vertex
				res[0] = getBezierPoint(u + tesselation, v + tesselation, indicesPatch, 0);
				res[1] = getBezierPoint(u + tesselation, v + tesselation, indicesPatch, 1);
				res[2] = getBezierPoint(u + tesselation, v + tesselation, indicesPatch, 2);
				vrt[vertexCount] = res[0];
				vertexCount++;
				vrt[vertexCount] = res[1];
				vertexCount++;
				vrt[vertexCount] = res[2];
				vertexCount++;

				//Normals
				normU[0] = getBezierPointTangents(u + tesselation, v + tesselation, indicesPatch, 0, 1);
				normU[1] = getBezierPointTangents(u + tesselation, v + tesselation, indicesPatch, 1, 1);
				normU[2] = getBezierPointTangents(u + tesselation, v + tesselation, indicesPatch, 2, 1);
				normalize(normU);
				normV[0] = getBezierPointTangents(u + tesselation, v + tesselation, indicesPatch, 0, 0);
				normV[1] = getBezierPointTangents(u + tesselation, v + tesselation, indicesPatch, 1, 0);
				normV[2] = getBezierPointTangents(u + tesselation, v + tesselation, indicesPatch, 2, 0);
				normalize(normV);
				cross(normV,normU,norm);
				normalize(norm);
				n[normalCount] = norm[0];
				normalCount++;
				n[normalCount] = norm[1];
				normalCount++;
				n[normalCount] = norm[2];
				normalCount++;

				//Textures
				t[textureCount] = countu+1;
				textureCount++;
				t[textureCount] = countv+1;
				textureCount++;

				countu++;
				u += tesselation;
			}
			countu=0;
			v += tesselation;
			countv++;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, verticesGL[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (vertexCount-1), vrt, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, verticesGL[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (vertexCount-1), n, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, verticesGL[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (textureCount-1), t, GL_DYNAMIC_DRAW);
}

void renderScene(void) {
	float fps;
	int time;
	char s[64];

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	// light position
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	// light colors
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);

	gluLookAt(camX,camY,camZ,
					0.0,0.0,0.0,
				0.0f,1.0f,0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, verticesGL[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, verticesGL[1]);
	glNormalPointer(GL_FLOAT,0,0);

	glBindBuffer(GL_ARRAY_BUFFER, verticesGL[2]);
	glTexCoordPointer(2,GL_FLOAT,0,0);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		fps = frame*1000.0/(time-timebase);
		timebase = time;
		frame = 0;
		sprintf(s, "FPS: %f6.2", fps);
		glutSetWindowTitle(s);
	}

// End of frame
	glutSwapBuffers();
}

void processKeys(int key, int xx, int yy)
{
	switch(key) {

		case GLUT_KEY_RIGHT:
						alfa -=0.1; break;

		case GLUT_KEY_LEFT:
						alfa += 0.1; break;

		case GLUT_KEY_UP :
						beta += 0.1f;
						if (beta > 1.5f)
							beta = 1.5f;
						break;

		case GLUT_KEY_DOWN:
						beta -= 0.1f;
						if (beta < -1.5f)
							beta = -1.5f;
						break;

		case GLUT_KEY_PAGE_DOWN : radius -= 0.1f;
			if (radius < 0.1f)
				radius = 0.1f;
			break;

		case GLUT_KEY_PAGE_UP: radius += 0.1f; break;

	}
	sphericalToCartesian();

}



void initGL() {

// OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
// init
	sphericalToCartesian();

	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

//Light
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Texturas.
	glEnable(GL_TEXTURE_2D);
	patchFile = readPatchFile("teapot.patch");
	separeContent();
	preencheVectors(1);
	preencheVectors(2);
	prepareTeapot();
}


int main(int argc, char **argv) {

// init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("CG@DI-UM");

// callback registration
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// keyboard callback registration
	glutSpecialFunc(processKeys);
	GLenum err = glewInit();
	if (err != GLEW_OK)  printf(" Error initializing GLEW! \n");
	else printf("Initializing GLEW succeeded!\n");

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

  //Colocar aqui leitura de textura e imagem com o DevIL

	glEnableClientState(GL_VERTEX_ARRAY);
	glGenBuffers(number, verticesGL);
	glEnableClientState(GL_VERTEX_ARRAY);

	initGL();
	glutMainLoop();
	return 0;
}
