#ifndef PARSER_H
#define PARSER_H

using namespace Eigen;
using namespace std;

/* This class handles the parsing of the input .bez file
 *
 *
 *
 */

struct patch {
	vector<Vector3f> p0;
	vector<Vector3f> p1;
	vector<Vector3f> p2;
	vector<Vector3f> p3;
	patch() {
		vector<Vector3f> a;
		p0 = a;
		p1 = a;
		p2 = a;
		p3 = a;
	}
	patch(vector<Vector3f> v) {
		vector<Vector3f> a;
		vector<Vector3f> b;
		vector<Vector3f> c;
		vector<Vector3f> d;
		a.push_back(v[0]);
		a.push_back(v[1]);
		a.push_back(v[2]);
		a.push_back(v[3]);
		b.push_back(v[4]);
		b.push_back(v[5]);
		b.push_back(v[6]);
		b.push_back(v[7]);
		c.push_back(v[8]);
		c.push_back(v[9]);
		c.push_back(v[10]);
		c.push_back(v[11]);
		d.push_back(v[12]);
		d.push_back(v[13]);
		d.push_back(v[14]);
		d.push_back(v[15]);
		p0 = a;
		p1 = b;
		p2 = c;
		p3 = d;
	}
	patch(vector<Vector3f> a, vector<Vector3f> b, vector<Vector3f> c, vector<Vector3f> d) {
		p0 = a;
		p1 = b;
		p2 = c;
		p3 = d;
	}
};

class cParser {
public:
	cParser(int, char**);
	~cParser();
	vector<patch> ParseCommandLine();
	const float GetParam() const {return m_param;}
	
		
	
private:
	int m_argc;
	char** m_argv;
	float m_param;


};

#endif