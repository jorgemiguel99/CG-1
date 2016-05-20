#include <stdlib.h>
#include <stdio.h>
#include <Glew/glew.h>
#include <GL/glut.h>
#include <IL/il.h>
#include <math.h>

#define _PI_ 3.14159

unsigned int texID;

char *filename;
unsigned int t, tw, th;

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

int counter = 0;
GLuint buffers[3];

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "devil.lib")

int timebase = 0, frame = 0;

void textureCreation() {
	unsigned char *texData;
	ilGenImages(1, &t);
	ilBindImage(t);
	ilLoadImage((ILstring) "earth.jpg");
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	texData = ilGetData();

	// create a texture slot
	glGenTextures(1, &texID);

	// bind the slot
	glBindTexture(GL_TEXTURE_2D, texID);

	// define texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// send texture data to OpenGL

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
}

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

void prepareSphere(float r, float stacks, int slices) {
	int sphereCoordinatesSize;
	float *vertices, *n, *textures;
	int counterTex = 0;

	// a parte de cima e baixo contem  1 triangulo -> 3 vertices
	// restantes cont�m 2 triangulos -> 6 vertices
	sphereCoordinatesSize = ((stacks - 2) * slices * 6 + 2 * slices * 3) * 3;
	vertices = (float*) malloc(sizeof(float) * sphereCoordinatesSize);
	n = (float*) malloc(sizeof(float) * sphereCoordinatesSize);
	textures = (float*)malloc(sizeof(float) * sphereCoordinatesSize);

	float inicialPolar = -_PI_ / 2;
	float x = 0, y = 0, incX = tw / slices, incY = th / stacks;

	for (int i = 0; i < stacks; i++) {

		float polar = inicialPolar + i * (_PI_ / stacks);
		float polar2 = inicialPolar + (i + 1) * (_PI_ / stacks);

		for (int j = 0; j < slices; j++) {

			float longitude = j * (2 * _PI_ / slices);
			float longitude2 = (j + 1) * (2 * _PI_ / slices);

			// Vertices
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

			// Normals
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

			// Textures
			float zzz1 = y;
			float xxx1 = x;

			float zzz2 = y + incY;
			float xxx2 = x;

			float zzz3 = y + incY;
			float xxx3 = x + incX;

			float zzz4 = y;
			float xxx4 = x + incX;

			if (i == 0) {
				n[counter * 3 + 0] = xx3; n[counter * 3 + 1] = yy3; n[counter * 3 + 2] = zz3;
				vertices[counter * 3 + 0] = x3; vertices[counter * 3 + 1] = y3; vertices[counter * 3 + 2] = z3;
				textures[counterTex * 2 + 0] = xxx3; textures[counterTex * 2 + 1] = zzz3; 
				counter++; counterTex++;

				n[counter * 3 + 0] = xx4; n[counter * 3 + 1] = yy4; n[counter * 3 + 2] = zz4;
				vertices[counter * 3 + 0] = x4;	vertices[counter * 3 + 1] = y4;	vertices[counter * 3 + 2] = z4;
				textures[counterTex * 2 + 0] = xxx4; textures[counterTex * 2 + 1] = zzz4;
				counter++; counterTex++;

				n[counter * 3 + 0] = xx2; n[counter * 3 + 1] = yy2; n[counter * 3 + 2] = zz2;
				vertices[counter * 3 + 0] = x2;	vertices[counter * 3 + 1] = y2;	vertices[counter * 3 + 2] = z2;
				textures[counterTex * 2 + 0] = xxx2; textures[counterTex * 2 + 1] = zzz2;
				counter++; counterTex++;
			}
			else if (i == stacks - 1) {
				n[counter * 3 + 0] = xx1; n[counter * 3 + 1] = yy1; n[counter * 3 + 2] = zz1;
				vertices[counter * 3 + 0] = x1;	vertices[counter * 3 + 1] = y1;	vertices[counter * 3 + 2] = z1;
				textures[counterTex * 2 + 0] = xxx1; textures[counterTex * 2 + 1] = zzz1;
				counter++; counterTex++;

				n[counter * 3 + 0] = xx3; n[counter * 3 + 1] = yy3; n[counter * 3 + 2] = zz3;
				vertices[counter * 3 + 0] = x3;	vertices[counter * 3 + 1] = y3;	vertices[counter * 3 + 2] = z3;
				textures[counterTex * 2 + 0] = xxx3; textures[counterTex * 2 + 1] = zzz3; 
				counter++; counterTex++;

				n[counter * 3 + 0] = xx2; n[counter * 3 + 1] = yy2; n[counter * 3 + 2] = zz2;
				vertices[counter * 3 + 0] = x2;	vertices[counter * 3 + 1] = y2;	vertices[counter * 3 + 2] = z2;
				textures[counterTex * 2 + 0] = xxx2; textures[counterTex * 2 + 1] = zzz2;
				counter++; counterTex++;
			}
			else {
				n[counter * 3 + 0] = xx3; n[counter * 3 + 1] = yy3; n[counter * 3 + 2] = zz3;
				vertices[counter * 3 + 0] = x3; vertices[counter * 3 + 1] = y3; vertices[counter * 3 + 2] = z3;
				textures[counterTex * 2 + 0] = xxx3; textures[counterTex * 2 + 1] = zzz3; 
				counter++; counterTex++;

				n[counter * 3 + 0] = xx2; n[counter * 3 + 1] = yy2; n[counter * 3 + 2] = zz2;
				vertices[counter * 3 + 0] = x2;	vertices[counter * 3 + 1] = y2;	vertices[counter * 3 + 2] = z2;
				textures[counterTex * 2 + 0] = xxx2; textures[counterTex * 2 + 1] = zzz2;
				counter++; counterTex++;

				n[counter * 3 + 0] = xx1; n[counter * 3 + 1] = yy1; n[counter * 3 + 2] = zz1;
				vertices[counter * 3 + 0] = x1;	vertices[counter * 3 + 1] = y1;	vertices[counter * 3 + 2] = z1;
				textures[counterTex * 2 + 0] = xxx1; textures[counterTex * 2 + 1] = zzz1;
				counter++; counterTex++;

				n[counter * 3 + 0] = xx3; n[counter * 3 + 1] = yy3; n[counter * 3 + 2] = zz3;
				vertices[counter * 3 + 0] = x3;	vertices[counter * 3 + 1] = y3;	vertices[counter * 3 + 2] = z3;
				textures[counterTex * 2 + 0] = xxx3; textures[counterTex * 2 + 1] = zzz3; 
				counter++; counterTex++;

				n[counter * 3 + 0] = xx4; n[counter * 3 + 1] = yy4; n[counter * 3 + 2] = zz4;
				vertices[counter * 3 + 0] = x4;	vertices[counter * 3 + 1] = y4;	vertices[counter * 3 + 2] = z4;
				textures[counterTex * 2 + 0] = xxx4; textures[counterTex * 2 + 1] = zzz4;
				counter++; counterTex++;

				n[counter * 3 + 0] = xx2; n[counter * 3 + 1] = yy2; n[counter * 3 + 2] = zz2;
				vertices[counter * 3 + 0] = x2;	vertices[counter * 3 + 1] = y2;	vertices[counter * 3 + 2] = z2;
				textures[counterTex * 2 + 0] = xxx2; textures[counterTex * 2 + 1] = zzz2;
				counter++; counterTex++;
			}

			x += incX;
		}
		y += incY;
		x = 0;
	}

	glGenBuffers(3, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * counter * 3, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * counter * 3, n, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * counterTex, textures, GL_STATIC_DRAW);
	
	free(vertices);
	free(n);
	free(textures);
}

void drawSphere() {

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glNormalPointer(GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, counter);
}

void renderScene(void) {
	// Cor vermelha
	float red[4] = { 0.8f, 0.2f, 0.2f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
	float amb[4] = { 0.2, 0.2, 0.2, 1.0 };
	float diff[4] = { 1.0, 1.0, 1.0, 1.0 };
	float pos[4] = { 0.0, 0.0 ,1.0, 0.0 };

	float fps;
	int time;
	char s[64];

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	// light position
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	// light colors
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);
	drawSphere();

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		fps = frame*1000.0 / (time - timebase);
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
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	prepareSphere(0.5, 30, 30);
}


int main(int argc, char **argv) {

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

	glEnable(GL_TEXTURE_2D);

	// IL Init
	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	// Texture creation
	textureCreation();

	initGL();

	glutMainLoop();
	return 0;
}
