
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <fstream>

#include <string>


using namespace std;

// Declare radii of all 8 planets orbits

const int N = 8 + 6;

float PlanetsRadiusOrbits[8] = { 30,50,90,110,120,140,160,180 };
float MoonsRadiusOrbits[6] = {30,25,20,30,25,20 };
float Orbits[N] = { 30,50,90,110,120,140,160,180,30,25,20,30,25,20 };
string filename[N] = {"mercury" ,"venus","earth","mars","jupiter","saturn","uranus","neptune"
, "earthMoon","PhobosMoon", "CallistoMoon","TitanMoon","OberonMoon","TritonMoon" };

#define POINTS 8

void printXML(float p[N][POINTS][3]) {
	ofstream outputFile;
	outputFile.open("GenerateXML.txt");

	for (int i = 0; i < N; i++) {
		outputFile << "Modelo " << filename[i] << "\n";
		outputFile << "    <translate time=0>\n";
			for (int j = 0; j < POINTS; j++)
				outputFile << "	      <point X=" << p[i][j][0] << " Y=" << p[i][j][1] << " Z=" << p[i][j][2] << " />\n";
		outputFile << "    </translate>\n\n";
	}

	outputFile.close();
}

int main() {

	float p[N][POINTS][3];
	int i;
	for ( i = 0; i < N ; i++) {
	p[i][0][0] = (3.0 / 2)*Orbits[i];
	p[i][0][1] = 0;
	p[i][0][2] = 0;

	p[i][1][0] = (2.2 / 2)*Orbits[i];
	p[i][1][1] = 0;
	p[i][1][2] = (-1.5 / 2)*Orbits[i];

	p[i][2][0] = 0;
	p[i][2][1] = 0;
	p[i][2][2] = (-2.0 / 2)*Orbits[i];

	p[i][3][0] = (-2.2 / 2)*Orbits[i];
	p[i][3][1] = 0;
	p[i][3][2] = (-1.5 / 2)*Orbits[i];

	p[i][4][0] = (-3.0 / 2)*Orbits[i];
	p[i][4][1] = 0;
	p[i][4][2] = 0;

	p[i][5][0] = (-2.2 / 2)*Orbits[i];
	p[i][5][1] = 0;
	p[i][5][2] = (1.5 / 2)*Orbits[i];

	p[i][6][0] = 0;
	p[i][6][1] = 0;
	p[i][6][2] = (2.0 / 2)*Orbits[i];

	p[i][7][0] = (2.2 / 2)*Orbits[i];
	p[i][7][1] = 0;
	p[i][7][2] = (1.5 / 2)*Orbits[i];

	}
	printXML(p);
}

