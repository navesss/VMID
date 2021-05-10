//********************************************
// Utils3d.h
//********************************************
// pierre.alliez@cnet.francetelecom.fr
// Created : 29/01/97
// Modified : 19/02/97
//********************************************

#ifndef _UTILS_3D_
#define _UTILS_3D_

#include "Material.h"


CMaterial g_material[20];
CMaterial g_MaterialBack;
COLORREF g_InitColorGlBack = RGB(0,0,70);
COLORREF g_InitColorGlLightAmbient = RGB(255,255,255);

/////////////////////////////////////////////////////////////////////////////////
// by liuyushen 2003/08/17 添加的全局函数
// 初始化OpenGL的全局变量
void g_InitOpenGL()
{
	// center point
	//	g_material[0].SetAmbient( 1.0f, 1.0f, 1.0f, 1.0f);
	//	g_material[0].SetDiffuse( 1.0f, 1.0f, 1.0f, 1.0f);
	g_material[0].SetAmbient( 0.8f, 0.8f, 0.8f, 0.8f);
	g_material[0].SetDiffuse( 0.8f, 0.8f, 0.8f, 0.8f);
	//	g_material[0].SetAmbient( 0.2f, 0.2f, 0.2f, 0.2f);
	//	g_material[0].SetDiffuse( 0.2f, 0.2f, 0.2f, 0.2f);
	g_material[0].SetSpecular(0.5,0.5,0.5,1);
	g_material[0].SetShininess(80.0f);
	g_material[0].SetEmission(0.0f,0.0f,0.0f,1.0f);

	// x axis
	g_material[1].SetAmbient((float)255/255,(float)0/255,(float)0/255,1);
	g_material[1].SetDiffuse((float)255/255,(float)0/255,(float)0/255,1);
	g_material[1].SetSpecular(0,0,0,1);
	g_material[1].SetEmission(0,0,0,1);
	g_material[1].SetShininess(0);


	// y axis
	g_material[2].SetAmbient((float)0/255,(float)255/255,(float)0/255,1);
	g_material[2].SetDiffuse((float)0/255,(float)255/255,(float)0/255,1);
	g_material[2].SetSpecular(0,0,0,1);
	g_material[2].SetEmission(0,0,0,1);
	g_material[2].SetShininess(0);

	// z axis
	g_material[3].SetAmbient((float)0/255,(float)0/255,(float)255/255,1);
	g_material[3].SetDiffuse((float)0/255,(float)0/255,(float)255/255,1);
	g_material[3].SetSpecular(0,0,0,1);
	g_material[3].SetEmission(0,0,0,1);
	g_material[3].SetShininess(0);

	// Point set 1
	g_material[4].SetAmbient( (float)155/255,(float)155/255,(float)0/255, 1.0f);
	g_material[4].SetDiffuse( 1.0f, 1.0f, 0.0f, 1.0f);
	g_material[4].SetSpecular(0,0,0,1);
	g_material[4].SetEmission(0,0,0,1);
	g_material[4].SetShininess(0);

	// Point set 2
	g_material[5].SetAmbient((float)255/255,(float)0/255,(float)255/255,1);
	g_material[5].SetDiffuse((float)255/255,(float)0/255,(float)255/255,1);
	g_material[5].SetSpecular(0,0,0,1);
	g_material[5].SetEmission(0,0,0,1);
	g_material[5].SetShininess(80);

	// sub point set
	//	g_material[6].SetAmbient((float)155/255,(float)0/255,(float)155/255,1);
	//	g_material[6].SetDiffuse((float)155/255,(float)0/255,(float)155/255,1);
	g_material[6].SetAmbient((float)0/255,(float)180/255,(float)229/255,1);
	g_material[6].SetDiffuse((float)0/255,(float)180/255,(float)229/255,1);
	g_material[6].SetSpecular(0,0,0,1);
	g_material[6].SetEmission(0,0,0,1);
	g_material[6].SetShininess(0);

	// octree
	g_material[7].SetAmbient((float)150/255,(float)0/255,(float)0/255,0.5);
	g_material[7].SetDiffuse((float)150/255,(float)0/255,(float)0/255,0.5);
	g_material[7].SetSpecular(1.0, 0.0f, 0.0f, 0.5f);
	g_material[7].SetShininess(128.0f);
	g_material[7].SetEmission(0.0f,0.0f,0.0f,1.0f);

	// octree
	g_material[8].SetAmbient((float)50/255,(float)50/255,(float)50/255,0.5);
	g_material[8].SetDiffuse((float)50/255,(float)50/255,(float)50/255,0.5);
	g_material[8].SetSpecular(1.0, 0.0f, 0.0f, 0.5f);
	g_material[8].SetShininess(128.0f);
	g_material[8].SetEmission(0.0f,0.0f,0.0f,1.0f);

	// blending AdjFace left Materials
	g_material[9].SetAmbient((float)255/255,(float)255/255,(float)255/255,1);
	g_material[9].SetDiffuse((float)255/255,(float)255/255,(float)255/255,1);
	g_material[9].SetSpecular(0,0,0,1);
	g_material[9].SetEmission(0,0,0,1);
	g_material[9].SetShininess(128);

	// blending offset AdjFace left Materials
	g_MaterialBack.SetAmbient((float)70/255,(float)70/255,(float)70/255,1);
	g_MaterialBack.SetDiffuse((float)70/255,(float)70/255,(float)70/255,1);
	g_MaterialBack.SetSpecular(0,0,0,1);
	g_MaterialBack.SetEmission(0,0,0,1);
	g_MaterialBack.SetShininess(0);

	// 法向量材质(Vertex)
	g_material[10].SetAmbient((float)200/255,(float)0/255,(float)0/255,1);
	g_material[10].SetDiffuse((float)200/255,(float)0/255,(float)0/255,1);
	g_material[10].SetSpecular(0,0,0,1);
	g_material[10].SetEmission(0,0,0,1);
	g_material[10].SetShininess(0);
	// 法向量材质(Face)
	g_material[11].SetAmbient((float)0/255,(float)255/255,(float)0/255,1);
	g_material[11].SetDiffuse((float)0/255,(float)255/255,(float)0/255,1);
	g_material[11].SetSpecular(0,0,0,1);
	g_material[11].SetEmission(0,0,0,1);
	g_material[11].SetShininess(0);

	// Denoise: Fandisk 模型材质
	g_material[12].SetAmbient( 0.0f, 0.2f, 0.6f, 1.0f);
	g_material[12].SetDiffuse( 0.0f, 0.2f, 0.6f, 1.0f);
	g_material[12].SetSpecular(0.5,0.5,0.5,1);
	g_material[12].SetShininess(80.0f);
	g_material[12].SetEmission(0.0f,0.0f,0.0f,1.0f);

	// Denoise: cube 模型材质
	g_material[13].SetAmbient( (float)255/255,(float)165/255,(float)32/255, 1.0f);
	g_material[13].SetDiffuse( 0.5f, 0.5f, 0.0f, 1.0f);
	g_material[13].SetSpecular(0.5,0.5,0.5,1);
	g_material[13].SetShininess(80.0f);
	g_material[13].SetEmission(0.0f,0.0f,0.0f,1.0f);

	// Denoise: face_y 模型材质
	g_material[14].SetAmbient( (float)19/255,(float)162/255,(float)104/255, 1.0f);
	g_material[14].SetDiffuse( 0.5f, 0.5f, 0.0f, 1.0f);
	g_material[14].SetSpecular(0.5,0.5,0.5,1);
	g_material[14].SetShininess(80.0f);
	g_material[14].SetEmission(0.0f,0.0f,0.0f,1.0f);

	// Denoise: dinosaur_noise.lys 模型材质
	g_material[15].SetAmbient( (float)104/255,(float)19/255, (float)162/255, 1.0f);
	g_material[15].SetDiffuse( 0.0f, 0.5f, 0.5f, 1.0f);
	g_material[15].SetSpecular(0.5,0.5,0.5,1);
	g_material[15].SetShininess(80.0f);
	g_material[15].SetEmission(0.0f,0.0f,0.0f,1.0f);

	// new 模型材质 (10/10/2006)
	g_material[16].SetAmbient( (float)255/255,(float)165/255,(float)92/255, 1.0f);
	g_material[16].SetDiffuse( 0.5f, 0.5f, 0.0f, 1.0f);
	g_material[16].SetSpecular(0.5,0.5,0.5,1);
	g_material[16].SetShininess(80.0f);
	g_material[16].SetEmission(0.0f,0.0f,0.0f,1.0f);
}



#endif // _UTILS_3D_

