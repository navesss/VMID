

#ifndef __MRC2SURF_H_
#define __MRC2SURF_H_


//my includes:
#include "Vrmlobject.h"

class Volume;

VRMLobject* mrc2vrml(Volume *vol, float threshold);

#endif //__MRC2SURF_H_
