
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

	r->rotate = (vector<float>*)malloc(sizeof(vector<float>));
	r->rotate = new vector<float>();


	r->vboIndex = (vector<int>*)malloc(sizeof(vector<int>));
	r->vboIndex = new vector<int>();

	r->model_file = (vector<string>*)malloc(sizeof(vector<string>));
	r->model_file = new vector<string>();

	r->child = (node_group**)malloc(10 * sizeof(struct node_group));
	r->childIndex = 0;
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
		node->translate->push_back(atof(pTranslate->Attribute("X")));
		node->translate->push_back(atof(pTranslate->Attribute("Y")));
		node->translate->push_back(atof(pTranslate->Attribute("Z")));
	}

	TiXmlElement* pRotate = pGroup->FirstChildElement("rotate");
	if (pRotate)
	{
		node->rotate->push_back(atof(pRotate->Attribute("angle")));
		node->rotate->push_back(atof(pRotate->Attribute("axisX")));
		node->rotate->push_back(atof(pRotate->Attribute("axisY")));
		node->rotate->push_back(atof(pRotate->Attribute("axisZ")));
	}

	TiXmlElement* pModels = pGroup->FirstChildElement("models");
	if (pModels) {
		TiXmlElement* pModelFile = pModels->FirstChildElement("model");
		while (pModelFile) {
			string filename = (string)pModelFile->Attribute("file");

			node->model_file->push_back(filename);
			(*vboNbuffers)=(*vboNbuffers)+1;

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

