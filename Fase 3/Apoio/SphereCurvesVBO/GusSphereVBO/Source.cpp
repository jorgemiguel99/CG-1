#include <stdlib.h>
#include <stdio.h>
#include <Glew/glew.h>
#include <GL/glut.h>

#include <math.h>

#define _PI_ 3.14159

#pragma comment(lib,"glew32.lib")

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ = 5;

int startX, startY, tracking = 0;

int alphaMouse = 0, betaMouse = 0, rMouse = 5;

GLuint vertexCount, vertices;

// Sphere variables
float stacks = 30;
float slices = 30;
float sphereRadius = 1;

#define POINT_COUNT 4

// Points that make up the loop for catmull-rom interpolation
float p[POINT_COUNT][3] = { { 1, -1, 0 }, { 1, 1, 0 }, { -1, 1, 0 }, { -1, -1, 0 } };

void sphericalToCartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0) h = 1;

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

	float *vertexB;
	
	float r = sphereRadius;

	// a parte de cima e baixo contem  1 triangulo -> 3 vertices 
	// restantes cont�m 2 triangulos -> 6 vertices
	int sphereCoordinatesSize = ((stacks - 2)*slices * 6 + 2 * slices * 3) * 3;
	vertexB = (float *)malloc(sizeof(float) * sphereCoordinatesSize);


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

	free(vertexB);

}

void drawSphere() {

	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glColor3f(1.0f, 0.0f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void getCatmullRomPoint(float t, int *indices, float *res) {
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
void getGlobalCatmullRomPoint(float gt, float *res) {
	float t = gt * POINT_COUNT; // this is the real global t
	int index = floor(t); // which segment
	t = t - index; // where within the segment

	// indices store the points
	int indices[4];
	indices[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
	indices[1] = (indices[0] + 1) % POINT_COUNT;
	indices[2] = (indices[1] + 1) % POINT_COUNT;
	indices[3] = (indices[2] + 1) % POINT_COUNT;

	getCatmullRomPoint(t, indices, res);
}

void renderCatmullRomCurve() {
	float gt = 0.0;

	glBegin(GL_LINE_LOOP);
	while (gt < 1) {
		float res[3];
		getGlobalCatmullRomPoint(gt, res);
		glVertex3f(res[0], res[1], res[2]);
		gt += 0.0001;
	}
	glEnd();
}

void renderScene(void) {
	static float t = 0;
	float pos[4] = {1.0, 1.0, 1.0, 0.0}, res[3];
	char s[64];

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

	getGlobalCatmullRomPoint(t, res);
	renderCatmullRomCurve();

	glPushMatrix();
	glTranslatef(res[0], res[1], res[2]);
	drawSphere();
	glPopMatrix();

	// End of frame
	glutSwapBuffers();

	t += 0.01;
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
			alphaMouse += (xx - startX);
			betaMouse += (yy - startY);
		}
		else if (tracking == 2) {
			rMouse -= yy - startY;
			if (rMouse < 3) rMouse = 3.0;
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
		alphaAux = alphaMouse + deltaX;
		betaAux = betaMouse + deltaY;

		if (betaAux > 85.0) betaAux = 85.0;
		else if (betaAux < -85.0) betaAux = -85.0;
		rAux = rMouse;
	}
	else if (tracking == 2) {
		alphaAux = alphaMouse;
		betaAux = betaMouse;
		rAux = rMouse - deltaY;
		if (rAux < 3) rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux * sin(betaAux * 3.14 / 180.0);
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

	// mouse callbacks
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	// init GLEW
	glewInit(); 

	initGL();

	glutMainLoop();
}