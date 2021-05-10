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
	// ���ཻ
	BOOL operator & (const C_EntityBox& b) const;
	// ��
	C_EntityBox operator + (const C_EntityBox& b) const;
	// ��
	C_EntityBox operator * (const C_EntityBox& b) const;
	
//    void Draw(CDC* pDC, C_Canvas* pCanvas, DrawMode type);
//    void Serialize(CArchive& ar);
};

class C_EntityUcs
{
private:
	Point3D  m_origin;     // UCSԭ��
	Vector3D m_xaxis;      // WCS�µ�UCS��X�᷽��
	Vector3D m_zaxis;      // WCS�µ�UCS��Z�᷽��
	double   m_scale;	   // UCS�ı����任ϵ��
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
	Matrix4  GetWcToUc() const; // �õ���WCS�任��UCS�ľ���
	Matrix4  GetUcToWc() const; // �õ���UCS�任��WCS�ľ���
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
