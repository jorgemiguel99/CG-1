#ifndef READ_H

#define READ_H

#include </usr/local/Cellar/tinyxml/2.6.2/include/tinyxml.h> //-- MAC

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
#include <algorithm>

//#include "tinyxml\tinyxml.h"	// The place where the file "tinyxml.h" is located

#define EXTENDED_SIZE 10

using namespace std;

struct light_source {
    char* light_type; // POINT or SPOTLIGHT or ... -> 3 types studied in class
    float* position;
};

struct node_group {
	float translation_period;
	float** p;
	int pointIndex;
	float start_time;

	int colour[3] = { 255,255,255 };

	vector<float>* translate;
	vector<float>* rotateAxis;
	vector<float>* rotate_period;
	vector<float>* scale;
	vector<int>* vboIndex;

	vector<int>* imageIndex;

	vector<string>* model_file;
  vector<string>* model_texture; // textured model
  vector<float>* model_coloured_ambient; // Coloured model as ambR=0.2 ambG=0.2 ambB=0.2 or diffR=1.0 diffG=1.0 diffB=1.0 or specR=0.5 specG=0.5 specB=0.5 or emsR=0.8 emsG=0.8 emsB=0.8
  vector<float>* model_coloured_diffuse;
  vector<float>* model_coloured_specular;
  vector<float>* model_coloured_emissive;
  node_group** child; // multiple child pointers
	int childIndex;
};

struct scene {
	struct node_group* transformation_tree;
    light_source** lights;
	int light_counter;
};

void testTree(scene* sceneData);
int numberOfModels(node_group* node);
int tree_high(node_group* node);
vector<float> read3Dfile(string filename);
vector<float>** dividePoints(vector<float> filepoints);
vector<string> read3d(string figure);
string reading(vector<string> aux);
void readGroup(TiXmlElement *pGroup, node_group* node, int* vboNbuffers,int* imageCount);
scene* readXML(const char* f, int* vboNbuffers, int* vboNbuffersLights);


#endif
