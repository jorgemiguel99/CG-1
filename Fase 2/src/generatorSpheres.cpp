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
//#include <windows.h>
//#include <GL/glut.h>
#include <math.h>
#include <GLUT/glut.h> //-- MAC

using namespace std;

// Global variable for handling files
string filename;

// Dimensions of the figures
float radius, slices, stacks;

// Global variable store 3d file read
vector<string> file3dRead;

// Global variable process input
vector<string> splitted;

// Operation name has 100 char at max
#define MAX 100

// Returns the number to be used on the switch in main function; splitted[1] = operation name
int checkOP(string str) {
    if (str == "Sphere" || str == "sphere") return 1;
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
		case 1: // Sphere -> receives the radius, the slices and the stacks
			if (splitted.size() == 6) {
				filename = splitted[5];
				radius = stof(splitted[2]);
				slices = stof(splitted[3]);
				stacks = stof(splitted[4]);
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
			case 1: printSphere3d(); break;
			}
			cout << "File created" << endl;
		}
	}
	else cout << "Try again with this usage: generator operationName inputs filename" << endl;

}while (true);


	return 1;
}
