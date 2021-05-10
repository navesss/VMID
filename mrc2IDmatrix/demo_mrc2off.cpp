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
 *	2. Compute the ED, GD and ID distance matrix
 */
int main(int argc, char* argv[])
{
	if ( argc != 2 )
	{
		printf("Usage: morph <input mrc> \n") ;
		exit(0) ;
	}

	char name[1024] ;
	strncpy( name, argv[1], strlen(argv[1]) - 4 ) ;
	name[ strlen(argv[1]) - 4 ] = '\0' ;
	printf("from density map %s\n", name) ;


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

	float tmax = vol->getMax();;
	float tmin = vol->getMin();
	float threshold = 0.5*(tmax+tmin);
	vol->threshold2( threshold, 0, 1 ) ;

	char newname[1024];
	sprintf( newname, "%s.off", name ) ;
	vol->toOFFCells( newname, threshold ) ;
	sprintf( newname, "%s2.off", name ) ;
	vol->toOFFCells2( newname) ;

	
	return 1;
}

