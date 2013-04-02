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
	vector<Vector3f> r0;
	vector<Vector3f> r1;
	vector<Vector3f> r2;
	vector<Vector3f> r3;
	/*
	vector<Vector3f> c0;
	vector<Vector3f> c1;
	vector<Vector3f> c2;
	vector<Vector3f> c3;
	*/
	
	patch() {
		vector<Vector3f> a;
		r0 = a;
		r1 = a;
		r2 = a;
		r3 = a;
		/*
		c0 = a;
		c1 = a;
		c2 = a;
		c3 = a;
		*/
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
		r0 = a;
		r1 = b;
		r2 = c;
		r3 = d;
		/*
		c0 = a;
		c1 = b;
		c2 = c;
		c3 = d;
		
		vector<Vector3f> e;
		vector<Vector3f> f;
		vector<Vector3f> g;
		vector<Vector3f> h;
		e.push_back(v[0]);
		f.push_back(v[1]);
		g.push_back(v[2]);
		h.push_back(v[3]);
		e.push_back(v[4]);
		f.push_back(v[5]);
		g.push_back(v[6]);
		h.push_back(v[7]);
		e.push_back(v[8]);
		f.push_back(v[9]);
		g.push_back(v[10]);
		h.push_back(v[11]);
		e.push_back(v[12]);
		f.push_back(v[13]);
		g.push_back(v[14]);
		h.push_back(v[15]);
		c0 = e;
		c1 = f;
		c2 = g;
		c3 = h;
		*/
	}
};

class cParser {
public:
	cParser(int, char**);
	~cParser();
	vector<patch> ParseCommandLine();
		
	
private:
	int m_argc;
	char** m_argv;


};

#endif