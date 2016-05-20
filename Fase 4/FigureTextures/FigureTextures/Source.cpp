#include <stdlib.h>
#include <stdio.h>
#include <Glew/glew.h>
#include <GL/glut.h>
#include <IL/il.h>
#include <math.h>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "devil.lib")

#define _PI_ 3.14159

#define PLANE 1
#define BOX 2
#define CONE 3
#define CILINDER 4

unsigned int texID;
int figure = -1;

char *filename;
unsigned int t, tw, th;

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

GLuint buffers[3], vertexCount;

int timebase = 0, frame = 0;

void textureCreation() {
	unsigned char *texData;
	ilGenImages(1, &t);
	ilBindImage(t);
	if(figure == PLANE) ilLoadImage((ILstring) "plane.jpg");
	else if(figure == BOX) ilLoadImage((ILstring) "cilinder.jpg");
	else ilLoadImage((ILstring) "cilinder.jpg");
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

void preparePlane(float length, float width) {
	float *vertexB = (float*)malloc(sizeof(float) * 6 * 3);
	float *normalB = (float*)malloc(sizeof(float) * 6 * 3);
	float *textureB = (float*)malloc(sizeof(float) * 6 * 2);

	// Vertices
	vertexB[0] = 0.0f; vertexB[1] = 0.0f; vertexB[2] = 0.0f;
	vertexB[3] = length; vertexB[4] = 0.0f; vertexB[5] = 0.0f;
	vertexB[6] = 0.0f; vertexB[7] = width; vertexB[8] = 0.0f;

	vertexB[9] = length; vertexB[10] = 0.0f; vertexB[11] = 0.0f;
	vertexB[12] = length; vertexB[13] = width; vertexB[14] = 0.0f;
	vertexB[15] = 0.0f; vertexB[16] = width; vertexB[17] = 0.0f;

	// Normals
	normalB[0] = 0.0f; normalB[1] = 0.0f; normalB[2] = 1.0f;
	normalB[3] = 0.0f; normalB[4] = 0.0f; normalB[5] = 1.0f;
	normalB[6] = 0.0f; normalB[7] = 0.0f; normalB[8] = 1.0f;

	normalB[9] = 0.0f; normalB[10] = 0.0f; normalB[11] = 1.0f;
	normalB[12] = 0.0f; normalB[13] = 0.0f; normalB[14] = 1.0f;
	normalB[15] = 0.0f; normalB[16] = 0.0f; normalB[17] = 1.0f;

	// Textures
	textureB[0] = 0.0f; textureB[1] = 0.0f; 
	textureB[2] = 1.0f; textureB[3] = 0.0f; 
	textureB[4] = 0.0f; textureB[5] = 1.0f; 

	textureB[6] = 1.0f; textureB[7] = 0.0f; 
	textureB[8] = 1.0f; textureB[9] = 1.0f; 
	textureB[10] = 0.0f; textureB[11] = 1.0f; 

	vertexCount = 6;

	glGenBuffers(3, buffers);

	// Save data on the array of vertices
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, vertexB, GL_STATIC_DRAW);

	// Save data on the array of vertex
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, normalB, GL_STATIC_DRAW);

	// Sava data on the texture array
	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * vertexCount, textureB, GL_STATIC_DRAW);

	free(normalB);
	free(vertexB);
	free(textureB);
}

void prepareBox(float length, float width, float height) {
	float *vertexB = (float*)malloc(sizeof(float) * 6 * 6 * 3);
	float *normalB = (float*)malloc(sizeof(float) * 6 * 6 * 3);
	float *textureB = (float*)malloc(sizeof(float) * 6 * 6 * 2);

	int i = 0;

	// Vertices -------------------------------------------------------------
	// Front face
	vertexB[i++] = 0.0f; vertexB[i++] = 0.0f; vertexB[i++] = 0.0f;
	vertexB[i++] = width; vertexB[i++] = 0.0f; vertexB[i++] = 0.0f;
	vertexB[i++] = 0.0f; vertexB[i++] = height; vertexB[i++] = 0.0f;

	vertexB[i++] = width; vertexB[i++] = 0.0f; vertexB[i++] = 0.0f;
	vertexB[i++] = width; vertexB[i++] = height; vertexB[i++] = 0.0f;
	vertexB[i++] = 0.0f; vertexB[i++] = height; vertexB[i++] = 0.0f;

	// Back face
	vertexB[i++] = width; vertexB[i++] = 0.0f; vertexB[i++] = -length;
	vertexB[i++] = 0.0f; vertexB[i++] = 0.0f; vertexB[i++] = -length;
	vertexB[i++] = 0.0f; vertexB[i++] = height; vertexB[i++] = -length;

	vertexB[i++] = 0.0f; vertexB[i++] = height; vertexB[i++] = -length;
	vertexB[i++] = width; vertexB[i++] = height; vertexB[i++] = -length;
	vertexB[i++] = width; vertexB[i++] = 0.0f; vertexB[i++] = -length;

	// Right face
	vertexB[i++] = width; vertexB[i++] = 0.0f; vertexB[i++] = 0.0f;
	vertexB[i++] = width; vertexB[i++] = 0.0f; vertexB[i++] = -length;
	vertexB[i++] = width; vertexB[i++] = height; vertexB[i++] = 0.0f;

	vertexB[i++] = width; vertexB[i++] = 0.0f; vertexB[i++] = -length;
	vertexB[i++] = width; vertexB[i++] = height; vertexB[i++] = -length;
	vertexB[i++] = width; vertexB[i++] = height; vertexB[i++] = 0.0f;

	// Left face
	vertexB[i++] = 0.0f; vertexB[i++] = 0.0f; vertexB[i++] = 0.0f;
	vertexB[i++] = 0.0f; vertexB[i++] = height; vertexB[i++] = 0.0f;
	vertexB[i++] = 0.0f; vertexB[i++] = 0.0f; vertexB[i++] = -length;

	vertexB[i++] = 0.0f; vertexB[i++] = 0.0f; vertexB[i++] = -length;
	vertexB[i++] = 0.0f; vertexB[i++] = height; vertexB[i++] = 0.0f;
	vertexB[i++] = 0.0f; vertexB[i++] = height; vertexB[i++] = -length;

	// Upper face
	vertexB[i++] = 0.0f; vertexB[i++] = height; vertexB[i++] = 0.0f;
	vertexB[i++] = width; vertexB[i++] = height; vertexB[i++] = 0.0f;
	vertexB[i++] = 0.0f; vertexB[i++] = height; vertexB[i++] = -length;

	vertexB[i++] = 0.0f; vertexB[i++] = height; vertexB[i++] = -length;
	vertexB[i++] = width; vertexB[i++] = height; vertexB[i++] = 0.0f;
	vertexB[i++] = width; vertexB[i++] = height; vertexB[i++] = -length;

	// Lower face
	vertexB[i++] = 0.0f; vertexB[i++] = 0.0f; vertexB[i++] = 0.0f;
	vertexB[i++] = 0.0f; vertexB[i++] = 0.0f; vertexB[i++] = -length;
	vertexB[i++] = width; vertexB[i++] = 0.0f; vertexB[i++] = 0.0f;

	vertexB[i++] = 0.0f; vertexB[i++] = 0.0f; vertexB[i++] = -length;
	vertexB[i++] = width; vertexB[i++] = 0.0f; vertexB[i++] = -length;
	vertexB[i++] = width; vertexB[i++] = 0.0f; vertexB[i++] = 0.0f;

	i = 0;

	// Normals --------------------------------------------------------------
	// Front face
	normalB[i++] = 0.0f; normalB[i++] = 0.0f; normalB[i++] = 1.0f;
	normalB[i++] = 0.0f; normalB[i++] = 0.0f; normalB[i++] = 1.0f;
	normalB[i++] = 0.0f; normalB[i++] = 0.0f; normalB[i++] = 1.0f;

	normalB[i++] = 0.0f; normalB[i++] = 0.0f; normalB[i++] = 1.0f;
	normalB[i++] = 0.0f; normalB[i++] = 0.0f; normalB[i++] = 1.0f;
	normalB[i++] = 0.0f; normalB[i++] = 0.0f; normalB[i++] = 1.0f;

	// Back face
	normalB[i++] = 0.0f; normalB[i++] = 0.0f; normalB[i++] = -1.0f;
	normalB[i++] = 0.0f; normalB[i++] = 0.0f; normalB[i++] = -1.0f;
	normalB[i++] = 0.0f; normalB[i++] = 0.0f; normalB[i++] = -1.0f;

	normalB[i++] = 0.0f; normalB[i++] = 0.0f; normalB[i++] = -1.0f;
	normalB[i++] = 0.0f; normalB[i++] = 0.0f; normalB[i++] = -1.0f;
	normalB[i++] = 0.0f; normalB[i++] = 0.0f; normalB[i++] = -1.0f;

	// Right face
	normalB[i++] = 1.0f; normalB[i++] = 0.0f; normalB[i++] = 0.0f;
	normalB[i++] = 1.0f; normalB[i++] = 0.0f; normalB[i++] = 0.0f;
	normalB[i++] = 1.0f; normalB[i++] = 0.0f; normalB[i++] = 0.0f;

	normalB[i++] = 1.0f; normalB[i++] = 0.0f; normalB[i++] = 0.0f;
	normalB[i++] = 1.0f; normalB[i++] = 0.0f; normalB[i++] = 0.0f;
	normalB[i++] = 1.0f; normalB[i++] = 0.0f; normalB[i++] = 0.0f;

	// Left face
	normalB[i++] = -1.0f; normalB[i++] = 0.0f; normalB[i++] = 0.0f;
	normalB[i++] = -1.0f; normalB[i++] = 0.0f; normalB[i++] = 0.0f;
	normalB[i++] = -1.0f; normalB[i++] = 0.0f; normalB[i++] = 0.0f;

	normalB[i++] = -1.0f; normalB[i++] = 0.0f; normalB[i++] = 0.0f;
	normalB[i++] = -1.0f; normalB[i++] = 0.0f; normalB[i++] = 0.0f;
	normalB[i++] = -1.0f; normalB[i++] = 0.0f; normalB[i++] = 0.0f;

	// Upper face
	normalB[i++] = 0.0f; normalB[i++] = 1.0f; normalB[i++] = 0.0f;
	normalB[i++] = 0.0f; normalB[i++] = 1.0f; normalB[i++] = 0.0f;
	normalB[i++] = 0.0f; normalB[i++] = 1.0f; normalB[i++] = 0.0f;

	normalB[i++] = 0.0f; normalB[i++] = 1.0f; normalB[i++] = 0.0f;
	normalB[i++] = 0.0f; normalB[i++] = 1.0f; normalB[i++] = 0.0f;
	normalB[i++] = 0.0f; normalB[i++] = 1.0f; normalB[i++] = 0.0f;

	// Lower face
	normalB[i++] = 0.0f; normalB[i++] = -1.0f; normalB[i++] = 0.0f;
	normalB[i++] = 0.0f; normalB[i++] = -1.0f; normalB[i++] = 0.0f;
	normalB[i++] = 0.0f; normalB[i++] = -1.0f; normalB[i++] = 0.0f;

	normalB[i++] = 0.0f; normalB[i++] = -1.0f; normalB[i++] = 0.0f;
	normalB[i++] = 0.0f; normalB[i++] = -1.0f; normalB[i++] = 0.0f;
	normalB[i++] = 0.0f; normalB[i++] = -1.0f; normalB[i++] = 0.0f;

	i = 0;

	// Textures -------------------------------------------------------------
	// Front face
	textureB[i++] = 0.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 0.0f;
	textureB[i++] = 0.0f; textureB[i++] = 1.0f;

	textureB[i++] = 1.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 1.0f;
	textureB[i++] = 0.0f; textureB[i++] = 1.0f;

	// Back face
	textureB[i++] = 0.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 1.0f;

	textureB[i++] = 1.0f; textureB[i++] = 1.0f;
	textureB[i++] = 0.0f; textureB[i++] = 1.0f;
	textureB[i++] = 0.0f; textureB[i++] = 0.0f;

	// Right face
	textureB[i++] = 0.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 0.0f;
	textureB[i++] = 0.0f; textureB[i++] = 1.0f;

	textureB[i++] = 1.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 1.0f;
	textureB[i++] = 0.0f; textureB[i++] = 1.0f;

	// Left face
	textureB[i++] = 1.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 1.0f;
	textureB[i++] = 0.0f; textureB[i++] = 0.0f;

	textureB[i++] = 0.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 1.0f;
	textureB[i++] = 0.0f; textureB[i++] = 1.0f;

	// Upper face
	textureB[i++] = 0.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 0.0f;
	textureB[i++] = 0.0f; textureB[i++] = 1.0f;

	textureB[i++] = 0.0f; textureB[i++] = 1.0f;
	textureB[i++] = 1.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 1.0f;

	// Lower face
	textureB[i++] = 0.0f; textureB[i++] = 1.0f; 
	textureB[i++] = 0.0f; textureB[i++] = 0.0f; 
	textureB[i++] = 1.0f; textureB[i++] = 1.0f; 

	textureB[i++] = 0.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 1.0f;

	vertexCount = 36;

	glGenBuffers(3, buffers);

	// Save data on the array of vertices
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, vertexB, GL_STATIC_DRAW);

	// Save data on the array of vertex
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, normalB, GL_STATIC_DRAW);

	// Sava data on the texture array
	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * vertexCount, textureB, GL_STATIC_DRAW);

	free(normalB);
	free(vertexB);
	free(textureB);
}

void prepareCilinder(float height, float radius, int sides) {

	float *vertexB = (float*)malloc(sizeof(float) * 4 * 3 * 3 * sides);
	float *normalB = (float*)malloc(sizeof(float) * 4 * 3 * 3 * sides);
	float *textureB = (float*)malloc(sizeof(float) * 4 * 3 * 2 * sides);

	int vertex = 0;
	float delta = 2.0f * _PI_ / sides;

	for (int i = 0; i < sides; ++i) {
		// top
		// central point
		normalB[vertex * 3 + 0] = 0;
		normalB[vertex * 3 + 1] = 1;
		normalB[vertex * 3 + 2] = 0;

		vertexB[vertex * 3 + 0] = 0.0f;
		vertexB[vertex * 3 + 1] = height / 2.0f;
		vertexB[vertex * 3 + 2] = 0.0f;

		textureB[vertex * 2 + 0] = 0.4375;
		textureB[vertex * 2 + 1] = 0.1875;

		vertex++;
		normalB[vertex * 3 + 0] = 0;
		normalB[vertex * 3 + 1] = 1;
		normalB[vertex * 3 + 2] = 0;

		vertexB[vertex * 3 + 0] = radius * sin(i * delta);
		vertexB[vertex * 3 + 1] = height / 2.0f;
		vertexB[vertex * 3 + 2] = radius * cos(i * delta);

		textureB[vertex * 2 + 0] = 0.4375 + 0.1875 * sin(i * delta);
		textureB[vertex * 2 + 1] = 0.1875 - 0.1875 * cos(i * delta);

		vertex++;
		normalB[vertex * 3 + 0] = 0;
		normalB[vertex * 3 + 1] = 1;
		normalB[vertex * 3 + 2] = 0;

		vertexB[vertex * 3 + 0] = radius * sin((i + 1) * delta);
		vertexB[vertex * 3 + 1] = height / 2.0f;
		vertexB[vertex * 3 + 2] = radius * cos((i + 1) * delta);

		textureB[vertex * 2 + 0] = 0.4375 + 0.1875 * sin((i + 1) * delta);
		textureB[vertex * 2 + 1] = 0.1875 - 0.1875 * cos((i + 1) * delta);

		// body
		// triângulo 1
		vertex++;
		normalB[vertex * 3 + 0] = sin((i + 1) * delta);
		normalB[vertex * 3 + 1] = 0;
		normalB[vertex * 3 + 2] = cos((i + 1) * delta);

		vertexB[vertex * 3 + 0] = radius * sin((i + 1) * delta);
		vertexB[vertex * 3 + 1] = height / 2.0f;
		vertexB[vertex * 3 + 2] = radius * cos((i + 1) * delta);

		textureB[vertex * 2 + 0] = (i + 1.0f) / sides;
		textureB[vertex * 2 + 1] = 1.0f;

		vertex++;
		normalB[vertex * 3 + 0] = sin(i * delta);
		normalB[vertex * 3 + 1] = 0;
		normalB[vertex * 3 + 2] = cos(i * delta);

		vertexB[vertex * 3 + 0] = radius * sin(i * delta);
		vertexB[vertex * 3 + 1] = height / 2.0f;
		vertexB[vertex * 3 + 2] = radius * cos(i * delta);

		textureB[vertex * 2 + 0] = (i + 0.0f) / sides;
		textureB[vertex * 2 + 1] = 1.0f;

		vertex++;
		normalB[vertex * 3 + 0] = sin(i * delta);
		normalB[vertex * 3 + 1] = 0;
		normalB[vertex * 3 + 2] = cos(i * delta);

		vertexB[vertex * 3 + 0] = radius * sin(i * delta);
		vertexB[vertex * 3 + 1] = -height / 2.0f;
		vertexB[vertex * 3 + 2] = radius * cos(i * delta);

		textureB[vertex * 2 + 0] = (i + 0.0f) / sides;
		textureB[vertex * 2 + 1] = 0.375f;

		// triangle 2
		vertex++;
		normalB[vertex * 3 + 0] = sin((i + 1) * delta);
		normalB[vertex * 3 + 1] = 0;
		normalB[vertex * 3 + 2] = cos((i + 1) * delta);

		vertexB[vertex * 3 + 0] = radius * sin((i + 1) * delta);
		vertexB[vertex * 3 + 1] = -height / 2.0f;
		vertexB[vertex * 3 + 2] = radius * cos((i + 1) * delta);

		textureB[vertex * 2 + 0] = (i + 1.0f) / sides;
		textureB[vertex * 2 + 1] = 0.375f;

		vertex++;
		normalB[vertex * 3 + 0] = sin((i + 1) * delta);
		normalB[vertex * 3 + 1] = 0;
		normalB[vertex * 3 + 2] = cos((i + 1) * delta);

		vertexB[vertex * 3 + 0] = radius * sin((i + 1) * delta);
		vertexB[vertex * 3 + 1] = height / 2.0f;
		vertexB[vertex * 3 + 2] = radius * cos((i + 1) * delta);

		textureB[vertex * 2 + 0] = (i + 1.0f) / sides;
		textureB[vertex * 2 + 1] = 1.0f;

		vertex++;
		normalB[vertex * 3 + 0] = sin(i * delta);
		normalB[vertex * 3 + 1] = 0;
		normalB[vertex * 3 + 2] = cos(i * delta);

		vertexB[vertex * 3 + 0] = radius * sin(i * delta);
		vertexB[vertex * 3 + 1] = -height / 2.0f;
		vertexB[vertex * 3 + 2] = radius * cos(i * delta);

		textureB[vertex * 2 + 0] = (i + 0.0f) / sides;
		textureB[vertex * 2 + 1] = 0.375f;

		// base
		// central point
		vertex++;
		normalB[vertex * 3 + 0] = 0;
		normalB[vertex * 3 + 1] = -1;
		normalB[vertex * 3 + 2] = 0;

		vertexB[vertex * 3 + 0] = 0.0f;
		vertexB[vertex * 3 + 1] = -height / 2.0f;
		vertexB[vertex * 3 + 2] = 0.0f;

		textureB[vertex * 2 + 0] = 0.8125;
		textureB[vertex * 2 + 1] = 0.1875;

		vertex++;
		normalB[vertex * 3 + 0] = 0;
		normalB[vertex * 3 + 1] = -1;
		normalB[vertex * 3 + 2] = 0;

		vertexB[vertex * 3 + 0] = radius * sin((i + 1) * delta);
		vertexB[vertex * 3 + 1] = -height / 2.0f;
		vertexB[vertex * 3 + 2] = radius * cos((i + 1) * delta);

		textureB[vertex * 2 + 0] = 0.8125 + 0.1875 * sin((i + 1) * delta);
		textureB[vertex * 2 + 1] = 0.1875 + 0.1875 * cos((i + 1) * delta);

		vertex++;
		normalB[vertex * 3 + 0] = 0;
		normalB[vertex * 3 + 1] = -1;
		normalB[vertex * 3 + 2] = 0;

		vertexB[vertex * 3 + 0] = radius * sin(i * delta);
		vertexB[vertex * 3 + 1] = -height / 2.0f;
		vertexB[vertex * 3 + 2] = radius * cos(i * delta);

		textureB[vertex * 2 + 0] = 0.8125 + 0.1875 * sin(i * delta);
		textureB[vertex * 2 + 1] = 0.1875 + 0.1875 * cos(i * delta);

		vertex++;
	}

	vertexCount = vertex;

	glGenBuffers(3, buffers);

	// Save data on the array of vertices
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, vertexB, GL_STATIC_DRAW);

	// Save data on the array of vertex
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, normalB, GL_STATIC_DRAW);

	// Sava data on the texture array
	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * vertexCount, textureB, GL_STATIC_DRAW);

	free(normalB);
	free(vertexB);
	free(textureB);
}

void drawFigure() {
	glBindTexture(GL_TEXTURE_2D, texID);

	// Semantic of the array of vertices
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	// Semantic of the array of vertex
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glNormalPointer(GL_FLOAT, 0, 0); // vertex always have 3 components

	// Semantic of the texture array
	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}


void renderScene(void) {
	// Red color
	float red[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
	GLfloat amb[4] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat diff[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat pos[4] = { 0.0, 0.0 ,1.0, 0.0 };

	float fps;
	int time;
	char s[64];

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	// Putting the light
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);

	gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

	drawFigure();

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

	if (figure == PLANE) preparePlane(5, 5);
	else if (figure == BOX) prepareBox(1, 1, 1);
	else prepareCilinder(2, 1, 16);
}


void main(int argc, char **argv) {

	// Init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 320);
	glutCreateWindow("CG@DI-UM");

	do {
		printf("Insert option:\n1 - Plane;\n2 - Box;\n3 - Cone;\n4 - Cilinder;\n\nOption: ");
		scanf("%d", &figure);
		if (figure < 1 || figure > 4) printf("\n\nInvalid option!\n\n");
	} while (figure < 1 || figure > 4);

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
}
