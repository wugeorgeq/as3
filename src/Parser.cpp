#include "Parser.h"
#include <Eigen/Dense>

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
 * Pull patches and put in some kind of data structure
 */
 
int cParser::ParseCommandLine() {
	cout << "parse" << endl;
	return 1;
}
