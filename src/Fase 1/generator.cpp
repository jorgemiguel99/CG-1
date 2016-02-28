#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <windows.h>
#include <GL/glut.h>
#include <math.h>
//#include <GLUT/glut.h> //-- MAC

#define _USE_MATH_DEFINES

using namespace std;

// Global variables handling files
//string desktop = "C:\\Users\\Tiago\\Desktop\\Universidade\\3º Ano\\CG\\Projeto\\Fase 1\\Ficheiros 3d";
//string desktop = "/Users/zecarlos/Desktop/";// -- MAC
string filename,XMLfilename;

// Dimensions of the figures
float length, width, height, radius, slices, stacks;

// Variables used to move the camera
float moveX = 0.0f, moveY = 0.0f, moveZ = 0.0f, rotateX = 0.0f, rotateY = 0.0f, rotateZ = 0.0f, angle = 0;

// Global variable process input
vector<string> splitted;

// Global variable store 3d file read
vector<string> file3dRead;

// Operation name has 100 char at max
#define MAX 100

// Return the number to be used on the switch in main function; splitted[1] = operation name
int checkOP(string str) {
    if (str == "Plane" || str == "plane") return 1;
    else if (str == "Box" || str == "box") return 2;
    else if (str == "Sphere" || str == "sphere") return 3;
    else if (str == "Cone" || str == "cone") return 4;
    else return -1;
}

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

//Read the vertex of figure.3d files into a vector
vector<string> read3d(string figure) {
	vector<string> vecx;
	ifstream fileTXT(figure);
	string line;
	while (getline(fileTXT, line)) {
		//adding at the end of vector the line
		vecx.push_back(line);
	}
	int size = stoi(vecx[0]); // number of total vertex-- first line in .3d file
	// Testing the contents
	//for (int i=0; i <= size; i++) cout << vecx[i] << endl;
	//Returns vector with coordinates in each position
	//vecx[1..size]
	return vecx;
}

void drawRenderSceneFile3d(void){
  //Getting list of vertex from read3d
  //Split vector file3dRead from read3d into vertex
  int size = stoi(file3dRead[0]); // number of vertex
  vector<string> vrtx; // stores all vertex read from file3dRead vector<string> stof conversion later!!
  string line; // string processed of all file3dRead strings concatenated into one
  int conta=0;
  for (int i=1; i <= size; i++){
    if(conta!=0){ //adding ',' at the end of each line except the first
      //cout << line << endl;
      line = line + ", "+ file3dRead[i];
    }
    else{
      //cout << line << endl;
      line += file3dRead[i];
      conta++;
    }
  }
  //cout << line << "\n\n" << endl;
  vrtx = split(line, ',');

  //each line contains 3 float and we have size lines so we need to do size*3 in the condition
  //Testing the list of vertex read from file 3d
  //for(int j=1;j<size*3;j++) cout << vrtx[j] << endl;

  //Drawing process will now take place with vrtx contents starting in vrtx[1..size*3] because vrtx[0] has the number of lines

  // Clear buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set the Camera
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 5.0, 0.0, 1.0, 0.0, 0.0f, 1.0f, 0.0f);

  // Geometric Transformations
  glTranslatef(moveX, moveY, moveZ);
  glRotatef(angle, rotateX, rotateY, rotateZ);

  // Plane needs 6 vertex -- vrtx[1..18]
  if(size==6) {
    glBegin(GL_TRIANGLES);

    glColor3f(0, 1, 0);
    glVertex3f(stof(vrtx[1]), stof(vrtx[2]), stof(vrtx[3]));
    glVertex3f(stof(vrtx[4]), stof(vrtx[5]), stof(vrtx[6]));
    glVertex3f(stof(vrtx[7]), stof(vrtx[8]), stof(vrtx[9]));

    glColor3f(0, 1, 0);
    glVertex3f(stof(vrtx[10]), stof(vrtx[11]), stof(vrtx[12]));
    glVertex3f(stof(vrtx[13]), stof(vrtx[14]), stof(vrtx[15]));
    glVertex3f(stof(vrtx[16]), stof(vrtx[17]), stof(vrtx[18]));

  glEnd();
  }
  // Box needs 36 vertex  -- vrtx[1..108]
  else if(size==36) {
    glBegin(GL_TRIANGLES);

    // Lower face
    glColor3f(1, 0, 0);
    glVertex3f(stof(vrtx[1]), stof(vrtx[2]), stof(vrtx[3]));
    glVertex3f(stof(vrtx[4]), stof(vrtx[5]), stof(vrtx[6]));
    glVertex3f(stof(vrtx[7]), stof(vrtx[8]), stof(vrtx[9]));

    glColor3f(1, 0, 0);
    glVertex3f(stof(vrtx[10]), stof(vrtx[11]), stof(vrtx[12]));
    glVertex3f(stof(vrtx[13]), stof(vrtx[14]), stof(vrtx[15]));
    glVertex3f(stof(vrtx[16]), stof(vrtx[17]), stof(vrtx[18]));

    // Upper face
    glColor3f(1, 0, 0);
    glVertex3f(stof(vrtx[19]), stof(vrtx[20]), stof(vrtx[21]));
    glVertex3f(stof(vrtx[22]), stof(vrtx[23]), stof(vrtx[24]));
    glVertex3f(stof(vrtx[25]), stof(vrtx[26]), stof(vrtx[27]));

    glColor3f(1, 0, 0);
    glVertex3f(stof(vrtx[28]), stof(vrtx[29]), stof(vrtx[30]));
    glVertex3f(stof(vrtx[31]), stof(vrtx[32]), stof(vrtx[33]));
    glVertex3f(stof(vrtx[34]), stof(vrtx[35]), stof(vrtx[36]));

    // Right face
    glColor3f(0, 1, 0);
    glVertex3f(stof(vrtx[37]), stof(vrtx[38]), stof(vrtx[39]));
    glVertex3f(stof(vrtx[40]), stof(vrtx[41]), stof(vrtx[42]));
    glVertex3f(stof(vrtx[43]), stof(vrtx[44]), stof(vrtx[45]));

    glColor3f(0, 1, 0);
    glVertex3f(stof(vrtx[46]), stof(vrtx[47]), stof(vrtx[48]));
    glVertex3f(stof(vrtx[49]), stof(vrtx[50]), stof(vrtx[51]));
    glVertex3f(stof(vrtx[52]), stof(vrtx[53]), stof(vrtx[54]));

    // Left face
    glColor3f(0, 1, 0);
    glVertex3f(stof(vrtx[55]), stof(vrtx[56]), stof(vrtx[57]));
    glVertex3f(stof(vrtx[58]), stof(vrtx[59]), stof(vrtx[60]));
    glVertex3f(stof(vrtx[61]), stof(vrtx[62]), stof(vrtx[63]));

    glColor3f(0, 1, 0);
    glVertex3f(stof(vrtx[64]), stof(vrtx[65]), stof(vrtx[66]));
    glVertex3f(stof(vrtx[67]), stof(vrtx[68]), stof(vrtx[69]));
    glVertex3f(stof(vrtx[70]), stof(vrtx[71]), stof(vrtx[72]));

    // Front face
    glColor3f(0, 0, 1);
    glVertex3f(stof(vrtx[73]), stof(vrtx[74]), stof(vrtx[75]));
    glVertex3f(stof(vrtx[76]), stof(vrtx[77]), stof(vrtx[78]));
    glVertex3f(stof(vrtx[79]), stof(vrtx[80]), stof(vrtx[81]));

    glColor3f(0, 0, 1);
    glVertex3f(stof(vrtx[82]), stof(vrtx[83]), stof(vrtx[84]));
    glVertex3f(stof(vrtx[85]), stof(vrtx[86]), stof(vrtx[87]));
    glVertex3f(stof(vrtx[88]), stof(vrtx[89]), stof(vrtx[90]));

    // Back face
    glColor3f(0, 0, 1);
    glVertex3f(stof(vrtx[91]), stof(vrtx[92]), stof(vrtx[93]));
    glVertex3f(stof(vrtx[94]), stof(vrtx[95]), stof(vrtx[96]));
    glVertex3f(stof(vrtx[97]), stof(vrtx[98]), stof(vrtx[99]));

    glColor3f(0, 0, 1);
    glVertex3f(stof(vrtx[100]), stof(vrtx[101]), stof(vrtx[102]));
    glVertex3f(stof(vrtx[103]), stof(vrtx[104]), stof(vrtx[105]));
    glVertex3f(stof(vrtx[106]), stof(vrtx[107]), stof(vrtx[108]));

  glEnd();

  }
// Sphere needs 60 vertex -- CHANGE TO REAL NUMBER
  else if(size==60) {
      // Under construction...
  }
  //Cone needs 80 vertex -- CHANGE TO REAL NUMBER
  else if(size==80) {
  // Under construction...

}
}

// Print the figures on the .3d files
void print3d(string figure) {
	if (figure == "plane") {
		ofstream plane;
		plane.open(filename.c_str());
		plane << "6" << endl; // Total number of vertex

		plane << "0.0f, 0.0f, 0.0f" << endl;
		plane << length << ".0f" << ", 0.0f, 0.0f" << endl;
		plane << "0.0f, " << width << ".0f" << ", 0.0f" << endl;

		plane << length << ".0f" << ", 0.0f, 0.0f" << endl;
		plane << length << ".0f" << ", " << width << ".0f" << ", 0.0f" << endl;
		plane << "0.0f, " << width << ".0f" << ", 0.0f" << endl;

		plane.close();
	}
	else if (figure == "box") {
		ofstream box;
		box.open(filename.c_str());
		box << "36" << endl; // Total number of vertex

		// Lower face
		box << "0.0f, " << width << ".0f" << ", 0.0f" << endl;
		box << length << ".0f" << ", 0.0f, 0.0f" << endl;
		box << "0.0f, 0.0f, 0.0f" << endl;

		box << "0.0f, " << width << ".0f" << ", 0.0f" << endl;
		box << length << ".0f" << ", " << width << ".0f" << ", 0.0f" << endl;
		box << length << ".0f" << ", 0.0f, 0.0f" << endl;

		// Upper face
		box << "0.0f, 0.0f, " << height << ".0f" << endl;
		box << length << ".0f" << ", 0.0f, " << height << ".0f" << endl;
		box << "0.0f, " << width << ".0f, " << height << ".0f" << endl;

		box << length << ".0f" << ", 0.0f, " << height << ".0f" << endl;
		box << length << ".0f, " << width << ".0f, " << height << ".0f" << endl;
		box << "0.0f, " << width << ".0f, " << height << ".0f" << endl;

		// Right face
		box << length << ".0f" << ", 0.0f, 0.0f" << endl;
		box << length << ".0f" << ", " << width << ".0f" << ", 0.0f" << endl;
		box << length << ".0f" << ", 0.0f, " << height << ".0f" << endl;

		box << length << ".0f, " << width << ".0f, 0.0f" << endl;
		box << length << ".0f, " << width << ".0f, " << height << ".0f" << endl;
		box << length << ".0f" << ", 0.0f, " << height << ".0f" << endl;

		// Left Face
		box << "0.0f, " << width << ".0f" << ", 0.0f" << endl;
		box << "0.0f, 0.0f, " << height << ".0f" << endl;
		box << "0.0f, " << width << ".0f, " << height << ".0f" << endl;

		box << "0.0f, 0.0f, 0.0f" << endl;
		box << "0.0f, 0.0f, " << height << ".0f" << endl;
		box << "0.0f, " << width << ".0f" << ", 0.0f" << endl;

		// Front face
		box << "0.0f, 0.0f, 0.0f" << endl;
		box << length << ".0f" << ", 0.0f, 0.0f" << endl;
		box << "0.0f, 0.0f, " << height << ".0f" << endl;

		box << length << ".0f" << ", 0.0f, 0.0f" << endl;
		box << length << ".0f" << ", 0.0f, " << height << ".0f" << endl;
		box << "0.0f, 0.0f, " << height << ".0f" << endl;

		// Back face
		box << "0.0f, " << width << ".0f" << ", 0.0f" << endl;
		box << "0.0f, " << width << ".0f, " << height << ".0f" << endl;
		box << length << ".0f, " << width << ".0f, 0.0f" << endl;

		box << length << ".0f, " << width << ".0f, 0.0f" << endl;
		box << "0.0f, " << width << ".0f, " << height << ".0f" << endl;
		box << length << ".0f, " << width << ".0f, " << height << ".0f" << endl;

		box.close();
	}
}

void changeSize(int w, int h) {
    // Prevent a divide by zero, when window is too short
    if (h == 0) h = 1;

    // Compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);

    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

    // Return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the Camera
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 1.0, 0.0, 0.0f, 1.0f, 0.0f);

	// Geometric Transformations
	glTranslatef(moveX, moveY, moveZ);
	glRotatef(angle, rotateX, rotateY, rotateZ);

    // Plane
    if(checkOP(splitted[1])==1) {
		glBegin(GL_TRIANGLES);

		glColor3f(0, 1, 0);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(length, 0.0f, 0.0f);
		glVertex3f(0.0f, width, 0.0f);

		glColor3f(0, 1, 0);
		glVertex3f(length, 0.0f, 0.0f);
		glVertex3f(length, width, 0.0f);
		glVertex3f(0.0f, width, 0.0f);

		glEnd();

		print3d("plane");
    }
	// Box
    else if(checkOP(splitted[1])==2) {
		glBegin(GL_TRIANGLES);

		// Lower face
		glColor3f(1, 0, 0);
		glVertex3f(0.0f, width, 0.0f);
		glVertex3f(length, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);

		glColor3f(1, 0, 0);
		glVertex3f(0.0f, width, 0.0f);
		glVertex3f(length, width, 0.0f);
		glVertex3f(length, 0.0f, 0.0f);

		// Upper face
		glColor3f(1, 0, 0);
		glVertex3f(0.0f, 0.0f, height);
		glVertex3f(length, 0.0f, height);
		glVertex3f(0.0f, width, height);

		glColor3f(1, 0, 0);
		glVertex3f(length, 0.0f, height);
		glVertex3f(length, width, height);
		glVertex3f(0.0f, width, height);

		// Right face
		glColor3f(0, 1, 0);
		glVertex3f(length, 0.0f, 0.0f);
		glVertex3f(length, width, 0.0f);
		glVertex3f(length, 0.0f, height);

		glColor3f(0, 1, 0);
		glVertex3f(length, width, 0.0f);
		glVertex3f(length, width, height);
		glVertex3f(length, 0.0f, height);

		// Left face
		glColor3f(0, 1, 0);
		glVertex3f(0.0f, width, 0.0f);
		glVertex3f(0.0f, 0.0f, height);
		glVertex3f(0.0f, width, height);

		glColor3f(0, 1, 0);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, height);
		glVertex3f(0.0f, width, 0.0f);

		// Front face
		glColor3f(0, 0, 1);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(length, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, height);

		glColor3f(0, 0, 1);
		glVertex3f(length, 0.0f, 0.0f);
		glVertex3f(length, 0.0f, height);
		glVertex3f(0.0f, 0.0f, height);

		// Back face
		glColor3f(0, 0, 1);
		glVertex3f(0.0f, width, 0.0f);
		glVertex3f(0.0f, width, height);
		glVertex3f(length, width, 0.0f);

		glColor3f(0, 0, 1);
		glVertex3f(length, width, 0.0f);
		glVertex3f(0.0f, width, height);
		glVertex3f(length, width, height);

		glEnd();

		print3d("box");
    }
	// Sphere
    else if(checkOP(splitted[1])==3) {
        // Under construction...
    }
    else if(checkOP(splitted[1])==4) { //Cone
		// Under construction...
	}

    // End of frame
    glutSwapBuffers();
}

// Processing Keyboard Events
void translation(int key_code, int x, int y) {
	switch (key_code) {
		case GLUT_KEY_UP: moveY++; break;
		case GLUT_KEY_DOWN: moveY--; break;
		case GLUT_KEY_LEFT: moveX--; break;
		case GLUT_KEY_RIGHT: moveX++; break;
	}
	glutPostRedisplay();
}

void rotation(unsigned char key, int x, int y) {
	switch (key) {
		case '+': moveZ++; break;
		case '-': moveZ--; break;
		case 'd': angle -= 2; rotateZ = 1; rotateX = 0; rotateY = 0; break;
		case 'a': angle += 2; rotateZ = 1; rotateX = 0; rotateY = 0; break;
		case 'w': angle -= 2; rotateX = 1; rotateZ = 0; rotateY = 0; break;
		case 's': angle += 2; rotateX = 1; rotateZ = 0; rotateY = 0; break;
		case 'e': angle += 2; rotateY = 1; rotateZ = 0; rotateX = 0; break;
		case 'q': angle -= 2; rotateY = 1; rotateZ = 0; rotateX = 0; break;
		case 27 : exit(-1); // Esc -> leaves
	}
	glutPostRedisplay();
}

// Main function
int main(int argc, char **argv) {
  int read=0;
	string operationLine, operation;

	cout << "Insert your operation:" << endl;
	getline(cin, operationLine);
	splitted = split(operationLine, ' '); // Save a string on a vector with the substrings separated by a space

	// The function "stoff" transforms the content of a string in a float

	if (splitted[0] == "Generator" || splitted[0] == "generator") {
		switch (checkOP(splitted[1])) {
			case 1:	// Plane -> receives the length and the width
				if (splitted.size() == 5) {
					filename = splitted[4];
					length = stof(splitted[2]);
					width = stof(splitted[3]);
          //create plane.xml
          ofstream planeXML("plane.xml");
          planeXML << "<scene>\n\t<model file = î" << filename << "î />\n</scene>" << endl;
				}
				break;
			case 2: // Box -> receives the length, the width and the height
				if (splitted.size() == 6) {
			    filename = splitted[5];
					length = stof(splitted[2]);
					width = stof(splitted[3]);
					height = stof(splitted[4]);
          //create box.xml
          ofstream boxXML("box.xml");
          boxXML << "<scene>\n\t<model file = î" << filename << "î />\n</scene>" << endl;
				}
				break;
			case 3: // Sphere -> receives the radius, the slices and the stacks
				if (splitted.size() == 6) {
			    filename = splitted[5];
					radius = stof(splitted[2]);
					slices = stof(splitted[3]);
					stacks = stof(splitted[4]);
          //create sphere.xml
          ofstream sphereXML("sphere.xml");
          sphereXML << "<scene>\n\t<model file = î" << filename << "î />\n</scene>" << endl;
				}
				break;
			case 4: // Cone -> receives the base radius, the height, the slices and the stacks
				if (splitted.size() == 7) {
			    filename = splitted[6];
					radius = stof(splitted[2]);
					height = stof(splitted[3]);
					slices = stof(splitted[4]);
					stacks = stof(splitted[5]);
          //create cone.xml
          ofstream coneXML("cone.xml");
          coneXML << "<scene>\n\t<model file = î" << filename << "î />\n</scene>" << endl;
				}
				break;
			default:
				cout << "Not an operation of generator!\n" << endl;
				filename = "invalid.3d";
        //create invalid.3d
        ofstream invalid3d(filename);
        invalid3d << "Not an operation of generator!" << endl;
        //create invalid.xml
        ofstream invalidXML("invalid.xml");
        invalidXML << "<scene>\n\t<model file = î" << filename << "î />\n</scene>" << endl;
				break;
		}
	}
  else if(splitted[0] == "Read" || splitted[0] == "read"){
    //file to be read is splitted[1] = "filename.3d";
    file3dRead = read3d(splitted[1]);
    //file3dRead has file contents as strings and is used in drawRenderSceneFile3d() to draw that content
    read=1;
    //create read.xml
    ofstream readXML("read.xml");
    readXML << "<scene>\n\t<model file = î" << splitted[1] << "î />\n</scene>" << endl;
  }
	else {
    cout << "Try again! " << endl;
    filename = "invalid.3d";
    //create invalid.3d
    ofstream invalid3D(filename);
    invalid3D << "Not an operation through all program!" << endl;
    //create invalid.xml
    ofstream invalidxml("invalid.xml");
    invalidxml << "<scene>\n\t<model file = î" << filename << "î />\n</scene>" << endl;
  }

	// Init
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Solar System - Stage 1");

    // Callback registration
    if(read==1) {glutDisplayFunc(drawRenderSceneFile3d);} //draw the content of file read
    else {glutDisplayFunc(renderScene);} //draw normal content
    glutReshapeFunc(changeSize);

	   // Registration of the keyboard
	  glutSpecialFunc(translation);
	  glutKeyboardFunc(rotation);

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Enter GLUT's main loop
    glutMainLoop();

	return 1;
}
