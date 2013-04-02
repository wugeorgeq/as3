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
	vector<Vector3f> surfacePoint;
	vector<Vector3f> norm;
};

class cSubdivide {
public:
	cSubdivide();
	~cSubdivide();

	virtual curvePointAndDeriv bezCurveInterp(vector<Vector3f>, float) {}
	virtual surfacePointAndNorm bezPatchInterp(vector<Vector3f>, float, float) {}
	//virtual void subdividePatch(Vector3f);
	virtual void subdivideUniform(vector<Vector3f>) {}
	virtual void subdivideAdapt(Vector3f) {}
	float getParam() const {return param;}


private:
	float param;

};







#endif