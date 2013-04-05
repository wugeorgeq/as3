//Class that subdivides Bezier objects

#ifndef _SUBDIVIDE_H
#define _SUBDIVIDE_H

#include <vector>
#include <Eigen/Dense>
#include "Parser.h"

using namespace Eigen;

struct curvePointAndDeriv {
	Vector3f curvePoint;
	Vector3f deriv;
};

struct surfacePointAndNorm {
	Vector3f surfacePoint;
	Vector3f norm;
};

struct triangle {
	// should be length 6, storing u,v then actual coords
	vector<Vector3f> vertices;
	// should be length 3
	vector<Vector3f> norm;
};

class cSubdivide {
public:
	cSubdivide();
	~cSubdivide();

	static curvePointAndDeriv bezCurveInterp(vector<Vector3f>, float); 
	static surfacePointAndNorm bezPatchInterp(patch, float, float); 
	//virtual void subdividePatch(Vector3f);
	vector<surfacePointAndNorm> subdivideUniform(patch, float);
	vector<triangle> subdivideAdaptive(patch, float);
	void checkTri(triangle, patch, float);
	vector<triangle> getAdaptTri() {return adaptTri;}
private:
	vector<triangle> adaptTri;
};







#endif