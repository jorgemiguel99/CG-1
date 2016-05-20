#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GLUT/glut.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <math.h>

#define _PI_ 3.14159

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

GLfloat amb[4]  = {0.2, 0.2, 0.2, 1.0};
GLfloat diff[4] = {1.0, 1.0, 1.0, 1.0};
GLfloat pos[4]  = {1.0, 1.0, 1.0, 0.0};

int timebase = 0, frame = 0;

/* Texture related variables. */
unsigned int texID;
int cols, rows;

const int number = 3;
GLuint vertexCount, normalCount, textureCount, vertices[number];
float *v = new float[1000000];
float *n = new float[1000000];
float *t = new float[1000000];
void initVBO() {
	for (int i = 0;i<1000000;i++) {
		v[i]=0;
		n[i]=0;
		t[i]=0;
	}
}

void sphericalToCartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void prepareSphere(float r, float slices, float stacks) {
	initVBO();
	float x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4; // Vertex
	float xx1, yy1, zz1, xx2, yy2, zz2, xx3, yy3, zz3, xx4, yy4, zz4; // Normals
	float s1 ,t1 , s2, t2, s3, t3, s4, t4; // Textures
	float angleToRad = M_PI / 180.0f;

	float textureS = 0.0;
	float textureT = 0.0;
	float textureSIncrement = 1/slices;
	float textureTIncrement = 1/stacks;
	float longitudeIncrement = 360/stacks;
	float latitudeIncrement = 180/slices;

	for (float latitude = 0; latitude < 180; latitude += latitudeIncrement){

		for (float longitude = 0; longitude < 360; longitude += longitudeIncrement) {
			//Vertex 1
			z1=r*cos(latitude*angleToRad);
			x1=r*cos(longitude*angleToRad)*sin(latitude*angleToRad);
			y1=r*sin(longitude*angleToRad)*sin(latitude*angleToRad);

			//Normal 1
			zz1=cos(latitude*angleToRad);
			xx1=cos(longitude*angleToRad)*sin(latitude*angleToRad);
			yy1=sin(longitude*angleToRad)*sin(latitude*angleToRad);

			//Texture 1
			s1=longitude/360;
			t1=latitude/180;

			//Vertex 2
			z2=r*cos((latitude+latitudeIncrement)*angleToRad);
			x2=r*cos(longitude*angleToRad)*sin((latitude+latitudeIncrement)*angleToRad);
			y2=r*sin(longitude*angleToRad)*sin((latitude+latitudeIncrement)*angleToRad);

			//Normal 2
			zz2=cos((latitude+latitudeIncrement)*angleToRad);
			xx2=cos(longitude*angleToRad)*sin((latitude+latitudeIncrement)*angleToRad);
			yy2=sin(longitude*angleToRad)*sin((latitude+latitudeIncrement)*angleToRad);

			//Texture 2
			s2=longitude/360;
			t2=(latitude+latitudeIncrement)/(180);

			//Vertex 3
			z3=r*cos((latitude)*angleToRad);
			x3=r*cos((longitude+longitudeIncrement)*angleToRad)*sin((latitude)*angleToRad);
			y3=r*sin((longitude+longitudeIncrement)*angleToRad)*sin((latitude)*angleToRad);

			//Normal 3
			zz3=cos((latitude)*angleToRad);
			xx3=cos((longitude+longitudeIncrement)*angleToRad)*sin((latitude)*angleToRad);
			yy3=sin((longitude+longitudeIncrement)*angleToRad)*sin((latitude)*angleToRad);

			//Texture 3
			s3=(longitude+longitudeIncrement)/360;
			t3=(latitude)/180;

			//Vertex 4
			z4=r*cos((latitude+latitudeIncrement)*angleToRad);
			x4=r*cos((longitude+longitudeIncrement)*angleToRad)*sin((latitude+latitudeIncrement)*angleToRad);
			y4=r*sin((longitude+longitudeIncrement)*angleToRad)*sin((latitude+latitudeIncrement)*angleToRad);

			//Normal 4
			zz4=cos((latitude+latitudeIncrement)*angleToRad);
			xx4=cos((longitude+longitudeIncrement)*angleToRad)*sin((latitude+latitudeIncrement)*angleToRad);
			yy4=sin((longitude+longitudeIncrement)*angleToRad)*sin((latitude+latitudeIncrement)*angleToRad);

			//Texture 4
			s4=(longitude+longitudeIncrement)/360;
			t4=(latitude+latitudeIncrement)/180;

			// Filling VBO buffers
			// Vertex
			v[vertexCount] = x1;
			vertexCount++;
			v[vertexCount] = y1;
			vertexCount++;
			v[vertexCount] = z1;
			vertexCount++;

			v[vertexCount] = x2;
			vertexCount++;
			v[vertexCount] = y2;
			vertexCount++;
			v[vertexCount] = z2;
			vertexCount++;

			v[vertexCount] = x3;
			vertexCount++;
			v[vertexCount] = y3;
			vertexCount++;
			v[vertexCount] = z3;
			vertexCount++;

			v[vertexCount] = x3;
			vertexCount++;
			v[vertexCount] = y3;
			vertexCount++;
			v[vertexCount] = z3;
			vertexCount++;

			v[vertexCount] = x2;
			vertexCount++;
			v[vertexCount] = y2;
			vertexCount++;
			v[vertexCount] = z2;
			vertexCount++;

			v[vertexCount] = x4;
			vertexCount++;
			v[vertexCount] = y4;
			vertexCount++;
			v[vertexCount] = z4;
			vertexCount++;

			// Normals
			n[normalCount] = xx1;
			normalCount++;
			n[normalCount] = yy1;
			normalCount++;
			n[normalCount] = zz1;
			normalCount++;

			n[normalCount] = xx2;
			normalCount++;
			n[normalCount] = yy2;
			normalCount++;
			n[normalCount] = zz2;
			normalCount++;

			n[normalCount] = xx3;
			normalCount++;
			n[normalCount] = yy3;
			normalCount++;
			n[normalCount] = zz3;
			normalCount++;

			n[normalCount] = xx3;
			normalCount++;
			n[normalCount] = yy3;
			normalCount++;
			n[normalCount] = zz3;
			normalCount++;

			n[normalCount] = xx2;
			normalCount++;
			n[normalCount] = yy2;
			normalCount++;
			n[normalCount] = zz2;
			normalCount++;

			n[normalCount] = xx4;
			normalCount++;
			n[normalCount] = yy4;
			normalCount++;
			n[normalCount] = zz4;
			normalCount++;

			// Textures
			t[textureCount] = s1;
			textureCount++;
			t[textureCount] = t1;
			textureCount++;

			t[textureCount] = s2;
			textureCount++;
			t[textureCount] = t2;
			textureCount++;

			t[textureCount] = s3;
			textureCount++;
			t[textureCount] = t3;
			textureCount++;

			t[textureCount] = s3;
			textureCount++;
			t[textureCount] = t3;
			textureCount++;

			t[textureCount] = s2;
			textureCount++;
			t[textureCount] = t2;
			textureCount++;

			t[textureCount] = s4;
			textureCount++;
			t[textureCount] = t4;
			textureCount++;

			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, vertices[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (vertexCount-1), v, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vertices[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (vertexCount-1), n, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vertices[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (textureCount-1), t, GL_DYNAMIC_DRAW);
}



void renderScene(void) {
	float fps;
	int time;
	char s[64];

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	// light position
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	// light colors
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);

	gluLookAt(camX,camY,camZ,
					0.0,0.0,0.0,
				0.0f,1.0f,0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vertices[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertices[1]);
	glNormalPointer(GL_FLOAT,0,0);

	glBindBuffer(GL_ARRAY_BUFFER, vertices[2]);
	glTexCoordPointer(2,GL_FLOAT,0,0);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		fps = frame*1000.0/(time-timebase);
		timebase = time;
		frame = 0;
		sprintf(s, "FPS: %f6.2", fps);
		glutSetWindowTitle(s);
	}

// End of frame
	glutSwapBuffers();
}



void processKeys(int key, int xx, int yy)
{
	switch(key) {

		case GLUT_KEY_RIGHT:
						alfa -=0.1; break;

		case GLUT_KEY_LEFT:
						alfa += 0.1; break;

		case GLUT_KEY_UP :
						beta += 0.1f;
						if (beta > 1.5f)
							beta = 1.5f;
						break;

		case GLUT_KEY_DOWN:
						beta -= 0.1f;
						if (beta < -1.5f)
							beta = -1.5f;
						break;

		case GLUT_KEY_PAGE_DOWN : radius -= 0.1f;
			if (radius < 0.1f)
				radius = 0.1f;
			break;

		case GLUT_KEY_PAGE_UP: radius += 0.1f; break;

	}
	sphericalToCartesian();

}



void initGL() {

// OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
// init
	sphericalToCartesian();

	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

//Light
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Texturas.
	glEnable(GL_TEXTURE_2D);
	prepareSphere(1.5,30,30);
}


int main(int argc, char **argv) {

// init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("CG@DI-UM");

// callback registration
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// keyboard callback registration
	glutSpecialFunc(processKeys);
	GLenum err = glewInit();
	if (err != GLEW_OK)  printf(" Error initializing GLEW! \n");
	else printf("Initializing GLEW succeeded!\n");

	// Textures.
  cv::Mat img = cv::imread("earth.jpg");
  cols        = img.cols;
  rows        = img.rows;

  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cols, rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img.data);

	glEnableClientState(GL_VERTEX_ARRAY);
	glGenBuffers(number, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	initGL();
	glutMainLoop();
	return 0;
}
