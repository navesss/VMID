#ifndef SIGNATURE_RENDER_H 
#define SIGNATURE_RENDER_H 

#include "Point.h"

class ProteinSignature;

enum OCT_DRAW_TYPE{OCT_FRAME, OCT_FACE, OCT_BOTH};

void glDrawFrontFace(Point3D* b, Point3D* e);
void glDrawFrontRect(Point3D* b, Point3D* e);
void glDrawBackFace(Point3D* b, Point3D* e);
void glDrawBackRect(Point3D* b, Point3D* e);
void glDrawEdgeFaces(Point3D* b, Point3D* e);
void glDrawEdgeRects(Point3D* b, Point3D* e);

void DisplayCell(ProteinSignature *scene,Point3D *pCenter);

void Vector3fToPoint3D(vector3f *v, Point3D *p);

void Normalize(Point3D *p,Point3D *pCenter);

#endif