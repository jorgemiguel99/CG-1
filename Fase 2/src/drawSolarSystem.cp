#include <GLUT/glut.h>      //-- MAC
#include <OpenGL/glu.h>    //-- MAC
#include <OpenGL/gl.h>    //-- MAC
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
#include <vector>
#include <sstream>
#include <string>
#include <math.h>
// #include <windows.h>
// #include <GL/glut.h>
// #include <GL/glu.h>
// #include <GL/gl.h>
//#include "tinyxml\tinyxml.h"	// The place where the file "tinyxml.h" is located
#include </usr/local/Cellar/tinyxml/2.6.2/include/tinyxml.h> //-- MAC

#define _USE_MATH_DEFINES

using namespace std;


// Declared here and implemented after main in order to better organization of the code
void init();
void changeSize(int, int);
void renderScene(void);
void idleScene(void);
void keyPressed(unsigned char, int, int);
void newMenu (int id_op);
void keyboardExtra(int key_code, int x, int y);

// Global variable that allows to know if XML file was found and read
int read=0;

//Global Variables to Transformations
float px = 0.0, py = 200.0, pz = 300.0;

float rotate_y = 0;
float rotate_x = 0;
float rotate_z = 0;

float translate_y = 0;
float translate_x = 0;
float translate_z = 0;

// Declare radii of all 8 planets & Sun
float SunRadius         = 8.0;
float MercuryRadius     = 2.0;
float VenusRadius       = 3.0;
float TerraRadius       = 3.0;
float MarteRadius       = 4.0;
float JupiterRadius     = 8.0;
float UranoRadius       = 7.0;
float SaturnoRadius     = 9.0;
float NeptunoRadius     = 7.0;


// Declare radii of all 8 planets orbits
float MercuryRadiusOrbit     = 30.0;
float VenusRadiusOrbit       = 50.0;
float TerraRadiusOrbit       = 70.0;
float MarteRadiusOrbit       = 95.0;
float JupiterRadiusOrbit     = 110.0;
float UranoRadiusOrbit       = 150.0;
float SaturnoRadiusOrbit     = 200.0;
float NeptunoRadiusOrbit     = 250.0;

// Global variables that allows drawing circles os planets around the sun
float alphaM            = 0.0;
float alphaIncM         = (2 * M_PI)/10.0;

float alphaV            = 0.0;
float alphaIncV         = (2 * M_PI)/10.0;

float alphaT            = 0.0;
float alphaIncT         = (2 * M_PI)/10.0;

float alphaMT           = 0.0;
float alphaIncMT        = (2 * M_PI)/10.0;

float alphaJ            = 0.0;
float alphaIncJ         = (2 * M_PI)/10.0;

float alphaS            = 0.0;
float alphaIncS         = (2 * M_PI)/10.0;

float alphaU            = 0.0;
float alphaIncU         = (2 * M_PI)/10.0;

float alphaN            = 0.0;
float alphaIncN         = (2 * M_PI)/10.0;

// Global variables that allows controlling the xml content
int num_planets=0,num_moons=0;

// Global variables that have the xml content of the sun
int sun_X,sun_Y,sun_Z,sun_axisX,sun_axisY,sun_axisZ;
float sun_angle;
string sun_filename;

// Global variables that have the xml content of Mercury
int mercury_X,mercury_Y,mercury_Z,mercury_axisX,mercury_axisY,mercury_axisZ;
float mercury_angle;
string mercury_filename;

// Global variables that have the xml content of Venus
int venus_X,venus_Y,venus_Z,venus_axisX,venus_axisY,venus_axisZ;
float venus_angle;
string venus_filename;

// Global variables that have the xml content of Earth
int earth_X,earth_Y,earth_Z,earth_axisX,earth_axisY,earth_axisZ;
float earth_angle;
string earth_filename,earth_moon_filename;

// Global variables that have the xml content of Mars
int mars_X,mars_Y,mars_Z,mars_axisX,mars_axisY,mars_axisZ;
float mars_angle;
string mars_filename,mars_moon1_filename,mars_moon2_filename;

// Global variables that have the xml content of Jupiter
int jupiter_X,jupiter_Y,jupiter_Z,jupiter_axisX,jupiter_axisY,jupiter_axisZ;
float jupiter_angle;
string jupiter_filename,jupiter_moon1_filename,jupiter_moon2_filename,jupiter_moon3_filename,jupiter_moon4_filename;

// Global variables that have the xml content of Saturn
int saturn_X,saturn_Y,saturn_Z,saturn_axisX,saturn_axisY,saturn_axisZ;
float saturn_angle;
string saturn_filename,saturn_moon1_filename,saturn_moon2_filename;

// Global variables that have the xml content of Uranus
int uranus_X,uranus_Y,uranus_Z,uranus_axisX,uranus_axisY,uranus_axisZ;
float uranus_angle;
string uranus_filename,uranus_moon1_filename,uranus_moon2_filename,uranus_moon3_filename,uranus_moon4_filename,uranus_moon5_filename;

// Global variables that have the xml content of Neptune
int neptune_X,neptune_Y,neptune_Z,neptune_axisX,neptune_axisY,neptune_axisZ;
float neptune_angle;
string neptune_filename,neptune_moon1_filename,neptune_moon2_filename;

// Global variable store 3d planets read
vector<string> file3dRead_Sun;
vector<string> file3dRead_Mercury;
vector<string> file3dRead_Venus;
vector<string> file3dRead_Earth;
vector<string> file3dRead_Mars;
vector<string> file3dRead_Jupiter;
vector<string> file3dRead_Uranus;
vector<string> file3dRead_Saturn;
vector<string> file3dRead_Neptune;

// Global variable store 3d moons read

vector<string> file3dRead_earthMoon;
vector<string> file3dRead_PhobosMoon;
vector<string> file3dRead_DeimosMoon;
vector<string> file3dRead_GanymedeMoon;
vector<string> file3dRead_CallistoMoon;
vector<string> file3dRead_EuropaMoon;
vector<string> file3dRead_IoMoon;
vector<string> file3dRead_TitanMoon;
vector<string> file3dRead_EnceladusMoon;
vector<string> file3dRead_TitaniaMoon;
vector<string> file3dRead_OberonMoon;
vector<string> file3dRead_UmbrielMoon;
vector<string> file3dRead_ArielMoon;
vector<string> file3dRead_MirandaMoon;
vector<string> file3dRead_TritonMoon;
vector<string> file3dRead_ProteusMoon;

// Global variable process input
vector<string> splitted;

// Global variable with 3d vertex
vector<float> vrtx_sun;
vector<float> vrtx_Mercury;
vector<float> vrtx_Venus;
vector<float> vrtx_Earth;
vector<float> vrtx_Mars;
vector<float> vrtx_Jupiter;
vector<float> vrtx_Saturn;
vector<float> vrtx_Uranus;
vector<float> vrtx_Neptune;

vector<float> vrtx_earthMoon;
vector<float> vrtx_PhobosMoon;
vector<float> vrtx_DeimosMoon;
vector<float> vrtx_GanymedeMoon;
vector<float> vrtx_CallistoMoon;
vector<float> vrtx_EuropaMoon;
vector<float> vrtx_IoMoon;
vector<float> vrtx_TitanMoon;
vector<float> vrtx_EnceladusMoon;
vector<float> vrtx_TitaniaMoon;
vector<float> vrtx_OberonMoon;
vector<float> vrtx_UmbrielMoon;
vector<float> vrtx_ArielMoon;
vector<float> vrtx_MirandaMoon;
vector<float> vrtx_TritonMoon;
vector<float> vrtx_ProteusMoon;

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

// Reads the vertices of figure.3d files into a vector
vector<string> read3d(string figure) {
	vector<string> vecx;
	ifstream fileTXT(figure);
	string line;
	while (getline(fileTXT, line)) {
		vecx.push_back(line); // Adding the line at the end of vector
	}
	int size = stoi(vecx[0]); // Number of total vertices. It's the first line in the .3d file
	return vecx;
}

string reading(vector<string> aux){
	int size = stoi(aux[0]); // number of vertex
	int conta = 0;
	string line; // String processed of all file3dRead strings concatenated into one of plane.3d or box.3d

	for (int i = 1; i <= size; i++) {
		if (conta != 0) { // Adding ',' at the end of each line except the first
			line = line + " " + aux[i];
		}
		else {
			line += aux[i];
			conta++;
		}
	}
	return line;
}

void preencheVrtx(){
	string sun 				= reading(file3dRead_Sun);
	string mercury	  = reading(file3dRead_Mercury);
	string venus   	  = reading(file3dRead_Venus);
	string earth	    = reading(file3dRead_Earth);
	string mars	      = reading(file3dRead_Mars);
	string jupiter	  = reading(file3dRead_Jupiter);
	string saturn 	  = reading(file3dRead_Saturn);
	string uranus 	  = reading(file3dRead_Uranus);
	string neptune	  = reading(file3dRead_Neptune);

	string earthMoon		 = reading(file3dRead_earthMoon);
	string PhobosMoon		 = reading(file3dRead_PhobosMoon);
	string DeimosMoon    = reading(file3dRead_DeimosMoon);
	string GanymedeMoon  = reading(file3dRead_GanymedeMoon);
	string CallistoMoon  = reading(file3dRead_CallistoMoon);
	string EuropaMoon	 	 = reading(file3dRead_EuropaMoon);
	string IoMoon				 = reading(file3dRead_IoMoon);
	string TitanMoon		 = reading(file3dRead_TitanMoon);
	string EnceladusMoon = reading(file3dRead_Neptune);
	string TitaniaMoon   = reading(file3dRead_TitaniaMoon);
	string OberonMoon		 = reading(file3dRead_OberonMoon);
	string UmbrielMoon   = reading(file3dRead_UmbrielMoon);
	string ArielMoon 		 = reading(file3dRead_ArielMoon);
	string MirandaMoon 	 = reading(file3dRead_MirandaMoon);
	string TritonMoon 	 = reading(file3dRead_TritonMoon);
	string ProteusMoon	 = reading(file3dRead_ProteusMoon);

	// Build an istream that holds the input string
	istringstream is_sun(sun);
	istringstream is_mercury(mercury);
	istringstream is_venus(venus);
	istringstream is_earth(earth);
	istringstream is_mars(mars);
	istringstream is_jupiter(jupiter);
	istringstream is_saturn(saturn);
	istringstream is_uranus(uranus);
	istringstream is_neptune(neptune);

	istringstream is_earthMoon(earthMoon);
	istringstream is_PhobosMoon(PhobosMoon);
	istringstream is_DeimosMoon(DeimosMoon);
	istringstream is_GanymedeMoon(GanymedeMoon);
	istringstream is_CallistoMoon(CallistoMoon);
	istringstream is_EuropaMoon(EuropaMoon);
	istringstream is_IoMoon(IoMoon);
	istringstream is_TitanMoon(TitanMoon);
	istringstream is_EnceladusMoon(EnceladusMoon);
	istringstream is_TitaniaMoon(TitaniaMoon);
	istringstream is_OberonMoon(OberonMoon);
	istringstream is_UmbrielMoon(UmbrielMoon);
	istringstream is_ArielMoon(ArielMoon);
	istringstream is_MirandaMoon(MirandaMoon);
	istringstream is_TritonMoon(TritonMoon);
	istringstream is_ProteusMoon(ProteusMoon);

	// Iterates over the istream, using >> to grab floats and push_back to store them in the vector
	copy(istream_iterator<float>(is_sun), istream_iterator<float>(), back_inserter(vrtx_sun));
	copy(istream_iterator<float>(is_mercury), istream_iterator<float>(), back_inserter(vrtx_Mercury));
	copy(istream_iterator<float>(is_venus), istream_iterator<float>(), back_inserter(vrtx_Venus));
	copy(istream_iterator<float>(is_earth), istream_iterator<float>(), back_inserter(vrtx_Earth));
	copy(istream_iterator<float>(is_mars), istream_iterator<float>(), back_inserter(vrtx_Mars));
	copy(istream_iterator<float>(is_jupiter), istream_iterator<float>(), back_inserter(vrtx_Jupiter));
	copy(istream_iterator<float>(is_saturn), istream_iterator<float>(), back_inserter(vrtx_Saturn));
	copy(istream_iterator<float>(is_uranus), istream_iterator<float>(), back_inserter(vrtx_Uranus));
	copy(istream_iterator<float>(is_neptune), istream_iterator<float>(), back_inserter(vrtx_Neptune));

	copy(istream_iterator<float>(is_earthMoon), istream_iterator<float>(), back_inserter(vrtx_earthMoon));
	copy(istream_iterator<float>(is_PhobosMoon), istream_iterator<float>(), back_inserter(vrtx_PhobosMoon));
	copy(istream_iterator<float>(is_DeimosMoon), istream_iterator<float>(), back_inserter(vrtx_DeimosMoon));
	copy(istream_iterator<float>(is_GanymedeMoon), istream_iterator<float>(), back_inserter(vrtx_GanymedeMoon));
	copy(istream_iterator<float>(is_CallistoMoon), istream_iterator<float>(), back_inserter(vrtx_CallistoMoon));
	copy(istream_iterator<float>(is_EuropaMoon), istream_iterator<float>(), back_inserter(vrtx_EuropaMoon));
	copy(istream_iterator<float>(is_IoMoon), istream_iterator<float>(), back_inserter(vrtx_IoMoon));
	copy(istream_iterator<float>(is_TitanMoon), istream_iterator<float>(), back_inserter(vrtx_TitanMoon));
	copy(istream_iterator<float>(is_EnceladusMoon), istream_iterator<float>(), back_inserter(vrtx_EnceladusMoon));
	copy(istream_iterator<float>(is_TitaniaMoon), istream_iterator<float>(), back_inserter(vrtx_TitaniaMoon));
	copy(istream_iterator<float>(is_OberonMoon), istream_iterator<float>(), back_inserter(vrtx_OberonMoon));
	copy(istream_iterator<float>(is_UmbrielMoon), istream_iterator<float>(), back_inserter(vrtx_UmbrielMoon));
	copy(istream_iterator<float>(is_ArielMoon), istream_iterator<float>(), back_inserter(vrtx_ArielMoon));
	copy(istream_iterator<float>(is_MirandaMoon), istream_iterator<float>(), back_inserter(vrtx_MirandaMoon));
	copy(istream_iterator<float>(is_TritonMoon), istream_iterator<float>(), back_inserter(vrtx_TritonMoon));
	copy(istream_iterator<float>(is_ProteusMoon), istream_iterator<float>(), back_inserter(vrtx_ProteusMoon));
}

void fillTranslates(int x, int y, int z, int count){
  switch (count) {
    case 1:
           sun_X=x;
           sun_Y=y;
           sun_Z=z;
           break;
    case 2:
           mercury_X=x;
           mercury_Y=y;
           mercury_Z=z;
           break;
    case 3:
           venus_X=x;
           venus_Y=y;
           venus_Z=z;
           break;
    case 4:
           earth_X=x;
           earth_Y=y;
           earth_Z=z;
           break;
    case 5:
           mars_X=x;
           mars_Y=y;
           mars_Z=z;
           break;
    case 6:
           jupiter_X=x;
           jupiter_Y=y;
           jupiter_Z=z;
           break;
    case 7:
           saturn_X=x;
           saturn_Y=y;
           saturn_Z=z;
           break;
    case 8:
           uranus_X=x;
           uranus_Y=y;
           uranus_Z=z;
           break;
    case 9:
           neptune_X=x;
           neptune_Y=y;
           neptune_Z=z;
           break;

    default:
           break;
  }

}

void printTranslates(){
    printf("%d\n", sun_X);
    printf("%d\n", sun_Y);
    printf("%d\n", sun_Z);
    printf("%d\n", mercury_X);
    printf("%d\n", mercury_Y);
    printf("%d\n", mercury_Z);
    printf("%d\n", venus_X);
    printf("%d\n", venus_Y);
    printf("%d\n", venus_Z);
    printf("%d\n", earth_X);
    printf("%d\n", earth_Y);
    printf("%d\n", earth_Z);
    printf("%d\n", mars_X);
    printf("%d\n", mars_Y);
    printf("%d\n", mars_Z);
    printf("%d\n", jupiter_X);
    printf("%d\n", jupiter_Y);
    printf("%d\n", jupiter_Z);
    printf("%d\n", saturn_X);
    printf("%d\n", saturn_Y);
    printf("%d\n", saturn_Z);
    printf("%d\n", uranus_X);
    printf("%d\n", uranus_Y);
    printf("%d\n", uranus_Z);
    printf("%d\n", neptune_X);
    printf("%d\n", neptune_Y);
    printf("%d\n", neptune_Z);
}

void fillRotates(float ang, int x, int y, int z, int count){
  switch (count) {
    case 1:
           sun_angle=ang;
           sun_axisX=x;
           sun_axisY=y;
           sun_axisZ=z;
           break;
    case 2:
           mercury_angle=ang;
           mercury_axisX=x;
           mercury_axisY=y;
           mercury_axisZ=z;
           break;
    case 3:
           venus_angle=ang;
           venus_axisX=x;
           venus_axisY=y;
           venus_axisZ=z;
           break;
    case 4:
           earth_angle=ang;
           earth_axisX=x;
           earth_axisY=y;
           earth_axisZ=z;
           break;
    case 5:
           mars_angle=ang;
           mars_axisX=x;
           mars_axisY=y;
           mars_axisZ=z;
           break;
    case 6:
           jupiter_angle=ang;
           jupiter_axisX=x;
           jupiter_axisY=y;
           jupiter_axisZ=z;
           break;
    case 7:
           saturn_angle=ang;
           saturn_axisX=x;
           saturn_axisY=y;
           saturn_axisZ=z;
           break;
    case 8:
           uranus_angle=ang;
           uranus_axisX=x;
           uranus_axisY=y;
           uranus_axisZ=z;
           break;
    case 9:
           neptune_angle=ang;
           neptune_axisX=x;
           neptune_axisY=y;
           neptune_axisZ=z;
           break;

    default:
           break;
  }
}

void printRotates(){
    printf("%f\n", sun_angle);
    printf("%d\n", sun_axisX);
    printf("%d\n", sun_axisY);
    printf("%d\n", sun_axisZ);
    printf("%f\n", mercury_angle);
    printf("%d\n", mercury_axisX);
    printf("%d\n", mercury_axisY);
    printf("%d\n", mercury_axisZ);
    printf("%f\n", venus_angle);
    printf("%d\n", venus_axisX);
    printf("%d\n", venus_axisY);
    printf("%d\n", venus_axisZ);
    printf("%f\n", earth_angle);
    printf("%d\n", earth_axisX);
    printf("%d\n", earth_axisY);
    printf("%d\n", earth_axisZ);
    printf("%f\n", mars_angle);
    printf("%d\n", mars_axisX);
    printf("%d\n", mars_axisY);
    printf("%d\n", mars_axisZ);
    printf("%f\n", jupiter_angle);
    printf("%d\n", jupiter_axisX);
    printf("%d\n", jupiter_axisY);
    printf("%d\n", jupiter_axisZ);
    printf("%f\n", saturn_angle);
    printf("%d\n", saturn_axisX);
    printf("%d\n", saturn_axisY);
    printf("%d\n", saturn_axisZ);
    printf("%f\n", uranus_angle);
    printf("%d\n", uranus_axisX);
    printf("%d\n", uranus_axisY);
    printf("%d\n", uranus_axisZ);
    printf("%f\n", neptune_angle);
    printf("%d\n", neptune_axisX);
    printf("%d\n", neptune_axisY);
    printf("%d\n", neptune_axisZ);
}

void fillPlanetsFilenames(string name, int count){
  switch (count) {
    case 1:
           sun_filename=name;
           break;
    case 2:
           mercury_filename=name;
           break;
    case 3:
           venus_filename=name;
           break;
    case 4:
           earth_filename=name;
           break;
    case 5:
           mars_filename=name;
           break;
    case 6:
           jupiter_filename=name;
           break;
    case 7:
           saturn_filename=name;
           break;
    case 8:
           uranus_filename=name;
           break;
    case 9:
           neptune_filename=name;
           break;

    default:
           break;
  }
}

void printPlanetsFilenames(){
    printf("%s\n", sun_filename.c_str());
    printf("%s\n", mercury_filename.c_str());
    printf("%s\n", venus_filename.c_str());
    printf("%s\n", earth_filename.c_str());
    printf("%s\n", mars_filename.c_str());
    printf("%s\n", jupiter_filename.c_str());
    printf("%s\n", saturn_filename.c_str());
    printf("%s\n", uranus_filename.c_str());
    printf("%s\n", neptune_filename.c_str());
}

void fillMoonsFilenames(string name, int count){
  switch (count) {
    case 1:
           earth_moon_filename=name;
           break;
    case 2:
           mars_moon1_filename=name;
           break;
    case 3:
           mars_moon2_filename=name;
           break;
    case 4:
           jupiter_moon1_filename=name;
           break;
    case 5:
           jupiter_moon2_filename=name;
           break;
    case 6:
           jupiter_moon3_filename=name;
           break;
    case 7:
           jupiter_moon4_filename=name;
           break;
    case 8:
           saturn_moon1_filename=name;
           break;
    case 9:
           saturn_moon2_filename=name;
           break;
    case 10:
           uranus_moon1_filename=name;
           break;
    case 11:
           uranus_moon2_filename=name;
           break;
    case 12:
           uranus_moon3_filename=name;
           break;
    case 13:
           uranus_moon4_filename=name;
           break;
    case 14:
           uranus_moon5_filename=name;
           break;
    case 15:
           neptune_moon1_filename=name;
           break;
    case 16:
           neptune_moon2_filename=name;
           break;
    default:
           break;
  }
}

void printMoonsFilenames(){
    printf("%s\n", earth_moon_filename.c_str());
    printf("%s\n", mars_moon1_filename.c_str());
    printf("%s\n", mars_moon2_filename.c_str());
    printf("%s\n", jupiter_moon1_filename.c_str());
    printf("%s\n", jupiter_moon2_filename.c_str());
    printf("%s\n", jupiter_moon3_filename.c_str());
    printf("%s\n", jupiter_moon4_filename.c_str());
    printf("%s\n", saturn_moon1_filename.c_str());
    printf("%s\n", saturn_moon2_filename.c_str());
    printf("%s\n", uranus_moon1_filename.c_str());
    printf("%s\n", uranus_moon2_filename.c_str());
    printf("%s\n", uranus_moon3_filename.c_str());
    printf("%s\n", uranus_moon4_filename.c_str());
    printf("%s\n", uranus_moon5_filename.c_str());
    printf("%s\n", neptune_moon1_filename.c_str());
    printf("%s\n", neptune_moon2_filename.c_str());
}


void readXML(string f){

// Load the xml file, I put your XML in a file named test.xml
TiXmlDocument XMLdoc(f);
bool loadOkay = XMLdoc.LoadFile();
if (loadOkay)
{
    cout << "SolarSystemXML.xml loaded" << endl;
    read=1;
    TiXmlElement *pRoot, *pGroup, *pTranslate, *pRotate, *pModels, *pModelFile, *pMoons, *pModelsMoons, *pModelFileMoons;
    pRoot = XMLdoc.FirstChildElement( "scene" );
    if ( pRoot )
    {
        pGroup = pRoot->FirstChildElement("group" );

        while ( pGroup )
        {
            num_planets++;
            pTranslate = pGroup->FirstChildElement("translate" );
            while ( pTranslate )
            {
                //cout << "Translate X: " << pTranslate->Attribute("X") << endl;
                //cout << "Translate Y: " << pTranslate->Attribute("Y") << endl;
                //cout << "Translate Z: " << pTranslate->Attribute("Z") << endl;

                fillTranslates(atoi(pTranslate->Attribute("X")),atoi(pTranslate->Attribute("Y")),atoi(pTranslate->Attribute("Z")),num_planets);

                pTranslate = pTranslate->NextSiblingElement("translate");
            }

            pRotate = pGroup->FirstChildElement("rotate" );
            while ( pRotate )
            {
                //cout << "Angle: " << pRotate->Attribute("angle") << endl;
                //cout << "Axis X: " << pRotate->Attribute("axisX") << endl;
                //cout << "Axis Y: " << pRotate->Attribute("axisY") << endl;
                //cout << "Axis Z: " << pRotate->Attribute("axisZ") << endl;

                fillRotates(atof(pRotate->Attribute("angle")),atoi(pRotate->Attribute("axisX")),atoi(pRotate->Attribute("axisY")),atoi(pRotate->Attribute("axisZ")),num_planets);

                pRotate = pRotate->NextSiblingElement("rotate");
            }
            pModels = pGroup->FirstChildElement("models");
            while ( pModels )
            {
                //cout << "models: planets & sun" << endl;

                pModelFile = pModels->FirstChildElement("model");
                while( pModelFile )
                {
                    //cout << "Planet File: " << pModelFile->Attribute("file") << endl;

                    fillPlanetsFilenames(pModelFile->Attribute("file"),num_planets);

                    switch (num_planets) {
  								    case 1:
  								           file3dRead_Sun = read3d((string) pModelFile->Attribute("file"));
  								           break;
  										case 2:
  			 								     file3dRead_Mercury = read3d((string) pModelFile->Attribute("file"));
  			 								     break;
  										case 3:
  													 file3dRead_Venus = read3d((string) pModelFile->Attribute("file"));
  													 break;
  										case 4:
  													 file3dRead_Earth = read3d((string) pModelFile->Attribute("file"));
  													 break;
  										case 5:
  	 													file3dRead_Mars= read3d((string) pModelFile->Attribute("file"));
  	 													break;
  										case 6:
  													 file3dRead_Jupiter = read3d((string) pModelFile->Attribute("file"));
  													 break;
  										case 7:
  													 file3dRead_Saturn = read3d((string) pModelFile->Attribute("file"));
  													 break;
  										case 8:
  			 										 file3dRead_Uranus = read3d((string) pModelFile->Attribute("file"));
  			 										 break;
  			 							case 9:
  			 										 file3dRead_Neptune = read3d((string) pModelFile->Attribute("file"));
  			 										 break;
  										default:
  			 			 							 break;
  									}

                    pModelFile = pModelFile->NextSiblingElement("model");
                }

                pModels = pModels->NextSiblingElement("models");
            }

            pMoons = pGroup->FirstChildElement("group" );
            while ( pMoons )
            {
            pModelsMoons = pMoons->FirstChildElement("models");
            while ( pModelsMoons )
            {
                //cout << "models: moons" << endl;
                num_moons++;

                pModelFileMoons = pModelsMoons->FirstChildElement("model");
                while( pModelFileMoons )
                {
                    //cout << "Moon File: " << pModelFileMoons->Attribute("file") << endl;

                    fillMoonsFilenames(pModelFileMoons->Attribute("file"),num_moons);

                    switch (num_moons) {
  								    case 1:
  								           file3dRead_earthMoon = read3d((string) pModelFileMoons->Attribute("file"));
  								           break;
  										case 2:
  			 								     file3dRead_PhobosMoon= read3d((string) pModelFileMoons->Attribute("file"));
  			 								     break;
  										case 3:
  													 file3dRead_DeimosMoon = read3d((string) pModelFileMoons->Attribute("file"));
  													 break;
  										case 4:
  													 file3dRead_GanymedeMoon = read3d((string) pModelFileMoons->Attribute("file"));
  													 break;
  										case 5:
  	 													file3dRead_CallistoMoon= read3d((string) pModelFileMoons->Attribute("file"));
  	 													break;
  										case 6:
  													 file3dRead_EuropaMoon = read3d((string) pModelFileMoons->Attribute("file"));
  													 break;
  										case 7:
  													 file3dRead_IoMoon = read3d((string) pModelFileMoons->Attribute("file"));
  													 break;
  										case 8:
  			 										 file3dRead_TitanMoon = read3d((string) pModelFileMoons->Attribute("file"));
  			 										 break;
  			 							case 9:
  			 										 file3dRead_EnceladusMoon = read3d((string) pModelFileMoons->Attribute("file"));
  			 										 break;
  								    case 10:
  								           file3dRead_TitaniaMoon = read3d((string) pModelFileMoons->Attribute("file"));
  								           break;
  										case 11:
  			 								     file3dRead_OberonMoon = read3d((string) pModelFileMoons->Attribute("file"));
  			 								     break;
  										case 12:
  													 file3dRead_UmbrielMoon = read3d((string) pModelFileMoons->Attribute("file"));
  													 break;
  										case 13:
  													 file3dRead_ArielMoon = read3d((string) pModelFileMoons->Attribute("file"));
  													 break;
  										case 14:
  	 													file3dRead_MirandaMoon= read3d((string) pModelFileMoons->Attribute("file"));
  	 													break;
  										case 15:
  													 file3dRead_TritonMoon = read3d((string) pModelFileMoons->Attribute("file"));
  													 break;
  										case 16:
  													 file3dRead_ProteusMoon = read3d((string) pModelFileMoons->Attribute("file"));
  													 break;
  										default:
  			 										 break;
  									}

                    pModelFileMoons = pModelFileMoons->NextSiblingElement("model");
                }

                pModelsMoons = pModelsMoons->NextSiblingElement("models");
            }

              pMoons = pMoons->NextSiblingElement("group");
            }

            pGroup = pGroup->NextSiblingElement("group");
        }
    }
    else
    {
        cout << "Cannot find 'scene' node" << endl;
    }
}
  else
  {
        cout << "Cannot find XML file" << endl;
        read=0;
  }
}

int main(int argc, char **argv) {
  string operationLine, operation;

	cout << "Insert your operation:" << endl;
	getline(cin, operationLine);
	splitted = split(operationLine, ' '); // Save a string on a vector with the substrings separated by a space

	// The function "stoff" transforms the content of a string in a float

  if (splitted[0] == "Draw" || splitted[0] == "draw") {	// Draw -> receives the name of the .3d file
    if (splitted.size() == 1){
      readXML("SolarSystemXML.xml");
      if(read!=0){
        preencheVrtx();
        printf("Translates\n");
        printTranslates();
        printf("\n\nRotates\n");
        printRotates();
        printf("\n\nPlanets\n");
        printPlanetsFilenames();
        printf("\n\nMoons\n");
        printMoonsFilenames();

  /*
   * Init GLUT and the window.
   */
  glutInit(&argc, argv);

  // Sets up a double buffer with RGBA components and a depth component
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

  // Sets the window size to 512*512 square pixels
  glutInitWindowSize(1000, 1000);

  // Sets the window position to the upper left
  glutInitWindowPosition(0, 0);

  // Creates a window using internal glut functionality
  glutCreateWindow("Solar System - Stage 2");

  /*
   * Register functions that will process events.
   */

  // Callback function responsible for the window's contents.
  glutDisplayFunc(renderScene);
  // Function to be callend when the window is created or resized.
  glutReshapeFunc(changeSize);
  // Function called when the event queue is empty.
  glutIdleFunc(renderScene);
  // Function called when a key is pressed.
  glutKeyboardFunc(keyPressed);
  glutSpecialFunc(keyboardExtra);

  //menu
  glutCreateMenu(newMenu);
  glutAddMenuEntry("Increase Translate x value",'j');
  glutAddMenuEntry("Increase Translate y value",'k');
  glutAddMenuEntry("Increase Translate z value",'l');
  glutAddMenuEntry("Decrease Translate x value",'q');
  glutAddMenuEntry("Decrease Translate y value",'w');
  glutAddMenuEntry("Decrease Translate z value",'e');
  glutAddMenuEntry("GL_FRONT & GL_LINE",'z');
  glutAddMenuEntry("GL_FRONT & GL_POINT",'x');
  glutAddMenuEntry("GL_FRONT & GL_FILL",'c');
  glutAddMenuEntry("GL_BACK & GL_LINE",'v');
  glutAddMenuEntry("GL_BACK & GL_POINT",'b');
  glutAddMenuEntry("GL_BACK & GL_FILL",'n');
  glutAddMenuEntry("GL_FRONT_AND_BACK & GL_LINE",'m');
  glutAddMenuEntry("GL_FRONT_AND_BACK & GL_POINT",'o');
  glutAddMenuEntry("GL_FRONT_AND_BACK & GL_FILL",'p');
  glutAddMenuEntry("Change Colour to Default",'0');
  glutAddMenuEntry("Change Colour to Red",'1');
  glutAddMenuEntry("Change Colour to Blue",'2');
  glutAddMenuEntry("Change Colour to Green",'3');
  glutAddMenuEntry("Change Colour to Orange",'4');
  glutAddMenuEntry("Change Colour to Pink",'5');
  glutAddMenuEntry("Change Colour to Yellow",'6');
  glutAddMenuEntry("Change Colour to Bright Blue",'7');
  //button= GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, or GLUT MIDDLE_BUTTON
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  // put init here
  init();

  // enter GLUT's main cycle.
	glutMainLoop();
  }
 }
 else cout << "Try again! Amount of arguments incorrect" << endl;
}
else cout << "Try again! Use: draw <filename.3d>" << endl;
return 1;
}
void init () {
	glClearColor(0.0,0.0,0.0,0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
  //default
  glPolygonMode(GL_FRONT,GL_FILL);
}

void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0) h = 1;

	// compute window's aspect ratio
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
  glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {
	// Clear buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the camera.
	glLoadIdentity();
	gluLookAt(0.0,700.0,200.0,0.0,0.0,-1.0,0.0f,1.0f,0.0f);

  // Draw plane.
  glBegin(GL_TRIANGLES);
    glColor3f(0.5,0.5,0.5);
    glVertex3f(-600, 0, -600);
    glVertex3f(-600, 0, 600);
    glVertex3f(600, 0, 600);

    glVertex3f(600, 0, -600);
    glVertex3f(-600, 0, -600);
    glVertex3f(600, 0, 600);
  glEnd();

  // Sun.
  glColor3f(1.0, 1.0, 0.0);
  glTranslatef(0.0, 10.0, 0.0);
  glBegin(GL_TRIANGLES);
    for (int j = 0; j < vrtx_sun.size();){
        glVertex3f(vrtx_sun[j++], vrtx_sun[j++], vrtx_sun[j++]);
    }
  glEnd();

  // Mercury
  glColor3f(0.0, 0.5, 1);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(MercuryRadiusOrbit * cos(alphaM), 1.0, MercuryRadiusOrbit * sin(alphaM)); //proxima posicao no eixo XoZ
    glRotatef(10.0, 0.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
      for (int j = 0; j < vrtx_Mercury.size();){
          glVertex3f(vrtx_Mercury[j++], vrtx_Mercury[j++], vrtx_Mercury[j++]);
      }
    glEnd();
    glPopMatrix();
    alphaM -= alphaIncM; // andar em sentido contrario, em circulos
  }
  alphaM -= 100.0;

  // Venus
  glColor3f(1.0, 0.2, 0.2);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(VenusRadiusOrbit * cos(alphaV), 1.0, VenusRadiusOrbit * sin(alphaV));
    glBegin(GL_TRIANGLES);
      for (int j = 0; j < vrtx_Venus.size();){
          glVertex3f(vrtx_Venus[j++], vrtx_Venus[j++], vrtx_Venus[j++]);
      }
    glEnd();
    glPopMatrix();
    alphaV += alphaIncV;
  }
  alphaV += 100.0;

  // Terra
  glColor3f(1.0, 0.5, 1.0);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(TerraRadiusOrbit * cos(alphaT), 1.0, TerraRadiusOrbit * sin(alphaT));
    glBegin(GL_TRIANGLES);
      for (int j = 0; j < vrtx_Earth.size();){
          glVertex3f(vrtx_Earth[j++], vrtx_Earth[j++], vrtx_Earth[j++]);
      }
    glEnd();
    glPopMatrix();
    alphaT -= alphaIncT;
  }
  alphaT -= 100.0;

  // Marte
  glColor3f(0.0, 0.5, 1.0);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(MarteRadiusOrbit * cos(alphaMT), 1.0, MarteRadiusOrbit * sin(alphaMT));
    glBegin(GL_TRIANGLES);
      for (int j = 0; j < vrtx_Mars.size();){
          glVertex3f(vrtx_Mars[j++], vrtx_Mars[j++], vrtx_Mars[j++]);
      }
    glEnd();
    glPopMatrix();
    alphaMT += alphaIncMT;
  }
  alphaMT += 100.0;

  // Jupiter
  glColor3f(0.5, 0.5, 0.5);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(JupiterRadiusOrbit * cos(alphaJ), 1.0, JupiterRadiusOrbit * sin(alphaJ));
    glBegin(GL_TRIANGLES);
      for (int j = 0; j < vrtx_Jupiter.size();){
          glVertex3f(vrtx_Jupiter[j++], vrtx_Jupiter[j++], vrtx_Jupiter[j++]);
      }
    glEnd();
    glPopMatrix();
    alphaJ += alphaIncJ;
  }
  alphaJ += 100.0;

  // Saturno
  glColor3f(0.0, 0.0, 0.5);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(SaturnoRadiusOrbit * cos(alphaS), 1.0, SaturnoRadiusOrbit * sin(alphaS));
    glBegin(GL_TRIANGLES);
      for (int j = 0; j < vrtx_Saturn.size();){
          glVertex3f(vrtx_Saturn[j++], vrtx_Saturn[j++], vrtx_Saturn[j++]);
      }
    glEnd();
    glPopMatrix();
    alphaS -= alphaIncS;
  }
  alphaS -= 100.0;

  // Urano
  glColor3f(1.0, 0.5, 0.0);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(UranoRadiusOrbit * cos(alphaU), 1.0, UranoRadiusOrbit * sin(alphaU));
    glBegin(GL_TRIANGLES);
      for (int j = 0; j < vrtx_Uranus.size();){
          glVertex3f(vrtx_Uranus[j++], vrtx_Uranus[j++], vrtx_Uranus[j++]);
      }
    glEnd();
    glPopMatrix();
    alphaU -= alphaIncU;
  }
  alphaU -= 100.0;

  // Neptuno
  glColor3f(0.0, 1.0, 0.5);
  for (int i = 0; i < 1; i++) {
    glPushMatrix();
    glTranslatef(NeptunoRadiusOrbit * cos(alphaN), 1.0, NeptunoRadiusOrbit * sin(alphaN));
    glBegin(GL_TRIANGLES);
      for (int j = 0; j < vrtx_Neptune.size();){
          glVertex3f(vrtx_Neptune[j++], vrtx_Neptune[j++], vrtx_Neptune[j++]);
      }
    glEnd();
    glPopMatrix();
    alphaN += alphaIncN;
  }
  alphaN += 100.0;

	// End of frame.
	glutSwapBuffers();
}

// write function to process menu events
void newMenu (int id_op){
    keyPressed((unsigned char)id_op, 0, 0);
}

void keyPressed(unsigned char key, int x, int y) {
      if (key == 'j' || key == 'J') translate_x+=1;
      else if (key == 'k' || key == 'K') translate_y+=1;
      else if (key == 'l' || key == 'L') translate_z+=1;
      else if (key == 'q' || key == 'Q') translate_x-=1;
      else if (key == 'w' || key == 'W') translate_y-=1;
      else if (key == 'e' || key == 'E') translate_z-=1;
      else if (key == 'z' || key == 'Z') glPolygonMode(GL_FRONT,GL_LINE); //GL_FRONT & GL_LINE
      else if (key == 'x' || key == 'X') glPolygonMode(GL_FRONT,GL_POINT); //GL_FRONT & GL_POINT
      else if (key == 'c' || key == 'C') glPolygonMode(GL_FRONT,GL_FILL); //GL_FRONT & GL_FILL
      else if (key == 'v' || key == 'V') glPolygonMode(GL_BACK,GL_LINE); //GL_BACK & GL_LINE
      else if (key == 'b' || key == 'B') glPolygonMode(GL_BACK,GL_POINT); //GL_BACK & GL_POINT
      else if (key == 'n' || key == 'N') glPolygonMode(GL_BACK,GL_FILL); //GL_BACK & GL_FILL
      else if (key == 'm' || key == 'M') glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); //GL_FRONT_AND_BACK & GL_LINE
      else if (key == 'o' || key == 'O') glPolygonMode(GL_FRONT_AND_BACK,GL_POINT); //GL_FRONT_AND_BACK & GL_POINT
      else if (key == 'p' || key == 'P') glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); //GL_FRONT_AND_BACK & GL_FILL*/
      else if (key == '0'){ // DEFAULT IS WHITE
          glColor3f(1.0f,1.0f,1.0f);
      }
      else if (key == '1'){ // RED
          glColor3f(1.0f,0.0f,0.0f);
      }
      else if (key == '2'){ // BLUE
          glColor3f(0.0f,0.0f,1.0f);
      }
      else if (key == '3'){ // GREEN
          glColor3f(0.0f,1.0f,0.0f);
      }
      else if (key == '4'){ // ORANGE
          glColor3f(1.0f,0.5f,0.0f);
      }
      else if (key == '5'){ // PINK
          glColor3f(1.0f,0.5f,0.5f);
      }
      else if (key == '6'){ // YELLOW
          glColor3f(1.0f,1.0f,0.0f);
      }
      else if (key == '7'){ // BRIGHT BLUE
          glColor3f(0.0f,1.0f,1.0f);
      }
      else if (key==27) exit(-1);

      //when camera moves
      glutPostRedisplay();
}

void keyboardExtra(int key_code, int x, int y){
		/*  Right arrow key - decrease rotate_y by 5 */
		if (key_code == GLUT_KEY_RIGHT) rotate_y -= 5;
		/*  Left arrow key - increase rotate_y by 5 */
		else if (key_code == GLUT_KEY_LEFT) rotate_y += 5;
		/*  Up arrow key - increase rotate_x by 5 */
		else if (key_code == GLUT_KEY_UP) rotate_x += 5;
		/*  Down arrow key - decrease rotate_x by 5 */
		else if (key_code == GLUT_KEY_DOWN) rotate_x -= 5;
		/*   arrow key - decrease rotate_z by 5 */
		else if (key_code == GLUT_KEY_F9) rotate_z += 5;
		/*   arrow key - decrease rotate_z by 5 */
		else if (key_code == GLUT_KEY_F7) rotate_z -= 5;
		//when camera moves

		glutPostRedisplay();
}
