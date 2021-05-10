

//#include "stdafx.h"

#ifndef __CONFIGURATION_H_
#define __CONFIGURATION_H_

#include "Useful.h"
#include "Rotation.h"

//CLASS Configuration
//member varialbes:
typedef struct
{
	MH_BOOL C[8];
} Configuration;
Configuration Configuration_Configuration8(MH_BOOL AC0, MH_BOOL AC1, MH_BOOL AC2, MH_BOOL AC3,
	MH_BOOL AC4, MH_BOOL AC5, MH_BOOL AC6, MH_BOOL AC7);
Configuration Configuration_Configuration2(MH_BOOL AC[8]);
Configuration Configuration_Configuration1(int ByteConfig);
void Configuration_reset8(Configuration *This, MH_BOOL AC0, MH_BOOL AC1, MH_BOOL AC2, MH_BOOL AC3,
	MH_BOOL AC4, MH_BOOL AC5, MH_BOOL AC6, MH_BOOL AC7);
void Configuration_reset1(Configuration *This, int ByteConfig);
void Configuration_eq (Configuration *This, Configuration *AConfig);
int Configuration_IsSame(Configuration *This, Configuration *AConfig);
void Configuration_Apply(Configuration *This, Rotation *ARotation);
int Configuration_ToInt(Configuration *This);

#endif //__CONFIGURATION_H_
