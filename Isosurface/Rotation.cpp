

#include "rotation.h"
#include "useful.h"

Rotation Rotation_Rotation1(int Ar[8])
{
	int i;
	Rotation tmp;
	for (i=0 ; i<8 ; i++)
		tmp.r[i] = Ar[i];
	return tmp;
}

Rotation Rotation_Rotation8(	int Ar0, int Ar1, int Ar2, int Ar3,
	int Ar4, int Ar5, int Ar6, int Ar7)
{
	Rotation tmp;
	Rotation_reset(&tmp,Ar0,Ar1,Ar2,Ar3,Ar4,Ar5,Ar6,Ar7);
	return tmp;
}

void Rotation_reset(Rotation *This, int Ar0, int Ar1, int Ar2, int Ar3,
	int Ar4, int Ar5, int Ar6, int Ar7)
{
	This->r[0] = Ar0; This->r[1] = Ar1; This->r[2] = Ar2; This->r[3] = Ar3;
	This->r[4] = Ar4; This->r[5] = Ar5; This->r[6] = Ar6; This->r[7] = Ar7;
}
