// kmCluserting.cpp : implementation file
//

#include "ProteinSignature.h"
#include "KMlocal.h"			// k-means algorithms

KMterm  term(50, 0, 0, 0,              // run for 100 stages
			 0.10, 0.10, 3,             // other typical parameter values 
			 0.50, 10, 0.95);

KMdata* Point3D2kmPts(vector<Point3D*> pointArray)
{
	int  i, d, nPts, dim;

	nPts = pointArray.size();
	dim = 3;

	KMdata *dataPts = new KMdata(dim, nPts);	

	for (i = 0; i < nPts; i++) {
		{
			Point3D pt = *pointArray[i];

			for (d = 0; d < dim; d++) {

				KMpointArray pa = dataPts->getPts();
				pa[i][d] = (KMcoord) (pt[d]);
			}
		}
	}

	return dataPts;
}

void kml_sample_points(vector<Point3D*> pts, int sample_num, vector<Point3D> &samplePts)
{
	int		k	= sample_num;			// number of centers
	int		dim	= 3;			// dimension
	int		nPts	= sample_num;			// number of data points

	KMdata *dataPts;

	dataPts = Point3D2kmPts(pts);
	dataPts->buildKcTree();			// build filtering structure
	KMfilterCenters ctrs(k, *dataPts);		// allocate centers

	// run the algorithm
	KMlocalLloyds       kmAlg(ctrs, term);	// repeated Lloyd's
	// KMlocalSwap      kmAlg(ctrs, term);	// Swap heuristic
	// KMlocalEZ_Hybrid kmAlg(ctrs, term);	// EZ-Hybrid heuristic
	// KMlocalHybrid    kmAlg(ctrs, term);	// Hybrid heuristic
	ctrs = kmAlg.execute();			// execute

	
	int i;
	samplePts.clear();
	for(i=0; i<sample_num; i++)
	{
		float x, y, z;
		x = ctrs[i][0];
		y = ctrs[i][1];
		z = ctrs[i][2];
		Point3D cpt(x,y,z);

		samplePts.push_back(cpt);
	}

	delete dataPts;
}

void kml_sample_points2(vector<Point3D*> pts, int sample_num, vector<Point3D> &samplePts)
{
	int		k	= sample_num;			// number of centers
	int		dim	= 3;			// dimension
	int		nPts	= sample_num;			// number of data points

	KMdata *dataPts;

	dataPts = Point3D2kmPts(pts);
	dataPts->buildKcTree();			// build filtering structure
	KMfilterCenters ctrs(k, *dataPts);		// allocate centers

	// run the algorithm
	KMlocalLloyds       kmAlg(ctrs, term);	// repeated Lloyd's
	// KMlocalSwap      kmAlg(ctrs, term);	// Swap heuristic
	// KMlocalEZ_Hybrid kmAlg(ctrs, term);	// EZ-Hybrid heuristic
	// KMlocalHybrid    kmAlg(ctrs, term);	// Hybrid heuristic
	ctrs = kmAlg.execute();			// execute

	KMctrIdxArray closeCtr = new KMctrIdx[dataPts->getNPts()];
	double* sqDist = new double[dataPts->getNPts()];
	ctrs.getAssignments(closeCtr, sqDist);

	vector<vector<int>> cluster_closeCtr_array;
	vector<vector<float>> cluster_sqDist_array;
	
	cluster_closeCtr_array.resize(k);
	cluster_sqDist_array.resize(k);

	int i, j;

	for(i = 0; i < dataPts->getNPts(); i++) 
	{
		
		int cluster_Idx = closeCtr[i];
		cluster_closeCtr_array[cluster_Idx].push_back(i);
		cluster_sqDist_array[cluster_Idx].push_back(sqDist[i]);
	}

	samplePts.clear();
	for(i=0; i<k; i++)
	{
		vector<int> one_closeCtr = cluster_closeCtr_array[i];
		vector<float> one_sqDist = cluster_sqDist_array[i];

		float min_dist = one_sqDist[0];	
		int min_id = one_closeCtr[0];
		for(j=1; j<one_sqDist.size(); j++)
		{
			if(one_sqDist[j] < min_dist)
			{
				min_dist = one_sqDist[j];
				min_id = one_closeCtr[j];
			}
		}

		float x, y, z;

		KMpointArray pa = dataPts->getPts();
		x = pa[min_id][0];
		y = pa[min_id][1];
		z = pa[min_id][2];
		Point3D cpt(x,y,z);

		samplePts.push_back(cpt);

	}

	delete dataPts;
	delete [] closeCtr;
	delete [] sqDist;
}