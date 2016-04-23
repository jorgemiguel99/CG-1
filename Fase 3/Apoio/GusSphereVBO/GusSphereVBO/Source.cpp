#include <stdlib.h>
#include <stdio.h>
#include <glew.h>
#include <GL/glut.h>

#include <math.h>

#include <iostream>

#define _PI_ 3.14159

#pragma comment(lib,"glew32.lib")

using namespace std;


float alfa = 0.0f, beta = 0.0f, radius = 5;
float camX, camY, camZ;

GLuint vertexCount, vertices;

int timebase = 0, frame = 0;

// Sphere variables
int stacks = 30;
int slices = 30;
float sphereRadius = 1;

#define stacksC 30
#define slicesC 30
#define VERTEXB_SIZE (((stacksC - 2)*slicesC * 6 + 2 * slicesC * 3) * 3)

void sphericalToCartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45, ratio, 1, 1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}



void prepareSphere() {

	
	
	float r = sphereRadius;

	// a parte de cima e baixo contem  1 triangulo -> 3 vertices 
	// restantes contêm 2 triangulos -> 6 vertices
	int sphereCoordinatesSize = ((stacks - 2)*slices * 6 + 2 * slices * 3) * 3;
	

	//float *vertexB;
	//vertexB = (float *)malloc(sizeof(float) * sphereCoordinatesSize);
	
	float vertexB[VERTEXB_SIZE];
	
	int vertex = 0;

	float inicialPolar = -_PI_ / 2;

	for (int i = 0; i < stacks; i++) {

		float polar = inicialPolar + i * (_PI_ / stacks);
		float polar2 = inicialPolar + (i + 1) * (_PI_ / stacks);

		for (int j = 0; j < slices; j++) {

			float longitude = j * (2 * _PI_ / slices);
			float longitude2 = (j + 1) * (2 * _PI_ / slices);


			float z1 = r * cos(polar) * cos(longitude);
			float x1 = r * cos(polar) * sin(longitude);
			float y1 = r * sin(polar);

			float z2 = r * cos(polar2) * cos(longitude);
			float x2 = r * cos(polar2) * sin(longitude);
			float y2 = r * sin(polar2);

			float z3 = r * cos(polar) * cos(longitude2);
			float x3 = r * cos(polar) * sin(longitude2);
			float y3 = r * sin(polar);

			float z4 = r * cos(polar2) * cos(longitude2);
			float x4 = r * cos(polar2) * sin(longitude2);
			float y4 = r * sin(polar2);

			if (i == 0) {

				vertexB[vertex * 3 + 0] = x3; vertexB[vertex * 3 + 1] = y3; vertexB[vertex * 3 + 2] = z3;
				vertex++;
				vertexB[vertex * 3 + 0] = x4;	vertexB[vertex * 3 + 1] = y4;	vertexB[vertex * 3 + 2] = z4;
				vertex++;
				vertexB[vertex * 3 + 0] = x2;	vertexB[vertex * 3 + 1] = y2;	vertexB[vertex * 3 + 2] = z2;
				vertex++;
			}
			else if (i == stacks - 1) {
				vertexB[vertex * 3 + 0] = x1;	vertexB[vertex * 3 + 1] = y1;	vertexB[vertex * 3 + 2] = z1;
				vertex++;
				vertexB[vertex * 3 + 0] = x3;	vertexB[vertex * 3 + 1] = y3;	vertexB[vertex * 3 + 2] = z3;
				vertex++;
				vertexB[vertex * 3 + 0] = x2;	vertexB[vertex * 3 + 1] = y2;	vertexB[vertex * 3 + 2] = z2;
				vertex++;
			}
			else {
				vertexB[vertex * 3 + 0] = x3; vertexB[vertex * 3 + 1] = y3; vertexB[vertex * 3 + 2] = z3;
				vertex++;
				vertexB[vertex * 3 + 0] = x2;	vertexB[vertex * 3 + 1] = y2;	vertexB[vertex * 3 + 2] = z2;
				vertex++;
				vertexB[vertex * 3 + 0] = x1;	vertexB[vertex * 3 + 1] = y1;	vertexB[vertex * 3 + 2] = z1;
				vertex++;

				vertexB[vertex * 3 + 0] = x3;	vertexB[vertex * 3 + 1] = y3;	vertexB[vertex * 3 + 2] = z3;
				vertex++;
				vertexB[vertex * 3 + 0] = x4;	vertexB[vertex * 3 + 1] = y4;	vertexB[vertex * 3 + 2] = z4;
				vertex++;
				vertexB[vertex * 3 + 0] = x2;	vertexB[vertex * 3 + 1] = y2;	vertexB[vertex * 3 + 2] = z2;
				vertex++;
			}
		}
	}

	vertexCount = vertex;

	glGenBuffers(1, &vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, vertexB, GL_STATIC_DRAW);

	//free(vertexB);

}


void drawSphere() {

	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}


void renderScene(void) {

	float pos[4] = { 1.0, 1.0, 1.0, 0.0 };
	float fps;
	int time;
	char s[64];

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	drawSphere();

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		fps = frame*1000.0 / (time - timebase);
		timebase = time;
		frame = 0;
		 sprintf_s(s, "FPS: %f6.2", fps);
		glutSetWindowTitle(s);
	}

	// End of frame
	glutSwapBuffers();
}



void processKeys(int key, int xx, int yy)
{
	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 0.1f;
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
	glEnableClientState(GL_VERTEX_ARRAY);

	prepareSphere();
}


void main(int argc, char **argv) {
	// init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 320);
	glutCreateWindow("CG@DI-UM");

	// callback registration
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	// keyboard callback registration
	glutSpecialFunc(processKeys);

	// init GLEW
	glewInit(); 

	initGL();

	glutMainLoop();
}