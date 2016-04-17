#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <stdio.h>
#include <Glew/glew.h>
#include <GL/glut.h>
#include <math.h>

#pragma comment(lib,"glew32.lib")

float alphaCam = 0.0f, betaCam = 0.0f, radius = 5.0f;
float camX = 0, camY, camZ = 5;

int startX, startY, tracking = 0;
int alpha = 0, beta = 0, r = 5;

int i, j;
int raio = 1, slices = 200, stacks = 300;

// Declare variables for VBO id
int nvertices = 12 * slices * stacks; // Number of vertices

const int n = 1;
int arraySize = sizeof(float) * nvertices;
GLuint buffers[n];

// Array for vertices
float *vertexB = new float[nvertices];

// Array for indices
unsigned int *indices = new unsigned int[nvertices];

// First – the starting index
int first = 0;

// Count – the number of vertices (not triangles) to draw
int count = nvertices / 3;

// Fill arrays with vertex values
void initVertex() {
	for (i = 0; i < nvertices; i++) vertexB[i] = 0;
}

// Convert spherical coordinates into cartesian
void sphericalToCartesian() {
	camX = radius * cos(betaCam) * sin(alphaCam);
	camY = radius * sin(betaCam);
	camZ = radius * cos(betaCam) * cos(alphaCam);
}

void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0) h = 1;

	// Compute window's aspect ratio
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

// Draw the cilinder with VBO's
void drawCylinder() {
	// Bind and semantics
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glColor3f(1.0f, 0.0f, 0.0f);

	// Draw without index list
	glDrawArrays(GL_TRIANGLES, first, count);
}

// Create the VBO's for the cylinder
void prepareCylinder() {
	// Enable buffer
	glEnableClientState(GL_VERTEX_ARRAY);

	int k = 0;

	// Allocate and fill vertex array
	float inicialPolar = - M_PI / 2;

	for (int i = 0; i < stacks; i++) {

		float polar = inicialPolar + i * (M_PI / stacks);
		float polar2 = inicialPolar + (i + 1) * (M_PI / stacks);

		for (int j = 0; j < slices; j++) {

			float longitude = j * (2 * M_PI / slices);
			float longitude2 = (j + 1) * (2 * M_PI / slices);

			vertexB[k++] = raio * cos(polar) * cos(longitude);
			vertexB[k++] = raio * cos(polar) * sin(longitude);
			vertexB[k++] = raio * sin(polar);

			vertexB[k++] = raio * cos(polar2) * cos(longitude);
			vertexB[k++] = raio * cos(polar2) * sin(longitude);
			vertexB[k++] = raio * sin(polar2);

			vertexB[k++] = raio * cos(polar) * cos(longitude2);
			vertexB[k++] = raio * cos(polar) * sin(longitude2);
			vertexB[k++] = raio * sin(polar);

			vertexB[k++] = raio * cos(polar2) * cos(longitude2);
			vertexB[k++] = raio * cos(polar2) * sin(longitude2);
			vertexB[k++] = raio * sin(polar2);
		}
	}

	// Generate VBO's
	glGenBuffers(n, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, arraySize, vertexB, GL_STATIC_DRAW);
}

void renderScene(void) {
	float pos[4] = {1.0, 1.0, 1.0, 0.0};

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

	drawCylinder();

	// End of frame
	glutSwapBuffers();
}

void processMouseButtons(int button, int state, int xx, int yy) {
	if (state == GLUT_DOWN) {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON) tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON) tracking = 2;
		else tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			r -= yy - startY;
			if (r < 3) r = 3.0;
		}
		tracking = 0;
	}
}

void processMouseMotion(int xx, int yy) {
	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking) return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {
		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0) betaAux = 85.0;
		else if (betaAux < -85.0) betaAux = -85.0;
		rAux = r;
	}
	else if (tracking == 2) {
		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3) rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux * sin(betaAux * 3.14 / 180.0);
}

int main(int argc, char **argv) {
	// Initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 320);
	glutCreateWindow("CG@DI-UM");

	// Callback registry
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	
	// Mouse callbacks
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	// Init glew
	GLenum err = glewInit();

	// OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Init
	if (err != GLEW_OK)  printf(" Error initializing GLEW! \n");
	else printf("Initializing GLEW succeeded!\n");
	initVertex();
	sphericalToCartesian();
	prepareCylinder();

	// Enter GLUTs main cycle
	glutMainLoop();
	return 0;
}