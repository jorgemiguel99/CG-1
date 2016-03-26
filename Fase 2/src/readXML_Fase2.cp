#include <stdio.h>
#include <iostream>
//#include "tinyxml\tinyxml.h"	// The place where the file "tinyxml.h" is located
#include </usr/local/Cellar/tinyxml/2.6.2/include/tinyxml.h> //-- MAC

using namespace std;

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
            pTranslate = pGroup->FirstChildElement("translate" );
            while ( pTranslate )
            {
                cout << "Translate X: " << pTranslate->Attribute("X") << endl;
                cout << "Translate Y: " << pTranslate->Attribute("Y") << endl;
                cout << "Translate Z: " << pTranslate->Attribute("Z") << endl;

                pTranslate = pTranslate->NextSiblingElement("translate");
            }

            pRotate = pGroup->FirstChildElement("rotate" );
            while ( pRotate )
            {
                cout << "Angle: " << pRotate->Attribute("angle") << endl;
                cout << "Axis X: " << pRotate->Attribute("axisX") << endl;
                cout << "Axis Y: " << pRotate->Attribute("axisY") << endl;
                cout << "Axis Z: " << pRotate->Attribute("axisZ") << endl;

                pRotate = pRotate->NextSiblingElement("rotate");
            }
            pModels = pGroup->FirstChildElement("models");
            while ( pModels )
            {
                //cout << "models: planets & sun" << endl;

                pModelFile = pModels->FirstChildElement("model");
                while( pModelFile )
                {
                    cout << "Planet File: " << pModelFile->Attribute("file") << endl;

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

                pModelFileMoons = pModelsMoons->FirstChildElement("model");
                while( pModelFileMoons )
                {
                    cout << "Moon File: " << pModelFileMoons->Attribute("file") << endl;

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

int main(int argc, char **argv) {
			readXML();
}
