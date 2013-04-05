#include <vector>
#include <cmath>
#include "Subdivide.h"
#include "Parser.h"

using namespace Eigen;
using namespace std;

cSubdivide::cSubdivide()
{
	vector<triangle> newVect;
	adaptTri = newVect;
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

vector<surfacePointAndNorm> cSubdivide::subdivideUniform(patch patch, float step){

	vector<surfacePointAndNorm> savedSurfacePointAndNormal;

	//compute # of subdivisions there are for this step size
	float numDiv = 1/step;

	//for each parametric value of u
	for(int i = 0; i <= numDiv; i++){
		float u = i*step;

		for(int j = 0; j <= numDiv; j++){
			float v = j*step;
			surfacePointAndNorm x = bezPatchInterp(patch, u, v);
			savedSurfacePointAndNormal.push_back(x);
			
		}
	}
	return savedSurfacePointAndNormal;

}

vector<triangle> cSubdivide::subdivideAdaptive(patch patch, float param) {
	// create 2d array of floats for u, v for the patch
	Vector3f coords[4][4];
	// aaaand 1d array lol
	vector<Vector3f> cList;
	float third = 1.0/3.0;
	Vector3f in;
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			in << (i*third), (j*third), 0;
			coords[i][j] = in;
			cList.push_back(in);
		}
	}
	
	// first need to create initial list of triangles
	vector<triangle> tList;
	Vector3f c0, c1, c2, c3;
	surfacePointAndNorm spn0, spn1, spn2, spn3;
	triangle t0, t1;
	for (int i = 0; i < 11; i++) {
		// 6 vectors out of 4 -> 2 triangles per loop
		c0 = cList[i];
		c1 = cList[i+1];
		c2 = cList[i+5];
		c3 = cList[i+4];
		// get surface point and normals of 4 points
		spn0 = cSubdivide::bezPatchInterp(patch, c0(0), c0(1));
		spn1 = cSubdivide::bezPatchInterp(patch, c1(0), c1(1));
		spn2 = cSubdivide::bezPatchInterp(patch, c2(0), c2(1));
		spn3 = cSubdivide::bezPatchInterp(patch, c3(0), c3(1));
		// form the length 6 vectors with u, v, surface points
		vector<Vector3f> t0ver, t1ver;
		t0ver.push_back(c0);
		t0ver.push_back(c1);
		t0ver.push_back(c3);
		t0ver.push_back(spn0.surfacePoint);
		t0ver.push_back(spn1.surfacePoint);
		t0ver.push_back(spn3.surfacePoint);
		t1ver.push_back(c1);
		t1ver.push_back(c2);
		t1ver.push_back(c3);
		t1ver.push_back(spn1.surfacePoint);
		t1ver.push_back(spn2.surfacePoint);
		t1ver.push_back(spn3.surfacePoint);
		// form length three norm vectors
		vector<Vector3f> t0norm, t1norm;
		t0norm.push_back(spn0.norm);
		t0norm.push_back(spn1.norm);
		t0norm.push_back(spn3.norm);
		t1norm.push_back(spn1.norm);
		t1norm.push_back(spn2.norm);
		t1norm.push_back(spn3.norm);
		t0.vertices = t0ver;
		t1.vertices = t1ver;
		t0.norm = t0norm;
		t1.norm = t1norm;
		tList.push_back(t0);
		tList.push_back(t1);
		if ((i==2)||(i==6)) {
			i++;
		}
	}
	
	// cout << "right before checking triangles" << endl;
	for (int i = 0; i < 18; i++) {
		// cout << "checking triangle vertices" << endl;
		cSubdivide::checkTri(tList[i], patch, param);
		
	}
	
	
	return this->adaptTri;
}

void cSubdivide::checkTri(triangle tri, patch patch, float param) {
	// getting the u,v coordinates
	Vector3f uv0 = tri.vertices[0];
	Vector3f uv1 = tri.vertices[1];
	Vector3f uv2 = tri.vertices[2];
	Vector3f v0 = tri.vertices[3];
	Vector3f v1 = tri.vertices[4];
	Vector3f v2 = tri.vertices[5];
	Vector3f mid_v0 = (v0+v2)/2;
	Vector3f mid_v1 = (v0+v1)/2;
	Vector3f mid_v2 = (v1+v2)/2;
	Vector3f tri_norm0 = tri.norm[0];
	Vector3f tri_norm1 = tri.norm[1];
	Vector3f tri_norm2 = tri.norm[2];
	Vector3f mid_uv0 = (uv0+uv2)/2;
	Vector3f mid_uv1 = (uv0+uv1)/2;
	Vector3f mid_uv2 = (uv1+uv2)/2;
	surfacePointAndNorm spn0 = cSubdivide::bezPatchInterp(patch, mid_uv0(0), mid_uv0(1));
	surfacePointAndNorm spn1 = cSubdivide::bezPatchInterp(patch, mid_uv1(0), mid_uv1(1));
	surfacePointAndNorm spn2 = cSubdivide::bezPatchInterp(patch, mid_uv2(0), mid_uv2(1));
	Vector3f b0 = spn0.surfacePoint;
	Vector3f b1 = spn1.surfacePoint;
	Vector3f b2 = spn2.surfacePoint;
	Vector3f mid_norm0 = spn0.norm;
	Vector3f mid_norm1 = spn1.norm;
	Vector3f mid_norm2 = spn2.norm;
	int int0 = 1;
	int int1 = 1;
	int int2 = 1;
	// cout << "b0 " << endl << b0 << endl;
	// cout << "mid_uv0 " << endl << mid_uv0 << endl;
	int test0 = abs((b0-mid_v0).norm());
	int test1 = abs((b1-mid_v1).norm());
	int test2 = abs((b2-mid_v2).norm());
	/*
	cout << "right before midpoint check " << test0 << endl;
	cout << test1 << endl;
	cout << test2 << endl;
	*/
	if (test0 < param) {
	/*
		cout << "yes int0 is 0" << endl;
		cout << "b0 " << endl << b0 << endl;
		cout << "mid_v0 " << endl << mid_v0 << endl;
		int test0 = abs((b0-mid_v0).norm());
		cout << "test0 " << test0 << endl;
		cout << "param " << param << endl;
	*/
		int0 = 0;
	}
	if (test1 < param) {
		// cout << "yes int1 is 0" << endl;
		int1 = 0;
	}
	if (test2 < param) {
		// cout << "yes int2 is 0" << endl;
		int2 = 0;
	}
	if ((int0==0)&&(int1==0)&&(int2==0)) {
		this->adaptTri.push_back(tri);
		//cout << "pushing onto stack!" << endl;
		return;
	} else {
		cout << "something's wrong lol" << endl; 
	}
	if ((int0==0)&&(int1==0)&&(int2==1)) {
		triangle t0, t1;
		t0.vertices.push_back(uv0);
		t0.vertices.push_back(uv1);
		t0.vertices.push_back(mid_uv0);
		t0.vertices.push_back(v0);
		t0.vertices.push_back(v1);
		t0.vertices.push_back(b0);
		t0.norm.push_back(tri_norm0);
		t0.norm.push_back(tri_norm1);
		t0.norm.push_back(mid_norm0);
		t1.vertices.push_back(mid_uv0);
		t1.vertices.push_back(uv1);
		t1.vertices.push_back(uv2);
		t1.vertices.push_back(b0);
		t1.vertices.push_back(v1);
		t1.vertices.push_back(v2);
		t1.norm.push_back(mid_norm0);
		t1.norm.push_back(tri_norm1);
		t1.norm.push_back(tri_norm2);
		cSubdivide::checkTri(t0, patch, param);
		cSubdivide::checkTri(t1, patch, param);
	} else if ((int0==0)&&(int1==1)&&(int2==0)) {
		triangle t0, t1;
		t0.vertices.push_back(uv0);
		t0.vertices.push_back(mid_uv1);
		t0.vertices.push_back(uv2);
		t0.vertices.push_back(v0);
		t0.vertices.push_back(b1);
		t0.vertices.push_back(v2);
		t0.norm.push_back(tri_norm0);
		t0.norm.push_back(mid_norm1);
		t0.norm.push_back(tri_norm2);
		t1.vertices.push_back(mid_uv1);
		t1.vertices.push_back(uv1);
		t1.vertices.push_back(uv2);
		t1.vertices.push_back(b1);
		t1.vertices.push_back(v1);
		t1.vertices.push_back(v2);
		t1.norm.push_back(mid_norm1);
		t1.norm.push_back(tri_norm1);
		t1.norm.push_back(tri_norm2);
		cSubdivide::checkTri(t0, patch, param);
		cSubdivide::checkTri(t1, patch, param);
	} else if ((int0==1)&&(int1==0)&&(int2==0)) {
		triangle t0, t1;
		t0.vertices.push_back(uv0);
		t0.vertices.push_back(mid_uv2);
		t0.vertices.push_back(uv2);
		t0.vertices.push_back(v0);
		t0.vertices.push_back(b2);
		t0.vertices.push_back(v2);
		t0.norm.push_back(tri_norm0);
		t0.norm.push_back(mid_norm2);
		t0.norm.push_back(tri_norm2);
		t1.vertices.push_back(uv0);
		t1.vertices.push_back(uv1);
		t1.vertices.push_back(mid_uv2);
		t1.vertices.push_back(v0);
		t1.vertices.push_back(v1);
		t1.vertices.push_back(b2);
		t1.norm.push_back(tri_norm0);
		t1.norm.push_back(tri_norm1);
		t1.norm.push_back(mid_norm2);
		cSubdivide::checkTri(t0, patch, param);
		cSubdivide::checkTri(t1, patch, param);
	} else if ((int0==0)&&(int1==1)&&(int2==1)) {
		triangle t0, t1, t2;
		t0.vertices.push_back(uv0);
		t0.vertices.push_back(mid_uv1);
		t0.vertices.push_back(mid_uv0);
		t0.vertices.push_back(v0);
		t0.vertices.push_back(b1);
		t0.vertices.push_back(b0);
		t0.norm.push_back(tri_norm0);
		t0.norm.push_back(mid_norm1);
		t0.norm.push_back(mid_norm0);
		t1.vertices.push_back(mid_uv0);
		t1.vertices.push_back(mid_uv1);
		t1.vertices.push_back(uv2);
		t1.vertices.push_back(b0);
		t1.vertices.push_back(b1);
		t1.vertices.push_back(v2);
		t1.norm.push_back(mid_norm0);
		t1.norm.push_back(mid_norm1);
		t1.norm.push_back(tri_norm2);
		t2.vertices.push_back(mid_uv1);
		t2.vertices.push_back(uv1);
		t2.vertices.push_back(uv2);
		t2.vertices.push_back(b1);
		t2.vertices.push_back(v1);
		t2.vertices.push_back(v2);
		t2.norm.push_back(mid_norm1);
		t2.norm.push_back(tri_norm1);
		t2.norm.push_back(tri_norm2);
		cSubdivide::checkTri(t0, patch, param);
		cSubdivide::checkTri(t1, patch, param);
		cSubdivide::checkTri(t2, patch, param);
	} else if ((int0==1)&&(int1==1)&&(int2==0)) {
		triangle t0, t1, t2;
		t0.vertices.push_back(uv0);
		t0.vertices.push_back(mid_uv2);
		t0.vertices.push_back(uv2);
		t0.vertices.push_back(v0);
		t0.vertices.push_back(b2);
		t0.vertices.push_back(v2);
		t0.norm.push_back(tri_norm0);
		t0.norm.push_back(mid_norm2);
		t0.norm.push_back(tri_norm2);
		t1.vertices.push_back(uv0);
		t1.vertices.push_back(mid_uv1);
		t1.vertices.push_back(mid_uv2);
		t1.vertices.push_back(v0);
		t1.vertices.push_back(b1);
		t1.vertices.push_back(b2);
		t1.norm.push_back(tri_norm0);
		t1.norm.push_back(mid_norm1);
		t1.norm.push_back(mid_norm2);
		t2.vertices.push_back(mid_uv1);
		t2.vertices.push_back(uv1);
		t2.vertices.push_back(mid_uv2);
		t2.vertices.push_back(b1);
		t2.vertices.push_back(v1);
		t2.vertices.push_back(b2);
		t2.norm.push_back(mid_norm1);
		t2.norm.push_back(tri_norm1);
		t2.norm.push_back(mid_norm2);
		cSubdivide::checkTri(t0, patch, param);
		cSubdivide::checkTri(t1, patch, param);
		cSubdivide::checkTri(t2, patch, param);	
	} else if ((int0==1)&&(int1==0)&&(int2==1)) {
		triangle t0, t1, t2;
		t0.vertices.push_back(mid_uv0);
		t0.vertices.push_back(mid_uv2);
		t0.vertices.push_back(uv2);
		t0.vertices.push_back(b0);
		t0.vertices.push_back(b2);
		t0.vertices.push_back(v2);
		t0.norm.push_back(mid_norm0);
		t0.norm.push_back(mid_norm2);
		t0.norm.push_back(tri_norm2);
		t1.vertices.push_back(uv0);
		t1.vertices.push_back(uv1);
		t1.vertices.push_back(mid_uv0);
		t1.vertices.push_back(v0);
		t1.vertices.push_back(v1);
		t1.vertices.push_back(b0);
		t1.norm.push_back(tri_norm0);
		t1.norm.push_back(tri_norm1);
		t1.norm.push_back(mid_norm0);
		t2.vertices.push_back(mid_uv0);
		t2.vertices.push_back(uv1);
		t2.vertices.push_back(mid_uv2);
		t2.vertices.push_back(b0);
		t2.vertices.push_back(v1);
		t2.vertices.push_back(b2);
		t2.norm.push_back(mid_norm0);
		t2.norm.push_back(tri_norm1);
		t2.norm.push_back(mid_norm2);
		cSubdivide::checkTri(t0, patch, param);
		cSubdivide::checkTri(t1, patch, param);
		cSubdivide::checkTri(t2, patch, param);	
	} else if ((int0==1)&&(int1==1)&&(int2==1)) {
		// cout << "here at worst case subdivide" << endl;
		triangle t0, t1, t2, t3;
		t0.vertices.push_back(mid_uv0);
		t0.vertices.push_back(mid_uv2);
		t0.vertices.push_back(uv2);
		t0.vertices.push_back(b0);
		t0.vertices.push_back(b2);
		t0.vertices.push_back(v2);
		t0.norm.push_back(mid_norm0);
		t0.norm.push_back(mid_norm2);
		t0.norm.push_back(tri_norm2);
		t1.vertices.push_back(uv0);
		t1.vertices.push_back(mid_uv1);
		t1.vertices.push_back(mid_uv0);
		t1.vertices.push_back(v0);
		t1.vertices.push_back(b1);
		t1.vertices.push_back(b0);
		t1.norm.push_back(tri_norm0);
		t1.norm.push_back(mid_norm1);
		t1.norm.push_back(mid_norm0);
		t2.vertices.push_back(mid_uv0);
		t2.vertices.push_back(mid_uv1);
		t2.vertices.push_back(mid_uv2);
		t2.vertices.push_back(b0);
		t2.vertices.push_back(b1);
		t2.vertices.push_back(b2);
		t2.norm.push_back(mid_norm0);
		t2.norm.push_back(mid_norm1);
		t2.norm.push_back(mid_norm2);
		t3.vertices.push_back(mid_uv1);
		t3.vertices.push_back(uv1);
		t3.vertices.push_back(mid_uv2);
		t3.vertices.push_back(b1);
		t3.vertices.push_back(v1);
		t3.vertices.push_back(b2);
		t3.norm.push_back(mid_norm1);
		t3.norm.push_back(tri_norm1);
		t3.norm.push_back(mid_norm2);
		cSubdivide::checkTri(t0, patch, param);
		cSubdivide::checkTri(t1, patch, param);
		cSubdivide::checkTri(t2, patch, param);
		cSubdivide::checkTri(t3, patch, param);	
	}
	

}













