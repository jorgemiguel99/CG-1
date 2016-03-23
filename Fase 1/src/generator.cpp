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

// Dimensions of the figures
float length, width, height, radius, slices, stacks;

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

	int sphereVertex = (stacks - 2)*slices * 6 + 2 * slices * 3;
	sphere << sphereVertex << endl; // Total number of vertices

	int i, j;
	float r = radius;
	float inicialPolar = - M_PI / 2;

	for (i = 0; i < stacks; i++) {
		float polar = inicialPolar + i * (M_PI / stacks);
		float polar2 = inicialPolar + (i + 1) * (M_PI / stacks);

		for (j = 0; j < slices; j++) {

			float longitude = j * (2 * M_PI / slices);
			float longitude2 = (j + 1) * (2 * M_PI / slices);

			float z1 = r * cos(polar) * cos(longitude);
			float x1 = r * cos(polar) * sin(longitude);
			float y1 = r * sin(polar);

			float z2 = r * cos(polar2) * cos(longitude);
			float x2 = r * cos(polar2) * sin(longitude);
			float y2 = r * sin(polar2);

			float z3 = r * cos(polar) * cos(longitude2);
			float x3 = r * cos(polar) * sin(longitude2);
			float y3 = r * sin(polar);

			float z4 = r * cos(polar2) * cos(longitude2);
			float x4 = r * cos(polar2) * sin(longitude2);
			float y4 = r * sin(polar2);

			if (i == 0) {
				sphere << x3 << " " << y3 << " " << z3 << endl;
				sphere << x4 << " " << y4 << " " << z4 << endl;
				sphere << x2 << " " << y2 << " " << z2 << endl;
			}
			else if (i == stacks - 1) {
				sphere << x1 << " " << y1 << " " << z1 << endl;
				sphere << x3 << " " << y3 << " " << z3 << endl;
				sphere << x2 << " " << y2 << " " << z2 << endl;
			}
			else {
				sphere << x3 << " " << y3 << " " << z3 << endl;
				sphere << x2 << " " << y2 << " " << z2 << endl;
				sphere << x1 << " " << y1 << " " << z1 << endl;

				sphere << x3 << " " << y3 << " " << z3 << endl;
				sphere << x4 << " " << y4 << " " << z4 << endl;
				sphere << x2 << " " << y2 << " " << z2 << endl;
			}
		}
	}
	sphere.close();
}

void printCone3d() {
	ofstream cone;
	cone.open(filename.c_str());

	// pontos base + pontos lados + pontos do topo (slice so com triangulos)
	int coneVertex = slices * 3 + (stacks - 1)*slices * 6 + slices * 3;
	cone << coneVertex << endl; // Total number of vertices

	float ratio = height / radius;

	for (int i = 0; i < slices; i++) {

		float alpha = i * (2 * M_PI) / slices;
		float beta = (2 * M_PI) / slices;

		// Base
		float x1 = radius*sin(alpha);
		float y1 = 0;
		float z1 = radius*cos(alpha);

		float x2 = radius*sin(alpha + beta);
		float y2 = 0;
		float z2 = radius*cos(alpha + beta);

		// Base
		cone << "0.0 0.0 0.0" << endl;
		cone << x2 << " " << y2 << " " << z2 << endl;
		cone << x1 << " " << y1 << " " << z1 << endl;

		float radiusBot = radius;
		float radiusTop;

		for (int j = 0; j < stacks; j++, radiusBot = radiusTop) {
			// Sides
			float y = j * (height / stacks);
			float yn = (j + 1) * (height / stacks);

			float h = height - yn; // altura do triangulo superior
			radiusTop = h / ratio; // radius do triangulo superior

								   /* Points

								   3____4
								   |    |
								   |____|
								   1    2
								   */

			float x1 = radiusBot*sin(alpha);
			float y1 = y;
			float z1 = radiusBot*cos(alpha);

			float x2 = radiusBot*sin(alpha + beta);
			float y2 = y;
			float z2 = radiusBot*cos(alpha + beta);


			float x3 = radiusTop*sin(alpha);
			float y3 = yn;
			float z3 = radiusTop*cos(alpha);

			float x4 = radiusTop*sin(alpha + beta);
			float y4 = yn;
			float z4 = radiusTop*cos(alpha + beta);

			if (j == stacks - 1) { // top slice
				cone << x3 << " " << y3 << " " << z3 << endl;
				cone << x1 << " " << y1 << " " << z1 << endl;
				cone << x2 << " " << y2 << " " << z2 << endl;

			}
			else {
				// T: 3 - 1 - 2
				cone << x3 << " " << y3 << " " << z3 << endl;
				cone << x1 << " " << y1 << " " << z1 << endl;
				cone << x2 << " " << y2 << " " << z2 << endl;

				// T: 3 - 2 - 4
				cone << x3 << " " << y3 << " " << z3 << endl;
				cone << x2 << " " << y2 << " " << z2 << endl;
				cone << x4 << " " << y4 << " " << z4 << endl;
			}
		}

	}
	cone.close();
}

// Main function
int main(int argc, char **argv) {
	string operationLine, operation;

	do{

	int wrong = 0;
	cout << "Insert your operation:" << endl;
	cout << "Press ctrl + c to exit" << endl;

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
			else { cout << "Number of arguments wrong!" << endl; wrong = 1; }
			break;
		case 2: // Box -> receives the length, the width and the height
			if (splitted.size() == 6) {
				filename = splitted[5];
				length = stof(splitted[2]);
				width = stof(splitted[3]);
				height = stof(splitted[4]);
			}
			else { cout << "Number of arguments wrong!" << endl; wrong = 1; }
			break;
		case 3: // Sphere -> receives the radius, the slices and the stacks
			if (splitted.size() == 6) {
				filename = splitted[5];
				radius = stof(splitted[2]);
				slices = stof(splitted[3]);
				stacks = stof(splitted[4]);
			}
			else { cout << "Number of arguments wrong!" << endl; wrong = 1; }
			break;
		case 4: // Cone -> receives the base radius, the height, the slices and the stacks
			if (splitted.size() == 7) {
				filename = splitted[6];
				radius = stof(splitted[2]);
				height = stof(splitted[3]);
				slices = stof(splitted[4]);
				stacks = stof(splitted[5]);
			}
			else { cout << "Number of arguments wrong!" << endl; wrong = 1; }
			break;
		default:
			wrong = 1;
			cout << "Not an operation!\n" << endl;
			filename = "invalid.3d";
			break;
		}

		if (wrong == 0) {
			switch (checkOP(splitted[1])) {
			case 1: printPlane3d(); break;
			case 2: printBox3d(); break;
			case 3: printSphere3d(); break;
			case 4: printCone3d(); break;
			}
			cout << "File created" << endl;
		}
	}
	else cout << "Try again with this usage: generator operationName inputs filename" << endl;

}while (true);


	return 1;
}
