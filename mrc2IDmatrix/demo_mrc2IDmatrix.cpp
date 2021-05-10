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


/*
 *	Example:
 *  1. Read a MRC file, store the data into the Volume class
 *	2. Compute the ED, GD and ID distance matrix
 */
int main(int argc, char* argv[])
{
	if ( argc < 5 )
	{
		printf("Usage: volume <input mrc> <sample number> <sample method: 1 random; 2 uniform> <output ID matrix><Octree Depth>\n") ;
		exit(0) ;
	}

	int depth=0;
	if(argc == 6) 
		sscanf( argv[5], "%d", &depth ) ;

	int sample_num, sample_method;

	sscanf( argv[2], "%d", &sample_num ) ;
	sscanf( argv[3], "%d", &sample_method ) ;

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
		delete reader;
	} 
	else if ( strstr( argv[1], ".sof" ) != NULL || strstr( argv[1], ".SOF" ) != NULL)
	{
		printf("Reading SOF file format.\n") ;
		modelType = 1 ;

		reader = new SOFReader( argv[1] ) ;
		vol = reader->getVolume() ;
//		vol->floodFill( 0 ) ;
		delete reader;
	}
	else
	{
		printf("Unrecognized file format! Only MRC files are accepted.\n") ;
		exit(0) ;
	}

	ProteinSignature pro(vol);
	if(depth != 0)
		pro.BuildOctree(depth);

//	int sampleNum = 100;
//	Matrix2D *ED = pro.ED_matrix(sampleNum);
//	ED->toFile("ED.txt");
	
	Matrix2D *ID = pro.ID_matrix(sample_num, sample_method);
	//ID->toFile("ID.txt");
	ID->toFile(argv[4]);

	// free memory
	//delete ED;
	delete ID;
	
	return 1;
}

