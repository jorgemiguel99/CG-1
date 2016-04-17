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

// Global variable process input
vector<string> splitted;

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

void printVector(vector<string> vec){
    int i;
    for(i=0;i<vec.size();i++){
        cout << "#" << i << " => " << vec[i] << endl;
    }
}

// Main function
int main(int argc, char **argv) {
	string operationLine, operation, answer, answer2;

	do{
	cout << "Insert your operation:" << endl;
	cout << "Press ctrl + c to exit" << endl;

	getline(cin, operationLine);
	splitted = split(operationLine, ' '); // Save a string on a vector with the substrings separated by a space

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
            }
    }
    else {cout << "Number of arguments wrong!" << endl;}

    }while (true);
 return 1;
}
