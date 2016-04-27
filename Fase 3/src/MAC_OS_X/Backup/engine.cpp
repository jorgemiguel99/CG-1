
/* MAC INCLUDES */
//#include <GLUT/glut.h>    
//#include <OpenGL/glu.h>   
//#include <OpenGL/gl.h>    

/* tinyxml is included in file transformationTree.h 
		MAC -> Change location path
*/

//windows includeS
 #include <windows.h>
#include<Glew\glew.h>
 #include <GL/glut.h>
#include <GL/glu.h>
 #include <GL/gl.h>




#include "read.h"



#pragma comment(lib,"glew32.lib")

// Declared here and implemented after main in order to better organization of the code
void initGl();
void initVBOS();
void changeSize(int, int);
void renderScene(void);
void keyPressed(unsigned char, int, int);
void newMenu (int id_op);
void keyboardExtra(int key_code, int x, int y);
void bufferData(node_group* group, int* bufferIndex);

/* scene is a structure with xml group information*/
scene* sceneData;

// Global Variables to Transformations
float px = 0.0, py = 100.0, pz = 100.0;

float rotate_y = 0;
float rotate_x = 0;
float rotate_z = 0;

float translate_y = 0;
float translate_x = 0;
float translate_z = 0;

// Global variable process input
vector<string> splitted;

// VBOS variables
#define MAX_STACKS 100
#define MAX_SLICES 100
#define VERTEX_MAX_SIZE (((MAX_STACKS - 2)*MAX_SLICES * 6 + 2 * MAX_SLICES * 3) * 3)
float vboVerticeBuffer[VERTEX_MAX_SIZE];
int nBuffers; // Planetas + luas
GLuint *vertexCount, *buffer;



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

	cout << "Insert your operation:" << endl;
	getline(cin, operationLine);
	splitted = split(operationLine, ' '); // Save a string on a vector with the substrings separated by a space

	// The function "stoff" transforms the content of a string in a float

  if (splitted[0] == "Draw" || splitted[0] == "draw") {	// Draw -> receives the name of the .3d file
    if (splitted.size() == 1){
      sceneData = readXML("SolarSystemXML.xml",&nBuffers);

//	  testTree(sceneData);
				
  glutInit(&argc, argv);

  // Sets up a double buffer with RGBA components and a depth component
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

  // Sets the window size to 512*512 square pixels
  glutInitWindowSize(1000, 1000);

  // Sets the window position to the upper left
  glutInitWindowPosition(0, 0);

  // Creates a window using internal glut functionality
  glutCreateWindow("Solar System - Stage 2");

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
  glutAddMenuEntry("GL_FRONT & GL_FILL",'z');
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
	glutAddMenuEntry("Stop Translation around Sun",'s');
	glutAddMenuEntry("Restart Translation around Sun",'r');
  //button= GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, or GLUT MIDDLE_BUTTON
  glutAttachMenu(GLUT_RIGHT_BUTTON);


  glewInit();
  initGl();
  initVBOS();

  // enter GLUT's main cycle.
	glutMainLoop();
  
 }
 else cout << "Try again! Amount of arguments incorrect" << endl;
}
else cout << "Try again! Use: draw <filename.3d>" << endl;
return 1;
}
void initGl () {
	glClearColor(0.0,0.0,0.0,0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
  //default
	glPolygonMode(GL_FRONT,GL_LINE);

	glEnableClientState(GL_VERTEX_ARRAY);
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
	buffer =(GLuint*) malloc(nBuffers * sizeof(GLuint));
	vertexCount = (GLuint*)malloc(nBuffers * sizeof(GLuint));
	glGenBuffers(nBuffers, buffer);

	int bufferIndex = 0; node_group* root = sceneData->transformation_tree;
	bufferData(root,&bufferIndex);

}
void bufferData(node_group* group, int* bufferIndex) {

	for (int i = 0; i < group->model_file->size(); i++) {
		string filename = group->model_file->at(i);
		vector<float> points = read3Dfile(filename);

		for (int p = 0; p < points.size(); p++)
			vboVerticeBuffer[p] = points[p];

		vertexCount[*bufferIndex] = points.size() / 3;
		glBindBuffer(GL_ARRAY_BUFFER, buffer[*bufferIndex]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount[*bufferIndex] * 3, vboVerticeBuffer, GL_STATIC_DRAW);
		group->vboIndex->push_back(*bufferIndex);
		(*bufferIndex)++;
	}
	for (int i = 0; i < group->childIndex; i++) {
		bufferData(group->child[i], bufferIndex);
	}
}


void renderTree(node_group* node) {

	if(node->translate->size()>0)
		glTranslatef(node->translate->at(0), node->translate->at(1), node->translate->at(2));

	if(node->rotate->size()>0)
		glRotatef(node->rotate->at(0), node->rotate->at(1), node->rotate->at(2), node->rotate->at(3));

	for (int i = 0; i < node->vboIndex->size(); i++) {
		glBindBuffer(GL_ARRAY_BUFFER, buffer[node->vboIndex->at(i)]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount[node->vboIndex->at(i)]);
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
	gluLookAt(px, py, pz,0.0,0.0,-1.0,0.0f,1.0f,0.0f);

	glRotatef( rotate_x, 1.0, 0.0, 0.0 );
	glRotatef( rotate_y, 0.0, 1.0, 0.0 );
	glRotatef( rotate_z, 0.0, 0.0, 1.0 );
	glTranslatef(translate_x,translate_y,translate_z);
    // Draw plane.
    /*
	glBegin(GL_TRIANGLES);

    glEnd();
	*/
	glPushMatrix();
	renderTree(sceneData->transformation_tree);
	glPopMatrix();
			// glutWireTorus(1.5, 10, 20, 20);

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

