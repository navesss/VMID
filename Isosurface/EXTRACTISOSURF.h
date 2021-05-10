

#ifndef __EXTRACT_ISO_SURF_H_
#define __EXTRACT_ISO_SURF_H_

//#include "pifcorrect.h"
#include "DataArray.h"
#include "Useful.h"

//void ExtractIsoSurf(char *InputName, char *OutputName, int Treashhold,BOOL Format254);

//void ExtractIsoSurf2(PifFile *Input3DMap, char *OutputName, int Treashhold, BOOL Format254);
void ExtractIsoSurf3(char *InputName, char *OutputName, float Treashhold,
					/*tmp*/MH_BOOL Format254);


// input DataArray; output VRMLobject
VRMLobject* ExtractIsoSurf3(DataArray *MyDataArray, float Treashhold,
							/*tmp*/MH_BOOL Format254);

#endif //__EXTRACT_ISO_SURF_H_


