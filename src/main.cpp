#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include <math.h>
#include <Eigen/Dense>

#include "Parser.h"
#include "Subdivide.h"

#define PI 3.14159265  // Should be used from mathlib

using namespace std;
using namespace Eigen;

cParser* c_Parser = NULL;
float step;
cSubdivide* c_Sub = NULL;
//vector<surfacePointAndNorm> tempSurf;
vector< vector<surfacePointAndNorm> > allUniform;

//want a global vector of patches, just to draw patches and see what it looks like
vector<patch> patchesTesting;

//****************************************************
// Some Classes
//****************************************************
class Viewport;

class Viewport {
  public:
    int w, h; // width and height
};

//****************************************************
// Global Variables
//****************************************************
Viewport	viewport;

//****************************************************
// Simple init function
//****************************************************
void initScene(){
	/*
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	*/
  // Nothing to do here for this simple example.

}


//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;

  glViewport (0,0,viewport.w,viewport.h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluOrtho2D(0, viewport.w, 0, viewport.h);
  
  // from example_00
  glOrtho(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);

}


//****************************************************
// A routine to set a pixel by drawing a GL point.  This is not a
// general purpose routine as it assumes a lot of stuff specific to
// this example.
//****************************************************

void setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f(r, g, b);
  glVertex2f(x + 0.5, y + 0.5);   // The 0.5 is to target pixel
  // centers 
  // Note: Need to check for gap
  // bug on inst machines.
}

//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {

	glClear(GL_COLOR_BUFFER_BIT);       // clear the color buffer

	glMatrixMode(GL_MODELVIEW);             // indicate we are specifying camera transformations
	glLoadIdentity();               // make sure transformation is "zero'd"
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  
	//glColor3f(1.0f,0.0f,0.0f);                   // setting the color to pure red 90% for the rect
	
	/*
	// testing silliness
	
	glBegin(GL_POLYGON);                         // draw rectangle 
  //glVertex3f(x val, y val, z val (won't change the point because of the projection type));
	glVertex3f(-0.8f, 0.0f, 0.0f);               // bottom left corner of rectangle
	glVertex3f(-0.8f, 0.5f, 0.0f);               // top left corner of rectangle
	glVertex3f( 0.0f, 0.5f, 0.0f);               // top right corner of rectangle
	glVertex3f( 0.0f, 0.0f, 0.0f);               // bottom right corner of rectangle
	glEnd();
	
	glColor3f(1.0f,1.0f,0.0f);
	
	glBegin(GL_POLYGON);                         // draw rectangle 
  //glVertex3f(x val, y val, z val (won't change the point because of the projection type));
	glVertex3f(-0.2f, 0.0f, -1.0f);               // bottom left corner of rectangle
	glVertex3f(-0.2f, 0.2f, -1.0f);               // top left corner of rectangle
	glVertex3f( 0.0f, 0.2f, -1.0f);               // top right corner of rectangle
	glVertex3f( 0.0f, 0.0f, -1.0f);               // bottom right corner of rectangle
	glEnd();
	
	glBegin(GL_POINTS);
	glVertex3f(1.5f, 1.5f, 1.5f);
	glEnd();
	*/
	
	/*
	glBegin(GL_POINTS);
	
	
	// this is the code for drawing points after uniform subdivision
	for (int i = 0; i < allUniform.size(); i++) {
		vector<surfacePointAndNorm> surf = allUniform[i];
		for (int j = 0; j < surf.size(); j++) {
			float p0 = surf[j].surfacePoint(0);
			float p1 = surf[j].surfacePoint(1);
			float p2 = surf[j].surfacePoint(2);
			glVertex3f(p0, p1, p2);
		}
	}
	glEnd();
	*/
	
	/*
	//testing with patches for points
	cout << "num of patches " << patchesTesting.size() << endl;
	float f0, f1, f2;
	Vector3f tempVect0, tempVect1, tempVect2, tempVect3;
	for (int i = 0; i < patchesTesting.size(); i++) {
		patch patchTemp = patchesTesting[i];
		vector<Vector3f> p0 = patchTemp.p0;
		vector<Vector3f> p1 = patchTemp.p1;
		vector<Vector3f> p2 = patchTemp.p2;
		vector<Vector3f> p3 = patchTemp.p3;
		for (int k = 0; k < 4; k++) {
			tempVect0 = p0[k];
			f0 = tempVect0(0);
			f1 = tempVect0(1);
			f2 = tempVect0(2);
			glVertex3f(f0,f1,f2);
			tempVect1 = p1[k];
			f0 = tempVect1(0);
			f1 = tempVect1(1);
			f2 = tempVect1(2);
			glVertex3f(f0,f1,f2);
			tempVect2 = p2[k];
			f0 = tempVect2(0);
			f1 = tempVect2(1);
			f2 = tempVect2(2);
			glVertex3f(f0,f1,f2);
			tempVect3 = p3[k];
			f0 = tempVect3(0);
			f1 = tempVect3(1);
			f2 = tempVect3(2);
			glVertex3f(f0,f1,f2);
		}
	}
	*/
	int numDiv = int((1/step)+.5);
	int loopEnd;
	Vector3f vect0, vect1, vect2, vect3;
	// drawing polygons from vertex of SurfacePointAndNormals
	for (int i = 0; i < allUniform.size(); i++) {
		vector<surfacePointAndNorm> surf = allUniform[i];
		loopEnd = ((numDiv+1)*(numDiv+1))-(numDiv+2);
		for (int j = 0; j < loopEnd; j++) {
			glBegin(GL_POLYGON); 
			vect0 = surf[j].surfacePoint;
			glVertex3f(vect0(0), vect0(1), vect0(2));
			vect1 = surf[j+1].surfacePoint;
			glVertex3f(vect1(0), vect1(1), vect1(2));
			vect2 = surf[j+numDiv+2].surfacePoint;
			glVertex3f(vect2(0), vect2(1), vect2(2));
			vect3 = surf[j+numDiv+1].surfacePoint;
			glVertex3f(vect3(0), vect3(1), vect3(2));
			glEnd();
			if (((j+1)%(numDiv+1))==0) {
				j++;
			}
		}
	}
	
	
	glFlush();
  	glutSwapBuffers();          // swap buffers (we earlier set double buffer)
}


int main(int argc, char** argv) {

	
	// parser class takes in .bez file
	// for example, make, then type in ./as3 patches/test.bez .1
	c_Parser = new cParser(argc, argv);
	// vector of patches is returned from parser
	vector<patch> vect_patch = c_Parser->ParseCommandLine();
	int numOfPatches = vect_patch.size();
	
	// step size from parser
	step = c_Parser->GetParam();
	cout << step << endl;
	
	c_Sub = new cSubdivide();
	//vector<surfacePointAndNorm> unif_surf_norm = c_Sub->subdivideUniform(vect_patch[0]);
	
	for (int i = 0; i < numOfPatches; i++) {
		vector<surfacePointAndNorm> unifSurfPoints = c_Sub->subdivideUniform(vect_patch[i], step);
		allUniform.push_back(unifSurfPoints);
	}
	
	c_Sub->subdivideAdaptive(vect_patch[0], step);
	
	//tempSurf = unif_surf_norm;
	
	//testing with patches to see what looks like
	patchesTesting = vect_patch;
	
	
	
	//This initializes glut
  	glutInit(&argc, argv);

  	//This tells glut to use a double-buffered window with red, green, and blue channels 
  	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  	// Initalize theviewport size
  	viewport.w = 400;
  	viewport.h = 400;

  	//The size and position of the window
  	glutInitWindowSize(viewport.w, viewport.h);
  	glutInitWindowPosition(0,0);
  	glutCreateWindow(argv[0]);
  
  	initScene();							// quick function to set up scene

  	glutDisplayFunc(myDisplay);				// function to run when its time to draw something
  	glutReshapeFunc(myReshape);				// function to run when the window gets resized

  	glutMainLoop();							// infinite loop that will keep drawing and resizing
  	// and whatever else
  	
  	delete c_Parser;
  	delete c_Sub;
	return 0;

}






