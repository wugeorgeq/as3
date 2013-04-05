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

GLenum shadeModel = GL_SMOOTH;
GLenum polygonMode = GL_FILL; 

cParser* c_Parser = NULL;
float step;
cSubdivide* c_Sub = NULL;
//vector<surfacePointAndNorm> tempSurf;
vector< vector<surfacePointAndNorm> > allUniform;

//want a global vector of patches, just to draw patches and see what it looks like
vector<patch> patchesTesting;

// want a global vector of triangles
vector< vector<triangle> > triTesting;

vector<triangle> allAdapt;

// int uniform or adaptive, uniform = 0, adaptive = 1
int u_or_a;





// from website
float XUP[3] = {1,0,0}, XUN[3] = {-1, 0, 0},
      YUP[3] = {0,1,0}, YUN[3] = { 0,-1, 0},
      ZUP[3] = {0,0,1}, ZUN[3] = { 0, 0,-1},
      ORG[3] = {0,0,0};
 
GLfloat viewangle = 0, tippangle = 0, traj[120][3];
 
GLfloat d[3] = {0.1, 0.1, 0.1};
 
GLfloat  xAngle = 0.0, yAngle = 0.0, zAngle = 0.0;








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
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); 
	glEnable(GL_NORMALIZE);
	//GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat light_ambient0[] = {0.2f, 0.2f, 0.2f, 1.0f};
	GLfloat light_diffuse0[] = {0.8f, 0.8f, 0.8f, 1.0f};
	GLfloat light_specular0[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);
	GLfloat lightPos0[] = {0.0f, 2.0f, -0.5f, 1.0f};
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	GLfloat green[] = {0.0f, 1.0f, 0.0f, 1.0f};
	GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};
	GLfloat blue[] = {0.0f, 0.0f, 1.0f, 1.0f};
	GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat lowAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
	GLfloat shininess[] = {128.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT, blue);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, lowAmbient);

	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mcolor);
	//glLightModelfv(GL_LIGH_MODEL_AMBIENT, global_ambient);
	// Nothing to do here for this simple example.

}


//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;
  
  if(h == 0) { h = 1; }
  float ratio = 1.0* w / h;

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
// toggle commands
//****************************************************
void mykeyFunc(unsigned char key, int x, int y){
  if(key == 'w'){
    if(polygonMode == GL_FILL){
      polygonMode == GL_LINE;
    }else{
      polygonMode == GL_FILL;
    }
  }else if(key == 's'){
    if(shadeModel == GL_FLAT){
      shadeModel = GL_SMOOTH;
    }else{
      shadeModel = GL_FLAT;
    }
  }
  	if (key == 27)
		exit(0);
		
	switch (key) {
       case 'j' : d[0] += 0.1;  break;
       case 'k' : d[1] += 0.1;  break;
       case 'l' : d[2] += 0.1;  break;
       }
       glutPostRedisplay();
}

/*
void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 0.1f;

	switch (key) {
		case GLUT_KEY_LEFT :
			cout << "left" << endl;
			
			angle -= 0.01f;
			lx = sin(angle);
			lz = -cos(angle);
		
			break;
		case GLUT_KEY_RIGHT :
			cout << "right" << endl;
			
			angle += 0.01f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_UP :
			cout << "up" << endl;
			
			x += lx * fraction;
			z += lz * fraction;
			break;
		case GLUT_KEY_DOWN :
			cout << "down" << endl;
			
			x -= lx * fraction;
			z -= lz * fraction;
			break;
	}
}
*/

void Special_Keys (int key, int x, int y)
{
    switch (key) {
 
       case GLUT_KEY_LEFT :  viewangle -= 5;  break;
       case GLUT_KEY_RIGHT:  viewangle += 5;  break;
       case GLUT_KEY_UP   :  tippangle -= 5;  break;
       case GLUT_KEY_DOWN :  tippangle += 5;  break;
 
       default: printf ("   Special key %c == %d\n", key, key);
    }
 
    glutPostRedisplay();
}

//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       // clear the color buffer
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);             // indicate we are specifying camera transformations
	glLoadIdentity();               // make sure transformation is "zero'd"
	
	
	// cout << "called" << endl;
	
	glRotatef (tippangle, 1,0,0);  // Up and down arrow keys 'tip' view.
    glRotatef (viewangle, 0,0,1);  // Right/left arrow keys 'turn' view.
	
    glShadeModel(shadeModel);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	
	glPushMatrix();
	glTranslatef (d[0], d[1], d[2]);    // Move box down X axis.
  
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
	
	/*
	// this is testing the triangles formed from the patches, begin adaptive testing
	Vector3f v0, v1, v2;
	for (int i = 0; i < triTesting.size(); i++) {
		vector<triangle> surf = triTesting[i];
		for (int j = 0; j < surf.size(); j++) {
			triangle tri = surf[j];
			v0 = tri.vertices[3];
			// cout << v0 << endl << endl;
			v1 = tri.vertices[4];
			// cout << v1 << endl << endl;
			v2 = tri.vertices[5];
			// cout << v2 << endl << endl;
			glBegin(GL_POLYGON);
			glVertex3f(v0(0), v0(1), v0(2));
			glVertex3f(v1(0), v1(1), v1(2));
			glVertex3f(v2(0), v2(1), v2(2));
			glEnd();
		}
	}
	*/
	
	
	if (u_or_a == 0) {	
	
		// this is uniform subdivision, drawing polygons
		int numDiv = int((1/step)+.5);
		int loopEnd;
		Vector3f vect0, vect1, vect2, vect3, norm0;
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
				if (((j+2)%(numDiv+1))==0) {
					j++;
				}
			}
		}
		
	} else {
	
		//drawing triangles from adaptiveSubdivision
		Vector3f v0, v1, v2;
		for (int i = 0; i < allAdapt.size(); i++) {
			triangle tri = allAdapt[i];
			v0 = tri.vertices[3];
			v1 = tri.vertices[4];
			v2 = tri.vertices[5];
			glBegin(GL_POLYGON);
			glVertex3f(v0(0), v0(1), v0(2));
			glVertex3f(v1(0), v1(1), v1(2));
			glVertex3f(v2(0), v2(1), v2(2));
			glEnd();
		}
	}
	glPopMatrix();
	
	glFlush();
  	glutSwapBuffers();          // swap buffers (we earlier set double buffer)
}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27)
		exit(0);
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
	
	// uniform or adaptive from parser
	u_or_a = c_Parser->GetAdapt();
	
	c_Sub = new cSubdivide();
	//vector<surfacePointAndNorm> unif_surf_norm = c_Sub->subdivideUniform(vect_patch[0]);
	
	if (u_or_a == 0) {
		for (int i = 0; i < numOfPatches; i++) {
			vector<surfacePointAndNorm> unifSurfPoints = c_Sub->subdivideUniform(vect_patch[i], step);
			allUniform.push_back(unifSurfPoints);
		}
	} else {
		for (int i = 0; i < numOfPatches; i++) {
			vector<triangle> tri = c_Sub->subdivideAdaptive(vect_patch[i], step);
			triTesting.push_back(tri);
		}
		allAdapt = c_Sub->getAdaptTri();
	}
	
	//tempSurf = unif_surf_norm;
	
	//testing with patches to see what looks like
	patchesTesting = vect_patch;
	
	
	
	//This initializes glut
  	glutInit(&argc, argv);

  	//This tells glut to use a double-buffered window with red, green, and blue channels 
  	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  	// Initalize theviewport size
  	viewport.w = 400;
  	viewport.h = 400;

  	//The size and position of the window
  	glutInitWindowSize(viewport.w, viewport.h);
  	glutInitWindowPosition(0,0);
  	glutCreateWindow(argv[0]);
  
  	initScene();							// quick function to set up scene

    
    glutReshapeFunc(myReshape);        // function to run when the window gets resized
  	glutDisplayFunc(myDisplay);				// function to run when its time to draw somethin
  	glutKeyboardFunc(mykeyFunc);
  	glutSpecialFunc(Special_Keys);

  	glutMainLoop();							// infinite loop that will keep drawing and resizing
  	// and whatever else
  	
  	delete c_Parser;
  	delete c_Sub;
	return 0;

}






