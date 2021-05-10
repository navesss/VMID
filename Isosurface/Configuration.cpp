

#include "Configuration.h"

Configuration Configuration_Configuration8(MH_BOOL AC0, MH_BOOL AC1, MH_BOOL AC2, MH_BOOL AC3,
	MH_BOOL AC4, MH_BOOL AC5, MH_BOOL AC6, MH_BOOL AC7)
{
	Configuration tmp;
	Configuration_reset8(&tmp,AC0, AC1, AC2, AC3, AC4, AC5, AC6, AC7);
	return tmp;
}

Configuration Configuration_Configuration2(MH_BOOL AC[8])
{
	Configuration tmp;
	int i;
	for (i=0 ; i<8 ; i++)
		tmp.C[i] = AC[i];
	return tmp;
}

Configuration Configuration_Configuration1(int ByteConfig)
{
	Configuration tmp;
	Configuration_reset1(&tmp,ByteConfig);
	return tmp;
}

void Configuration_reset8(Configuration *This, MH_BOOL AC0, MH_BOOL AC1, MH_BOOL AC2, MH_BOOL AC3,
	MH_BOOL AC4, MH_BOOL AC5, MH_BOOL AC6, MH_BOOL AC7)
{
	This->C[0] = AC0; This->C[1] = AC1; This->C[2] = AC2; This->C[3] = AC3;
	This->C[4] = AC4; This->C[5] = AC5; This->C[6] = AC6; This->C[7] = AC7;
}

void Configuration_reset1(Configuration *This, int ByteConfig)
{
	int i;

	int mask [8];
	mask[0]=1;mask[1]=2;mask[2]=4;mask[3]=8;
	mask[4]=16;mask[5]=32;mask[6]=64;mask[7]=128;
	for (i=0; i<8 ; i++)
		This->C[i] = (ByteConfig & mask[i])!=0;
}

void Configuration_eq (Configuration *This, Configuration *AConfig)
{
	int i;
	for (i=0 ; i<8 ; i++)
		This->C[i] = AConfig->C[i];
}

int Configuration_IsSame(Configuration *This, Configuration *AConfig)
{
	return (
		This->C[0] == AConfig->C[0] && This->C[1] == AConfig->C[1] &&
		This->C[2] == AConfig->C[2] && This->C[3] == AConfig->C[3] &&
		This->C[4] == AConfig->C[4] && This->C[5] == AConfig->C[5] &&
		This->C[6] == AConfig->C[6] && This->C[7] == AConfig->C[7]
	);
}

void Configuration_Apply(Configuration *This, Rotation *ARotation)
{
	MH_BOOL tmp[8];
	int i;
	for (i=0 ; i<8 ; i++)
	{
		tmp[ARotation->r[i]] = This->C[i];
	}
	for (i=0 ; i<8 ; i++)
	{
		This->C[i] = tmp[i];
	}
}

int Configuration_ToInt(Configuration *This)
{
	int i;
	int tmp = 0;
	int BitValue = 1;
	for (i=0 ; i<8 ; i++)
	{
		if (This->C[i]) tmp+= BitValue;
		BitValue *= 2;
	}
	return tmp;
}
