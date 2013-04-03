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
	// should be length 5, storing first two u,v then second three actual coords
	vector<Vector3f> vertices;
	// should be length 3
	vector<Vector3f> norm;
};

class cSubdivide {
public:
	cSubdivide();
	~cSubdivide();

	curvePointAndDeriv bezCurveInterp(vector<Vector3f>, float); 
	surfacePointAndNorm bezPatchInterp(patch, float, float); 
	//virtual void subdividePatch(Vector3f);
	vector<surfacePointAndNorm> subdivideUniform(patch, float);
	vector<triangle> subdivideAdaptive(patch, float);
	float getParam() const {return param;}


private:
	float param;

};







#endif