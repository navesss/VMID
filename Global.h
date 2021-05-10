// Global.h

//#include <Point2.h>

#ifndef _GLOBAL_3D_
#define _GLOBAL_3D_

typedef CArray<int, int>		CIntArray;
typedef CArray<BOOL, BOOL>		CBOOLArray;
typedef CArray<double, double>	CDoubleArray;

// 拾取
class CMaterial;

extern CMaterial g_material[20];
extern CMaterial g_MaterialBack;
// 初始化默认的光源颜色
extern COLORREF g_InitColorGlLightAmbient;
// 初始化默认的背景颜色
extern COLORREF g_InitColorGlBack;

// 显示蒙皮生成的Nurbs面
extern BOOL g_bShowNurbsSurf;
extern int GLPointSize;

const int  g_ProgressRange	=	100;

#undef TOLER
#undef TOLERANGLE

#define TOLER		1e-10
#define TOLERANGLE	1e-10

/* Constants rounded for 21 decimals. */
#undef PI
#undef PI2

#define PI          3.14159265358979323846
#define	PI2	        6.28318530717958647692
#define PI_2        1.57079632679489661923
#define PI_4        0.785398163397448309616
#define PI_8        0.392699081698724154808
#define PI_16       0.196349540849362077404
#define M_E         2.71828182845904523536
#define M_LOG2E     1.44269504088896340736
#define M_LOG10E    0.434294481903251827651
#define M_LN2       0.693147180559945309417
#define M_LN10      2.30258509299404568402
#define M_SQRT2     1.41421356237309504880
#define M_SQRT2_2   0.707106781186547524401

#define DEGREE		(PI / 180.0)

#define ItIs(aPointer, aClass)		aPointer->IsKindOf(RUNTIME_CLASS(aClass))
#define SafeItIs(aPointer, aClass)	(aPointer && aPointer->IsKindOf(RUNTIME_CLASS(aClass)))
#define iszero(tol,x)				(((x)>-(tol) && (x)<(tol)))

//***************************************************
// Type by liuyushen 2003/06/01
//***************************************************
// Draw type
#define FLAT 0
#define WIRE 1
#define POINT 2
#define CONF 3
#define SMOOTH 4
#define RAY 5
#define POINT_SPHERE 6

//---------------------------------------------------
#include <mmsystem.h>

#define WM_G_SHOWPROGRESS			WM_USER + 1
#define	WM_G_SETPROGRESSPOS			WM_USER + 2
#define WM_G_SETPROGRESSTEXT		WM_USER + 3
#define WM_G_SETSTATUSTEXTS			WM_USER + 4
#define WM_G_SETSTATUSTEXTU			WM_USER + 5

void SetStatusText(LPCSTR text, int pane = 0);
void SetStatusText(UINT textid, int pane = 0);
void ShowProgress(BOOL show, int total);
void SetProgressPos(int pos);
void SetProgressText(LPCSTR text);

// add by liuyushen 2005/04/19
extern int g_MAXDEPTH;
extern double g_timeCost;

extern DWORD iStartTime, iEndTime;

#define STARTTIME iStartTime = timeGetTime();
#define ENDTIME   iEndTime = timeGetTime();
#define SHOWTIME  TRACE("#Total time: %f\n",(double)(iEndTime-iStartTime)/CLOCKS_PER_SEC);
//---------------------------------------------------

#endif // 