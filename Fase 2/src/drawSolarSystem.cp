#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
// #include <windows.h>
// #include <GL/glut.h>
// #include "tinyxml\tinyxml.h"	// The place where the file "tinyxml.h" is located
#include <math.h>
#include <GLUT/glut.h> //-- MAC
#include </usr/local/Cellar/tinyxml/2.6.2/include/tinyxml.h> //-- MAC

using namespace std;

int num_moons=0,num_planets=0,read=0;
//Global Variables to Transformations
float px = 0.0, py = 200.0, pz = 300.0;

float rotate_y = 0;
float rotate_x = 0;
float rotate_z = 0;

float translate_y = 0;
float translate_x = 0;
float translate_z = 0;

// Global variable store 3d planets read
vector<string> file3dRead_Sun;
vector<string> file3dRead_Mercury;
vector<string> file3dRead_Venus;
vector<string> file3dRead_Earth;
vector<string> file3dRead_Mars;
vector<string> file3dRead_Jupiter;
vector<string> file3dRead_Uranus;
vector<string> file3dRead_Saturn;
vector<string> file3dRead_Neptune;

// Global variable store 3d moons read

vector<string> file3dRead_earthMoon;
vector<string> file3dRead_PhobosMoon;
vector<string> file3dRead_DeimosMoon;
vector<string> file3dRead_GanymedeMoon;
vector<string> file3dRead_CallistoMoon;
vector<string> file3dRead_EuropaMoon;
vector<string> file3dRead_IoMoon;
vector<string> file3dRead_TitanMoon;
vector<string> file3dRead_EnceladusMoon;
vector<string> file3dRead_TitaniaMoon;
vector<string> file3dRead_OberonMoon;
vector<string> file3dRead_UmbrielMoon;
vector<string> file3dRead_ArielMoon;
vector<string> file3dRead_MirandaMoon;
vector<string> file3dRead_TritonMoon;
vector<string> file3dRead_ProteusMoon;


// Global variable process input
vector<string> splitted;

// Global variable with 3d vertex
vector<float> vrtx_sun;
vector<float> vrtx_Mercury;
vector<float> vrtx_Venus;
vector<float> vrtx_Earth;
vector<float> vrtx_Mars;
vector<float> vrtx_Jupiter;
vector<float> vrtx_Saturn;
vector<float> vrtx_Uranus;
vector<float> vrtx_Neptune;

vector<float> vrtx_earthMoon;
vector<float> vrtx_PhobosMoon;
vector<float> vrtx_DeimosMoon;
vector<float> vrtx_GanymedeMoon;
vector<float> vrtx_CallistoMoon;
vector<float> vrtx_EuropaMoon;
vector<float> vrtx_IoMoon;
vector<float> vrtx_TitanMoon;
vector<float> vrtx_EnceladusMoon;
vector<float> vrtx_TitaniaMoon;
vector<float> vrtx_OberonMoon;
vector<float> vrtx_UmbrielMoon;
vector<float> vrtx_ArielMoon;
vector<float> vrtx_MirandaMoon;
vector<float> vrtx_TritonMoon;
vector<float> vrtx_ProteusMoon;

// Declare radii of all 8 planets orbits
float MercuryRadiusOrbit     = 20.0;
float VenusRadiusOrbit       = 30.0;
float TerraRadiusOrbit       = 40.0;
float MarteRadiusOrbit       = 50.0;
float JupiterRadiusOrbit     = 65.0;
float UranoRadiusOrbit       = 80.0;
float SaturnoRadiusOrbit     = 95.0;
float NeptunoRadiusOrbit     = 100.0;

// Global variables that allows drawing circles os planets around the sun
float alphaM            = 0.0;
float alphaIncM         = (2 * M_PI)/10.0;

float alphaV            = 0.0;
float alphaIncV         = (2 * M_PI)/10.0;

float alphaT            = 0.0;
float alphaIncT         = (2 * M_PI)/10.0;

float alphaMT           = 0.0;
float alphaIncMT        = (2 * M_PI)/10.0;

float alphaJ            = 0.0;
float alphaIncJ         = (2 * M_PI)/10.0;

float alphaS            = 0.0;
float alphaIncS         = (2 * M_PI)/10.0;

float alphaU            = 0.0;
float alphaIncU         = (2 * M_PI)/10.0;

float alphaN            = 0.0;
float alphaIncN         = (2 * M_PI)/10.0;

// Operation name has 100 char at max
#define MAX 100

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

// Reads the vertices of figure.3d files into a vector
vector<string> read3d(string figure) {
	vector<string> vecx;
	ifstream fileTXT(figure);
	string line;
	while (getline(fileTXT, line)) {
		vecx.push_back(line); // Adding the line at the end of vector
	}
	int size = stoi(vecx[0]); // Number of total vertices. It's the first line in the .3d file
	return vecx;
}

string reading(vector<string> aux){
	int size = stoi(aux[0]); // number of vertex
	int conta = 0;
	string line; // String processed of all file3dRead strings concatenated into one of plane.3d or box.3d

	for (int i = 1; i <= size; i++) {
		if (conta != 0) { // Adding ',' at the end of each line except the first
			line = line + " " + aux[i];
		}
		else {
			line += aux[i];
			conta++;
		}
	}
	return line;
}

void preencheVrtx(){
	string sun 				= reading(file3dRead_Sun); // String processed of all file3dRead strings concatenated into one of plane.3d or box.3d
	string mercury	  = reading(file3dRead_Mercury);
	string venus   	  = reading(file3dRead_Venus);
	string earth	    = reading(file3dRead_Earth);
	string mars	      = reading(file3dRead_Mars);
	string jupiter	  = reading(file3dRead_Jupiter);
	string saturn 	  = reading(file3dRead_Saturn);
	string uranus 	  = reading(file3dRead_Uranus);
	string neptune	  = reading(file3dRead_Neptune);

	string earthMoon		 = reading(file3dRead_earthMoon);
	string PhobosMoon		 = reading(file3dRead_PhobosMoon);
	string DeimosMoon    = reading(file3dRead_DeimosMoon);
	string GanymedeMoon  = reading(file3dRead_GanymedeMoon);
	string CallistoMoon  = reading(file3dRead_CallistoMoon);
	string EuropaMoon	 	 = reading(file3dRead_EuropaMoon);
	string IoMoon				 = reading(file3dRead_IoMoon);
	string TitanMoon		 = reading(file3dRead_TitanMoon);
	string EnceladusMoon = reading(file3dRead_Neptune);
	string TitaniaMoon   = reading(file3dRead_TitaniaMoon);
	string OberonMoon		 = reading(file3dRead_OberonMoon);
	string UmbrielMoon   = reading(file3dRead_UmbrielMoon);
	string ArielMoon 		 = reading(file3dRead_ArielMoon);
	string MirandaMoon 	 = reading(file3dRead_MirandaMoon);
	string TritonMoon 	 = reading(file3dRead_TritonMoon);
	string ProteusMoon	 = reading(file3dRead_ProteusMoon);

	// Build an istream that holds the input string
	istringstream is_sun(sun);
	istringstream is_mercury(mercury);
	istringstream is_venus(venus);
	istringstream is_earth(earth);
	istringstream is_mars(mars);
	istringstream is_jupiter(jupiter);
	istringstream is_saturn(saturn);
	istringstream is_uranus(uranus);
	istringstream is_neptune(neptune);

	istringstream is_earthMoon(earthMoon);
	istringstream is_PhobosMoon(PhobosMoon);
	istringstream is_DeimosMoon(DeimosMoon);
	istringstream is_GanymedeMoon(GanymedeMoon);
	istringstream is_CallistoMoon(CallistoMoon);
	istringstream is_EuropaMoon(EuropaMoon);
	istringstream is_IoMoon(IoMoon);
	istringstream is_TitanMoon(TitanMoon);
	istringstream is_EnceladusMoon(EnceladusMoon);
	istringstream is_TitaniaMoon(TitaniaMoon);
	istringstream is_OberonMoon(OberonMoon);
	istringstream is_UmbrielMoon(UmbrielMoon);
	istringstream is_ArielMoon(ArielMoon);
	istringstream is_MirandaMoon(MirandaMoon);
	istringstream is_TritonMoon(TritonMoon);
	istringstream is_ProteusMoon(ProteusMoon);

	// Iterates over the istream, using >> to grab floats and push_back to store them in the vector
	copy(istream_iterator<float>(is_sun), istream_iterator<float>(), back_inserter(vrtx_sun));
	copy(istream_iterator<float>(is_mercury), istream_iterator<float>(), back_inserter(vrtx_Mercury));
	copy(istream_iterator<float>(is_venus), istream_iterator<float>(), back_inserter(vrtx_Venus));
	copy(istream_iterator<float>(is_earth), istream_iterator<float>(), back_inserter(vrtx_Earth));
	copy(istream_iterator<float>(is_mars), istream_iterator<float>(), back_inserter(vrtx_Mars));
	copy(istream_iterator<float>(is_jupiter), istream_iterator<float>(), back_inserter(vrtx_Jupiter));
	copy(istream_iterator<float>(is_saturn), istream_iterator<float>(), back_inserter(vrtx_Saturn));
	copy(istream_iterator<float>(is_uranus), istream_iterator<float>(), back_inserter(vrtx_Uranus));
	copy(istream_iterator<float>(is_neptune), istream_iterator<float>(), back_inserter(vrtx_Neptune));

	copy(istream_iterator<float>(is_earthMoon), istream_iterator<float>(), back_inserter(vrtx_earthMoon));
	copy(istream_iterator<float>(is_PhobosMoon), istream_iterator<float>(), back_inserter(vrtx_PhobosMoon));
	copy(istream_iterator<float>(is_DeimosMoon), istream_iterator<float>(), back_inserter(vrtx_DeimosMoon));
	copy(istream_iterator<float>(is_GanymedeMoon), istream_iterator<float>(), back_inserter(vrtx_GanymedeMoon));
	copy(istream_iterator<float>(is_CallistoMoon), istream_iterator<float>(), back_inserter(vrtx_CallistoMoon));
	copy(istream_iterator<float>(is_EuropaMoon), istream_iterator<float>(), back_inserter(vrtx_EuropaMoon));
	copy(istream_iterator<float>(is_IoMoon), istream_iterator<float>(), back_inserter(vrtx_IoMoon));
	copy(istream_iterator<float>(is_TitanMoon), istream_iterator<float>(), back_inserter(vrtx_TitanMoon));
	copy(istream_iterator<float>(is_EnceladusMoon), istream_iterator<float>(), back_inserter(vrtx_EnceladusMoon));
	copy(istream_iterator<float>(is_TitaniaMoon), istream_iterator<float>(), back_inserter(vrtx_TitaniaMoon));
	copy(istream_iterator<float>(is_OberonMoon), istream_iterator<float>(), back_inserter(vrtx_OberonMoon));
	copy(istream_iterator<float>(is_UmbrielMoon), istream_iterator<float>(), back_inserter(vrtx_UmbrielMoon));
	copy(istream_iterator<float>(is_ArielMoon), istream_iterator<float>(), back_inserter(vrtx_ArielMoon));
	copy(istream_iterator<float>(is_MirandaMoon), istream_iterator<float>(), back_inserter(vrtx_MirandaMoon));
	copy(istream_iterator<float>(is_TritonMoon), istream_iterator<float>(), back_inserter(vrtx_TritonMoon));
	copy(istream_iterator<float>(is_ProteusMoon), istream_iterator<float>(), back_inserter(vrtx_ProteusMoon));
}

// Show file.3d content in file3dRead vector
void drawRenderSceneFile3d(void) {
		// Clear buffers.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set the camera.
		glLoadIdentity();
		gluLookAt(0.0,200.0,300.0,0.0,0.0,-1.0,0.0f,1.0f,0.0f);

	  // Draw plane.
	  glBegin(GL_TRIANGLES);
	    glVertex3f(-100, 0, -100);
	    glVertex3f(-100, 0, 100);
	    glVertex3f(100, 0, 100);

	    glVertex3f(100, 0, -100);
	    glVertex3f(-100, 0, -100);
	    glVertex3f(100, 0, 100);
	  glEnd();

	  // Sun.
	  glTranslatef(0.0, 20.0, 0.0);
	  glBegin(GL_TRIANGLES);

	    for (int j = 0; j < vrtx_sun.size();)
	            glVertex3f(vrtx_sun[j++], vrtx_sun[j++], vrtx_sun[j++]);

		glEnd();

	  // Mercury
	  for (int i = 0; i < 1; i++) {
	    glPushMatrix();
	    glTranslatef(MercuryRadiusOrbit * cos(alphaM), 1.0, MercuryRadiusOrbit * sin(alphaM)); //proxima posicao no eixo XoZ
	    glRotatef(10.0, 0.0, 1.0, 0.0);
	    glBegin(GL_TRIANGLES);

	      for (int j = 0; j < vrtx_Mercury.size();)
	              glVertex3f(vrtx_Mercury[j++], vrtx_Mercury[j++], vrtx_Mercury[j++]);

	  	glEnd();
	    glPopMatrix();
			alphaM -= alphaIncM; // andar em sentido contrario, em circulos
	  }
	  alphaM -= 100.0;

	  // Venus
	  for (int i = 0; i < 1; i++) {
	    glPushMatrix();
	    glTranslatef(VenusRadiusOrbit * cos(alphaV), 1.0, VenusRadiusOrbit * sin(alphaV));
			glBegin(GL_TRIANGLES);

				for (int j = 0; j < vrtx_Venus.size();)
								glVertex3f(vrtx_Venus[j++], vrtx_Venus[j++], vrtx_Venus[j++]);

			glEnd();
	    glPopMatrix();
			alphaV += alphaIncV;
	  }
	  alphaV += 100.0;

	  // Terra
	  for (int i = 0; i < 1; i++) {
	    glPushMatrix();
	    glTranslatef(TerraRadiusOrbit * cos(alphaT), 1.0, TerraRadiusOrbit * sin(alphaT));
			glBegin(GL_TRIANGLES);

				for (int j = 0; j < vrtx_Earth.size();)
								glVertex3f(vrtx_Earth[j++], vrtx_Earth[j++], vrtx_Earth[j++]);

			glEnd();
	    glPopMatrix();
			alphaT -= alphaIncT;
	  }
	  alphaT -= 100.0;

	  // Marte
	  for (int i = 0; i < 1; i++) {
	    glPushMatrix();
	    glTranslatef(MarteRadiusOrbit * cos(alphaMT), 1.0, MarteRadiusOrbit * sin(alphaMT));
			glBegin(GL_TRIANGLES);

				for (int j = 0; j < vrtx_Mars.size();)
								glVertex3f(vrtx_Mars[j++], vrtx_Mars[j++], vrtx_Mars[j++]);

			glEnd();
	    glPopMatrix();
			alphaMT += alphaIncMT;
	  }
	  alphaMT += 100.0;

	  // Jupiter
	  for (int i = 0; i < 1; i++) {
	    glPushMatrix();
	    glTranslatef(JupiterRadiusOrbit * cos(alphaJ), 1.0, JupiterRadiusOrbit * sin(alphaJ));
			glBegin(GL_TRIANGLES);

				for (int j = 0; j < vrtx_Jupiter.size();)
								glVertex3f(vrtx_Jupiter[j++], vrtx_Jupiter[j++], vrtx_Jupiter[j++]);

			glEnd();
	    glPopMatrix();
			alphaJ += alphaIncJ;
	  }
	  alphaJ += 100.0;

	  // Saturno
	  for (int i = 0; i < 1; i++) {
	    glPushMatrix();
	    glTranslatef(SaturnoRadiusOrbit * cos(alphaS), 1.0, SaturnoRadiusOrbit * sin(alphaS));
			glBegin(GL_TRIANGLES);

				for (int j = 0; j < vrtx_Saturn.size();)
								glVertex3f(vrtx_Saturn[j++], vrtx_Saturn[j++], vrtx_Saturn[j++]);

			glEnd();
	    glPopMatrix();
			alphaS -= alphaIncS;
	  }
	  alphaS -= 100.0;

	  // Urano
	  for (int i = 0; i < 1; i++) {
	    glPushMatrix();
	    glTranslatef(UranoRadiusOrbit * cos(alphaU), 1.0, UranoRadiusOrbit * sin(alphaU));
			glBegin(GL_TRIANGLES);

				for (int j = 0; j < vrtx_Uranus.size();)
								glVertex3f(vrtx_Uranus[j++], vrtx_Uranus[j++], vrtx_Uranus[j++]);

			glEnd();
	    glPopMatrix();
			alphaU -= alphaIncU;
	  }
	  alphaU -= 100.0;

	  // Neptuno
	  for (int i = 0; i < 1; i++) {
	    glPushMatrix();
	    glTranslatef(NeptunoRadiusOrbit * cos(alphaN), 1.0, NeptunoRadiusOrbit * sin(alphaN));
			glBegin(GL_TRIANGLES);

				for (int j = 0; j < vrtx_Neptune.size();)
								glVertex3f(vrtx_Neptune[j++], vrtx_Neptune[j++], vrtx_Neptune[j++]);

			glEnd();
	    glPopMatrix();
			alphaN += alphaIncN;
	  }
	  alphaN += 100.0;

		// End of frame.
		glutSwapBuffers();

}

void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0) h = 1;

	// compute window's aspect ratio
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

// Processing Keyboard Events
void keyboardNormal (unsigned char key, int x, int y){

    if (key == 'j' || key == 'J') translate_x+=1;
    else if (key == 'k' || key == 'K') translate_y+=1;
    else if (key == 'l' || key == 'L') translate_z+=1;
    else if (key == 'q' || key == 'Q') translate_x-=1;
    else if (key == 'w' || key == 'W') translate_y-=1;
    else if (key == 'e' || key == 'E') translate_z-=1;
    else if (key == 'z' || key == 'Z') glPolygonMode(GL_FRONT,GL_LINE); //GL_FRONT & GL_LINE
    else if (key == 'x' || key == 'X') glPolygonMode(GL_FRONT,GL_POINT); //GL_FRONT & GL_POINT
    else if (key == 'c' || key == 'C') glPolygonMode(GL_FRONT,GL_FILL); //GL_FRONT & GL_FILL
    else if (key == 'v' || key == 'V') glPolygonMode(GL_BACK,GL_LINE); //GL_BACK & GL_LINE
    else if (key == 'b' || key == 'B') glPolygonMode(GL_BACK,GL_POINT); //GL_BACK & GL_POINT
    else if (key == 'n' || key == 'N') glPolygonMode(GL_BACK,GL_FILL); //GL_BACK & GL_FILL
    else if (key == 'm' || key == 'M') glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); //GL_FRONT_AND_BACK & GL_LINE
    else if (key == 'o' || key == 'O') glPolygonMode(GL_FRONT_AND_BACK,GL_POINT); //GL_FRONT_AND_BACK & GL_POINT
    else if (key == 'p' || key == 'P') glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); //GL_FRONT_AND_BACK & GL_FILL*/
    else if (key == '0'){ // DEFAULT IS WHITE
        glColor3f(1.0f,1.0f,1.0f);
    }
    else if (key == '1'){ // RED
        glColor3f(1.0f,0.0f,0.0f);
    }
    else if (key == '2'){ // BLUE
        glColor3f(0.0f,0.0f,1.0f);
    }
    else if (key == '3'){ // GREEN
        glColor3f(0.0f,1.0f,0.0f);
    }
    else if (key == '4'){ // ORANGE
        glColor3f(1.0f,0.5f,0.0f);
    }
    else if (key == '5'){ // PINK
        glColor3f(1.0f,0.5f,0.5f);
    }
    else if (key == '6'){ // YELLOW
        glColor3f(1.0f,1.0f,0.0f);
    }
    else if (key == '7'){ // BRIGHT BLUE
        glColor3f(0.0f,1.0f,1.0f);
    }
    else if (key==27) exit(-1);

    //when camera moves
    glutPostRedisplay();
}

//Examples of key_code: GLUT_KEY_F1 ; GLUT_KEY_UP.
void keyboardExtra(int key_code, int x, int y){
		/*  Right arrow key - decrease rotate_y by 5 */
		if (key_code == GLUT_KEY_RIGHT) rotate_y -= 5;
		/*  Left arrow key - increase rotate_y by 5 */
		else if (key_code == GLUT_KEY_LEFT) rotate_y += 5;
		/*  Up arrow key - increase rotate_x by 5 */
		else if (key_code == GLUT_KEY_UP) rotate_x += 5;
		/*  Down arrow key - decrease rotate_x by 5 */
		else if (key_code == GLUT_KEY_DOWN) rotate_x -= 5;
		/*   arrow key - decrease rotate_z by 5 */
		else if (key_code == GLUT_KEY_F9) rotate_z += 5;
		/*   arrow key - decrease rotate_z by 5 */
		else if (key_code == GLUT_KEY_F7) rotate_z -= 5;
		//when camera moves

		glutPostRedisplay();
}

void readXML(const char* file){

TiXmlDocument XMLdoc(file);
bool loadOkay = XMLdoc.LoadFile();
if (loadOkay)
{
    cout << "SolarSystemXML.xml loaded" << endl;
    read=1;
    TiXmlElement *pRoot, *pGroup, *pModels, *pModelFile, *pMoons, *pModelsMoons, *pModelFileMoons;
    pRoot = XMLdoc.FirstChildElement( "scene" );
    if ( pRoot )
    {
        pGroup = pRoot->FirstChildElement("group" );
        while ( pGroup )
        {
						num_planets++;
            pModels = pGroup->FirstChildElement("models");
            while ( pModels )
            {
                pModelFile = pModels->FirstChildElement("model");
                while( pModelFile )
                {
									switch (num_planets) {
								    case 1:
								           file3dRead_Sun = read3d((string) pModelFile->Attribute("file"));
								           break;
										case 2:
			 								     file3dRead_Mercury = read3d((string) pModelFile->Attribute("file"));
			 								     break;
										case 3:
													 file3dRead_Venus = read3d((string) pModelFile->Attribute("file"));
													 break;
										case 4:
													 file3dRead_Earth = read3d((string) pModelFile->Attribute("file"));
													 break;
										case 5:
	 													file3dRead_Mars= read3d((string) pModelFile->Attribute("file"));
	 													break;
										case 6:
													 file3dRead_Jupiter = read3d((string) pModelFile->Attribute("file"));
													 break;
										case 7:
													 file3dRead_Saturn = read3d((string) pModelFile->Attribute("file"));
													 break;
										case 8:
			 										 file3dRead_Uranus = read3d((string) pModelFile->Attribute("file"));
			 										 break;
			 							case 9:
			 										 file3dRead_Neptune = read3d((string) pModelFile->Attribute("file"));
			 										 break;
										default:
			 			 							 break;
									}
                   pModelFile = pModelFile->NextSiblingElement("model");
                }
                pModels = pModels->NextSiblingElement("models");
            }

            pMoons = pGroup->FirstChildElement("group" );
            while ( pMoons )
            {
            pModelsMoons = pMoons->FirstChildElement("models");
            while ( pModelsMoons )
            {
								num_moons++;
                pModelFileMoons = pModelsMoons->FirstChildElement("model");
                while( pModelFileMoons )
                {
									switch (num_moons) {
								    case 1:
								           file3dRead_earthMoon = read3d((string) pModelFileMoons->Attribute("file"));
								           break;
										case 2:
			 								     file3dRead_PhobosMoon= read3d((string) pModelFileMoons->Attribute("file"));
			 								     break;
										case 3:
													 file3dRead_DeimosMoon = read3d((string) pModelFileMoons->Attribute("file"));
													 break;
										case 4:
													 file3dRead_GanymedeMoon = read3d((string) pModelFileMoons->Attribute("file"));
													 break;
										case 5:
	 													file3dRead_CallistoMoon= read3d((string) pModelFileMoons->Attribute("file"));
	 													break;
										case 6:
													 file3dRead_EuropaMoon = read3d((string) pModelFileMoons->Attribute("file"));
													 break;
										case 7:
													 file3dRead_IoMoon = read3d((string) pModelFileMoons->Attribute("file"));
													 break;
										case 8:
			 										 file3dRead_TitanMoon = read3d((string) pModelFileMoons->Attribute("file"));
			 										 break;
			 							case 9:
			 										 file3dRead_EnceladusMoon = read3d((string) pModelFileMoons->Attribute("file"));
			 										 break;
								    case 10:
								           file3dRead_TitaniaMoon = read3d((string) pModelFileMoons->Attribute("file"));
								           break;
										case 11:
			 								     file3dRead_OberonMoon = read3d((string) pModelFileMoons->Attribute("file"));
			 								     break;
										case 12:
													 file3dRead_UmbrielMoon = read3d((string) pModelFileMoons->Attribute("file"));
													 break;
										case 13:
													 file3dRead_ArielMoon = read3d((string) pModelFileMoons->Attribute("file"));
													 break;
										case 14:
	 													file3dRead_MirandaMoon= read3d((string) pModelFileMoons->Attribute("file"));
	 													break;
										case 15:
													 file3dRead_TritonMoon = read3d((string) pModelFileMoons->Attribute("file"));
													 break;
										case 16:
													 file3dRead_ProteusMoon = read3d((string) pModelFileMoons->Attribute("file"));
													 break;
										default:
			 										 break;
									}
                  pModelFileMoons = pModelFileMoons->NextSiblingElement("model");
                }

                pModelsMoons = pModelsMoons->NextSiblingElement("models");
            }

              pMoons = pMoons->NextSiblingElement("group");
            }

            pGroup = pGroup->NextSiblingElement("group");
        }
    }
    else
    {
        cout << "Cannot find 'scene' node" << endl;
    }
}
  else
  {
        cout << "Cannot find XML file" << endl;
        read=0;
  }
}




// write function to process menu events
void newMenu (int id_op){
    keyboardNormal((unsigned char)id_op, 0, 0);
}

// Main function
int main(int argc, char **argv) {
	string operationLine, operation;

	cout << "Insert your operation:" << endl;
	getline(cin, operationLine);
	splitted = split(operationLine, ' '); // Save a string on a vector with the substrings separated by a space

	// The function "stoff" transforms the content of a string in a float

	if (splitted[0] == "Draw" || splitted[0] == "draw") {	// Draw -> receives the name of the .3d file
		if (splitted.size() == 1){
			readXML("SolarSystemXML.xml");

			// Init
			glutInit(&argc, argv);
			glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
			glutInitWindowPosition(100, 100);
			glutInitWindowSize(800, 800);
			glutCreateWindow("Solar System - Stage 2");

			// Callback registration
			preencheVrtx();
			glutDisplayFunc(drawRenderSceneFile3d);
			glutReshapeFunc(changeSize);

			// Registration of the keyboard
			glutKeyboardFunc(keyboardNormal);
			glutSpecialFunc(keyboardExtra);


      //menu
      glutCreateMenu(newMenu);
      glutAddMenuEntry("Increase Translate x value",'j');
      glutAddMenuEntry("Increase Translate y value",'k');
      glutAddMenuEntry("Increase Translate z value",'l');
      glutAddMenuEntry("Decrease Translate x value",'q');
      glutAddMenuEntry("Decrease Translate y value",'w');
      glutAddMenuEntry("Decrease Translate z value",'e');
      glutAddMenuEntry("GL_FRONT & GL_LINE",'z');
      glutAddMenuEntry("GL_FRONT & GL_POINT",'x');
      glutAddMenuEntry("GL_FRONT & GL_FILL",'c');
      glutAddMenuEntry("GL_BACK & GL_LINE",'v');
      glutAddMenuEntry("GL_BACK & GL_POINT",'b');
      glutAddMenuEntry("GL_BACK & GL_FILL",'n');
      glutAddMenuEntry("GL_FRONT_AND_BACK & GL_LINE",'m');
      glutAddMenuEntry("GL_FRONT_AND_BACK & GL_POINT",'o');
      glutAddMenuEntry("GL_FRONT_AND_BACK & GL_FILL",'p');
      glutAddMenuEntry("Change Colour to Default",'0');
      glutAddMenuEntry("Change Colour to Red",'1');
      glutAddMenuEntry("Change Colour to Blue",'2');
      glutAddMenuEntry("Change Colour to Green",'3');
      glutAddMenuEntry("Change Colour to Orange",'4');
      glutAddMenuEntry("Change Colour to Pink",'5');
      glutAddMenuEntry("Change Colour to Yellow",'6');
      glutAddMenuEntry("Change Colour to Bright Blue",'7');
      //button= GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, or GLUT MIDDLE_BUTTON
      glutAttachMenu(GLUT_RIGHT_BUTTON);

			// OpenGL settings
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);

      //default
      glPolygonMode(GL_FRONT,GL_FILL);

			// Enter GLUT's main loop
			glutMainLoop();
		}
		else cout << "Try again! Amount of arguments incorrect" << endl;
	}
	else cout << "Try again! Use: draw <filename.3d>" << endl;

	return 1;
}
