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
// #include <windows.h>
// #include <GL/glut.h>
#include <math.h>
#include <GLUT/glut.h> //-- MAC

#define _PI_ 3.14159

using namespace std;

// Global variable for handling files
string filename;

// Dimensions of the figures
float radius, slices, stacks, width, length, height;
int sides;

// Global variable process input
vector<string> splitted;


// Declare radii of all 8 planets & Sun
float SunRadius = 8.0;
float MercuryRadius = 2.0;
float VenusRadius = 3.0;
float EarthRadius = 3.0;
float MarsRadius = 4.0;
float JupiterRadius = 8.0;
float UranusRadius = 7.0;
float SaturnRadius = 9.0;
float NeptuneRadius = 7.0;


// Declare radii of all 16 moons which is 1/3 of correspondent planet
float radius_earthMoon = EarthRadius / 3;
float radius_PhobosMoon = MarsRadius / 3;
float radius_DeimosMoon = MarsRadius / 3;
float radius_GanymedeMoon = JupiterRadius / 3;
float radius_CallistoMoon = JupiterRadius / 3;
float radius_EuropaMoon = JupiterRadius / 3;
float radius_IoMoon = JupiterRadius / 3;
float radius_TitanMoon = SaturnRadius / 3;
float radius_EnceladusMoon = SaturnRadius / 3;
float radius_TitaniaMoon = UranusRadius / 3;
float radius_OberonMoon = UranusRadius / 3;
float radius_UmbrielMoon = UranusRadius / 3;
float radius_ArielMoon = UranusRadius / 3;
float radius_MirandaMoon = UranusRadius / 3;
float radius_TritonMoon = NeptuneRadius / 3;
float radius_ProteusMoon = NeptuneRadius / 3;

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

// Operation name has 100 char at max
#define MAX 100

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

void printContent() {
	int c = 0;
	cout << "#Patches => " << patchesString.size() << endl;
	while (c < patchesString.size()) {
		cout << "Patch #" << c << " => " << patchesString[c] << endl;
		c++;
	}
	cout << "#Control Points => " << verticesString.size() << endl;
	c = 0;
	while (c < verticesString.size()) {
		cout << "Control Point #" << c << " => " << verticesString[c] << endl;
		c++;
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

void printFloatVector() {
	vector<float> aux;
	for (int j = 0; j < patches.size(); j++) {
		aux.erase(aux.begin(), aux.end());
		aux = patches[j];
		for (int k = 0; k<aux.size(); k++) {
			cout << "patches #" << k << " => " << aux[k] << endl;
		}
	}
	for (int j = 0; j<vertices.size(); j++) {
		aux.erase(aux.begin(), aux.end());
		aux = vertices[j];
		for (int k = 0; k<aux.size(); k++) {
			cout << "vertices #" << k << " => " << aux[k] << endl;
		}
	}
}

void printVector(vector<string> vec) {
	int i;
	for (i = 0; i<vec.size(); i++) {
		cout << "#" << i << " => " << vec[i] << endl;
	}
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

void printTeapot() {
	float res[3], normU[3], normV[3], norm[3]; //X,Y,Z
	ofstream teapot;
	teapot.open(filename.c_str());
	float **teapotPoints = (float**) malloc(1000000 * sizeof(float**)); // Saves the points of the teapot to be able to count their number first and print them after that
	float **teapotNormals = (float**) malloc(1000000 * sizeof(float**)); // Saves the normals of the teapot
	float **teapotTextures = (float**) malloc(1000000 * sizeof(float**)); // Saves the texture coordinates of the teapot
	int pointNumber = 0; // Number of vertices of the teapot
	int normalNumber = 0; // Number of normals of the teapot
	int textureNumber = 0; // Number of texture coordinates of the teapot
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
				teapotPoints[pointNumber] = (float*) malloc(3 * sizeof(float*));
				res[0] = getBezierPoint(u, v, indicesPatch, 0);
				res[1] = getBezierPoint(u, v, indicesPatch, 1);
				res[2] = getBezierPoint(u, v, indicesPatch, 2);
				teapotPoints[pointNumber][0] = res[0];
				teapotPoints[pointNumber][1] = res[1];
				teapotPoints[pointNumber][2] = res[2];
				//Normals
				teapotNormals[pointNumber] = (float*) malloc(3 * sizeof(float*));
				normU[0] = getBezierPointTangents(u, v, indicesPatch, 0, 1);
				normU[1] = getBezierPointTangents(u, v, indicesPatch, 1, 1);
				normU[2] = getBezierPointTangents(u, v, indicesPatch, 2, 1);
				normalize(normU);
				normV[0] = getBezierPointTangents(u, v, indicesPatch, 0, 0);
				normV[1] = getBezierPointTangents(u, v, indicesPatch, 1, 0);
				normV[2] = getBezierPointTangents(u, v, indicesPatch, 2, 0);
				normalize(normV);
				cross(normV,normU,norm);
				teapotNormals[normalNumber][0] = norm[0];
				teapotNormals[normalNumber][1] = norm[1];
				teapotNormals[normalNumber][2] = norm[2];
				//Textures
				teapotTextures[textureNumber] = (float*) malloc(2 * sizeof(float*));
				teapotTextures[textureNumber][0] = countu;
				teapotTextures[textureNumber][1] = countv;
				pointNumber++;
				normalNumber++;
				textureNumber++;

				//Vertex
				teapotPoints[pointNumber] = (float*) malloc(3 * sizeof(float*));
				res[0] = getBezierPoint(u + tesselation, v, indicesPatch, 0);
				res[1] = getBezierPoint(u + tesselation, v, indicesPatch, 1);
				res[2] = getBezierPoint(u + tesselation, v, indicesPatch, 2);
				teapotPoints[pointNumber][0] = res[0];
				teapotPoints[pointNumber][1] = res[1];
				teapotPoints[pointNumber][2] = res[2];
				//Normals
				teapotNormals[pointNumber] = (float*) malloc(3 * sizeof(float*));
				normU[0] = getBezierPointTangents(u + tesselation, v, indicesPatch, 0, 1);
				normU[1] = getBezierPointTangents(u + tesselation, v, indicesPatch, 1, 1);
				normU[2] = getBezierPointTangents(u + tesselation, v, indicesPatch, 2, 1);
				normalize(normU);
				normV[0] = getBezierPointTangents(u + tesselation, v, indicesPatch, 0, 0);
				normV[1] = getBezierPointTangents(u + tesselation, v, indicesPatch, 1, 0);
				normV[2] = getBezierPointTangents(u + tesselation, v, indicesPatch, 2, 0);
				normalize(normV);
				cross(normV,normU,norm);
				teapotNormals[normalNumber][0] = norm[0];
				teapotNormals[normalNumber][1] = norm[1];
				teapotNormals[normalNumber][2] = norm[2];
				//Textures
				teapotTextures[textureNumber] = (float*) malloc(2 * sizeof(float*));
				teapotTextures[textureNumber][0] = countu+1;
				teapotTextures[textureNumber][1] = countv;
				pointNumber++;
				normalNumber++;
				textureNumber++;

				//Vertex
				teapotPoints[pointNumber] = (float*) malloc(3 * sizeof(float*));
				res[0] = getBezierPoint(u, v + tesselation, indicesPatch, 0);
				res[1] = getBezierPoint(u, v + tesselation, indicesPatch, 1);
				res[2] = getBezierPoint(u, v + tesselation, indicesPatch, 2);
				teapotPoints[pointNumber][0] = res[0];
				teapotPoints[pointNumber][1] = res[1];
				teapotPoints[pointNumber][2] = res[2];
				//Normals
				teapotNormals[pointNumber] = (float*) malloc(3 * sizeof(float*));
				normU[0] = getBezierPointTangents(u, v + tesselation, indicesPatch, 0, 1);
				normU[1] = getBezierPointTangents(u, v + tesselation, indicesPatch, 1, 1);
				normU[2] = getBezierPointTangents(u, v + tesselation, indicesPatch, 2, 1);
				normalize(normU);
				normV[0] = getBezierPointTangents(u, v + tesselation, indicesPatch, 0, 0);
				normV[1] = getBezierPointTangents(u, v + tesselation, indicesPatch, 1, 0);
				normV[2] = getBezierPointTangents(u, v + tesselation, indicesPatch, 2, 0);
				normalize(normV);
				cross(normV,normU,norm);
				teapotNormals[normalNumber][0] = norm[0];
				teapotNormals[normalNumber][1] = norm[1];
				teapotNormals[normalNumber][2] = norm[2];
				//Textures
				teapotTextures[textureNumber] = (float*) malloc(2 * sizeof(float*));
				teapotTextures[textureNumber][0] = countu;
				teapotTextures[textureNumber][1] = countv+1;
				pointNumber++;
				normalNumber++;
				textureNumber++;

				//Vertex
				teapotPoints[pointNumber] = (float*) malloc(3 * sizeof(float*));
				res[0] = getBezierPoint(u, v + tesselation, indicesPatch, 0);
				res[1] = getBezierPoint(u, v + tesselation, indicesPatch, 1);
				res[2] = getBezierPoint(u, v + tesselation, indicesPatch, 2);
				teapotPoints[pointNumber][0] = res[0];
				teapotPoints[pointNumber][1] = res[1];
				teapotPoints[pointNumber][2] = res[2];
				//Normals
				teapotNormals[pointNumber] = (float*) malloc(3 * sizeof(float*));
				normU[0] = getBezierPointTangents(u, v + tesselation, indicesPatch, 0, 1);
				normU[1] = getBezierPointTangents(u, v + tesselation, indicesPatch, 1, 1);
				normU[2] = getBezierPointTangents(u, v + tesselation, indicesPatch, 2, 1);
				normalize(normU);
				normV[0] = getBezierPointTangents(u, v + tesselation, indicesPatch, 0, 0);
				normV[1] = getBezierPointTangents(u, v + tesselation, indicesPatch, 1, 0);
				normV[2] = getBezierPointTangents(u, v + tesselation, indicesPatch, 2, 0);
				normalize(normV);
				cross(normV,normU,norm);
				teapotNormals[normalNumber][0] = norm[0];
				teapotNormals[normalNumber][1] = norm[1];
				teapotNormals[normalNumber][2] = norm[2];
				//Textures
				teapotTextures[textureNumber] = (float*) malloc(2 * sizeof(float*));
				teapotTextures[textureNumber][0] = countu;
				teapotTextures[textureNumber][1] = countv+1;
				pointNumber++;
				normalNumber++;
				textureNumber++;

				//Vertex
				teapotPoints[pointNumber] = (float*) malloc(3 * sizeof(float*));
				res[0] = getBezierPoint(u + tesselation, v, indicesPatch, 0);
				res[1] = getBezierPoint(u + tesselation, v, indicesPatch, 1);
				res[2] = getBezierPoint(u + tesselation, v, indicesPatch, 2);
				teapotPoints[pointNumber][0] = res[0];
				teapotPoints[pointNumber][1] = res[1];
				teapotPoints[pointNumber][2] = res[2];
				//Normals
				teapotNormals[pointNumber] = (float*) malloc(3 * sizeof(float*));
				normU[0] = getBezierPointTangents(u + tesselation, v, indicesPatch, 0, 1);
				normU[1] = getBezierPointTangents(u + tesselation, v, indicesPatch, 1, 1);
				normU[2] = getBezierPointTangents(u + tesselation, v, indicesPatch, 2, 1);
				normalize(normU);
				normV[0] = getBezierPointTangents(u + tesselation, v, indicesPatch, 0, 0);
				normV[1] = getBezierPointTangents(u + tesselation, v, indicesPatch, 1, 0);
				normV[2] = getBezierPointTangents(u + tesselation, v, indicesPatch, 2, 0);
				normalize(normV);
				cross(normV,normU,norm);
				teapotNormals[normalNumber][0] = norm[0];
				teapotNormals[normalNumber][1] = norm[1];
				teapotNormals[normalNumber][2] = norm[2];
				//Textures
				teapotTextures[textureNumber] = (float*) malloc(2 * sizeof(float*));
				teapotTextures[textureNumber][0] = countu+1;
				teapotTextures[textureNumber][1] = countv;
				pointNumber++;
				normalNumber++;
				textureNumber++;

				//Vertex
				teapotPoints[pointNumber] = (float*) malloc(3 * sizeof(float*));
				res[0] = getBezierPoint(u + tesselation, v + tesselation, indicesPatch, 0);
				res[1] = getBezierPoint(u + tesselation, v + tesselation, indicesPatch, 1);
				res[2] = getBezierPoint(u + tesselation, v + tesselation, indicesPatch, 2);
				teapotPoints[pointNumber][0] = res[0];
				teapotPoints[pointNumber][1] = res[1];
				teapotPoints[pointNumber][2] = res[2];
				//Normals
				teapotNormals[pointNumber] = (float*) malloc(3 * sizeof(float*));
				normU[0] = getBezierPointTangents(u + tesselation, v + tesselation, indicesPatch, 0, 1);
				normU[1] = getBezierPointTangents(u + tesselation, v + tesselation, indicesPatch, 1, 1);
				normU[2] = getBezierPointTangents(u + tesselation, v + tesselation, indicesPatch, 2, 1);
				normalize(normU);
				normV[0] = getBezierPointTangents(u + tesselation, v + tesselation, indicesPatch, 0, 0);
				normV[1] = getBezierPointTangents(u + tesselation, v + tesselation, indicesPatch, 1, 0);
				normV[2] = getBezierPointTangents(u + tesselation, v + tesselation, indicesPatch, 2, 0);
				normalize(normV);
				cross(normV,normU,norm);
				teapotNormals[normalNumber][0] = norm[0];
				teapotNormals[normalNumber][1] = norm[1];
				teapotNormals[normalNumber][2] = norm[2];
				//Textures
				teapotTextures[textureNumber] = (float*) malloc(2 * sizeof(float*));
				teapotTextures[textureNumber][0] = countu+1;
				teapotTextures[textureNumber][1] = countv+1;
				pointNumber++;
				normalNumber++;
				textureNumber++;

        countu++;
				u += tesselation;
			}
			countu=0;
			v += tesselation;
      countv++;
		}
	}
		teapot << (pointNumber+normalNumber+textureNumber) << endl;
		for (int i = 0; i < pointNumber; i++) {
			teapot << teapotPoints[i][0] << " " << teapotPoints[i][1] << " " << teapotPoints[i][2] << endl;
			teapot << teapotNormals[i][0] << " " << teapotNormals[i][1] << " " << teapotNormals[i][2] << endl;
			teapot << teapotTextures[i][0] << " " << teapotTextures[i][1] << endl;
		}

		teapot.close();
	}

// Prints the sphere vertices on the .3d file
void printSphere3d() {
	ofstream sphere;
	sphere.open(filename.c_str());

	float x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4; // Vertex
	float xx1, yy1, zz1, xx2, yy2, zz2, xx3, yy3, zz3, xx4, yy4, zz4; // Normals
	float s1 ,t1 , s2, t2, s3, t3, s4, t4; // Textures
	float angleToRad = M_PI / 180.0f;
	float r = radius;
	float textureS = 0.0;
	float textureT = 0.0;
	float textureSIncrement = 1/slices;
	float textureTIncrement = 1/stacks;
	float longitudeIncrement = 360/stacks;
	float latitudeIncrement = 180/slices;

	int totalLines = 18*(360/longitudeIncrement)*(180/latitudeIncrement);
	sphere << totalLines << endl;

	for (float latitude = 0; latitude < 180; latitude += latitudeIncrement){

		for (float longitude = 0; longitude < 360; longitude += longitudeIncrement) {
			//Vertex 1
			z1=r*cos(latitude*angleToRad);
			x1=r*cos(longitude*angleToRad)*sin(latitude*angleToRad);
			y1=r*sin(longitude*angleToRad)*sin(latitude*angleToRad);

			//Normal 1
			zz1=cos(latitude*angleToRad);
			xx1=cos(longitude*angleToRad)*sin(latitude*angleToRad);
			yy1=sin(longitude*angleToRad)*sin(latitude*angleToRad);

			//Texture 1
			s1=longitude/360;
			t1=latitude/180;

			//Vertex 2
			z2=r*cos((latitude+latitudeIncrement)*angleToRad);
			x2=r*cos(longitude*angleToRad)*sin((latitude+latitudeIncrement)*angleToRad);
			y2=r*sin(longitude*angleToRad)*sin((latitude+latitudeIncrement)*angleToRad);

			//Normal 2
			zz2=cos((latitude+latitudeIncrement)*angleToRad);
			xx2=cos(longitude*angleToRad)*sin((latitude+latitudeIncrement)*angleToRad);
			yy2=sin(longitude*angleToRad)*sin((latitude+latitudeIncrement)*angleToRad);

			//Texture 2
			s2=longitude/360;
			t2=(latitude+latitudeIncrement)/180;

			//Vertex 3
			z3=r*cos((latitude)*angleToRad);
			x3=r*cos((longitude+longitudeIncrement)*angleToRad)*sin((latitude)*angleToRad);
			y3=r*sin((longitude+longitudeIncrement)*angleToRad)*sin((latitude)*angleToRad);

			//Normal 3
			zz3=cos((latitude)*angleToRad);
			xx3=cos((longitude+longitudeIncrement)*angleToRad)*sin((latitude)*angleToRad);
			yy3=sin((longitude+longitudeIncrement)*angleToRad)*sin((latitude)*angleToRad);

			//Texture 3
			s3=(longitude+longitudeIncrement)/360;
			t3=(latitude)/180;

			//Vertex 4
			z4=r*cos((latitude+latitudeIncrement)*angleToRad);
			x4=r*cos((longitude+longitudeIncrement)*angleToRad)*sin((latitude+latitudeIncrement)*angleToRad);
			y4=r*sin((longitude+longitudeIncrement)*angleToRad)*sin((latitude+latitudeIncrement)*angleToRad);

			//Normal 4
			zz4=cos((latitude+latitudeIncrement)*angleToRad);
			xx4=cos((longitude+longitudeIncrement)*angleToRad)*sin((latitude+latitudeIncrement)*angleToRad);
			yy4=sin((longitude+longitudeIncrement)*angleToRad)*sin((latitude+latitudeIncrement)*angleToRad);

			//Texture 4
			s4=(longitude+longitudeIncrement)/360;
			t4=(latitude+latitudeIncrement)/180;

			// Filling file.3d
			// First line is Vertex Coordinates (3)
			// Second line is Normal Coordinates (3)
			// Third line is Texture Coordinates (2)

			sphere << x1 << " " << y1 << " " << z1 << endl; // Vertex
			sphere << xx1 << " " << yy1 << " " << zz1 << endl; // Normals
			sphere << s1 << " " << t1 << endl; // Texture

			sphere << x2 << " " << y2 << " " << z2 << endl;
			sphere << xx2 << " " << yy2 << " " << zz2 << endl;
			sphere << s2 << " " << t2 << endl;

			sphere << x3 << " " << y3 << " " << z3 << endl;
			sphere << xx3 << " " << yy3 << " " << zz3 << endl;
			sphere << s3 << " " << t3 << endl;

			sphere << x3 << " " << y3 << " " << z3 << endl;
			sphere << xx3 << " " << yy3 << " " << zz3 << endl;
			sphere << s3 << " " << t3 << endl;

			sphere << x2 << " " << y2 << " " << z2 << endl;
			sphere << xx2 << " " << yy2 << " " << zz2 << endl;
			sphere << s2 << " " << t2 << endl;

			sphere << x4 << " " << y4 << " " << z4 << endl;
			sphere << xx4 << " " << yy4 << " " << zz4 << endl;
			sphere << s4 << " " << t4 << endl;
			}
		}
		sphere.close();
}

void printPlaneFigure3d() {
	ofstream plane;
	plane.open(filename.c_str());

	plane << "6" << endl;

	plane << "0.0 " << "0.0 " << "0.0" << endl; // Vertex
	plane << "0.0 " << "0.0 " << "1.0" << endl; // Normals
	plane << "0.0 " << "0.0" << endl; // Texture

	plane << length << ".0" << " 0.0 " << "0.0" << endl;
	plane << "0.0 " << "0.0 " << "1.0" << endl;
	plane << "1.0 " << "0.0" << endl;

	plane << "0.0 " << width << ".0" << " 0.0" << endl;
	plane << "0.0 " << "0.0 " << "1.0" << endl;
	plane << "0.0 " << "1.0" << endl;

	plane << length << ".0" << " 0.0 " << "0.0" << endl;
	plane << "0.0 " << "0.0 " << "1.0" << endl;
	plane << "1.0 " << "0.0" << endl;

	plane << length << ".0" << " " << width << ".0" << " 0.0" << endl;
	plane << "0.0 " << "0.0 " << "1.0" << endl;
	plane << "1.0 " << "1.0" << endl;

	plane << "0.0 " << width << ".0" << " 0.0" << endl;
	plane << "0.0 " << "0.0 " << "1.0" << endl;
	plane << "0.0 " << "1.0" << endl;

	plane.close();
}

void printBoxFigure3d() {
	ofstream box;
	box.open(filename.c_str());

	box << "36" << endl;

	box << "0 " << "0 " << "0" << endl;
	box << "0 " << "0 " << "1" << endl;
	box << "0 " << "0" << endl;

	box << width << " 0 " << "0" << endl;
	box << "0 " << "0 " << "1" << endl;
	box << "1 " << "0" << endl;

	box << "0 " << height << " 0" << endl;
	box << "0 " << "0 " << "1" << endl;
	box << "0 " << "1" << endl;

	box << width << " 0 " << "0" << endl;
	box << "0 " << "0 " << "1" << endl;
	box << "1 " << "0" << endl;

	box << width << " " << height << " 0" << endl;
	box << "0 " << "0 " << "1" << endl;
	box << "1 " << "1" << endl;

	box << "0 " << height << " 0" << endl;
	box << "0 " << "0 " << "1" << endl;
	box << "0 " << "1" << endl;

	box << width << " 0 " << -length << endl;
	box << "0 " << "0 " << "-1" << endl;
	box << "0 " << "0" << endl;

	box << "0 " << "0 " << -length << endl;
	box << "0 " << "0 " << "-1" << endl;
	box << "1 " << "0" << endl;

	box << "0 " << height << " " << -length << endl;
	box << "0 " << "0 " << "-1" << endl;
	box << "1 " << "1" << endl;

	box << "0 " << height << " " << -length << endl;
	box << "0 " << "0 " << "-1" << endl;
	box << "1 " << "1" << endl;

	box << width << " " << height << " " << -length << endl;
	box << "0 " << "0 " << "-1" << endl;
	box << "0 " << "1" << endl;

	box << width << " 0 " << -length << endl;
	box << "0 " << "0 " << "-1" << endl;
	box << "0 " << "0" << endl;

	box << width << " 0 " << "0" << endl;
	box << "1 " << "0 " << "0" << endl;
	box << "0 " << "0" << endl;

	box << width << " 0 " << -length << endl;
	box << "1 " << "0 " << "0" << endl;
	box << "1 " << "0" << endl;

	box << width << " " << height << " 0" << endl;
	box << "1 " << "0 " << "0" << endl;
	box << "0 " << "1" << endl;

	box << width << " 0 " << -length << endl;
	box << "1 " << "0 " << "0" << endl;
	box << "1 " << "0" << endl;

	box << width << " " << height << " " << -length << endl;
	box << "1 " << "0 " << "0" << endl;
	box << "1 " << "1" << endl;

	box << width << " " << height << " 0" << endl;
	box << "1 " << "0 " << "0" << endl;
	box << "0 " << "1" << endl;

	box << "0 " << "0 " << "0" << endl;
	box << "-1 " << "0 " << "0" << endl;
	box << "1 " << "0" << endl;

	box << "0 " << height << " 0" << endl;
	box << "-1 " << "0 " << "0" << endl;
	box << "1 " << "1" << endl;

	box << "0 " << "0 " << -length << endl;
	box << "-1 " << "0 " << "0" << endl;
	box << "0 " << "0" << endl;

	box << "0 " << "0 " << -length << endl;
	box << "-1 " << "0 " << "0" << endl;
	box << "0 " << "0" << endl;

	box << "0 " << height << " 0" << endl;
	box << "-1 " << "0 " << "0" << endl;
	box << "1 " << "1" << endl;

	box << width << " " << height << " " << -length << endl;
	box << "-1 " << "0 " << "0" << endl;
	box << "0 " << "1" << endl;

	box << "0 " << height << " 0" << endl;
	box << "0 " << "1 " << "0" << endl;
	box << "0 " << "0" << endl;

	box << width << " " << height << " 0" << endl;
	box << "0 " << "1 " << "0" << endl;
	box << "1 " << "0" << endl;

	box << "0 " << height << " " << -length << endl;
	box << "0 " << "1 " << "0" << endl;
	box << "0 " << "1" << endl;

	box << "0 " << height << " " << -length << endl;
	box << "0 " << "1 " << "0" << endl;
	box << "0 " << "1" << endl;

	box << width << " " << height << " 0" << endl;
	box << "0 " << "1 " << "0" << endl;
	box << "1 " << "0" << endl;

	box << width << " " << height << " " << -length << endl;
	box << "0 " << "1 " << "0" << endl;
	box << "1 " << "1" << endl;

	box << "0 " << "0 " << "0" << endl;
	box << "0 " << "-1 " << "0" << endl;
	box << "0 " << "1" << endl;

	box << "0 " << "0 " << -length << endl;
	box << "0 " << "-1 " << "0" << endl;
	box << "0 " << "0" << endl;

	box << width << " 0 " << "0" << endl;
	box << "0 " << "-1 " << "0" << endl;
	box << "1 " << "1" << endl;

	box << "0 " << "0 " << -length << endl;
	box << "0 " << "-1 " << "0" << endl;
	box << "0 " << "0" << endl;

	box << width << " 0 " << -length << endl;
	box << "0 " << "-1 " << "0" << endl;
	box << "1 " << "0" << endl;

	box << width << " 0 " << "0" << endl;
	box << "0 " << "-1 " << "0" << endl;
	box << "1 " << "1" << endl;

	box.close();
}

void printCilinderFigure3d() {
	ofstream cilindro;
	cilindro.open("cilindro.3d");

	float delta = 2.0f * _PI_ / sides;

	for (int i = 0; i < sides; ++i) {
		// top
		// central point
		cilindro << "0.0" << " " << height /2.0f << " " << "0.0" << endl; // Vertex
		cilindro << "0.0" << " " << "1.0" << " " << "0.0" << endl; // Normals
		cilindro << "0.4375" << " " << "0.1875" << endl; // Texture

		cilindro << radius * sin(i * delta) << " " << height /2.0f << " " << radius * cos(i * delta) << endl;
		cilindro << "0.0" << " " << "1.0" << " " << "0.0" << endl;
		cilindro << 0.4375 + 0.1875 * sin(i * delta) << " " << 0.1875 - 0.1875 * cos(i * delta) << endl;

		cilindro << radius * sin( (i+1) * delta) << " " << height /2.0f << " " << radius * cos( (i+1) * delta) << endl;
		cilindro << "0.0" << " " << "1.0" << " " << "0.0" << endl;
		cilindro << 0.4375 + 0.1875 * sin((i + 1) * delta) << " " << 0.1875 - 0.1875 * cos((i + 1) * delta) << endl;

		// body
		// triângulo 1
		cilindro << radius * sin( (i+1) * delta) << " " << height /2.0f << " " << radius * cos( (i+1) * delta) << endl;
		cilindro << sin((i + 1) * delta) << " " << "0.0" << " " << cos((i + 1) * delta) << endl;
		cilindro << (i + 1.0) / sides << " " << "1.0" << endl;

		cilindro << radius * sin( i * delta) << " " << height /2.0f << " " << radius * cos( i * delta) << endl;
		cilindro << sin(i * delta) << " " << "0.0" << " " << cos(i * delta) << endl;
		cilindro << (i + 0.0) / sides << " " << "1.0" << endl;

		cilindro << radius * sin( i * delta) << " " << -height /2.0f << " " << radius * cos( i * delta) << endl;
		cilindro << sin(i * delta) << " " << "0.0" << " " << cos(i * delta) << endl;
		cilindro << (i + 0.0) / sides << " " << "0.375" << endl;

		// triangle 2
		cilindro << radius * sin( (i+1) * delta) << " " << -height /2.0f << " " << radius * cos( (i+1) * delta) << endl;
		cilindro << sin((i + 1) * delta) << " " << "0.0" << " " << cos((i + 1) * delta) << endl;
		cilindro << (i + 1.0) / sides << " " << "0.375" << endl;

		cilindro << radius * sin( (i+1) * delta) << " " << height /2.0f << " " << radius * cos( (i+1) * delta) << endl;
		cilindro << sin((i + 1) * delta) << " " << "0.0" << " " << cos((i + 1) * delta) << endl;
		cilindro << (i + 1.0) / sides << " " << "1.0" << endl;

		cilindro << radius * sin(i * delta) << " " << -height /2.0f << " " << radius * cos(i * delta) << endl;
		cilindro << sin(i * delta) << " " << "0.0" << " " << cos(i * delta) << endl;
		cilindro << (i + 0.0) / sides << " " << "0.375" << endl;

		// base
		// central point
		cilindro << "0.0" << " " << -height /2.0f << " " << "0.0" << endl;
		cilindro << "0.0" << " " << "-1.0" << " " << "0.0" << endl;
		cilindro << "0.8125" << " " << "0.1875" << endl;

		cilindro << radius * sin( (i+1) * delta) << " " << -height /2.0f << " " << radius * cos( (i+1) * delta) << endl;
		cilindro << "0.0" << " " << "-1.0" << " " << "0.0" << endl;
		cilindro << 0.8125 + 0.1875 * sin((i + 1) * delta) << " " << 0.1875 + 0.1875 * cos((i + 1) * delta) << endl;

		cilindro << radius * sin( i * delta) << " " << -height /2.0f << " " << radius * cos( i * delta) << endl;
		cilindro << "0.0" << " " << "-1.0" << " " << "0.0" << endl;
		cilindro << 0.8125 + 0.1875 * sin(i * delta) << " " << 0.1875 + 0.1875 * cos(i * delta) << endl;
	}
	cilindro.close();
}


// Main function
int main(int argc, char **argv) {
	string operationLine, operation, answer, answer2;

	do {
		cout << "Insert your operation:" << endl;
		cout << "Press ctrl + c to exit" << endl;

		getline(cin, operationLine);
		splitted = split(operationLine, ' '); // Save a string on a vector with the substrings separated by a space

		// The function "stof" transforms the content of a string in a float

		if (splitted[0] == "Generate" || splitted[0] == "generate") {
			if (splitted.size() == 1) {
				// Teapot/Comet
				patchFile = readPatchFile("teapot.patch");
				filename = "teapot.3d";
				separeContent();
				preencheVectors(1);
				preencheVectors(2);
				printTeapot();
				cout << "teapot.3d created" << endl;

				// Sun and planets
				filename = "sun.3d";
				radius = SunRadius;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "sun.3d created" << endl;

				filename = "mercury.3d";
				radius = MercuryRadius;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "mercury.3d created" << endl;

				filename = "venus.3d";
				radius = VenusRadius;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "venus.3d created" << endl;

				filename = "earth.3d";
				radius = EarthRadius;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "earth.3d created" << endl;

				filename = "mars.3d";
				radius = MarsRadius;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "mars.3d created" << endl;

				filename = "jupiter.3d";
				radius = JupiterRadius;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "jupiter.3d created" << endl;

				filename = "saturn.3d";
				radius = SaturnRadius;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "saturn.3d created" << endl;

				filename = "uranus.3d";
				radius = UranusRadius;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "uranus.3d created" << endl;

				filename = "neptune.3d";
				radius = NeptuneRadius;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "neptune.3d created" << endl;

				// Moons
				filename = "earthMoon.3d";
				radius = radius_earthMoon;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "earthMoon.3d created" << endl;

				filename = "PhobosMoon.3d";
				radius = radius_PhobosMoon;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "PhobosMoon.3d created" << endl;

				filename = "DeimosMoon.3d";
				radius = radius_DeimosMoon;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "DeimosMoon.3d created" << endl;

				filename = "GanymedeMoon.3d";
				radius = radius_GanymedeMoon;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "GanymedeMoon.3d created" << endl;

				filename = "CallistoMoon.3d";
				radius = radius_CallistoMoon;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "CallistoMoon.3d created" << endl;

				filename = "EuropaMoon.3d";
				radius = radius_EuropaMoon;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "EuropaMoon.3d created" << endl;

				filename = "IoMoon.3d";
				radius = radius_IoMoon;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "IoMoon.3d created" << endl;

				filename = "TitanMoon.3d";
				radius = radius_TitanMoon;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "TitanMoon.3d created" << endl;

				filename = "EnceladusMoon.3d";
				radius = radius_EnceladusMoon;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "EnceladusMoon.3d created" << endl;

				filename = "TitaniaMoon.3d";
				radius = radius_TitaniaMoon;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "TitaniaMoon.3d created" << endl;

				filename = "OberonMoon.3d";
				radius = radius_OberonMoon;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "OberonMoon.3d created" << endl;

				filename = "UmbrielMoon.3d";
				radius = radius_UmbrielMoon;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "UmbrielMoon.3d created" << endl;

				filename = "ArielMoon.3d";
				radius = radius_ArielMoon;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "ArielMoon.3d created" << endl;

				filename = "MirandaMoon.3d";
				radius = radius_MirandaMoon;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "MirandaMoon.3d created" << endl;

				filename = "TritonMoon.3d";
				radius = radius_TritonMoon;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "TritonMoon.3d created" << endl;

				filename = "ProteusMoon.3d";
				radius = radius_ProteusMoon;
				slices = 30;
				stacks = 30;
				printSphere3d();
				cout << "ProteusMoon.3d created" << endl;

				filename = "planeFigure.3d";
				width = 10;
				length = 10;
				printPlaneFigure3d();
				cout << "planeFigure.3d created" << endl;

				filename = "boxFigure.3d";
				width = 10;
				length = 10;
				height = 10;
				printBoxFigure3d();
				cout << "boxFigure.3d created" << endl;

				filename = "cilinderFigure.3d";
				height = 2;
				radius = 1;
				sides = 16;
				printCilinderFigure3d();
				cout << "cilinderFigure.3d created" << endl;

				do {
					cout << "Do you wish adding more Planets, Moons? Type yes or no" << endl;
					getline(cin, answer);
					if (answer == "yes" || answer == "YES") {
						cout << "Type filename.3d please" << endl;
						getline(cin, filename);
						cout << "Type radius please" << endl;
						getline(cin, answer2);
						radius = stof(answer2);
						cout << "Type slices please" << endl;
						getline(cin, answer2);
						slices = stof(answer2);
						cout << "Type stacks please" << endl;
						getline(cin, answer2);
						stacks = stof(answer2);
						printSphere3d();
						cout << filename << " created" << endl;
					}
				} while (answer == "yes" || answer == "YES");
				cout << "GoodBye!!" << endl;
			}
		}
		else { cout << "Number of arguments wrong!" << endl; }

	} while (true);

	return 1;
}
