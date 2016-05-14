
#include "read.h"

void testTree(scene* sceneData) {

	int altura = tree_high(sceneData->transformation_tree);
	cout << "Group Tree size:" << altura << "\n";

	cout << "Number of model files:" << numberOfModels(sceneData->transformation_tree) << "\n";

}

node_group* initNodeGroup() {
	node_group* r = (node_group*)malloc(sizeof(struct node_group));
	r->translate = (vector<float>*)malloc(sizeof(vector<float>));
	r->translate = new vector<float>();

	r->rotateAxis = (vector<float>*)malloc(sizeof(vector<float>));
	r->rotateAxis = new vector<float>();

	r->vboIndex = (vector<int>*)malloc(sizeof(vector<int>));
	r->vboIndex = new vector<int>();

	r->model_file = (vector<string>*)malloc(sizeof(vector<string>));
	r->model_file = new vector<string>();
    
    r->model_texture = (vector<string>*)malloc(sizeof(vector<string>));
    r->model_texture = new vector<string>();

	r->child = (node_group**)malloc(10 * sizeof(struct node_group));
	r->childIndex = 0;

	r->rotate_period = (vector<float>*)malloc(sizeof(vector<float>));
	r->rotate_period = new vector<float>();

	r->translation_period = 0;
	
	r->pointIndex=0;
	
	return r;
}

int numberOfModels(node_group* node) {
	int r = 0;
	for (int i = 0; i < node->childIndex; i++) {
		r += numberOfModels(node->child[i]);
	}
	return r + node->model_file->size();
}

int tree_high(node_group* node) {
	int r = 0, max = 0;
	for (int i = 0; i < node->childIndex; i++) {
		r = tree_high(node->child[i]);
		if (r >= max) max = r;
	}
	return max + 1;
}


scene* readXML(const char* f, int* vboNbuffers) {

	TiXmlDocument XMLdoc(f);
	bool loadOkay = XMLdoc.LoadFile();
	if (!loadOkay)	return nullptr;
	TiXmlElement *pRoot = XMLdoc.FirstChildElement("scene");
	scene* sceneData = (scene*)malloc(sizeof(struct scene));
	
	node_group* firstGroup = initNodeGroup();

	sceneData->transformation_tree = firstGroup;
	TiXmlElement *pGroup = pRoot->FirstChildElement("group");

	readGroup(pGroup, firstGroup, vboNbuffers);

	return sceneData;
}



void readGroup(TiXmlElement *pGroup, node_group* node,int* vboNbuffers) {
	TiXmlElement* pTranslate = pGroup->FirstChildElement("translate");
	if (pTranslate) {
		const char* time = pTranslate->Attribute("time");
		if (time) {
			node->translation_period = atof(time);

			TiXmlElement* pPoint = pTranslate->FirstChildElement("point");
			
			while (pPoint) {
				if (node->pointIndex == 0) {
					node->p = (float**)malloc(EXTENDED_SIZE * sizeof(float*));
				}
				node->p[node->pointIndex] = (float*)malloc(3 * sizeof(float));
				float x = atof(pPoint->Attribute("X"));
				node->p[node->pointIndex][0] = 4;
				float y = atof(pPoint->Attribute("Y"));
				float z = atof(pPoint->Attribute("Z"));

				node->p[node->pointIndex][0] = x;
				node->p[node->pointIndex][1] = y;
				node->p[node->pointIndex][2] = z;
				/*
				node->p[node->pointIndex][0] = atof(pPoint->Attribute("X"));
				node->p[node->pointIndex][1] = (atof(pPoint->Attribute("Y")));
				node->p[node->pointIndex][2] = (atof(pPoint->Attribute("Z")));
				*/
				pPoint = pPoint->NextSiblingElement("point");
				node->pointIndex++;
			}
		} else {
			node->translate->push_back(atof(pTranslate->Attribute("X")));
			node->translate->push_back(atof(pTranslate->Attribute("Y")));
			node->translate->push_back(atof(pTranslate->Attribute("Z")));
		}
	}

	TiXmlElement* pRotate = pGroup->FirstChildElement("rotate");
	while (pRotate)
	{

		if (const char* angle = pRotate->Attribute("angle")) {
			node->rotateAxis->push_back(atof(angle));
			node->rotateAxis->push_back(atof(pRotate->Attribute("axisX")));
			node->rotateAxis->push_back(atof(pRotate->Attribute("axisY")));
			node->rotateAxis->push_back(atof(pRotate->Attribute("axisZ")));
		}
		else if (const char* time = pRotate->Attribute("time")) {
			node->rotate_period->push_back(atof(time));
			node->rotate_period->push_back(atof(pRotate->Attribute("axisX")));
			node->rotate_period->push_back(atof(pRotate->Attribute("axisY")));
			node->rotate_period->push_back(atof(pRotate->Attribute("axisZ")));
		}
		pRotate = pRotate->NextSiblingElement("rotate");
	}

	TiXmlElement* pCor = pGroup->FirstChildElement("colour");
	if (pCor) {
		node->colour[0] = atoi(pCor->Attribute("R"));
		node->colour[1] = atoi(pCor->Attribute("G"));
		node->colour[2] = atoi(pCor->Attribute("B"));
	}

	TiXmlElement* pModels = pGroup->FirstChildElement("models");
	if (pModels) {
		TiXmlElement* pModelFileAndTexture = pModels->FirstChildElement("model");
		while (pModelFileAndTexture) {
			string filename = (string)pModelFileAndTexture->Attribute("file");
			node->model_file->push_back(filename);
            
            string texture = (string)pModelFileAndTexture->Attribute("texture");
            node->model_texture->push_back(texture);
            
			(*vboNbuffers)=(*vboNbuffers)+2;

			pModelFile = pModelFile->NextSiblingElement("model");
		}
	}

	TiXmlElement* child_group = pGroup->FirstChildElement("group");
	while (child_group)
	{
		node_group* newChild = initNodeGroup();
		node->child[node->childIndex++] = newChild;
		readGroup(child_group, newChild,vboNbuffers);

		child_group = child_group->NextSiblingElement();
	}
}


string reading(vector<string> aux) {
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

vector<float> read3Dfile(string filename) {
	vector<string> aux2 = read3d(filename);
	string vrtxFile = reading(aux2);
	istringstream is_Vrtx(vrtxFile);
	vector<float> r;
	r.erase(r.begin(), r.end());
	copy(istream_iterator<float>(is_Vrtx), istream_iterator<float>(), back_inserter(r));
	return r;
}

