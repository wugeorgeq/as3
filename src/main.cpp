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
	
	cout << "testing Eigen" << endl;
	
	Vector3f testVect;
	testVect << 1, 2, 3;
	cout << testVect << endl;
	
	c_Parser = new cParser(argc, argv);
	int temp = c_Parser->ParseCommandLine();
	
	delete c_Parser;
	return 0;

}