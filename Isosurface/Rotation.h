


#ifndef __ROTATION_H_
#define __ROTATION_H_

//CLASS Rotation
//member vars:
typedef struct
{
	int r[8];
} Rotation;

//CLASS Rotation
//memer functions:
Rotation Rotation_Rotation1(int Ar[8]);
Rotation Rotation_Rotation8(	int Ar0, int Ar1, int Ar2, int Ar3,
	int Ar4, int Ar5, int Ar6, int Ar7);
void Rotation_reset(Rotation *This, int Ar0, int Ar1, int Ar2, int Ar3,
	int Ar4, int Ar5, int Ar6, int Ar7);
	
#endif //__ROTATION_H_
