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

using namespace std;

// Global variable for handling files
string filename;

// Dimensions of the figures
float radius, slices, stacks;

// Global variable process input
vector<string> splitted;


// Declare radii of all 8 planets & Sun
float SunRadius         = 8.0;
float MercuryRadius     = 2.0;
float VenusRadius       = 3.0;
float EarthRadius       = 3.0;
float MarsRadius        = 4.0;
float JupiterRadius     = 8.0;
float UranusRadius      = 7.0;
float SaturnRadius      = 9.0;
float NeptuneRadius     = 7.0;


// Declare radii of all 16 moons which is 1/3 of correspondent planet
float radius_earthMoon      = EarthRadius/3;
float radius_PhobosMoon     = MarsRadius/3;
float radius_DeimosMoon     = MarsRadius/3;
float radius_GanymedeMoon   = JupiterRadius/3;
float radius_CallistoMoon   = JupiterRadius/3;
float radius_EuropaMoon     = JupiterRadius/3;
float radius_IoMoon         = JupiterRadius/3;
float radius_TitanMoon      = SaturnRadius/3;
float radius_EnceladusMoon  = SaturnRadius/3;
float radius_TitaniaMoon    = UranusRadius/3;
float radius_OberonMoon     = UranusRadius/3;
float radius_UmbrielMoon    = UranusRadius/3;
float radius_ArielMoon      = UranusRadius/3;
float radius_MirandaMoon    = UranusRadius/3;
float radius_TritonMoon     = NeptuneRadius/3;
float radius_ProteusMoon    = NeptuneRadius/3;

// Global variable to save patch file content
vector<string> patchFile;

// Global variable to save number of patches
int numberOfPatches=0;

// Global variable to save number of control points
int numberOfControlPoints=0;

// Global variable to save separated content patches
vector<string> patches;

// Global variable to save separated content control points
vector<string> controlPoints;

// Global variable to save patches floats
vector< vector<float> > patchesValues;

// Global variable to save control points floats
vector< vector<float> > controlPointsValues;

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
    numberOfPatches        = stoi(vecx[0]); // Number of total patches
    numberOfControlPoints  = stoi(vecx[numberOfPatches+1]); // Number of total control points
    return vecx;
}

void separeContent(){
    string aux;
    int i,j;
    for(i=1;i<=numberOfPatches;i++){
        aux="";
        aux=patchFile[i];
        patches.push_back(aux);
    }
    for(j=i+1;j<=numberOfControlPoints+numberOfPatches+1;j++){
        aux="";
        aux=patchFile[j];
        controlPoints.push_back(aux);
    }
}

void printContent(){
    int c=0;
    cout << "#Patches => " << patches.size() << endl;
    while(c<patches.size()){
        cout << "Patch #" << c << " => " << patches[c] << endl;
        c++;
    }
    cout << "#Control Points => " << controlPoints.size() << endl;
    c=0;
    while(c<controlPoints.size()){
        cout << "Control Point #" << c << " => " << controlPoints[c] << endl;
        c++;
    }
}

string parsing(string aux){
    string auxParsed="";
    int auxSize = aux.size();
    //cout << "Size of String input: " << auxSize << endl;
    int count=0;
    while(count<=auxSize){
        if(aux[count]!=','){
            auxParsed+=aux[count];
        }
        count++;
    }
    //cout << auxParsed << endl;
    return auxParsed;
}

void adicionaValues(vector<float> aux, int flag){
    switch (flag) {
        case 1:
            patchesValues.push_back(aux);
            break;
        case 2:
            controlPointsValues.push_back(aux);
            break;
    }
}

vector<float> values;

void preencheVectors(int flag){
    if(flag==1){ //patches
        for (int j = 0; j < patches.size();j++){
            string valuesParsed = parsing(patches[j]);
            istringstream is_Value(valuesParsed);
            values.erase (values.begin(),values.end());
            copy(istream_iterator<float>(is_Value), istream_iterator<float>(), back_inserter(values));
            adicionaValues(values, 1);
        }
    }else if(flag==2){ // controlPoints
        for (int j = 0; j < controlPoints.size();j++){
            string valuesParsed = parsing(controlPoints[j]);
            istringstream is_Value(valuesParsed);
            values.erase (values.begin(),values.end());
            copy(istream_iterator<float>(is_Value), istream_iterator<float>(), back_inserter(values));
            adicionaValues(values, 2);
        }
    }
}

void printFloatVector(){
    vector<float> aux;
    for (int j = 0; j < patchesValues.size();j++){
        aux.erase(aux.begin(),aux.end());
        aux=patchesValues[j];
        for(int k=0;k<aux.size();k++){
            cout << "patchesValues #" << k << " => " << aux[k] << endl;
        }
    }
    for (int j=0;j<controlPointsValues.size();j++){
        aux.erase(aux.begin(),aux.end());
        aux=controlPointsValues[j];
        for(int k=0;k<aux.size();k++){
            cout << "controlPointsValues #" << k << " => " << aux[k] << endl;
        }
    }
}

void printVector(vector<string> vec){
    int i;
    for(i=0;i<vec.size();i++){
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
	string operationLine, operation, answer, answer2;

	do{
	cout << "Insert your operation:" << endl;
	cout << "Press ctrl + c to exit" << endl;

	getline(cin, operationLine);
	splitted = split(operationLine, ' '); // Save a string on a vector with the substrings separated by a space

	// The function "stof" transforms the content of a string in a float
  // generate

	if (splitted[0] == "Generate" || splitted[0] == "generate") {
			if (splitted.size() == 1) {
          //TODO CRIAR O NOVO MODELO COM BASE NA PATCH LIDA
          patchFile=readPatchFile("teapot.patch");
          //cout << "Number of Patches read is " << numberOfPatches << endl;
          //cout << "Number of Control Points read is " << numberOfControlPoints << endl;
          //printVector(patchFile);
          separeContent();
          //printContent();
          //parsing("0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15");
          preencheVectors(1);
          preencheVectors(2);
          //cout << "Floats vector " << patchesValues.size() << endl;
          //cout << "Floats vector " << controlPointsValues.size() << endl;
          //printFloatVector();
                
          //Sun && Planets
          filename = "sun.3d";
          radius=SunRadius;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "sun.3d created" << endl;

          filename = "mercury.3d";
          radius=MercuryRadius;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "mercury.3d created" << endl;

          filename = "venus.3d";
          radius=VenusRadius;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "venus.3d created" << endl;

          filename = "earth.3d";
          radius=EarthRadius;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "earth.3d created" << endl;

          filename = "mars.3d";
          radius=MarsRadius;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "mars.3d created" << endl;

          filename = "jupiter.3d";
          radius=JupiterRadius;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "jupiter.3d created" << endl;

          filename = "saturn.3d";
          radius=SaturnRadius;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "saturn.3d created" << endl;

          filename = "uranus.3d";
          radius=UranusRadius;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "uranus.3d created" << endl;

          filename = "neptune.3d";
          radius=NeptuneRadius;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "neptune.3d created" << endl;

          //Moons 1/3 radii of correspondent planet
          filename = "earthMoon.3d";
          radius=radius_earthMoon;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "earthMoon.3d created" << endl;

          filename = "PhobosMoon.3d";
          radius=radius_PhobosMoon;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "PhobosMoon.3d created" << endl;

          filename = "DeimosMoon.3d";
          radius=radius_DeimosMoon;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "DeimosMoon.3d created" << endl;

          filename = "GanymedeMoon.3d";
          radius=radius_GanymedeMoon;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "GanymedeMoon.3d created" << endl;

          filename = "CallistoMoon.3d";
          radius=radius_CallistoMoon;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "CallistoMoon.3d created" << endl;

          filename = "EuropaMoon.3d";
          radius=radius_EuropaMoon;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "EuropaMoon.3d created" << endl;

          filename = "IoMoon.3d";
          radius=radius_IoMoon;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "IoMoon.3d created" << endl;

          filename = "TitanMoon.3d";
          radius=radius_TitanMoon;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "TitanMoon.3d created" << endl;

          filename = "EnceladusMoon.3d";
          radius=radius_EnceladusMoon;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "EnceladusMoon.3d created" << endl;

          filename = "TitaniaMoon.3d";
          radius=radius_TitaniaMoon;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "TitaniaMoon.3d created" << endl;

          filename = "OberonMoon.3d";
          radius=radius_OberonMoon;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "OberonMoon.3d created" << endl;

          filename = "UmbrielMoon.3d";
          radius=radius_UmbrielMoon;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "UmbrielMoon.3d created" << endl;

          filename = "ArielMoon.3d";
          radius=radius_ArielMoon;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "ArielMoon.3d created" << endl;

          filename = "MirandaMoon.3d";
          radius=radius_MirandaMoon;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "MirandaMoon.3d created" << endl;

          filename = "TritonMoon.3d";
          radius=radius_TritonMoon;
          slices=30;
          stacks=30;
          printSphere3d();
    			cout << "TritonMoon.3d created" << endl;

          filename = "ProteusMoon.3d";
          radius=radius_ProteusMoon;
          slices=30;
          stacks=30;
          printSphere3d();
          cout << "ProteusMoon.3d created" << endl;

          do{
          cout << "Do you wish adding more Planets or Moons? Type yes or no" << endl;
          getline(cin, answer);
          if(answer == "yes" || answer == "YES"){
            cout << "Type filename.3d please" << endl;
            getline(cin, filename);
            cout << "Type radius please" << endl;
            getline(cin, answer2);
            radius=stof(answer2);
            cout << "Type slices please" << endl;
            getline(cin, answer2);
            slices=stof(answer2);
            cout << "Type stacks please" << endl;
            getline(cin, answer2);
            stacks=stof(answer2);
            printSphere3d();
            cout << filename << " created" << endl;
          }
        }while(answer == "yes" || answer == "YES");
        cout << "GoodBye!!" << endl;
			}
  }
  else {cout << "Number of arguments wrong!" << endl;}

}while (true);

 return 1;
}
