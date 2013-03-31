//Class that subdivides Bezier objects

#ifndef _SUBDIVIDE_H
#define -_SUBDIVIDE_H

#include <vector>
#include "Parser.h"

struct curvePointAndDeriv {
	float curvePoint;
	float deriv;
};

struct surfacePointAndNorm {
	float surfacePoint;
	vector<Vector3f> norm;
};

class Subdivide {
public:
	//cSubdivide(curvePointAndDeriv, surfacePointAndNorm);
	//virtual cSubdivide();

	virtual curvePointAndDeriv* bezCurveInterp(Vector3f, float) {}
	virtual surfacePointAndNorm* bezPatchInterp(Vector3f, float, float) {}
	//virtual void subdividePatch(Vector3f);
	virtual void subdivideUniform(Vector3f) {}
	virtual void subdivideAdapt(Vector3f) {}
	float getParam() const {return param;}


private:
	float param;

};







#endif