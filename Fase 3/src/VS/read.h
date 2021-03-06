
#ifndef READ_H

#define READ_H

//#include </usr/local/Cellar/tinyxml/2.6.2/include/tinyxml.h> //-- MAC

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

#include "tinyxml\tinyxml.h"	// The place where the file "tinyxml.h" is located

#define EXTENDED_SIZE 10

using namespace std;


extern struct node_group {
	float translation_period;
	float** p;
	int pointIndex;

	int colour[3] = { 255,255,255 };

	vector<float>* translate;
	vector<float>* rotateAxis;
	vector<float>* rotate_period;
	vector<int>* vboIndex;
	vector<string>* model_file;
	node_group** child; // multiple child pointers
	int childIndex;
};

extern struct scene {
	struct node_group* transformation_tree;
	// lights[]
};




void testTree(scene* sceneData);
int numberOfModels(node_group* node);
int tree_high(node_group* node);
vector<float> read3Dfile(string filename);
vector<string> read3d(string figure);
string reading(vector<string> aux);
void readGroup(TiXmlElement *pGroup, node_group* node, int* vboNbuffers);
scene* readXML(const char* f, int* vboNbuffers);


#endif