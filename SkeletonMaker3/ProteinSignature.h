/**
 * Protein Distance Signature for 3D shape

 * Author : Yushen Liu, liu28@purdue.edu
 *          Yi Fang, fang4@purdue.edu
 * Date   : April 4, 2008
 */


#ifndef PROTEIN_SIGNATURE_H
#define PROTEIN_SIGNATURE_H

#pragma warning(disable: 4018 4244 4996 4305 4267)

#include <vector>
#include <algorithm>
#include "volume.h"
#include "point.h"
#include <map>
#include <string>
#include <iostream>
#include <assert.h>
#include "ocTree.h"
#include "Matrix2D.h"
#include "CprojectPts.h"

using namespace std;

#define CS_EPS 1e-8

#define DIS_EPS1 1.7321 // sqrt(3): 1.73205..., the largest distance between neighborhood
#define DIS_EPS2 0.9 
#define DIS_EPS3 1.42 // sqrt(2)
#define DIS_EPS_PARAMTER 1.74
#define DIS_EPS_PARAMTER2 1.414
#define DIS_SAMPLEPOINTS_NUM	2000


// grid 3D point coordinate (x, y, z) corresponds the grid position
//struct gridPoint3D
//{
//	int x, y, z;
//	long id;
//};

typedef float F32;
struct NewPoint
{
	F32 x,y,z;
};

static int fuck_cad = 0;

struct CS_Box{
	Point3D  m_min,m_max;
};

struct Box
{
	NewPoint min,max;
};

class ProteinSignature
{
public:
	Volume		*m_vol;		// input the protein volume model

	vector<Point3D*>	m_borderPts;	// border grid points
	vector<Point3D*>	m_inPts;		// inside grid points

	vector<Point3D>		m_sampleBorderPts; 

	//八叉树相关
	float XCordMin, YCordMin, ZCordMin;
	float XCordMax, YCordMax, ZCordMax;

	OctTree*	m_octtree;
    
	double m_CostByTree;  //建立八叉树所费时间
	double m_CostByCheckVisibility; //检查可见性花费时间

	float  m_accuracy;

public:
	ProteinSignature();
	ProteinSignature(Volume *vol);
	~ProteinSignature();

	void Init();

	// Compute Euclidean Distance(ED) matrix
	Matrix2D* ED_matrix(int sample_num, int sample_method=1);

	// Compute Geodesic Distance(GD) matrix
	Matrix2D* GD_matrix(int sample_num, int sample_method=1);

	// Compute Inner Distance(ID) matrix
	Matrix2D* ID_matrix(int sample_num, int sample_method=1);

	// Check the visibility of point pairs
	void checkVisibility(Matrix2D *D, vector<Point3D> &samplePts);

	// Check if the line segment if inside the volume
	bool checkInsideIntersect(Point3D p1, Point3D p2, vector<float> &breakpoints);

	// Check if the line segment if inside the volume
	bool checkInsideIntersect2(Point3D p1, Point3D p2, vector<float> &paramArray);

	// 我们的方法
	bool checkInsideIntersect3(Point3D p1, Point3D p2);

public:
	void CalBox(vector<Point3D*> borderPts);
	vector<int>	_isurfels;
	int m_refine_depth;
	vector<OCNode*> _boundarycell;


	void BuildOctree(int depth);
	void octree_construction(OCNode* n, int depth);

	vector<OCNode*>& GetBoundaryCell(void);
	Point3D* GetBorderPt(int index);
private:
	bool isInFNE(Point3D* ptr, float midx, float midy, float midz);
	bool isInFNW(Point3D* ptr, float midx, float midy, float midz);
	bool isInFSW(Point3D* ptr, float midx, float midy, float midz);
	bool isInFSE(Point3D* ptr, float midx, float midy, float midz);
	bool isInBNW(Point3D* ptr, float midx, float midy, float midz);
	bool isInBSW(Point3D* ptr, float midx, float midy, float midz);
	bool isInBSE(Point3D* ptr, float midx, float midy, float midz);
	bool isInBNE(Point3D* ptr, float midx, float midy, float midz);
	void OctreeSpeed(Point3D pa,Point3D pb,vector<CprojectPts> *paramArray);
	void Vector3fToPoint3D(vector3f& v, Point3D& p);
	bool intersect(const NewPoint &start, const NewPoint &end,const Box &box, F32 *time);
};

// compute Euclidean distance matrix
Matrix2D* L2_distance(vector<Point3D> &pts);

inline float PointProjecToLineParam(Point3D p, Point3D ps, Point3D pe)
{
	Vector3D vn = pe - ps;
	vn.SetUnit();

	Vector3D v0 = p - ps;
	float t = v0*vn;

	return t;
}

inline double PointLineSegDistance(Point3D p, Point3D ps, Point3D pe)
{
   double  dd;
   Vector3D vn = pe - ps;
   vn.SetUnit();                   // Get unit vector 
   dd = Length(vn^(p - ps));   // Obtain distance 
   return(dd);
}

//点与线段求交(魏开敏：2)  
inline int PointLineSegInters (Point3D p, Point3D ps, Point3D pe, float eps=CS_EPS) 
{
	if ( PointLineSegDistance(p, ps, pe)  <= eps ) 
		if ( ((p - ps) * (p - pe) <= eps ) || 
            (Distance(p, ps) < eps) || (Distance(p, pe) < eps) )  
			return 1;
		return 0;
}  

inline int ClusterInterPointsNum(vector<CprojectPts> &paramArray, float eps, vector<CprojectPts> &breakpoints)
{
	breakpoints.clear();

	std::sort(paramArray.begin(), paramArray.end());
	int i, n = paramArray.size();
	int nInter = 0;

	CprojectPts pt0,pt1;

	if(n > 0)
		nInter = 1;
	else
		return nInter;

	pt0=paramArray[0];
	
	for(i=1; i<n; i++)
	{
		pt1=paramArray[i];
		
		if(abs(pt1.m_f - pt0.m_f) > eps)
		{
			nInter ++;

			// breakpoints contains the breaking or seperated points ([t0 t1])
			// in this internal, no inter points
			breakpoints.push_back(pt0);
			breakpoints.push_back(pt1);
		}

		pt0 = pt1;
	}

	return nInter;
}

inline bool CheckInterInside(vector<float> &paramArray, float eps, vector<float> breakpoints, float len)
{
	bool isInside = false;
	
	assert(breakpoints.size() >= 2);

	float b1, b2; // two breakpoints
	b1 = breakpoints[0];
	b2 = breakpoints[1];

	float tmin, tmax;

	std::sort(paramArray.begin(), paramArray.end());

	int i, n = paramArray.size();
	int nInter = 1;

	if(n == 0) // If no inside point, it means that the line is outside of the volume
		return false;
	else if(n == 1) // if one inside point only is found, check this point with the breakpoints
	{
		tmin = paramArray[0];
	
		if(abs(tmin-b1)>eps || abs(tmin-b2)>eps)
			return false;
		else
			return true;
	}
	else if(n == 2)
	{
		tmin = paramArray[0];
		tmax = paramArray[paramArray.size()-1];
		if(abs(tmin-b1)>eps || abs(tmax-b2)>eps || abs(tmax-tmin)>eps)
			return false;
		else
			return true;
	}
	else
	{
		//tmin = paramArray[0];
		//tmax = paramArray[paramArray.size()-1];
		//if(abs(tmin-b1)>eps || abs(tmax-b2)>eps)
		//	return false;
		//else
		//	return true;
	}

	// select t0 in (0, len)
	float t0, t1;
	
	t0 = paramArray[0];
	int st_id = 0;
	for(i=1; i<n; i++)
	{
		if(t0 >= 0 && t0 <= len)
			break;
		else
		{
			t0 = paramArray[i];
			st_id = i;
		}
	}

	tmin = t0;

	// count the inter number
	for(i=st_id; i<n; i++)
	{
		t1 = paramArray[i];

		if(t1 > len)
			break;

		if(abs(t1 - t0) > eps)
			nInter ++;

		t0 = t1;
	}

	tmax = t1;


	if(nInter == 1) // one intersection points
	{
		b1 = breakpoints[0];
		b2 = breakpoints[1];

		if(abs(tmin-b1)>3*eps || abs(tmax-b2)>3*eps)
			isInside = false;
		else
			isInside = true;
	}
	else
		isInside = false;

	return isInside;
}

inline bool CheckInterInside2(vector<float> &paramArray, float eps, float len)
{
	bool isInside = false;

	std::sort(paramArray.begin(), paramArray.end());

	int i, n = paramArray.size();
	int nInter = 1;

	if(n < 2) // If no inside point, it means that the line is outside of the volume
		return false;

	nInter = 1;

	float t0 = paramArray[0];

	for(i=1; i<n; i++)
	{
		float t1 = paramArray[i];

		if(abs(t1 - t0) > eps)
		{
			nInter ++;
		}

		t0 = t1;
	}

	if(nInter > 1)
		isInside = false;
	else
		isInside = true;

	return isInside;
}

////////////////////////////////////////////////////////////////////////////
// Additional Functions of processing volume model
////////////////////////////////////////////////////////////////////////////

// Extract boundary and inside points
void VolumeCollectBorderInsidePoints(Volume *vol, int conn, 
									 vector<Point3D*> &borderPts, vector<Point3D*> &inPts);

// Sample points form point set
void VolumeSampleBorderPoints(vector<Point3D*> pts, int sample_num, 
							  vector<Point3D> &samplePts);

void VolumeSampleBorderPoints_fy(vector<Point3D*> pts, int sample_num, 
								 vector<Point3D> &samplePts,int flag);

void kmean_fy(vector<Point3D*> pts, int sample_num, 
		   vector<Point3D> &samplePts);

/*
* sample points using 'Efficient Algorithms for K-Means Clustering'
* http://www.cs.umd.edu/~mount/Projects/KMeans/
*/
// execution parameters (see KMterm.h and KMlocal.h)
void kml_sample_points(vector<Point3D*> pts, int sample_num, vector<Point3D> &samplePts);

// output the sample points most close to center points
void kml_sample_points2(vector<Point3D*> pts, int sample_num, vector<Point3D> &samplePts);


#endif