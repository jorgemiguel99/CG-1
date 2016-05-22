#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GLUT/glut.h>

#include <math.h>

#define _PI_ 3.14159

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

GLuint normalCount,vertexCount, vertices[2];


int timebase = 0, frame = 0;


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

void prepareSphere(float r, float stacks, int slices) {
	int sphereCoordinatesSize;
	float *vertexB,*n;

	// a parte de cima e baixo contem  1 triangulo -> 3 vertices
	// restantes cont�m 2 triangulos -> 6 vertices
	sphereCoordinatesSize = ((stacks - 2)*slices * 6 + 2 * slices * 3) * 3;
	vertexB = (float *)malloc(sizeof(float) * sphereCoordinatesSize);
	n = (float *)malloc(sizeof(float) * sphereCoordinatesSize);
	int vertex = 0,normal=0;

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

			float zz1 = cos(polar) * cos(longitude);
			float xx1 = cos(polar) * sin(longitude);
			float yy1 = sin(polar);

			float zz2 = cos(polar2) * cos(longitude);
			float xx2 = cos(polar2) * sin(longitude);
			float yy2 = sin(polar2);

			float zz3 = cos(polar) * cos(longitude2);
			float xx3 = cos(polar) * sin(longitude2);
			float yy3 = sin(polar);

			float zz4 = cos(polar2) * cos(longitude2);
			float xx4 = cos(polar2) * sin(longitude2);
			float yy4 = sin(polar2);

			if (i == 0) {
				n[normal * 3 + 0] = xx3; n[normal * 3 + 1] = yy3; n[normal * 3 + 2] = zz3;
				vertexB[vertex * 3 + 0] = x3; vertexB[vertex * 3 + 1] = y3; vertexB[vertex * 3 + 2] = z3;
				vertex++;
				normal++;
				n[normal * 3 + 0] = xx4; n[normal * 3 + 1] = yy4; n[normal * 3 + 2] = zz4;
				vertexB[vertex * 3 + 0] = x4;	vertexB[vertex * 3 + 1] = y4;	vertexB[vertex * 3 + 2] = z4;
				vertex++;
				normal++;
				n[normal * 3 + 0] = xx2; n[normal * 3 + 1] = yy2; n[normal * 3 + 2] = zz2;
				vertexB[vertex * 3 + 0] = x2;	vertexB[vertex * 3 + 1] = y2;	vertexB[vertex * 3 + 2] = z2;
				vertex++;
				normal++;
			}
			else if (i == stacks - 1) {
				n[normal * 3 + 0] = xx1; n[normal * 3 + 1] = yy1; n[normal * 3 + 2] = zz1;
				vertexB[vertex * 3 + 0] = x1;	vertexB[vertex * 3 + 1] = y1;	vertexB[vertex * 3 + 2] = z1;
				vertex++;
				normal++;
				n[normal * 3 + 0] = xx3; n[normal * 3 + 1] = yy3; n[normal * 3 + 2] = zz3;
				vertexB[vertex * 3 + 0] = x3;	vertexB[vertex * 3 + 1] = y3;	vertexB[vertex * 3 + 2] = z3;
				vertex++;
				normal++;
				n[normal * 3 + 0] = xx2; n[normal * 3 + 1] = yy2; n[normal * 3 + 2] = zz2;
				vertexB[vertex * 3 + 0] = x2;	vertexB[vertex * 3 + 1] = y2;	vertexB[vertex * 3 + 2] = z2;
				vertex++;
				normal++;
			}
			else {
				n[normal * 3 + 0] = xx3; n[normal * 3 + 1] = yy3; n[normal * 3 + 2] = zz3;
				vertexB[vertex * 3 + 0] = x3; vertexB[vertex * 3 + 1] = y3; vertexB[vertex * 3 + 2] = z3;
				vertex++;
				normal++;
				n[normal * 3 + 0] = xx2; n[normal * 3 + 1] = yy2; n[normal * 3 + 2] = zz2;
				vertexB[vertex * 3 + 0] = x2;	vertexB[vertex * 3 + 1] = y2;	vertexB[vertex * 3 + 2] = z2;
				vertex++;
				normal++;
				n[normal * 3 + 0] = xx1; n[normal * 3 + 1] = yy1; n[normal * 3 + 2] = zz1;
				vertexB[vertex * 3 + 0] = x1;	vertexB[vertex * 3 + 1] = y1;	vertexB[vertex * 3 + 2] = z1;
				vertex++;
				normal++;

				n[normal * 3 + 0] = xx3; n[normal * 3 + 1] = yy3; n[normal * 3 + 2] = zz3;
				vertexB[vertex * 3 + 0] = x3;	vertexB[vertex * 3 + 1] = y3;	vertexB[vertex * 3 + 2] = z3;
				vertex++;
				normal++;
				n[normal * 3 + 0] = xx4; n[normal * 3 + 1] = yy4; n[normal * 3 + 2] = zz4;
				vertexB[vertex * 3 + 0] = x4;	vertexB[vertex * 3 + 1] = y4;	vertexB[vertex * 3 + 2] = z4;
				vertex++;
				normal++;
				n[normal * 3 + 0] = xx2; n[normal * 3 + 1] = yy2; n[normal * 3 + 2] = zz2;
				vertexB[vertex * 3 + 0] = x2;	vertexB[vertex * 3 + 1] = y2;	vertexB[vertex * 3 + 2] = z2;
				vertex++;
				normal++;
			}
	}
  }
	vertexCount = vertex;
	normalCount = normal;

	glGenBuffers(2, vertices);
	glBindBuffer(GL_ARRAY_BUFFER,vertices[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, vertexB, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER,vertices[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, n, GL_STATIC_DRAW);

	free(vertexB);
	free(n);

}

void drawSphere() {

	glBindBuffer(GL_ARRAY_BUFFER,vertices[0]);
	glVertexPointer(3,GL_FLOAT,0,0);

	glBindBuffer(GL_ARRAY_BUFFER,vertices[1]);
	glNormalPointer(GL_FLOAT,0,0);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

/*
glMaterialfv(GL_FRONT, component, array);
glMaterialf(GL_FRONT,GL_SHININESS,value);
Component:
       GL_DIFFUSE
       GL_AMBIENT
       GL_SPECULAR
       GL_EMISSION
       GL_AMBIENT_AND_DIFFUSE
*/

void renderScene(void) {
	//Cor vermelha
	float red[4] = {0.8f, 0.2f, 0.2f, 1.0f};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
	GLfloat amb[4] = {0.2, 0.2, 0.2, 1.0};
	GLfloat diff[4] = {1.0, 1.0, 1.0, 1.0};
	GLfloat pos[4] = {0.0, 0.0 ,1.0, 0.0};
	//float pos[4] = {1.0, 1.0, 1.0, 0.0};
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
	drawSphere();

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
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	prepareSphere(0.5,30,30);
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

// init GLEW
	glewInit();

	initGL();

	glutMainLoop();
	return 0;
}
