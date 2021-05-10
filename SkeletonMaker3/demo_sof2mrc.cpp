// my_volume.cpp : Test the volume class
//

#include "volume.h"
#include "reader.h"
#include "ProteinSignature.h"
//#include <string.h>

/* Output parameters */
//#define WRITE_CELLS
//#define WRITE_DISTANCE_MAP
//#define WRITE_SEGMENTATION
//#define WRITE_ORIGINALS
//#define WRITE_PQR
//#define WRITE_DX

// This function only for SOF -- > MRC file
// The original SOF file: value 0 or 1 denoting if it is inside or outside
void MRC_in_out(Volume *vol, int in, int out)
{

	int i, j, k, m, nx, ny, nz;
	
	nx = vol->getSizeX();
	ny = vol->getSizeY();
	nz = vol->getSizeZ();

	Point3D *gp;
	

	// collect the boundary points
	for ( k = 0 ; k < nx ; k ++ )
		for ( j = 0 ; j < ny ; j ++ )
			for ( i = 0 ; i < nz ; i ++ )
			{
				double value = vol->getDataAt( i, j, k );

				if ( value == 0 )
					vol->setDataAt( i, j, k, in ) ;
				else
					vol->setDataAt( i, j, k, out ) ;
			}

}

/*
 *	Example:
 *  1. Read a MRC file, store the data into the Volume class
 *	2. Compute the ED, GD and ID distance matrix
 */
int main(int argc, char* argv[])
{
	if ( argc != 3 )
	{
		printf("Usage: volume <input sof> <output mrc>\n") ;
		exit(0) ;
	}

	/****** Read volume ******/
	printf("Initialize volume...") ;
	int modelType ; // 0 for MRC, 1 for SOF
	VolumeReader* reader ;
	Volume * vol;

	if ( strstr( argv[1], ".sof" ) != NULL || strstr( argv[1], ".SOF" ) != NULL )
	{
		printf("Reading SOF file format.\n") ;
		modelType = 1 ;

		reader = new SOFReader( argv[1] ) ;
		vol = reader->getVolume() ;
		vol->floodFill( 0 ) ;


		printf("Writing MRC file format.\n") ;
		// set internal points as 1; out points as 0
		MRC_in_out(vol,1,0);
		vol->toMRCFile( argv[2] ) ;
	}
	
	return 1;
}

