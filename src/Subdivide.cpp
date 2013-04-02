#include <vector>
#include "Subdivide.h"
#include "Parser.h"

using namespace Eigen;
using namespace std;

cSubdivide::cSubdivide()
{

}

<<<<<<< HEAD
curvePointAndDeriv cSubdivide::bezCurveInterp(vector<Vector3f> curve, float pValue){
=======
	curvePointAndDeriv cSubdivide::bezCurveInterp(Vector3f curve, float pValue){
>>>>>>> tried to change patch, but failed lol
	curvePointAndDeriv pair;

	//split 3 segments of the curve into two: AB and BC
	Vector3f a = curve[0] * (1.0-pValue) + curve[1] * pValue;
	Vector3f b = curve[1] * (1.0-pValue) + curve[2] * pValue;
	Vector3f c = curve[2] * (1.0-pValue) + curve[3] * pValue;

	//split AB and BC to form a new segment DE
	Vector3f d = a * (1.0-pValue) + b * pValue;
	Vector3f e = b * (1.0-pValue) + c * pValue;

	//pick the right point on DE
	Vector3f p = d * (1.0-pValue) + e * pValue;

	//compute the derivative
	Vector3f dPdu = 3 * (e-d);

	pair.curvePoint = p;
	pair.deriv = dPdu;
	return pair;
}

surfacePointAndNorm cSubdivide::bezPatchInterp(vector<Vector3f> patch, float u , float v){
	surfacePointAndNorm pair2;

	//make control points for a Bezier curve in v
	vector<curvePointAndDeriv> vCurve;
	vCurve(0) = bezCurveInterp(patch.r0, u);
	vCurve(1) = bezCurveInterp(patch.r1, u);
	vCurve(2) = bezCurveInterp(patch.r2, u);
	vCurve(3) = bezCurveInterp(patch.r3, u);

	//make control points for a Bezier curve in u
	vector<curvePointAndDeriv> uCurve;
	uCurve(0) = bezCurveInterp(patch.c0, v);
	uCurve(1) = bezCurveInterp(patch.c1, v);
	uCurve(2) = bezCurveInterp(patch.c2, v);
	uCurve(3) = bezCurveInterp(patch.c3, v);

	//evaluate surface and derivative for u and v
	curvePointAndDeriv newV, newU;
	newV = bezCurveInterp(vCurve, v);
	newU = bezCurveInterp(uCurve, u);

	//cross product of partials to find normal
	Vector3f n = (newU.deriv).cross(newV.deriv);
	n = n/length(n);

	pair2.surfacePoint = newV.curvePoint;
	pair2.norm = n;

	return pair2;
}

void cSubdivide::subdivideUniform(vector<Vector3f> patch){
	vector<Vector3f> savedSurfacePointAndNormal;
	int index = 0;
	float step = getParam();

	//compute # of subdivisions there are for this step size
	float numDiv = 1/step;

	//for each parametric value of u
	for(int i = 0; i <= numDiv; i+=numDiv){
		float u = i*step;

		for(int j = 0; j <= numDiv; j+=numDiv){
			float v = j*step;

			surfacePointAndNorm x = bezPatchInterp(patch, u, v);
			savedSurfacePointAndNormal(index) = x;
			index++;
		}
	}

}

cSubdivide::~cSubdivide() {

}
