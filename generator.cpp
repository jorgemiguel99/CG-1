
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

//operation name has 100 char at max
#define MAX 100

//loads the coordenates to make a box
void box(float X, float Y, float Z) {
	cout << "This is the Drawing of a Box!!!" << endl;
}

//loads the coordenates to make a plane with square == 2 triangles => 4 points => 8 coordenates X,Y
void plane(float Xa, float Ya, float Xb, float Yb, float Xc, float Yc, float Xd, float Yd) {
	//put the formulas to draw the plane
	cout << "This is the Drawing of a Plane!!!" << endl;
}

//loads the inputs in order to produce a sphere
void sphere(float radiusV, float sliceV, float stackV) {
	//put the formulas to draw the sphere
	cout << "This is the Drawing of a Sphere!!!" << endl;
}

//loads the inputs in order to produce a cone
void cone(float bottomRadius, float heightC, float slicesC, float stacksC) {
	cout << "This is the Drawing of a Cone!!!" << endl;
}

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

int main(int argc, char **argv) {

	string operationLine, operation, filename, fileExtension;
	fileExtension = ".3d";
	cout << "Insert your operation:" << endl;
	getline(cin, operationLine);
	//guarda a string no vector com as substrings separadas por um espaço
	vector<string> splitted = split(operationLine, ' ');

	 //Percorrer string operationLine
	/* Teste!!!
	 int i;
	 cout << "Vector Size = " << splitted.size() << endl;
	 for (i = 0; i < splitted.size(); i++)
		cout << "Vector Position-" << i << "===>" << splitted[i] << endl;

	 float aux = stof(splitted[1]);
	 cout << "StringToFloat ---->" << aux << endl;
	 */

	//stof is a function that transforms the content of string into float
	 
	switch (checkOP(splitted[0])) {

	case 1:		  if (splitted.size() == 10) {
					plane(stof(splitted[1]), stof(splitted[2]), stof(splitted[3]), stof(splitted[4]), stof(splitted[5]), stof(splitted[6]), stof(splitted[7]), stof(splitted[8]));
					filename = splitted[9] + fileExtension;
				  }
				  break;
	case 2:       if (splitted.size() == 5) {
					box(stof(splitted[1]), stof(splitted[2]), stof(splitted[3]));
					filename = splitted[4] + fileExtension;
				  }
				  break;
	case 3:       if (splitted.size() == 5) {
					sphere(stof(splitted[1]), stof(splitted[2]), stof(splitted[3]));
					filename = splitted[4] + fileExtension;
				  }
				  break;
	case 4:       if (splitted.size() == 6) {
					cone(stof(splitted[1]), stof(splitted[2]), stof(splitted[3]), stof(splitted[4]));
					filename = splitted[5] + fileExtension;
				  }
				  break;
	default:      cout << "Not an operation!\n" << endl;
				  filename = "invalid.3d";
				  break;
	}
	return 1;
}
