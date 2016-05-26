
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

	r->scale = (vector<float>*)malloc(sizeof(vector<float>));
	r->scale = new vector<float>();

	r->vboIndex = (vector<int>*)malloc(sizeof(vector<int>));
	r->vboIndex = new vector<int>();

	r->imageIndex = (vector<int>*)malloc(sizeof(vector<int>));
	r->imageIndex = new vector<int>();

	r->model_file = (vector<string>*)malloc(sizeof(vector<string>));
	r->model_file = new vector<string>();

    r->model_texture = (vector<string>*)malloc(sizeof(vector<string>));
    r->model_texture = new vector<string>();

    r->model_coloured_ambient = (vector<float>*)malloc(sizeof(vector<float>));
    r->model_coloured_ambient = new vector<float>();

    r->model_coloured_diffuse = (vector<float>*)malloc(sizeof(vector<float>));
    r->model_coloured_diffuse = new vector<float>();

    r->model_coloured_specular = (vector<float>*)malloc(sizeof(vector<float>));
    r->model_coloured_specular = new vector<float>();

    r->model_coloured_emissive = (vector<float>*)malloc(sizeof(vector<float>));
    r->model_coloured_emissive = new vector<float>();

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


scene* readXML(const char* f, int* vboNbuffers, int* vboNbuffersTextures) {

	TiXmlDocument XMLdoc(f);
	bool loadOkay = XMLdoc.LoadFile();
	if (!loadOkay)	return nullptr;

	TiXmlElement *pRoot = XMLdoc.FirstChildElement("scene");
	scene* sceneData = (scene*)malloc(sizeof(struct scene));

	//Groups
	node_group* firstGroup = initNodeGroup();
	sceneData->transformation_tree = firstGroup;
	TiXmlElement *pGroup = pRoot->FirstChildElement("group");

	readGroup(pGroup, firstGroup, vboNbuffers, vboNbuffersTextures);

	//Lights

	sceneData->lights = (light_source**)malloc(20 * sizeof(light_source*));
	sceneData->light_counter = 0;

	TiXmlElement *pGroupLight = pRoot->FirstChildElement("lights");
	TiXmlElement* pLight = pGroupLight->FirstChildElement("light");
	while (pLight) {
		light_source* light = (light_source*)malloc(sizeof(light_source));

		string typeL = (string)pLight->Attribute("type");
		light->light_type = strdup(typeL.data());

		light->position = (float*)malloc(4 * sizeof(float));
		light->position[0] = (atof(pLight->Attribute("posX")));
		light->position[1] = (atof(pLight->Attribute("posY")));
		light->position[2] = (atof(pLight->Attribute("posZ")));
		light->position[3] = 0;
		//if(strcmp(typeL.data(),"POINT") == 0 ) light->position[3] = 1; else light->position[3] = 0;


		sceneData->lights[sceneData->light_counter++] = light;
		pLight = pLight->NextSiblingElement("light");
	}


	return sceneData;
}



void readGroup(TiXmlElement *pGroup, node_group* node, int* vboNbuffers,int* imageCount) {
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
				float y = atof(pPoint->Attribute("Y"));
				float z = atof(pPoint->Attribute("Z"));

				node->p[node->pointIndex][0] = x;
				node->p[node->pointIndex][1] = y;
				node->p[node->pointIndex][2] = z;
				node->p[node->pointIndex][3] = 1;


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


	TiXmlElement* pScale = pGroup->FirstChildElement("scale");
	if(pScale){

			node->scale->push_back(atof(pScale->Attribute("X")));
			node->scale->push_back(atof(pScale->Attribute("Y")));
			node->scale->push_back(atof(pScale->Attribute("Z")));

		//pScale = pScale->NextSiblingElement("scale");
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

            if (const char* ambient = pModelFileAndTexture->Attribute("ambR")) {
                node->model_coloured_ambient->push_back(atof(ambient));
                node->model_coloured_ambient->push_back(atof(pModelFileAndTexture->Attribute("ambG")));
                node->model_coloured_ambient->push_back(atof(pModelFileAndTexture->Attribute("ambB")));
            }
            else if (const char* diffuse = pModelFileAndTexture->Attribute("diffR")) {
                node->model_coloured_diffuse->push_back(atof(diffuse));
                node->model_coloured_diffuse->push_back(atof(pModelFileAndTexture->Attribute("diffG")));
                node->model_coloured_diffuse->push_back(atof(pModelFileAndTexture->Attribute("diffB")));
            }
            else if (const char* specular = pModelFileAndTexture->Attribute("specR")) {
                node->model_coloured_specular->push_back(atof(specular));
                node->model_coloured_specular->push_back(atof(pModelFileAndTexture->Attribute("specG")));
                node->model_coloured_specular->push_back(atof(pModelFileAndTexture->Attribute("specB")));
            }
            else if (const char* emissive = pModelFileAndTexture->Attribute("emsR")) {
                node->model_coloured_emissive->push_back(atof(emissive));
                node->model_coloured_emissive->push_back(atof(pModelFileAndTexture->Attribute("emsG")));
                node->model_coloured_emissive->push_back(atof(pModelFileAndTexture->Attribute("emsB")));
            }

			if (const char* texture = pModelFileAndTexture->Attribute("texture")){
                node->model_texture->push_back(texture);
				(*imageCount) = (*imageCount)+1;
            }

			(*vboNbuffers)=(*vboNbuffers)+1;

			pModelFileAndTexture = pModelFileAndTexture->NextSiblingElement("model");
		}
	}

	TiXmlElement* child_group = pGroup->FirstChildElement("group");
	while (child_group)
	{
		node_group* newChild = initNodeGroup();
		node->child[node->childIndex++] = newChild;
		readGroup(child_group, newChild,vboNbuffers,imageCount);

		child_group = child_group->NextSiblingElement();
	}
}

string reading(vector<string> aux) {
	int conta = 0;
	string line; // String processed of all file3dRead strings concatenated into one of plane.3d or box.3d

	for (int i = 0; aux[i]!="End"; i++) {
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

/*
	Separates Vertexs, Normals and Texture points and stores them on array
	File pointes order
		for each:
			Vertex = 3 points
			Normals = 3 points
			Texture = 2 points
*/
vector<float>** dividePoints(vector<float> filepoints) {

	vector<float>** r = (vector<float>**) malloc(3 * sizeof(vector<float>*));
	for (int j = 0; j < 3;j++)
		r[j] = new vector<float>();

	int i=0;
	for (float p : filepoints) {
		// 3 3 2
		int pos = i++ % 8;
		if (pos < 3) r[0]->push_back(p);
		else if (pos < 6) r[1]->push_back(p);
		else if (pos < 8) r[2]->push_back(p);
	}
	return r;
}
