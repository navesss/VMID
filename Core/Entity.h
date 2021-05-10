#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "point.h"
#include "matrix.h"


/////////////////////////////////////////////////////
///////       Root class declaration         ////////
/////////////////////////////////////////////////////
class C_EntityBox
{
private:
	Point3D m_min, m_max;
public:
	C_EntityBox();
	C_EntityBox(Point3D min, Point3D max);
	void GetBox(Point3D &min, Point3D &max);
	C_EntityBox GetBox();
	void SetBox(Point3D min, Point3D max);
	void SetBox(C_EntityBox box);
	// 判相交
	BOOL operator & (const C_EntityBox& b) const;
	// 求并
	C_EntityBox operator + (const C_EntityBox& b) const;
	// 求交
	C_EntityBox operator * (const C_EntityBox& b) const;
	
//    void Draw(CDC* pDC, C_Canvas* pCanvas, DrawMode type);
//    void Serialize(CArchive& ar);
};

class C_EntityUcs
{
private:
	Point3D  m_origin;     // UCS原点
	Vector3D m_xaxis;      // WCS下的UCS的X轴方向
	Vector3D m_zaxis;      // WCS下的UCS的Z轴方向
	double   m_scale;	   // UCS的比例变换系数
public:
	C_EntityUcs();
	C_EntityUcs(const C_EntityUcs& ucs);
	C_EntityUcs(Point3D origin, Vector3D xaxis, Vector3D zaxis, double scale=1.0);

	Point3D  GetOrigin();
	Vector3D GetUcsX();
	Vector3D GetUcsY();
	Vector3D GetUcsZ();
	void     SetUcs(Point3D origin, Vector3D xaxis, Vector3D zaxis, double scale=1.0);
	void     SetUcs(double a, double b, double c, double d);
	Matrix4  GetWcToUc() const; // 得到将WCS变换至UCS的矩阵
	Matrix4  GetUcToWc() const; // 得到将UCS变换至WCS的矩阵
	Point3D  UcToWc(Point3D point);
	Point3D  WcToUc(Point3D point);

	double GetScale(); 
	void SetScale(double scale); 

	void Transform(Matrix4 matrix);

	//	void Serialize(CArchive& ar);

	C_EntityUcs operator * (const C_EntityUcs& ucs);
	void operator *= (const C_EntityUcs& ucs);
	BOOL operator == (const C_EntityUcs& ucs);
};


#endif
