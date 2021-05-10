// my_volume.cpp : Thinning the volume
//

#include "volume.h"
#include "reader.h"
//#include <string.h>

/* Output parameters */
//#define WRITE_CELLS
//#define WRITE_DISTANCE_MAP
//#define WRITE_SEGMENTATION
//#define WRITE_ORIGINALS
//#define WRITE_PQR
//#define WRITE_DX


/*
 *	Example:
 *  1. Read a MRC file, store the data into the Volume class
 *	2. Write the Volume class into a MRC file
 *	3. Use the other software for display
 */
int main(int argc, char* argv[])
{
	if ( argc != 2 )
	{
		printf("Usage: volume <input mrc>\n") ;
		exit(0) ;
	}

	/****** Read volume ******/
	printf("Initialize volume...") ;
	int modelType ; // 0 for MRC, 1 for SOF
	VolumeReader* reader ;
	Volume * vol;

	if ( strstr( argv[1], ".mrc" ) != NULL || strstr( argv[1], ".MRC" ) != NULL )
	{
		printf("Reading MRC file format.\n") ;
		modelType = 0 ;

		reader = MRCReaderPicker::pick( argv[1] ) ;
		vol = reader->getVolume() ;
	} 
	else
	{
		printf("Unrecognized file format! Only MRC files are accepted.\n") ;
		exit(0) ;
	}

#ifdef WRITE_ORIGINALS
	vol->toMRCFile( "test2.mrc" ) ;
	exit( 0 ) ; 
#endif

	/*** Step 1: Get plates ***/

	float threshold = 0.02;
	
	/****** Thinning ******/
	int flag = 5;
	switch( flag )
	{
	case 5 : 
		vol->palagyiSurfaceSkeleton( threshold ) ;
		break ;
	case 6 : 
		vol->bertrandSurfaceSkeleton( threshold ) ;
		break ;
	case 7 : 
		vol->sequentialSkeleton( threshold, 1, 0 ) ;
		break ;
	}

	/****** Write out ******/
	vol->toMRCFile( "s_skeleton.mrc" ) ;

	delete vol;

	return 1;
}

