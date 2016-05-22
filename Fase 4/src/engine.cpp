#include "read.h"

/* MAC INCLUDES */
/*
#include <GL/glew.h>
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
*/
/* tinyxml is included in file transformationTree.h
MAC -> Change location path
*/

//windows includeS
#include <IL/il.h>
#pragma comment(lib, "devil.lib")

#include <windows.h>
#include <Glew\glew.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#pragma comment(lib,"glew32.lib")


// Declared here and implemented after main in order to better organization of the code
void initGl();
void initVBOS();
void changeSize(int, int);
void renderScene(void);
void newMenu (int id_op);
void keyPressed(unsigned char, int, int);
void keyboardExtra(int key_code, int x, int y);
void processMouseButtons(int, int, int, int);
void processMouseMotion(int, int);
void bufferData(node_group* group, int* bufferIndex,int* imageIndex);

/* scene is a structure with xml group information*/
scene* sceneData;

// Global Variables to Transformations
float px = 0.0, py = 100.0, pz = 100.0;
int startX, startY, tracking = 0;
int alpha = 0, beta = 0, r = 50;

float rotate_y = 0;
float rotate_x = 0;
float rotate_z = 0;

float translate_y = 0;
float translate_x = 0;
float translate_z = 0;

// Global variable process input
vector<string> splitted;

int nBuffers; // Planetas + luas
GLuint *vertexCount, *buffer;

/* Texture related variables. */
int imageCount;
GLuint* texID;

// Files Path
const char* Path3d = "3d/";
const char* PathTextures = "textures/";
const char* PathXml = "xml/";


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


int main(int argc, char **argv) {
  string operationLine, operation;
  
	cout << "Insert the xml file name" << endl;

	getline(cin, operationLine);
	/*
	char* xmlName = (char*)malloc((5 + operationLine.size())*sizeof(char)); xmlName[0] = '\0';
	strcat(xmlName, PathXml);
	strcat(xmlName, operationLine.data());
	*/
    //sceneData = readXML("earth.xml",&nBuffers,&imageCount);
	sceneData = readXML(operationLine.data(),&nBuffers,&imageCount);
	if (!sceneData) { printf("Failed to read XML\n"); return 0; }
	else
	 printf("Loading file data\n");

	  //testTree(sceneData);

  glutInit(&argc, argv);

  // Sets up a double buffer with RGBA components and a depth component
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

  // Sets the window size to 512*512 square pixels
  glutInitWindowSize(1000, 1000);

  // Sets the window position to the upper left
  glutInitWindowPosition(0, 0);

  // Creates a window using internal glut functionality
  glutCreateWindow("Solar System - Stage 4");

  // Callback function responsible for the window's contents.
  glutDisplayFunc(renderScene);
  // Function to be callend when the window is created or resized.
  glutReshapeFunc(changeSize);
  // Function called when the event queue is empty.
  glutIdleFunc(renderScene);
	// Function called when a key is pressed.
  glutKeyboardFunc(keyPressed);
  glutSpecialFunc(keyboardExtra);
  // mouse callbacks
  glutMouseFunc(processMouseButtons);
  glutMotionFunc(processMouseMotion);
  //menu
  glutCreateMenu(newMenu);
  glutAddMenuEntry("Increase Translate x value",'j');
  glutAddMenuEntry("Increase Translate y value",'k');
  glutAddMenuEntry("Increase Translate z value",'l');
  glutAddMenuEntry("Decrease Translate x value",'q');
  glutAddMenuEntry("Decrease Translate y value",'w');
  glutAddMenuEntry("Decrease Translate z value",'e');
  glutAddMenuEntry("GL_FRONT & GL_FILL",'z');
  glutAddMenuEntry("GL_FRONT & GL_POINT",'x');
  glutAddMenuEntry("GL_FRONT & GL_FILL",'c');
  glutAddMenuEntry("GL_BACK & GL_LINE",'v');
  glutAddMenuEntry("GL_BACK & GL_POINT",'b');
  glutAddMenuEntry("GL_BACK & GL_FILL",'n');
  glutAddMenuEntry("GL_FRONT_AND_BACK & GL_LINE",'m');
  glutAddMenuEntry("GL_FRONT_AND_BACK & GL_POINT",'o');
  glutAddMenuEntry("GL_FRONT_AND_BACK & GL_FILL",'p');
  //button= GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, or GLUT MIDDLE_BUTTON
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  ilInit();

  glewInit();
  initGl();
  initVBOS();

  // enter GLUT's main cycle.
	glutMainLoop();
  /*

 else cout << "Try again! Amount of arguments incorrect" << endl;
}
else cout << "Try again! Use: draw <filename.3d>" << endl;
*/
return 1;
}
void initGl () {
	glClearColor(0.0,0.0,0.0,0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
  //default
	glPolygonMode(GL_FRONT, GL_FILL);
	//glPolygonMode(GL_FRONT,GL_LINE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	//Light
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glDisable(GL_COLOR_MATERIAL); // to enable light materials

// Texturas.
	glEnable(GL_TEXTURE_2D);
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


/*
	Coloca os pontos dos planetas no buffer* seguidos
	No mesmo buffer, no final dos planetas coloca as luas seguidas
*/


void initVBOS() {
	int Nb = nBuffers + imageCount * 3;

	buffer =(GLuint*) malloc(Nb * sizeof(GLuint));
	vertexCount = (GLuint*)malloc(Nb * sizeof(GLuint));
	glGenBuffers(Nb, buffer);


	texID = (GLuint*)malloc(imageCount * sizeof(GLuint));
	glGenTextures(imageCount, texID);


	int bufferIndex = 0,imageIndex = 0;
	node_group* root = sceneData->transformation_tree;
	bufferData(root,&bufferIndex,&imageIndex);

}

/* Buffer models points from files to vbos*/
void bufferData(node_group* group, int* bufferIndex, int* imageIndex){

	for (int i = 0; i < (int)group->model_file->size(); i++) {

		if ( i < (int)group->model_texture->size()) {
			// Windows textures
			unsigned int t;
			ilGenImages(1, &t);
			ilBindImage(t);

			ilLoadImage((ILstring)group->model_texture->at(i).data());

			unsigned int cols = ilGetInteger(IL_IMAGE_WIDTH);
			unsigned int rows = ilGetInteger(IL_IMAGE_HEIGHT);

			ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

			unsigned char *texData = ilGetData();

			//Textures MAC OS X
		  /*
			cv::Mat img = cv::imread(group->model_texture->at(i).data());
			unsigned int cols        = img.cols;
			unsigned int rows        = img.rows;
		  */
			glBindTexture(GL_TEXTURE_2D, texID[*imageIndex]);
		  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cols, rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData); // GL_RGBA MAC -> GL_BGR

	
			vector<float> filePoints = read3Dfile(group->model_file->at(i).data());


			vector<float>** vnt = dividePoints(filePoints);

			vertexCount[*bufferIndex] =  vnt[0]->size() / 3;

			glBindBuffer(GL_ARRAY_BUFFER, buffer[*bufferIndex]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vnt[0]->size()  , vnt[0]->data(), GL_DYNAMIC_DRAW);
			vertexCount[*bufferIndex] = vnt[0]->size() / 3;
			group->vboIndex->push_back(*bufferIndex);
			(*bufferIndex)++;

			glBindBuffer(GL_ARRAY_BUFFER, buffer[*bufferIndex]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vnt[1]->size(), vnt[1]->data(), GL_DYNAMIC_DRAW);
			(*bufferIndex)++;

			glBindBuffer(GL_ARRAY_BUFFER, buffer[*bufferIndex]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vnt[2]->size(), vnt[2]->data(), GL_DYNAMIC_DRAW);


			group->imageIndex->push_back(*imageIndex);
			*imageIndex = *imageIndex + 1;
			(*bufferIndex)++;
		}
		else {


			vector<float> filePoints = read3Dfile(group->model_file->at(i).data());

			vector<float>** vnt = dividePoints(filePoints);
			vector<float> points = *vnt[0];

			vertexCount[*bufferIndex] = points.size() / 3;
			glBindBuffer(GL_ARRAY_BUFFER, buffer[*bufferIndex]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount[*bufferIndex] * 3, points.data(), GL_STATIC_DRAW);
			group->vboIndex->push_back(*bufferIndex);
			(*bufferIndex)++;
		}
	}

	for (int i = 0; i < group->childIndex; i++) {
		bufferData(group->child[i], bufferIndex,imageIndex);
	}
}


void getCatmullRomPoint(float t, int *indices, float *res,float** p) {
	// catmull-rom matrix
	float m[4][4] = { { -0.5f, 1.5f, -1.5f, 0.5f },{ 1.0f, -2.5f, 2.0f, -0.5f },{ -0.5f, 0.0f, 0.5f, 0.0f },{ 0.0f, 1.0f, 0.0f, 0.0f } };

	res[0] = 0.0; res[1] = 0.0; res[2] = 0.0;

	float T[4] = { powf(t, 3), powf(t, 2), t, 1 }, mult[4];

	// mult = T * M
	for (int i = 0; i < 4; i++) {
		mult[i] = T[0] * m[0][i] + T[1] * m[1][i] + T[2] * m[2][i] + T[3] * m[3][i];
	}

	// res = mult * p
	for (int i = 0; i < 3; i++) {
		res[i] = mult[0] * p[indices[0]][i] + mult[1] * p[indices[1]][i] + mult[2] * p[indices[2]][i] + mult[3] * p[indices[3]][i];
	}
}

// given global t, returns the point in the curve
void getGlobalCatmullRomPoint(float gt, float *res, float** p,int point_count) {
	float t = gt * point_count; // this is the real global t
	int index = floor(t); // which segment
	t = t - index; // where within the segment

				   // indices store the points
	int indices[4];
	indices[0] = (index + point_count - 1) % point_count;
	indices[1] = (indices[0] + 1) % point_count;
	indices[2] = (indices[1] + 1) % point_count;
	indices[3] = (indices[2] + 1) % point_count;

	getCatmullRomPoint(t, indices, res,p);
}

void renderCatmullRomCurve(float** p,int point_count) {
	float gt = 0.0;

	glBegin(GL_LINE_LOOP);
	while (gt < 1) {
		float res[3];
		getGlobalCatmullRomPoint(gt, res,p,point_count);
		glVertex3f(res[0], res[1], res[2]);
		gt += 0.0001;
	}
	glEnd();
}

void renderTree(node_group* node) {
	if (node->pointIndex > 0) {
		float taux = glutGet(GLUT_ELAPSED_TIME) * 0.001 /node->translation_period;
		float t = taux - (int)taux;

		float res[3];
		renderCatmullRomCurve(node->p, node->pointIndex);
		getGlobalCatmullRomPoint(t, res,node->p,node->pointIndex);

		glTranslatef(res[0], res[1], res[2]);
	}
	else if (node->translate->size() > 0) {
		glTranslatef(node->translate->at(0), node->translate->at(1), node->translate->at(2));
	}
	if (node->rotateAxis->size() > 0) {
		glRotatef(node->rotateAxis->at(0), node->rotateAxis->at(1), node->rotateAxis->at(2), node->rotateAxis->at(3));
	}
	if (node->scale->size() > 0) {
		glScalef(node->scale->at(0), node->scale->at(1), node->scale->at(2));
	}
	if (node->rotate_period->size() > 0) {
		float currentTime = glutGet(GLUT_ELAPSED_TIME) * 0.001;
		int voltas = currentTime / node->rotate_period->at(0);
		/*
					t -> angle
					period -> 360
		*/
		float t = currentTime - voltas * node->rotate_period->at(0);

		float angle = t * 360 / node->rotate_period->at(0);

		glRotatef(angle, node->rotate_period->at(1), node->rotate_period->at(2), node->rotate_period->at(3));
	}

	if (node->model_coloured_specular->size() > 0) {
		GLfloat lightColor[] = { node->model_coloured_specular->at(0), node->model_coloured_specular->at(1), node->model_coloured_specular->at(2), 1.0f };
		//Specular light component
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);

	}
	if (node->model_coloured_diffuse->size() > 0) {
		GLfloat lightColor[] = { node->model_coloured_diffuse->at(0), node->model_coloured_diffuse->at(1), node->model_coloured_diffuse->at(2), 1.0f };
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	}
	if (node->model_coloured_emissive->size() > 0) {
		GLfloat lightColor[] = { node->model_coloured_emissive->at(0), node->model_coloured_emissive->at(1), node->model_coloured_emissive->at(2), 1.0f };
		glLightfv(GL_LIGHT0, GL_EMISSION, lightColor);
	}
	if (node->model_coloured_ambient->size() > 0) {
		GLfloat lightColor[] = { node->model_coloured_ambient->at(0), node->model_coloured_ambient->at(1), node->model_coloured_ambient->at(2), 1.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightColor);
	}


	for (int i = 0; i < (int)node->vboIndex->size(); i++) {
		if (i < (int)node->model_texture->size() ) {

			glBindTexture(GL_TEXTURE_2D, texID[node->imageIndex->at(i)]);
			glBindBuffer(GL_ARRAY_BUFFER, buffer[node->vboIndex->at(i)]);
			glVertexPointer(3, GL_FLOAT, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, buffer[node->vboIndex->at(i) + 1]);
		    glNormalPointer(GL_FLOAT, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, buffer[node->vboIndex->at(i) + 2]);
			glTexCoordPointer(2, GL_FLOAT, 0, 0);

			glDrawArrays(GL_TRIANGLES, 0, vertexCount[node->vboIndex->at(i)]);
			glBindTexture(GL_TEXTURE_2D, 0 );
		}
		else {

			glColor3f((float)node->colour[0] / 255, (float)node->colour[1] / 255, (float)node->colour[2] / 255);

			glBindBuffer(GL_ARRAY_BUFFER, buffer[node->vboIndex->at(i)]);
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount[node->vboIndex->at(i)]);

			glColor3f(1, 1, 1);
		}
	}

	for (int i = 0; i < node->childIndex; i++) {
		glPushMatrix();
		renderTree(node->child[i]);
		glPopMatrix();
	}
}

void renderScene(void) {
	// Clear buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set the camera.
	glLoadIdentity();


	for (int i = 0; i < sceneData->light_counter; i++)
		glLightfv(GL_LIGHT0, GL_POSITION, sceneData->lights[i]->position);

	//printf("Position light y:%f\n", sceneData->lights->at(0)->position[1]);
	//printf("type %s\n", sceneData->lights->at(0)->light_type);

	gluLookAt(px, py, pz,0.0,0.0,-1.0,0.0f,1.0f,0.0f);

	glRotatef( rotate_x, 1.0, 0.0, 0.0 );
	glRotatef( rotate_y, 0.0, 1.0, 0.0 );
	glRotatef( rotate_z, 0.0, 0.0, 1.0 );
	glTranslatef(translate_x,translate_y,translate_z);

	glPushMatrix();
	renderTree(sceneData->transformation_tree);
	glPopMatrix();

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
      else if (key == 'z' || key == 'Z') glPolygonMode(GL_FRONT,GL_FILL); //GL_FRONT & GL_FILL
      else if (key == 'x' || key == 'X') glPolygonMode(GL_FRONT,GL_POINT); //GL_FRONT & GL_POINT
      else if (key == 'c' || key == 'C') glPolygonMode(GL_FRONT,GL_FILL); //GL_FRONT & GL_FILL
      else if (key == 'v' || key == 'V') glPolygonMode(GL_BACK,GL_LINE); //GL_BACK & GL_LINE
      else if (key == 'b' || key == 'B') glPolygonMode(GL_BACK,GL_POINT); //GL_BACK & GL_POINT
      else if (key == 'n' || key == 'N') glPolygonMode(GL_BACK,GL_FILL); //GL_BACK & GL_FILL
      else if (key == 'm' || key == 'M') glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); //GL_FRONT_AND_BACK & GL_LINE
      else if (key == 'o' || key == 'O') glPolygonMode(GL_FRONT_AND_BACK,GL_POINT); //GL_FRONT_AND_BACK & GL_POINT
      else if (key == 'p' || key == 'P') glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); //GL_FRONT_AND_BACK & GL_FILL*/
			// Move only camera
			else if (key == 'g') py += 50;
			else if (key == 'h') py -= 50;
			else if (key == 'i') pz += 50;
			else if (key == 'u') pz += 50;
      else if (key==27) exit(-1);

      //when camera moves
      glutPostRedisplay();
}

void keyboardExtra(int key_code, int x, int y) {
	// Move only rotate and translate parameters
	/*  Right arrow key - decrease rotate_y by 5*/
	if (key_code == GLUT_KEY_RIGHT) rotate_y -= 5;
	/*  Left arrow key - increase rotate_y by 5*/
	else if (key_code == GLUT_KEY_LEFT) rotate_y += 5;
	/*  Up arrow key - increase rotate_x by 5*/
	else if (key_code == GLUT_KEY_UP) rotate_x += 5;
	/*  Down arrow key - decrease rotate_x by 5*/
	else if (key_code == GLUT_KEY_DOWN) rotate_x -= 5;
	/*   arrow key - decrease rotate_z by 5*/
	else if (key_code == GLUT_KEY_F9) rotate_z += 5;
	/*   arrow key - decrease rotate_z by 5*/
	else if (key_code == GLUT_KEY_F7) rotate_z -= 5;
	//when camera moves

	glutPostRedisplay();
}

void processMouseButtons(int button, int state, int xx, int yy)
{
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {

			r -= yy - startY;
			if (r < 3)
				r = 3.0;
		}
		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy)
{
	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {

		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 50)
			rAux = 50;
	}
	px = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
  pz = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	py = rAux *	sin(betaAux * 3.14 / 180.0);
}
