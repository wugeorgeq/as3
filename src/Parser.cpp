#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <Eigen/Dense>
#include <map>
#include <stack>
#include <cfloat>

#include "Parser.h"

using namespace std;
using namespace Eigen;

cParser::cParser(int argc, char** argv):
	m_argc(argc), m_argv(argv)
{

}

cParser::~cParser() {

}

/* Parse Command Line
 * Tell program to exit if incorrect parameter given 
 * Pull patches and put in vector of patches
 * patch struct found in Parser.h
 */
vector<patch> cParser::ParseCommandLine() {
	// print something if commands are incorrect
	if ((m_argc != 4) && (m_argc != 3)) {
		printf("./as3 [INPUT_FILE] [PARAMETER] [OPTIONAL_FLAG] \n");
	}
	
	m_param = atof(m_argv[2]);
		
	ifstream inputfile (m_argv[1]);
	string line;
	vector<patch> p;
	if (inputfile.is_open()) {
		getline(inputfile, line);
		double numPatches = atof(line.c_str());
		// each patch has 4 lines, total of 16 ctrl points
		for (int i = 0; i < numPatches; i++) {
			// this vector will have 16 ctrl points
			vector<Vector3f> lineVect;
			// each line in patch has 4 ctrl points
			for (int j = 0; j < 4; j++) {
				getline(inputfile, line);
				vector<string> params;
				istringstream linestream(line);
				// Copy all the words in the command to our vector
				copy(istream_iterator<string>(linestream),
					istream_iterator<string>(),
					back_inserter<vector<string> >(params));
				for (int k = 0; k < 4; k++) {
					Vector3f vect(atof(params[k*3].c_str()), atof(params[k*3+1].c_str()), atof(params[k*3+2].c_str()));
					lineVect.push_back(vect);
				}
			}
			patch pTemp(lineVect);
			p.push_back(pTemp);
			// getline to account for the empty line
			getline(inputfile, line);
		}
	}
	inputfile.close();
	return p;

}


















