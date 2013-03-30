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

#define PI 3.14159265  // Should be used from mathlib

using namespace std;
using namespace Eigen;

cParser* c_Parser = NULL;

int main(int argc, char** argv) {

	cout << "in main" << endl;
	
	// parser class takes in .bez file
	// for example, make, then type in ./as3 patches/test.bez .1
	c_Parser = new cParser(argc, argv);
	// vector of patches is returned from parser
	vector<patch> temp = c_Parser->ParseCommandLine();
	
	// gonna test to see if patch stuffy stuff worked
	patch patch2 = temp[1];
	Vector3f walla = patch2.p2[1];
	cout << "should be .33 .66 2.00 " << endl;
	cout << walla << endl;
	
	delete c_Parser;
	return 0;

}






