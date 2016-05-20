void printTeapot() {
	float res[3];
	ofstream teapot;
	teapot.open(filename.c_str());
	float **teapotPoints = (float**) malloc(1000000 * sizeof(float**)); // Saves the points of the teapot to be able to count their number first and print them after that
	int pointNumber = 0; // Number of vertices of the teapot
	float tesselation = 0.1;

	for (int patch = 0; patch < numPatches; patch++) {
		vector<float> indicesPatch = patches[patch];
		float v = 0.0;
    int countv=0;
		while (v <= 1) {
			float u = 0.0;
      int countu=0;
			while (u <= 1) {
				teapotPoints[pointNumber] = (float*) malloc(3 * sizeof(float*));
				res[0] = getBezierPoint(u, v, indicesPatch, 0);
				res[1] = getBezierPoint(u, v, indicesPatch, 1);
				res[2] = getBezierPoint(u, v, indicesPatch, 2);
				teapotPoints[pointNumber][0] = res[0];
				teapotPoints[pointNumber][1] = res[1];
				teapotPoints[pointNumber][2] = res[2];
				//Texture
				teapotTexts[pointNumber][0] = countu;
				teapotTexts[pointNumber][1] = countv;
				pointNumber++;

				teapotPoints[pointNumber] = (float*) malloc(3 * sizeof(float*));
				res[0] = getBezierPoint(u + tesselation, v, indicesPatch, 0);
				res[1] = getBezierPoint(u + tesselation, v, indicesPatch, 1);
				res[2] = getBezierPoint(u + tesselation, v, indicesPatch, 2);
				teapotPoints[pointNumber][0] = res[0];
				teapotPoints[pointNumber][1] = res[1];
				teapotPoints[pointNumber][2] = res[2];
				//Texture
				teapotTexts[pointNumber][0] = countu+1;
				teapotTexts[pointNumber][1] = countv;
				pointNumber++;

				teapotPoints[pointNumber] = (float*) malloc(3 * sizeof(float*));
				res[0] = getBezierPoint(u, v + tesselation, indicesPatch, 0);
				res[1] = getBezierPoint(u, v + tesselation, indicesPatch, 1);
				res[2] = getBezierPoint(u, v + tesselation, indicesPatch, 2);
				teapotPoints[pointNumber][0] = res[0];
				teapotPoints[pointNumber][1] = res[1];
				teapotPoints[pointNumber][2] = res[2];
				//Texture
				teapotTexts[pointNumber][0] = countu;
				teapotTexts[pointNumber][1] = countv+1;
				pointNumber++;

				teapotPoints[pointNumber] = (float*) malloc(3 * sizeof(float*));
				res[0] = getBezierPoint(u, v + tesselation, indicesPatch, 0);
				res[1] = getBezierPoint(u, v + tesselation, indicesPatch, 1);
				res[2] = getBezierPoint(u, v + tesselation, indicesPatch, 2);
				teapotPoints[pointNumber][0] = res[0];
				teapotPoints[pointNumber][1] = res[1];
				teapotPoints[pointNumber][2] = res[2];
				//Texture
				teapotTexts[pointNumber][0] = countu;
				teapotTexts[pointNumber][1] = countv+1;
				pointNumber++;

				teapotPoints[pointNumber] = (float*) malloc(3 * sizeof(float*));
				res[0] = getBezierPoint(u + tesselation, v, indicesPatch, 0);
				res[1] = getBezierPoint(u + tesselation, v, indicesPatch, 1);
				res[2] = getBezierPoint(u + tesselation, v, indicesPatch, 2);
				teapotPoints[pointNumber][0] = res[0];
				teapotPoints[pointNumber][1] = res[1];
				teapotPoints[pointNumber][2] = res[2];
				//Texture
				teapotTexts[pointNumber][0] = countu+1;
				teapotTexts[pointNumber][1] = countv;
				pointNumber++;

				teapotPoints[pointNumber] = (float*) malloc(3 * sizeof(float*));
				res[0] = getBezierPoint(u + tesselation, v + tesselation, indicesPatch, 0);
				res[1] = getBezierPoint(u + tesselation, v + tesselation, indicesPatch, 1);
				res[2] = getBezierPoint(u + tesselation, v + tesselation, indicesPatch, 2);
				teapotPoints[pointNumber][0] = res[0];
				teapotPoints[pointNumber][1] = res[1];
				teapotPoints[pointNumber][2] = res[2];
        //Texture
        teapotTexts[pointNumber][0] = countu+1;
				teapotTexts[pointNumber][1] = countv+1;
				pointNumber++;
        countu++;
				u += tesselation;
			}
			countu=0;
			v += tesselation;
      countv++;
		}
	}

	void prepareTexture() { //Engine
	unsigned int t, tw, th;
	unsigned char *texData;
	ilGenImages(1, &t);
	ilBindImage(t);
	ilLoadImage((ILstring)"relva1.jpg");
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	texData = ilGetData();
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
}
