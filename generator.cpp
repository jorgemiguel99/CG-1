#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <sstream>
#include <windows.h>
using namespace std;

string desktop = "C:\\Users\\zecar\\Desktop\\";
string filename, fileExtension = ".3d";

//operation name has 100 char at max
#define MAX 100

//loads the coordenates to make a box
void box(float X, float Y, float Z) {
	cout << "This is the Drawing of a Box with " << X << ", " << Y << " and " << Z << " as inputs in file: " << filename << endl;
}

//loads the coordenates to make a plane with square == 2 triangles => 4 points => 8 coordenates X,Y
void plane(float Xa, float Ya, float Xb, float Yb, float Xc, float Yc, float Xd, float Yd) {
	//put the formulas to draw the plane
	cout << "This is the Drawing of a Plane with "
		<< "(" << Xa << "," << Ya << ")" << ";"
		<< "(" << Xb << "," << Yb << ")" << ";"
		<< "(" << Xc << "," << Yc << ")" << " & "
		<< "(" << Xd << "," << Yd << ")" << "in file: "
		<< filename
		<< endl;
}

//loads the inputs in order to produce a sphere
void sphere(float radiusV, float sliceV, float stackV) {
	//put the formulas to draw the sphere
	cout << "This is the Drawing of a Sphere with " << radiusV << ", " << sliceV << " and " << stackV << " as inputs in file: " << filename << endl;
}

//loads the inputs in order to produce a cone
void cone(float bottomRadius, float heightC, float slicesC, float stacksC) {
	cout << "This is the Drawing of a Cone with " << bottomRadius << ", " << heightC << ", " << slicesC << " and " << stacksC << " as inputs in file: " << filename << endl;
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
	fileXML << "<scene>\n\t<model file = " << fileNm << " />\n</scene>" << endl;
	fileXML.close();

	ofstream desktopXMLFile(desktop + aux);
	desktopXMLFile << "<scene>\n\t<model file = " << fileNm << " />\n</scene>" << endl;

	/* write xml into .3d file create before
	ofstream file;
	file.open(fileNm.c_str());
	file << "<scene>\n\t<model file = " << fileNm << " />\n</scene>" << endl;
	file.close();

	ofstream desktopFile(desktop + filename);
	desktopFile << "<scene>\n\t<model file = " << fileNm << " />\n</scene>" << endl;
	*/
}

int main(int argc, char **argv) {

	string operationLine, operation;

	cout << "Insert your operation:" << endl;
	getline(cin, operationLine);
	//guarda a string no vector com as substrings separadas por um espaηo
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
	if (splitted[0] == "Generator" || splitted[0] == "generator") {
		switch (checkOP(splitted[1])) {

		case 1:		  if (splitted.size() == 11) {
			                filename = splitted[10] + fileExtension;
							plane(stof(splitted[2]), stof(splitted[3]), stof(splitted[4]), stof(splitted[5]), stof(splitted[6]), stof(splitted[7]), stof(splitted[8]), stof(splitted[9]));
					  }
					  break;
		case 2:       if (splitted.size() == 6) {
			                filename = splitted[5] + fileExtension;
							box(stof(splitted[2]), stof(splitted[3]), stof(splitted[4]));
					  }
					  break;
		case 3:       if (splitted.size() == 6) {
			                filename = splitted[5] + fileExtension;
							sphere(stof(splitted[2]), stof(splitted[3]), stof(splitted[4]));
					  }
					  break;
		case 4:       if (splitted.size() == 7) {
			                filename = splitted[6] + fileExtension;
							cone(stof(splitted[2]), stof(splitted[3]), stof(splitted[4]), stof(splitted[5]));
					  }
					  break;
		default:      cout << "Not an operation!\n" << endl;
					  filename = "invalid.3d";
					  break;
		}
	}
	else cout << "Try again with this usage: generator operationName inputs filename" << endl;

	create3dFile(filename);
	writeXMLFile(filename);
	return 1;
}
