#include <stdio.h>
#include <iostream>
#include <string>

//#include "tinyxml\tinyxml.h"	// The place where the file "tinyxml.h" is located
#include </usr/local/Cellar/tinyxml/2.6.2/include/tinyxml.h> //-- MAC
#include "readXML_Fase2.h"

using namespace std;

int num_planets=0,num_moons=0;

int sun_X,sun_Y,sun_Z,sun_axisX,sun_axisY,sun_axisZ;
float sun_angle;
string sun_filename;

int mercury_X,mercury_Y,mercury_Z,mercury_axisX,mercury_axisY,mercury_axisZ;
float mercury_angle;
string mercury_filename;

int venus_X,venus_Y,venus_Z,venus_axisX,venus_axisY,venus_axisZ;
float venus_angle;
string venus_filename;

int earth_X,earth_Y,earth_Z,earth_axisX,earth_axisY,earth_axisZ;
float earth_angle;
string earth_filename,earth_moon_filename;

int mars_X,mars_Y,mars_Z,mars_axisX,mars_axisY,mars_axisZ;
float mars_angle;
string mars_filename,mars_moon1_filename,mars_moon2_filename;

int jupiter_X,jupiter_Y,jupiter_Z,jupiter_axisX,jupiter_axisY,jupiter_axisZ;
float jupiter_angle;
string jupiter_filename,jupiter_moon1_filename,jupiter_moon2_filename,jupiter_moon3_filename,jupiter_moon4_filename;

int saturn_X,saturn_Y,saturn_Z,saturn_axisX,saturn_axisY,saturn_axisZ;
float saturn_angle;
string saturn_filename,saturn_moon1_filename,saturn_moon2_filename;

int uranus_X,uranus_Y,uranus_Z,uranus_axisX,uranus_axisY,uranus_axisZ;
float uranus_angle;
string uranus_filename,uranus_moon1_filename,uranus_moon2_filename,uranus_moon3_filename,uranus_moon4_filename,uranus_moon5_filename;

int neptune_X,neptune_Y,neptune_Z,neptune_axisX,neptune_axisY,neptune_axisZ;
float neptune_angle;
string neptune_filename,neptune_moon1_filename,neptune_moon2_filename;

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


void readXML(){

// Load the xml file, I put your XML in a file named test.xml
TiXmlDocument XMLdoc("SolarSystemXML.xml");
bool loadOkay = XMLdoc.LoadFile();
if (loadOkay)
{
    cout << "SolarSystemXML.xml loaded" << endl;
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
}
