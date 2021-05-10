

//#include "pifcorrect.h"

#include "stdio.h"
#include "extractisosurf.h"

#include "volume.h"
#include "reader.h"


VRMLobject* mrc2vrml(Volume *vol, float threshold)
{

	int dimx, dimy, dimz;

	dimx = vol->getSizeX();
	dimy = vol->getSizeY();
	dimz = vol->getSizeZ();

	// 1. convert Volume to DataArray
	DataArray *MyDataArray = new DataArray(dimx, dimy, dimz);

	MyDataArray->SetParams(threshold, NULL);

	for ( int i = 0 ; i < dimz ; i ++ )
	{
		for ( int j = 0 ; j < dimy ; j ++ )
		{
			for ( int k = 0 ; k < dimx ; k ++ )
			{
				float d = vol->getDataAt(k, j, i);

				MyDataArray->SetValue(k, j, i, d);
			}
		}
	}

	// 2. extract isosurface from DataArray
	VRMLobject *MyVRMLobject = ExtractIsoSurf3(MyDataArray, threshold, 1);

	delete MyDataArray;

	return MyVRMLobject;
}


int oldmain(int argc, char **argv)
{
	if (argc==1)
	{
		// open mrc file
		VolumeReader* reader ;
		Volume * vol;

		reader = MRCReaderPicker::pick( "cube.mrc" ) ;
		vol = reader->getVolume() ;

		float tmax = vol->getMax();;
		float tmin = vol->getMin();
		float threshold = 0.5*(tmax+tmin);
		vol->threshold2( threshold, 0, 1 ) ;

		// mrc --> vrml
		VRMLobject *MyVRMLobject = mrc2vrml(vol, threshold);

		// output to file
		VRMLobject_SaveToFileAsVRML1_2(MyVRMLobject, "cube.wrl");

		// free
		delete reader;
		delete vol;
		VRMLobject_relese(MyVRMLobject);

		return 1;
	}

	return 1;
}
