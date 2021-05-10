#include "ProteinSignature.h"
#include "ProteinSignature.h"
#include "ProteinSignature.h"
#include "ProteinSignature.h"

#include "ProteinSignature.h"
#include "Matrix2D.h"
#include "ctime.h"



/////////////////////////////////////////////////////////////////
// class ProteinSignature
ProteinSignature::ProteinSignature()
{
	Init();
}

ProteinSignature::ProteinSignature(Volume *vol)
{
	Init();

	m_vol = vol;

	int conn = 26;
	printf("Collect boundary and inside points...\n");
	VolumeCollectBorderInsidePoints(vol, conn, m_borderPts, m_inPts);
	CalBox(m_borderPts);
}

ProteinSignature::~ProteinSignature()
{
	int i;

	for(i=0; i<m_borderPts.size(); i++)
		delete m_borderPts[i];
	m_borderPts.clear();

	for(i=0; i<m_inPts.size(); i++)
		delete m_inPts[i];
	m_inPts.clear();

	m_sampleBorderPts.clear();

	if(m_octtree)
		delete m_octtree;
}

void ProteinSignature::Init()
{
	int i;
	
	for(i=0; i<m_borderPts.size(); i++)
		delete m_borderPts[i];
	m_borderPts.clear();

	for(i=0; i<m_inPts.size(); i++)
		delete m_inPts[i];
	m_inPts.clear();

	m_sampleBorderPts.clear();
	m_octtree=NULL;
	m_CostByTree=0;
	m_CostByCheckVisibility=0;
}

// compute Euclidean Distance(ED) matrix
Matrix2D* ProteinSignature::ED_matrix(int sample_num, int sample_method)
{
	m_sampleBorderPts.clear(); 

	int sizex = m_vol->getSizeX();
	int sizey = m_vol->getSizeY();
	int sizez = m_vol->getSizeZ();

	float dmax = m_vol->getMax();
	float dmin = m_vol->getMin();
	float thr = (dmax + dmin)/2.0;

	// copy a new vol as bvol for processing
	Volume* bvol = new Volume(sizex, sizey, sizez, 0, 0, 0, m_vol);

	printf("Thresholding the volume to 0/1...\n");
	bvol->threshold2(thr, 0, 1) ;
	
	if(m_borderPts.size() == 0)
	{
		printf("Collect boundary and inside points...\n");
		int conn = 26;
		VolumeCollectBorderInsidePoints(bvol, conn, m_borderPts, m_inPts);
	}

	printf("Sample border points...\n");
	VolumeSampleBorderPoints_fy(m_borderPts, sample_num, m_sampleBorderPts, sample_method);

	printf("Compute the Euclidean Distance(ED) matrix...\n");
	Matrix2D *D = L2_distance(m_sampleBorderPts);

	delete bvol;

	return D;	
}

// compute Geodesic Distance(GD) matrix
// We only output the full Euclidean Distance(ED) matrix D, 
// and then import D into Matlab for computing the shortest distance
Matrix2D* ProteinSignature::GD_matrix(int sample_num, int sample_method)
{
	m_sampleBorderPts.clear(); 

	int sizex = m_vol->getSizeX();
	int sizey = m_vol->getSizeY();
	int sizez = m_vol->getSizeZ();

	float dmax = m_vol->getMax();
	float dmin = m_vol->getMin();
	float thr = (dmax + dmin)/2.0;

	// copy a new vol as bvol for processing
	Volume* bvol = new Volume(sizex, sizey, sizez, 0, 0, 0, m_vol);

	printf("Thresholding the volume to 0/1...\n");
	bvol->threshold2(thr, 0, 1) ;

	
	if(m_borderPts.size()==0)
	{
		printf("Collect boundary and inside points...\n");
		int conn = 26;
		VolumeCollectBorderInsidePoints(bvol, conn, m_borderPts, m_inPts);
	}

	printf("Sample border points...\n");
	VolumeSampleBorderPoints_fy(m_borderPts, sample_num, m_sampleBorderPts, sample_method);

	printf("Compute the Geodesic Distance(GD) matrix...\n");
	Matrix2D *D = L2_distance(m_sampleBorderPts);

	delete bvol;

	return D;
}

// compute Inner Distance(ID) matrix
Matrix2D* ProteinSignature::ID_matrix(int sample_num, int sample_method)
{
	m_sampleBorderPts.clear(); 

	int sizex = m_vol->getSizeX();
	int sizey = m_vol->getSizeY();
	int sizez = m_vol->getSizeZ();

	float dmax = m_vol->getMax();
	float dmin = m_vol->getMin();
	float thr = (dmax + dmin)/2.0;


	// copy a new vol as bvol for processing
	Volume* bvol = new Volume(sizex, sizey, sizez, 0, 0, 0, m_vol);

	printf("Thresholding the volume to 0/1...\n");
	bvol->threshold2(thr, 0, 1) ;

	// avoid the collect border points again
	if(m_borderPts.size() == 0)
	{
		printf("Collect boundary and inside points...\n");
		int conn = 26;
		VolumeCollectBorderInsidePoints(bvol, conn, m_borderPts, m_inPts);
	}

	if(sample_method == 1)
		printf("Random Sample border points...\n");
	if(sample_method == 2)
		printf("Uniform Sample border points...\n");

	VolumeSampleBorderPoints_fy(m_borderPts, sample_num, m_sampleBorderPts,sample_method);

	printf("Compute the Inner Distance(ID) matrix...\n");
	Matrix2D *D = NULL;
	D = L2_distance(m_sampleBorderPts);

	printf("Check the visibility of point pairs...\n");
	

	CTimer t;
	t.reset(); 
	checkVisibility(D, m_sampleBorderPts);
	m_CostByCheckVisibility = t.get_elapsed();


	delete bvol;

	return D;
}


// Check the visibility of point pairs
void ProteinSignature::checkVisibility(Matrix2D *D, vector<Point3D> &samplePts)
{
	int i, j, k, N, nPts, nInterPts, flag;
	float dis, t;
	bool bVisible = true;

	N = m_borderPts.size(); // all border points

	nPts = samplePts.size(); // sample border points nPts <= N
	float **mat = D->m_mat;
	vector<CprojectPts> paramArray;

	for(i=0; i<nPts; i++)
	{
		for(j=i+1; j<nPts; j++)
		{
			bVisible = true;

			dis = mat[i][j];

			if(dis < DIS_EPS1)
				continue;

			// select one pair of points from sampling border points
			Point3D p1 = samplePts[i];
			Point3D p2 = samplePts[j];

			paramArray.clear();
			nInterPts = 0;
			
			// Check if the pair of points is visible under all border points
			if(m_octtree==NULL)
			{
				for(k=0; k<N; k++)
				{
					Point3D pt = *m_borderPts[k];

					float d1 = Distance(pt, p1);
					float d2 = Distance(pt, p2);
				
				// check the points inside the cylinder
					flag = PointLineSegInters(pt, p1, p2, m_accuracy);
					if(flag)
					{
						t = PointProjecToLineParam(pt, p1, p2);

						CprojectPts ppt;
						ppt.SetValue(t,pt);

						paramArray.push_back(ppt);
						continue;
					}
				}
			}
			else
				OctreeSpeed(p1,p2,&paramArray);
			

			vector<CprojectPts> breakpoints;
			// count the number of intersection points
			int nParam = ClusterInterPointsNum(paramArray, m_accuracy, breakpoints);

			if(nParam > 2) // multiple intersection points
			{
				bVisible = false;
			}
			else if(nParam == 2)
			{
				//bVisible = checkInsideIntersect(p1, p2, breakpoints);
				//bVisible = checkInsideIntersect2(p1, p2, pArray);
				bVisible = checkInsideIntersect3(breakpoints[0].m_pt, breakpoints[1].m_pt);
			}
			else if(nParam == 1) // one intersection point (tangency)
			{
				bVisible = true;
				//bVisible = false;
			}

			if(!bVisible)
				mat[i][j] = mat[j][i] = 0;

			//if(i==15 && mat[i][j]!=0)
			//{
			//	mat[i][j] = mat[j][i] = 2;
			//}
		}
	}
}

// Check if the line segment if inside the volume
bool ProteinSignature::checkInsideIntersect(Point3D p1, Point3D p2, vector<float> &breakpoints)
{
	int i, np;

	vector<Point3D*>	ptArray = m_inPts;
	np = ptArray.size();
	
	vector<float> paramArray;

	for(i=0; i<np; i++)
	{
		Point3D pt = *ptArray[i];
				
		int flag = PointLineSegInters(pt, p1, p2, DIS_EPS3);

		// the line segment is inside the volume if there is at least one point inside the cylinder
		if(flag)
		{
			float t = PointProjecToLineParam(pt, p1, p2);
			paramArray.push_back(t);
		}
	}

	float dis = Distance(p1, p2);
	
	// Add two endpoints due to they are not the inside points
	//paramArray.push_back(0);
	//paramArray.push_back(dis);
	
	//vector<float> breakpoints;
	//int nParam = ClusterInterPointsNum(paramArray, DIS_EPS_PARAMTER, breakpoints);
	bool isInside = CheckInterInside(paramArray, DIS_EPS_PARAMTER2, breakpoints, dis);
	
	return isInside;
}

// Check if the line segment if inside the volume
// paramArray: include the parameters of border points
bool ProteinSignature::checkInsideIntersect2(Point3D p1, Point3D p2, vector<float> &paramArray)
{
	int i;
	vector<Point3D*>	inPtsArray = m_inPts;

	for(i=0; i<inPtsArray.size(); i++)
	{
		Point3D pt = *inPtsArray[i];

		int flag = PointLineSegInters(pt, p1, p2, DIS_EPS3);

		// the line segment is inside the volume if there is at least one point inside the cylinder
		if(flag)
		{
			float t = PointProjecToLineParam(pt, p1, p2);
			paramArray.push_back(t);
		}
	}

	float dis = Distance(p1, p2);

	// Add two endpoints due to they are not the inside points
	//paramArray.push_back(0);
	//paramArray.push_back(dis);

	//vector<float> breakpoints;
	//int nParam = ClusterInterPointsNum(paramArray, DIS_EPS_PARAMTER, breakpoints);
	bool isInside = CheckInterInside2(paramArray, DIS_EPS_PARAMTER2, dis);

	return isInside;
}


/************************************************************************/

bool ProteinSignature::checkInsideIntersect3(Point3D p1, Point3D p2)
{
	Point3D midPoint((p1.x + p2.x)/2,(p1.y + p2.y)/2,(p1.z + p2.z)/2);
	Point3D pFpoint1((p1.x + midPoint.x)/2,(p1.y + midPoint.y)/2,(p1.z + midPoint.z)/2);
	Point3D pFpoint2((midPoint.x + p2.x)/2,(midPoint.y + p2.y)/2,(midPoint.z + p2.z)/2);

	bool b1,b2,b3;

	if(m_vol->getDataAt((int)midPoint.x, (int)midPoint.y, (int)midPoint.z)== 1)
		b1=true;
	else
		b1=false;

	if(m_vol->getDataAt((int)pFpoint1.x, (int)pFpoint1.y, (int)pFpoint1.z)== 1)
		b2=true;
	else
		b2=false;

	if(m_vol->getDataAt((int)pFpoint2.x, (int)pFpoint2.y, (int)pFpoint2.z)== 1)
		b3=true;
	else
		b3=false;


	//for (int i = 0; i < m_inPts.size(); ++i)
	//{
	//	if ((int)midPoint.x == (int)(m_inPts[i]->x) )
	//	{
	//		if ((int)midPoint.y == (int)(m_inPts[i]->y))
	//		{
	//			if ((int)midPoint.z == (int)(m_inPts[i]->z))
	//				return true;
	//		}
	//	}
	//}

	return b1 && b2 && b3;
}


/////////////////////////////////////////////////////////////////////////
// compute the full Euclidean distance matrix of volume model
Matrix2D* L2_distance(vector<Point3D> &pts)
{
	int i, j, n = pts.size();

	Matrix2D *D = new Matrix2D(n);

	for(i=0; i<n; i++)
		for(j=i+1; j<n; j++)
		{
			Point3D p1 = pts[i];
			Point3D p2 = pts[j];

			float dis = Distance(p1, p2);

			D->m_mat[i][j] = D->m_mat[j][i] = dis;
		}

	return D;
}

////////////////////////////////////////////////////////////////////////////
// Additional Functions of processing volume model
////////////////////////////////////////////////////////////////////////////

/*
input: conn may have the following scalar values
 6     three-dimensional six-connected neighborhood
 18    three-dimensional 18-connected neighborhood
 26    three-dimensional 26-connected neighborhood

  Note: the current is only implemented for conn==6; The other case can be built using a template N18 and N26
*/
void VolumeCollectBorderInsidePoints(Volume *vol, int conn, 
									 vector<Point3D*> &borderPts, vector<Point3D*> &inPts)
{
	borderPts.clear();
	inPts.clear();
	
	int i, j, k, m, sizex, sizey, sizez;
	
	sizex = vol->getSizeX();
	sizey = vol->getSizeY();
	sizez = vol->getSizeZ();

	Volume* indvol = new Volume( sizex, sizey, sizez, 0, 0, 0, vol ) ;

	Point3D *gp;
	

	// collect the boundary points
	for ( k = 0 ; k < sizex ; k ++ )
	{
		for ( j = 0 ; j < sizey ; j ++ )
		{
			for ( i = 0 ; i < sizez ; i ++ )
			{
				if ( indvol->getDataAt( i, j, k ) == 1 )
				{
					switch (conn)
					{
					case 6:
						// seach the 6 neighbor. It will be a border point if there a 0 point
						for ( m = 0 ; m < 6 ; m ++ )
						{
							if ( indvol->getDataAt( i + neighbor6[m][0], j + neighbor6[m][1], k + neighbor6[m][2] ) == 0 )
							{
								// set the border as -1 for the search the inside points
								indvol->setDataAt( i, j, k, -1 ) ;

								gp = new Point3D(i, j, k);

								borderPts.push_back(gp);
								break;
							}
						}
						break;
					case 26:
						// seach the 6 neighbor. It will be a border point if there a 0 point
						for ( m = 0 ; m < 26 ; m ++ )
						{
							if ( indvol->getDataAt( i + neighbor26[m][0], j + neighbor26[m][1], k + neighbor26[m][2] ) == 0 )
							{
								// set the border as -1 for the search the inside points
								indvol->setDataAt( i, j, k, -1 ) ;

								gp = new Point3D(i, j, k);

								borderPts.push_back(gp);
								break;
							}
						}
						break;
					}

				}
			}
		}
	}


	// collect the inside points (except the boundary points)
	for ( k = 0 ; k < sizex ; k ++ )
	{
		for ( j = 0 ; j < sizey ; j ++ )
		{
			for ( i = 0 ; i < sizez ; i ++ )
			{
				if ( indvol->getDataAt( i, j, k ) == 1 )
				{
					gp = new Point3D(i, j, k);
					
					inPts.push_back(gp);
				}
			}
		}
	}

	delete indvol;

}

// sample points form point set
void VolumeSampleBorderPoints(vector<Point3D*> pts, int sample_num, vector<Point3D*> &samplePts)
{
	samplePts.clear();

	int i, np;
	
	np = pts.size();
	
	if(sample_num > DIS_SAMPLEPOINTS_NUM)
		sample_num = DIS_SAMPLEPOINTS_NUM;
	
	if(sample_num > np)
		sample_num = np;

	if(sample_num == np)
	{
		samplePts = pts;
	}
	else
	{
		srand(0);

		for(i=0; i<sample_num; i++)
		{
			int ind = rand()%np;
			Point3D *gp = pts[ind];

			samplePts.push_back(gp);
		}
	}
}


/************************************************************************/
/*  K-Mean Sampling the points Sample_num               */
/************************************************************************/
void kmean_fy(vector<Point3D*> pts, int sample_num, vector<Point3D> &samplePts)
{
	
	const int maxstep=30;
	int i, j;
	bool haltflag=0;
	int steps=0;
	vector<Point3D> classsum(samplePts.size());
	vector<int> recordpointno(samplePts.size());

	while(!haltflag&&steps<maxstep)
	{
		
		for (i=0;i<pts.size();i++)
		{
			double dis;
			int belongtoclass=0;

			double tmpdis=sqrt( (pts[i]->x-samplePts[0].x)*(pts[i]->x-samplePts[0].x) + (pts[i]->y-samplePts[0].y)*(pts[i]->y-samplePts[0].y)+ (pts[i]->z-samplePts[0].z)*(pts[i]->z-samplePts[0].z)); 
			
			for (j=1;j<samplePts.size();j++)
			{
				dis=sqrt( (pts[i]->x-samplePts[j].x)*(pts[i]->x-samplePts[j].x) + (pts[i]->y-samplePts[j].y)*(pts[i]->y-samplePts[j].y)+ (pts[i]->z-samplePts[j].z)*(pts[i]->z-samplePts[j].z)); 
				if (dis<tmpdis)
				{
					belongtoclass=j;		
					tmpdis=dis;
				}
			}
			classsum[belongtoclass].x+=pts[i]->x;
			classsum[belongtoclass].y+=pts[i]->y;
			classsum[belongtoclass].z+=pts[i]->z;
			recordpointno[belongtoclass]=recordpointno[belongtoclass]+1;
		}

		for (j=0;j<samplePts.size();j++)
		{
			classsum[j].x=classsum[j].x/recordpointno[j];
			classsum[j].y=classsum[j].y/recordpointno[j];	
			classsum[j].z=classsum[j].z/recordpointno[j];
		}
   

		double sum=0;
		double dis;
		for (j=0;j<samplePts.size();j++)
		{
			dis=sqrt( (classsum[j].x-samplePts[j].x)*(classsum[j].x-samplePts[j].x) + (classsum[j].y-samplePts[j].y)*(classsum[j].y-samplePts[j].y)+ (classsum[j].z-samplePts[j].z)*(classsum[j].z-samplePts[j].z)) ;
			sum=sum+dis;
			samplePts[j].x=classsum[j].x;
			samplePts[j].y=classsum[j].y;
			samplePts[j].z=classsum[j].z;
				//if (samplePts[j]->x<1)
		}
		//cout<<"x:"<<samplePts[2]->x<<" y:"<<samplePts[2]->y<<" z:"<<samplePts[2]->z<<" steps:"<<steps<<"sum"<<sum<<endl;

		//cout<<"x:"<<samplePts[10]->x<<" y:"<<samplePts[10]->y<<" z:"<<samplePts[10]->z<<" steps:"<<steps<<"sum"<<sum<<endl;

		if (sum<0.01) haltflag=1;

		steps++;
		
		for (j=0;j<samplePts.size();j++)
		{
			classsum[j].x=0;
			classsum[j].y=0;
			classsum[j].z=0;
			recordpointno[j]=0;
		}

	
	}


	
}

// sample points maybe don't belong the original volume
void VolumeSampleBorderPoints_fy(vector<Point3D*> pts, int sample_num, 
								 vector<Point3D> &samplePts,int sample_method)
{
	int i, np;

	samplePts.clear();

	np = pts.size();

	if(sample_num > DIS_SAMPLEPOINTS_NUM)
		sample_num = DIS_SAMPLEPOINTS_NUM;

	if(sample_num > np)
		sample_num = np;

	if(sample_num == np)
	{
		for(i=0; i<pts.size(); i++)
			samplePts.push_back(*pts[i]);
	}
	else
	{
		if(sample_method==1) // random sampling
		{
		srand(0);
		for(i=0; i<sample_num; i++)
		  { 
			int ind = rand()%np;
			Point3D *gp = pts[ind];

			samplePts.push_back(*gp);
		  }
		}
		if (sample_method==2) // uniform sampling
		{
			srand(0);
			for(i=0; i<sample_num; i++)
			{ 
				int ind = rand()%np;
				Point3D *gp = pts[ind];
				samplePts.push_back(*gp);
			}
			//kmean_fy(pts,sample_num,samplePts);

			// using the new K-Means Clusering for unifrom sampling
			//kml_sample_points(pts, sample_num, samplePts);
			kml_sample_points2(pts, sample_num, samplePts);

		}



	}


}




void ProteinSignature::BuildOctree(int depth)
{
	CTimer t;
	t.reset();
	if(m_octtree)
	delete m_octtree;
	m_refine_depth=depth;
	_boundarycell.clear();

	vector3f* p1 = new vector3f(XCordMin, YCordMin, ZCordMin);
	vector3f* p2 = new vector3f(XCordMax, YCordMax, ZCordMax);

	m_octtree = new OctTree();
	m_octtree->SetCubeArea(p1,p2);
	m_octtree->GetRoot()->SetSurfelsArray(_isurfels);

	octree_construction(m_octtree->GetRoot(),0);

	m_CostByTree = t.get_elapsed();
}

void ProteinSignature::octree_construction(OCNode* n, int depth)
{
	n->SetDepth(depth);
	n->SetType2(BOUNDARY_CELL);	//non-empty cell

	//is an empty cell
	vector<int>isurfels = n->GetSurfelsArray();
	if (isurfels.size()<=0)
	{
		n->SetType2(UNKOWN_CELL);	//default empty cell type
		fuck_cad++;
		return;
	}

	//exceed the max depth
	if (depth >= m_refine_depth)
	{
		//add the boundarycell to array
		_boundarycell.push_back(n);
		fuck_cad++;
		/*
		for(int i=0; i<n->GetSurfelsArray().size(); i++)
		{
		_surfels[n->GetSurfelsArray()[i]].SetParent(n);
		}

		*/
		return;
	}
	else
	{					//creating eight quads to the root node
		float midx;
		float midy;
		float midz;
		midx = (n->GetEnd()->x + n->GetBegin()->x)/2;
		midy = (n->GetEnd()->y + n->GetBegin()->y)/2; 
		midz = (n->GetEnd()->z + n->GetBegin()->z)/2;
		OCNode* childFNW = new OCNode(F_NW,n);
		childFNW->SetBegin(new vector3f(n->GetBegin()->x , midy, n->GetBegin()->z));
		childFNW->SetEnd(new vector3f(midx , n->GetEnd()->y, midz));

		OCNode* childFNE = new OCNode(F_NE,n);
		childFNE->SetBegin(new vector3f(midx , midy,n->GetBegin()->z));
		childFNE->SetEnd(new vector3f(n->GetEnd()->x, n->GetEnd()->y, midz));

		OCNode* childFSW = new OCNode(F_SW,n);
		childFSW->SetBegin(new vector3f(n->GetBegin()->x , n->GetBegin()->y, n->GetBegin()->z));
		childFSW->SetEnd(new vector3f(midx , midy, midz));

		OCNode* childFSE = new OCNode(F_SE,n);
		childFSE->SetBegin(new vector3f(midx, n->GetBegin()->y, n->GetBegin()->z));
		childFSE->SetEnd(new vector3f(n->GetEnd()->x , midy, midz));

		OCNode* childBNW = new OCNode(B_NW,n);
		childBNW->SetBegin(new vector3f(n->GetBegin()->x , midy,midz));
		childBNW->SetEnd(new vector3f(midx , n->GetEnd()->y,n->GetEnd()->z));

		OCNode* childBNE = new OCNode(B_NE,n);
		childBNE->SetBegin(new vector3f(midx , midy, midz));
		childBNE->SetEnd(new vector3f(n->GetEnd()->x, n->GetEnd()->y,n->GetEnd()->z));

		OCNode* childBSW = new OCNode(B_SW,n);
		childBSW->SetBegin(new vector3f(n->GetBegin()->x , n->GetBegin()->y, midz));
		childBSW->SetEnd(new vector3f(midx , midy, n->GetEnd()->z));

		OCNode* childBSE = new OCNode(B_SE,n);
		childBSE->SetBegin(new vector3f(midx, n->GetBegin()->y,midz ));
		childBSE->SetEnd(new vector3f(n->GetEnd()->x , midy, n->GetEnd()->z));

		for (int i = 0; i < isurfels.size(); i++)
		{
			int index = isurfels[i];
			Point3D* curPoint = m_borderPts[index];
			if(isInFNE(curPoint, midx, midy, midz))
			{
				childFNE->AddSurfelsIndex(index);
			}
			else if(isInFNW(curPoint, midx, midy, midz)){
				childFNW->AddSurfelsIndex(index);
			}
			else if(isInFSW(curPoint, midx, midy, midz)){
				childFSW->AddSurfelsIndex(index);
			}
			else if(isInFSE(curPoint, midx, midy, midz)){
				childFSE->AddSurfelsIndex(index);
			}
			else if(isInBNE(curPoint, midx, midy, midz)){
				childBNE->AddSurfelsIndex(index);
			}
			else if(isInBNW(curPoint, midx, midy, midz)){
				childBNW->AddSurfelsIndex(index);
			}
			else if(isInBSW(curPoint, midx, midy, midz)){
				childBSW->AddSurfelsIndex(index);
			}
			else if(isInBSE(curPoint, midx, midy, midz)){
				childBSE->AddSurfelsIndex(index);
			}
		}  

		// Calling function recursively on all the eight quads

		octree_construction(childFNW, depth+1);

		octree_construction(childFNE, depth+1);

		octree_construction(childFSW, depth+1);

		octree_construction(childFSE, depth+1);


		octree_construction(childBNW, depth+1);

		octree_construction(childBNE, depth+1);

		octree_construction(childBSW, depth+1);

		octree_construction(childBSE, depth+1);		

	}

}

void ProteinSignature::CalBox(vector<Point3D*> borderPts)
{
	XCordMax=XCordMin=borderPts[0]->x;
	YCordMax=YCordMin=borderPts[0]->y;
	ZCordMax=ZCordMin=borderPts[0]->z;
	_isurfels.clear();
	_isurfels.push_back(0);
	for(int j=1;j<borderPts.size();j++)
	{
		if	(borderPts[j]->x < XCordMin)
			XCordMin = borderPts[j]->x;
		if	(borderPts[j]->x > XCordMax)
			XCordMax = borderPts[j]->x;
		if	(borderPts[j]->y < YCordMin)
			YCordMin = borderPts[j]->y;
		if	(borderPts[j]->y > YCordMax)
			YCordMax = borderPts[j]->y;
		if	(borderPts[j]->z < ZCordMin)
			ZCordMin = borderPts[j]->z;
		if	(borderPts[j]->z > ZCordMax)
			ZCordMax = borderPts[j]->z;
		_isurfels.push_back(j);
	}

	double dmax = XCordMax - XCordMin;
	if (dmax < YCordMax - YCordMin)
		dmax = YCordMax - YCordMin;
	if (dmax < ZCordMax - ZCordMin)
		dmax = ZCordMax - ZCordMin;

	dmax /= 2.0;
	double center = (XCordMin + XCordMax ) / 2;
	XCordMin = center - dmax; XCordMax = center + dmax;
	center = (YCordMin + YCordMax) / 2;
	YCordMin = center - dmax; YCordMax = center + dmax;
	center = (ZCordMin + ZCordMax) / 2;
	ZCordMin = center - dmax; ZCordMax = center + dmax;
}

bool ProteinSignature::isInFNE(Point3D* ptr, float midx, float midy, float midz)
{

	if((ptr->x > midx) && (ptr->y >midy) && (ptr->z < midz))
		return true;
	else 
		return false;
}

bool ProteinSignature::isInFNW(Point3D* ptr, float midx, float midy, float midz)
{
	if((ptr->x <= midx) && (ptr->y >midy) && (ptr->z < midz))	return true;
	else return false;
}

bool ProteinSignature::isInFSW(Point3D* ptr, float midx, float midy, float midz)
{
	if((ptr->x <= midx) && (ptr->y <= midy) && (ptr->z < midz)) return true;
	else return false;
}


bool ProteinSignature::isInFSE(Point3D* ptr, float midx, float midy, float midz)
{
	if((ptr->x > midx) && (ptr->y <= midy) && (ptr->z < midz))	return true;
	else return false;
}

bool ProteinSignature::isInBNW(Point3D* ptr, float midx, float midy, float midz)
{
	if((ptr->x <= midx) && (ptr->y >midy) && (ptr->z >= midz))	return true;
	else return false;
}

bool ProteinSignature::isInBSW(Point3D* ptr, float midx, float midy, float midz)
{
	if((ptr->x <= midx) && (ptr->y <= midy) && (ptr->z >= midz)) return true;
	else return false;
}

bool ProteinSignature::isInBSE(Point3D* ptr, float midx, float midy, float midz)
{
	if((ptr->x > midx) && (ptr->y <= midy) && (ptr->z >= midz))	return true;
	else return false;
}

bool ProteinSignature::isInBNE(Point3D* ptr, float midx, float midy, float midz)
{
	if((ptr->x > midx) && (ptr->y >midy) && (ptr->z >= midz))return true;
	else return false;
}

vector<OCNode*>& ProteinSignature::GetBoundaryCell(void)
{
	return _boundarycell;
}

Point3D* ProteinSignature::GetBorderPt(int index)
{
	return m_borderPts[index];
}
void ProteinSignature::OctreeSpeed(Point3D pa,Point3D pb,vector<CprojectPts> *paramArray)
{
	int				i,j,k,id,flag;
	float			t, dist, dir, dir1;
	vector<OCNode*>	*emptycell;
	vector3f		*b, *e, _pt, _vn, _vn1, _vn2, _ori, _pa;
	vector<int>		iniInterPtArray;
	Point3D			pt, min_pt, max_pt, ori;
	CS_Box			box;

	emptycell = &GetBoundaryCell();

	iniInterPtArray.clear();

	double timeCost;
	for(i=0; i<emptycell->size(); i++)
	{
		OCNode* boundary_cell = (*emptycell)[i];

		b = boundary_cell->GetBegin();
		e = boundary_cell->GetEnd();
		Vector3fToPoint3D(*b, min_pt);
		Vector3fToPoint3D(*e, max_pt);
		box.m_min = min_pt;
		box.m_max = max_pt;

		NewPoint start, end, min, max;
		start.x = pa.x;	start.y = pa.y;	start.z = pa.z;
		end.x = pb.x; end.y = pb.y;	end.z = pb.z;
		min.x = box.m_min.x; min.y = box.m_min.y; min.z = box.m_min.z;
		max.x = box.m_max.x; max.y = box.m_max.y; max.z = box.m_max.z;
		Box pbox;
		pbox.min = min; pbox.max = max;
		F32 time;

		// line 与 Node的包围盒求交
		if(intersect(start, end, pbox, &time) == false)
			continue;
//		m_inipt.push_back(i);
		vector<int>&	isurfels = boundary_cell->GetSurfelsArray();

		for (j = 0; j < isurfels.size(); j++)
		{
			id = isurfels[j];
			pt = *GetBorderPt(id);
			//m_occell.push_back(pt);
			flag = PointLineSegInters(pt, pa,pb, m_accuracy);
			if (flag)
			{
			//	m_testborder.push_back(pt);
				t = PointProjecToLineParam(pt, pa, pb);

				CprojectPts ppt;
				ppt.SetValue(t,pt);
				paramArray->push_back(ppt);
			}		

		}

	}

}

void ProteinSignature::Vector3fToPoint3D(vector3f& v, Point3D& p)
{
	p.x = v.x; p.y = v.y; p.z = v.z;
}

bool ProteinSignature::intersect(const NewPoint &start, const NewPoint &end,const Box &box, F32 *time)
{
	F32 st,et,fst = 0,fet = 1;   
	F32 const *bmin = &box.min.x;
	F32 const *bmax = &box.max.x;
	F32 const *si = &start.x;
	F32 const *ei = &end.x;
	for (int i = 0; i < 3; i++) {
		if (*si < *ei) {
			if (*si > *bmax || *ei < *bmin)
				return false;
			F32 di = *ei - *si;
			st = (*si < *bmin)? (*bmin - *si) / di: 0;
			et = (*ei > *bmax)? (*bmax - *si) / di: 1;
		}
		else {
			if (*ei > *bmax || *si < *bmin)
				return false;
			F32 di = *ei - *si;
			st = (*si > *bmax)? (*bmax - *si) / di: 0;
			et = (*ei < *bmin)? (*bmin - *si) / di: 1;
		}
		if (st > fst) fst = st;
		if (et < fet) fet = et;
		if (fet < fst)
			return false;
		bmin++; bmax++;
		si++; ei++;
	}
	*time = fst;
	return true;
}
