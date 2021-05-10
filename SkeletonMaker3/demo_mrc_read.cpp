// my_volume.cpp : Test the volume class
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
		printf("Usage: volume <input mrc> \n") ;
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
//	vol->toOFFCells( "../data/test.off" ) ;
	exit( 0 ) ; 
#endif

	/*** Step 1: Get plates ***/

	float threshold = 0.02;
	// First, threshold the input to 0/1
	Volume* bvol = new Volume( vol->getSizeX(), vol->getSizeY(), vol->getSizeZ(), 0, 0, 0, vol ) ;
	bvol->threshold2( threshold, 0, 1 ) ;
	
	bvol->toMRCFile( "test2.mrc" ) ;
	
	return 1;
}

