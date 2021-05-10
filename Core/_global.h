// _global.h 

#pragma once

// Draw type
#define DRAW_VOLUME_BORDER_POINTS	100
#define DRAW_VOLUME_BORDER_POINTS2	101
#define DRAW_VOLUME_FULL_POINTS		102
#define DRAW_VOLUME_MRC				103
#define DRAW_VOLUME_CELL			104
#define DRAW_VOLUME_CELL2			105
#define DRAW_VOLUME_SURFACE			106

//***************************************************
// Type by liuyushen 2003/06/01
//***************************************************
// Draw type
#define DRAW_MESH_FLAT				20
#define DRAW_MESH_WIRE				21
#define DRAW_MESH_POINT				22
#define DRAW_MESH_CONF				23
#define DRAW_MESH_SMOOTH			24
#define DRAW_MESH_RAY				25

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

// Draw type
#define	RETRIEVE_METHOD_ID			1
#define	RETRIEVE_METHOD_ED			2
#define	RETRIEVE_METHOD_GD			3