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
	if(figure == PLANE) ilLoadImage((ILstring) "planeBox.jpg");
	else if(figure == BOX) ilLoadImage((ILstring) "planeBox.jpg");
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

	// Vertices --------------------------------------------------------------
	// Back face
	vertexB[0] = 0.0f; vertexB[1] = width; vertexB[2] = 0.0f;
	vertexB[3] = length; vertexB[4] = 0.0f; vertexB[5] = 0.0f;
	vertexB[6] = 0.0f; vertexB[7] = 0.0f; vertexB[8] = 0.0f;

	vertexB[9] = 0.0f; vertexB[10] = width; vertexB[11] = 0.0f;
	vertexB[12] = length; vertexB[13] = width; vertexB[14] = 0.0f;
	vertexB[15] = length; vertexB[16] = 0.0f; vertexB[17] = 0.0f;

	// Upper face
	vertexB[18] = 0.0f; vertexB[19] = 0.0f; vertexB[20] = height;
	vertexB[21] = length; vertexB[22] = 0.0f; vertexB[23] = height;
	vertexB[24] = 0.0f; vertexB[25] = width; vertexB[26] = height;

	vertexB[27] = length; vertexB[28] = 0.0f; vertexB[29] = height;
	vertexB[30] = length; vertexB[31] = width; vertexB[32] = height;
	vertexB[33] = 0.0f; vertexB[34] = width; vertexB[35] = height;

	// Right face
	vertexB[36] = length; vertexB[37] = 0.0f; vertexB[38] = 0.0f;
	vertexB[39] = length; vertexB[40] = width; vertexB[41] = 0.0f;
	vertexB[42] = length; vertexB[43] = 0.0f; vertexB[44] = height;

	vertexB[45] = length; vertexB[46] = width; vertexB[47] = 0.0f;
	vertexB[48] = length; vertexB[49] = width; vertexB[50] = height;
	vertexB[51] = length; vertexB[52] = 0.0f; vertexB[53] = height;

	// Left face
	vertexB[54] = 0.0f; vertexB[55] = width; vertexB[56] = 0.0f;
	vertexB[57] = 0.0f; vertexB[58] = 0.0f; vertexB[59] = height;
	vertexB[60] = 0.0f; vertexB[61] = width; vertexB[62] = height;

	vertexB[63] = 0.0f; vertexB[64] = 0.0f; vertexB[65] = 0.0f;
	vertexB[66] = 0.0f; vertexB[67] = 0.0f; vertexB[68] = height;
	vertexB[69] = 0.0f; vertexB[70] = width; vertexB[71] = 0.0f;

	// Front face
	vertexB[72] = 0.0f; vertexB[73] = 0.0f; vertexB[74] = 0.0f;
	vertexB[75] = length; vertexB[76] = 0.0f; vertexB[77] = 0.0f;
	vertexB[78] = 0.0f; vertexB[79] = 0.0f; vertexB[80] = height;

	vertexB[81] = length; vertexB[82] = 0.0f; vertexB[83] = 0.0f;
	vertexB[84] = length; vertexB[85] = 0.0f; vertexB[86] = height;
	vertexB[87] = 0.0f; vertexB[88] = 0.0f; vertexB[89] = height;

	// Back face
	vertexB[90] = 0.0f; vertexB[91] = width; vertexB[92] = 0.0f;
	vertexB[93] = 0.0f; vertexB[94] = width; vertexB[95] = height;
	vertexB[96] = length; vertexB[97] = width; vertexB[98] = 0.0f;

	vertexB[99] = length; vertexB[100] = width; vertexB[101] = 0.0f;
	vertexB[102] = 0.0f; vertexB[103] = width; vertexB[104] = height;
	vertexB[105] = length; vertexB[106] = width; vertexB[107] = height;

	// Normals --------------------------------------------------------------------------
	// Back face
	normalB[0] = 0.0f; normalB[1] = 1.0f; normalB[2] = 0.0f;
	normalB[3] = 0.0f; normalB[4] = 1.0f; normalB[5] = 0.0f;
	normalB[6] = 0.0f; normalB[7] = 1.0f; normalB[8] = 0.0f;

	normalB[9] = 0.0f; normalB[10] = 1.0f; normalB[11] = 0.0f;
	normalB[12] = 0.0f; normalB[13] = -1.0f; normalB[14] = 0.0f;
	normalB[15] = 0.0f; normalB[16] = 1.0f; normalB[17] = 0.0f;

	// Upper face
	normalB[18] = 0.0f; normalB[19] = 1.0f; normalB[20] = 0.0f;
	normalB[21] = 0.0f; normalB[22] = 1.0f; normalB[23] = 0.0f;
	normalB[24] = 0.0f; normalB[25] = 1.0f; normalB[26] = 0.0f;

	normalB[27] = 0.0f; normalB[28] = 1.0f; normalB[29] = 0.0f;
	normalB[30] = 0.0f; normalB[31] = 1.0f; normalB[32] = 0.0f;
	normalB[33] = 0.0f; normalB[34] = 1.0f; normalB[35] = 0.0f;

	// Right face
	normalB[36] = 1.0f; normalB[37] = 0.0f; normalB[38] = 0.0f;
	normalB[39] = 1.0f; normalB[40] = 0.0f; normalB[41] = 0.0f;
	normalB[42] = 1.0f; normalB[43] = 0.0f; normalB[44] = 0.0f;

	normalB[45] = 1.0f; normalB[46] = 0.0f; normalB[47] = 0.0f;
	normalB[48] = 1.0f; normalB[49] = 0.0f; normalB[50] = 0.0f;
	normalB[51] = 1.0f; normalB[52] = 0.0f; normalB[53] = 0.0f;

	// Left face
	normalB[54] = -1.0f; normalB[55] = 0.0f; normalB[56] = 0.0f;
	normalB[57] = -1.0f; normalB[58] = 0.0f; normalB[59] = 0.0f;
	normalB[60] = -1.0f; normalB[61] = 0.0f; normalB[62] = 0.0f;

	normalB[63] = -1.0f; normalB[64] = 0.0f; normalB[65] = 0.0f;
	normalB[66] = -1.0f; normalB[67] = 0.0f; normalB[68] = 0.0f;
	normalB[69] = -1.0f; normalB[70] = 0.0f; normalB[71] = 0.0f;

	// Front face
	normalB[72] = 0.0f; normalB[73] = 0.0f; normalB[74] = -1.0f;
	normalB[75] = 0.0f; normalB[76] = 0.0f; normalB[77] = -1.0f;
	normalB[78] = 0.0f; normalB[79] = 0.0f; normalB[80] = -1.0f;

	normalB[81] = 0.0f; normalB[82] = 0.0f; normalB[83] = -1.0f;
	normalB[84] = 0.0f; normalB[85] = 0.0f; normalB[86] = -1.0f;
	normalB[87] = 0.0f; normalB[88] = 0.0f; normalB[89] = -1.0f;

	// Back face
	normalB[90] = 0.0f; normalB[91] = 0.0f; normalB[92] = 1.0f;
	normalB[93] = 0.0f; normalB[94] = 0.0f; normalB[95] = 1.0f;
	normalB[96] = 0.0f; normalB[97] = 0.0f; normalB[98] = 1.0f;

	normalB[99] = 0.0f; normalB[100] = 0.0f; normalB[101] = 1.0f;
	normalB[102] = 0.0f; normalB[103] = 0.0f; normalB[104] = 1.0f;
	normalB[105] = 0.0f; normalB[106] = 0.0f; normalB[107] = 1.0f;

	// Textures ----------------------------------------------------------
	// Lower face
	textureB[i++] = 0.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 0.0f;
	textureB[i++] = 0.0f; textureB[i++] = 1.0f;

	textureB[i++] = 1.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 1.0f;
	textureB[i++] = 0.0f; textureB[i++] = 1.0f;

	// Upper face
	textureB[i++] = 0.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 0.0f;
	textureB[i++] = 0.0f; textureB[i++] = 1.0f;

	textureB[i++] = 1.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 1.0f;
	textureB[i++] = 0.0f; textureB[i++] = 1.0f;

	// Right face
	textureB[i++] = 0.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 0.0f;
	textureB[i++] = 0.0f; textureB[i++] = 1.0f;

	textureB[i++] = 1.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 1.0f;
	textureB[i++] = 0.0f; textureB[i++] = 1.0f;

	// Left face
	textureB[i++] = 0.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 0.0f;
	textureB[i++] = 0.0f; textureB[i++] = 1.0f;

	textureB[i++] = 1.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 1.0f;
	textureB[i++] = 0.0f; textureB[i++] = 1.0f;

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
	textureB[i++] = 0.0f; textureB[i++] = 1.0f;

	textureB[i++] = 1.0f; textureB[i++] = 0.0f;
	textureB[i++] = 1.0f; textureB[i++] = 1.0f;
	textureB[i++] = 0.0f; textureB[i++] = 1.0f;

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
