#include <vector>
#include "Subdivide.h"
#include "Parser.h"

using namespace Eigen;
using namespace std;

cSubdivide::cSubdivide()
{

}
cSubdivide::~cSubdivide() {

}

curvePointAndDeriv cSubdivide::bezCurveInterp(vector<Vector3f> curve, float pValue){

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

surfacePointAndNorm cSubdivide::bezPatchInterp(patch p, float u , float v){

	surfacePointAndNorm pair2;

	//make control points for a Bezier curve in v
	vector<curvePointAndDeriv> vCurve;
	
	vCurve.push_back(bezCurveInterp(p.p0, u));
	vCurve.push_back(bezCurveInterp(p.p1, u));
	vCurve.push_back(bezCurveInterp(p.p2, u));
	vCurve.push_back(bezCurveInterp(p.p3, u));

	//make control points for a Bezier curve in u
	vector<curvePointAndDeriv> uCurve;
	
	vector<Vector3f> col0;
    col0.push_back(p.p0[0]);
    col0.push_back(p.p1[0]);
    col0.push_back(p.p2[0]);
    col0.push_back(p.p3[0]);

    vector<Vector3f> col1;
    col0.push_back(p.p0[1]);
	col0.push_back(p.p1[1]);
	col0.push_back(p.p2[1]);
	col0.push_back(p.p3[1]);

	vector<Vector3f> col2;
	col0.push_back(p.p0[2]);
	col0.push_back(p.p1[2]);
	col0.push_back(p.p2[2]);
	col0.push_back(p.p3[2]);

	vector<Vector3f> col3;
	col0.push_back(p.p0[3]);
	col0.push_back(p.p1[3]);
	col0.push_back(p.p2[3]);
	col0.push_back(p.p3[3]);
	
	uCurve.push_back(bezCurveInterp(p.p0, v));
	uCurve.push_back(bezCurveInterp(p.p1, v));
	uCurve.push_back(bezCurveInterp(p.p2, v));
	uCurve.push_back(bezCurveInterp(p.p3, v));

	//evaluate surface and derivative for u and v
	curvePointAndDeriv newV, newU;
	vector<Vector3f> tempU, tempV;
	for (int i = 0; i < 4; i++){
		tempU.push_back(uCurve[i].curvePoint);
		tempV.push_back(vCurve[i].curvePoint);
	}
	
	newV = bezCurveInterp(tempV, v);
	newU = bezCurveInterp(tempU, u);

	//cross product of partials to find normal
	Vector3f n = (newU.deriv).cross(newV.deriv);
	n = n/ n.norm();

	pair2.surfacePoint = newV.curvePoint;
	pair2.norm = n;

	return pair2;
}

vector<surfacePointAndNorm> cSubdivide::subdivideUniform(patch patch){

	vector<surfacePointAndNorm> savedSurfacePointAndNormal;
	float step = .1;

	//compute # of subdivisions there are for this step size
	float numDiv = 1/step;

	//for each parametric value of u
	for(int i = 0; i < numDiv; i++){
		float u = i*step;

		for(int j = 0; j < numDiv; j++){
			float v = j*step;
			surfacePointAndNorm x = bezPatchInterp(patch, u, v);
			savedSurfacePointAndNormal.push_back(x);
			
		}
	}
	return savedSurfacePointAndNormal;

}

