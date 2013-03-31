#include <vector>
#include "Subdivide.h"

using namespace Eigen;
using namespace std;

/*cSubdivide::cSubdivide()
{

}

curvePointAndDeriv cSubdivide::bezCurveInterp(Vector3f curve, float pValue){
	curvePointAndDeriv pair;

	//split 3 segments of the curve into two: AB and BC
	float a = curve(0) * (1.0-pValue) + curve(1) * pValue;
	float b = curve(1) * (1.0-pValue) + curve(2) * pValue;
	float c = curve(2) * (1.0-pValue) + curve(3) * pValue;

	//split AB and BC to form a new segment DE
	float d = a * (1.0-pValue) + b * pValue;
	float e = b * (1.0-pValue) + c * pValue;

	//pick the right point on DE
	float p = d * (1.0-pValue) + e * pValue;

	//comput the derivative
	float dPdu = 3 * (e-d);

	pair.curvePoint = p;
	pair.deriv = dPdu;
	return pair;
}

surfacePointAndNorm cSubdivide::bezPatchInterp(Vector3f patch, float u , float v){
	surfacePointAndNorm pair;

	//make control points for a Bezier curve in v
	curvePointAndDeriv vCurve;
	vCurve(0) = bezCurveInterp(patch(0), u);
	vCurve(1) = bezCurveInterp(patch(1), u);
	vCurve(2) = bezCurveInterp(patch(2), u);
	vCurve(3) = bezCurveInterp(patch(3), u);

	//make control points for a Bezier curve in u
	curvePointAndDeriv uCurve;
	uCurve(0) = bezCurveInterp(patch(0), v);
	uCurve(1) = bezCurveInterp(patch(1), v);
	uCurve(2) = bezCurveInterp(patch(2), v);
	uCurve(3) = bezCurveInterp(patch(3), v);

	//evaluate surface and derivative for u and v
	curvePointAndDeriv newV, newU;
	newV = bezCurveInterp(vCurve, v);
	newU = bezCurveInterp(uCurve, u);

	//cross product of partials to find normal
	Vector3f n;
	

	pair.surfacePoint = newV(0);
	pair.norm = n;

	return n; //FIX!!
}

void cSubdivide::subdivideUniform(Vector3f patch){
	vector<Vector3f> savedSurfacePointAndNormal;
	float step = getParam();

	//compute # of subdivisions there are for this step size
	float numDiv; //FIX!!!

	//for each parametric value of u
	for(int i = 0; i < numDiv; i++){
		float u = uPValue*step;

		for(int j = 0; j < numDiv; j++){
			float v = vPValue*step;

			surfacePointAndNorm x = bezPatchInterp(patch, u, v);
			savedSurfacePointAndNormal(j) = x;
		}
	}

}

cSubdivide::~cSubdivide() {

}*/