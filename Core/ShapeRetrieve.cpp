// ShapeRetrieve.cpp
//

#include "stdafx.h"
#include "ShapeRetrieve.h"

#include <cmath>
#include <vector>
#include <algorithm>
#include <map>

#include "VolumeVisualizerDoc.h"
#include "FileManager.h"
#include "get_pcr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace std;


bool retrieveComparator(CS_RetrievedModel &r1, CS_RetrievedModel &r2)
{
	return r1.m_score < r2.m_score;
}

void CS_RetrievedModel::set_file_name()
{
	m_file_name = get_file_name(m_dir_name);
}

void CS_RetrievedModel::set_file_name2()
{
	m_file_name2 = get_file_name2(m_file_name);
}

void CS_RetrievedModel::set_group_name()
{
	m_group_name = get_group_name(m_file_name);
}

void CS_RetrievedModel::set_group_id(int id)
{
	m_group_id = id;
}

void CS_RetrievedModel::add_group_id(int &id)
{
	if(m_group_id == -1)
		m_group_id = id ++;
}

//////////////////////////////////////////////////////////////
// CShapeDistribution

CShapeDistribution::CShapeDistribution()
{
	m_Distribution.clear();
}

CShapeDistribution::CShapeDistribution(CShapeDistribution *dist)
{
	m_Distribution = dist->m_Distribution;

}

CShapeDistribution::~CShapeDistribution()
{
	m_Distribution.clear();
}

double CompareDistributionHistogram( vector<long> &dis1, vector<long> &dis2, int n )
{
	int i, max;
	double sum = 0.0, result;
	double temp;


	int max1 = dis1.size();
	int max2 = dis2.size();

	if( max1 > max2 )
	{
		max = max1;

		for(  i = max2; i < max; i++ )
			dis2[i] = 0;

	} else	{
		max = max2;

		for( i = max1; i < max; i++ )
			dis1[ i ] = 0;
	}

	for( i = 0; i < max; i++ )
	{
		temp = abs( dis1[i] - dis2[i] ); 
		sum += pow( temp, n);
	}

	result = pow(sum, (double)(1.0/n));
	return result;
}


//////////////////////////////////////////////////////////////
// class CShapeRetrieve

CShapeRetrieve::CShapeRetrieve()
{
	Init();
}

CShapeRetrieve::CShapeRetrieve(CVolumeVisualizerDoc *doc)
{
	Init();
	m_doc = doc;
}

CShapeRetrieve::~CShapeRetrieve()
{
}

void CShapeRetrieve::Init()
{
	m_RetrievedModels.clear();

	m_doc = NULL;
}
void CShapeRetrieve::SetDocment(CVolumeVisualizerDoc *doc)
{
	m_doc = doc;
}

CVolumeVisualizerDoc* CShapeRetrieve::GetDocment()
{
	return m_doc;
}


void CShapeRetrieve::DoRetrieve(int method)
{
	vector<CShapeDistribution*>	distribution_array;
	CShapeDistribution	*SD1, *SD2;
	int					i, j, m, ItemCount, nb, group_id, temp_id;
	double				score;
	string				na1, na2, name1, name2;

	m_RetrievedModels.clear();
	SD1 = SD2 = NULL;

	// 0. pre-process read index library
	string shape_dir = string(m_doc->m_ShapeDir);
	string IndexDir;
	string dir("\\IndexLibrary\\ID.idx");

	//IndexDir = shape_dir + dir;
	switch (method)
	{
	case 1:
		IndexDir = string("C:\\BenchmarkFlex\\ID.idx");
		break;
	case 2:
		IndexDir = string("C:\\BenchmarkFlex\\ED.idx");
		break;
	case 3:
		IndexDir = string("C:\\BenchmarkFlex\\GD.idx");
		break;
	default:
		break;
	}
	//IndexDir = string("C:\\BenchmarkFlex\\ID.idx");

	//set the location of index-library
	FILE *fp = fopen(IndexDir.c_str(), "r");

	if( fp == NULL ) 
	{
		AfxMessageBox( "Fatal Error: can't open feature library.\nPlease check the FeatureLibrary.");
		return;
	}

	fseek( fp, 0L, SEEK_SET );

	if ( fscanf( fp, "%d\n", &ItemCount ) == 0 ) {
		AfxMessageBox( "Fatal Error: can't get data from feature library.");
		fclose(fp);
		return;
	}


	group_id = 1;
	for ( i=0; i < ItemCount; i++)
	{
		CS_RetrievedModel	RetrievedModel;
		char              RetrievaledFile[512];

		fgets( RetrievaledFile, 512, fp );
		m = strlen( RetrievaledFile );
		RetrievaledFile[ m-1 ] = '\0';
		// add the file name to retrieved model
		RetrievedModel.m_dir_name = string(RetrievaledFile);
		RetrievedModel.set_file_name();
		RetrievedModel.set_file_name2();
		RetrievedModel.set_group_name();
		RetrievedModel.m_score = 0.0;

		// add group id
		nb = m_RetrievedModels.size();
		na1 = RetrievedModel.m_group_name;
		for ( j = 0; j < nb; j++ )
		{
			na2 = m_RetrievedModels[j].m_group_name;
			temp_id = m_RetrievedModels[j].m_group_id;
			if(na1 == na2)
			{
				RetrievedModel.set_group_id(temp_id);
				break;
			}
		}
		if(j == nb)
			RetrievedModel.add_group_id(group_id);

		// add the retrieved into array
		m_RetrievedModels.push_back(RetrievedModel);

		SD2 = new CShapeDistribution();

		int bin_nb = 0;
		fscanf( fp, "%d\n", &bin_nb );
		for ( j = 0; j < bin_nb; j++ ) 
		{
			long distribution = 0;
			fscanf( fp, "%d\n", &distribution );
			SD2->m_Distribution.push_back(distribution);
		}

		distribution_array.push_back(SD2);
	}

	
	// 1. search or calculate the histogram SD1
	na1 = m_doc->m_filename;
	na1 = get_file_name2(na1);

	for ( i=0; i < ItemCount; i++)
	{
		CS_RetrievedModel	RetrievedModel = m_RetrievedModels[i];
		string na2 = RetrievedModel.m_file_name2;

		if(na1 == na2)
		{
			SD2 = distribution_array[i];
			SD1 = new CShapeDistribution(SD2);
			m_doc->m_group_id = RetrievedModel.m_group_id;
			break;
		}		
	}
	if(SD1 == NULL)
	{
		AfxMessageBox("The query model is no in the database. Do you want to compute its signature?");
		return;
	}

	// 2. search the histogram of ./IndexLibrary and compute the similarity score
	for ( i=0; i < ItemCount; i++)
	{
		SD2 = distribution_array[i];

		score = CompareDistributionHistogram( SD1->m_Distribution, SD2->m_Distribution, 1 );
		// add the score to retrieved model
		m_RetrievedModels[i].m_score = score;
	}

	fclose(fp);	

	ASSERT(m_RetrievedModels.size() > 0);

	// 3. Sort Retrieved Results
	std::sort(m_RetrievedModels.begin(), m_RetrievedModels.end(), retrieveComparator);


	// free
	for(i=0; i<distribution_array.size(); i++)
		delete distribution_array[i];
	distribution_array.clear();
	delete SD1;


	//get_PRCurves_data();
}

void CShapeRetrieve::get_PRCurves_data(string IndexDir)
{
	vector<CShapeDistribution*>	distribution_array;
	CShapeDistribution	*SD1, *SD2;
	int					i, j, m, ItemCount, nb, group_id, temp_id;
	double				score;
	string				na1, na2, name1, name2;

	m_RetrievedModels.clear();
	SD1 = SD2 = NULL;

	// 0. pre-process read index library
	string IndexRoot = get_file_direction(IndexDir);

	//set the location of index-library
	FILE *fp = fopen(IndexDir.c_str(), "r");

	if( fp == NULL ) 
	{
		AfxMessageBox( "Fatal Error: can't open feature library.\nPlease check the FeatureLibrary.");
		return;
	}

	fseek( fp, 0L, SEEK_SET );

	if ( fscanf( fp, "%d\n", &ItemCount ) == 0 ) {
		AfxMessageBox( "Fatal Error: can't get data from feature library.");
		fclose(fp);
		return;
	}


	group_id = 1;
	for ( i=0; i < ItemCount; i++)
	{
		CS_RetrievedModel	RetrievedModel;
		char              RetrievaledFile[512];

		fgets( RetrievaledFile, 512, fp );
		m = strlen( RetrievaledFile );
		RetrievaledFile[ m-1 ] = '\0';
		// add the file name to retrieved model
		RetrievedModel.m_dir_name = string(RetrievaledFile);
		RetrievedModel.set_file_name();
		RetrievedModel.set_file_name2();
		RetrievedModel.set_group_name();
		RetrievedModel.m_score = 0.0;

		// add group id
		nb = m_RetrievedModels.size();
		na1 = RetrievedModel.m_group_name;
		for ( j = 0; j < nb; j++ )
		{
			na2 = m_RetrievedModels[j].m_group_name;
			temp_id = m_RetrievedModels[j].m_group_id;
			if(na1 == na2)
			{
				RetrievedModel.set_group_id(temp_id);
				break;
			}
		}
		if(j == nb)
			RetrievedModel.add_group_id(group_id);

		// add the retrieved into array
		m_RetrievedModels.push_back(RetrievedModel);

		SD2 = new CShapeDistribution();

		int bin_nb = 0;
		fscanf( fp, "%d\n", &bin_nb );
		for ( j = 0; j < bin_nb; j++ ) 
		{
			long distribution = 0;
			fscanf( fp, "%d\n", &distribution );
			SD2->m_Distribution.push_back(distribution);
		}

		distribution_array.push_back(SD2);
	}

	fclose(fp);	

	// 2. search the histogram of ./IndexLibrary and compute the similarity score
	float* matrix = new float[ItemCount*ItemCount];
	for ( i=0; i < ItemCount; i++)
	{
		SD1 = distribution_array[i];
		for ( j=0; j < ItemCount; j++)
		{
			SD2 = distribution_array[j];

			score = CompareDistributionHistogram( SD1->m_Distribution, SD2->m_Distribution, 1 );
			// add the score to retrieved model
			matrix[i*ItemCount+j] = score;
		}
	}
	
	output_PRCurves_data(m_RetrievedModels, matrix, IndexRoot);

	// free
	for(i=0; i<distribution_array.size(); i++)
		delete distribution_array[i];
	distribution_array.clear();

	delete []matrix;
}

int CShapeRetrieve::output_PRCurves_data(vector<CS_RetrievedModel>	&RetrievedModels, 
										 float* matrix, string IndexDir)
{
	map<string, string> grps;	//protein => group
	map<string, int> res;		//protein => residue size
	map<int, int> yes;
	vector<string> proteins;
	
	int				i, j, total, ItemCount;
	
	ItemCount = m_RetrievedModels.size();
	total = ItemCount;

	string				filename1, filename2, filename3, model_name;
	CS_RetrievedModel	model;

	filename1 = IndexDir + string("proteins.txt");
	filename2 = IndexDir + string("types.txt");
	filename3 = IndexDir + string("evhmatrix.txt");

	// proteins
	FILE * fp = fopen(filename1.c_str(), "w");
	if (!fp) { printf("Can't open %s.\n", filename1.c_str()); return -1; }
	for(i=0; i<ItemCount; i++)
	{
		model = m_RetrievedModels[i];
		model_name = model.m_file_name;

		fprintf(fp, "%s\n", model_name.c_str());
	}

	fclose(fp);

	fp = fopen(filename2.c_str(), "w");
	if (!fp) { printf("Can't open %s.\n", filename2.c_str()); return -1; }
	for(i=0; i<ItemCount; i++)
	{
		model = m_RetrievedModels[i];
		model_name = model.m_file_name;
		int group_id = model.m_group_id;
		int res = 0;

		fprintf(fp, "%s Group%d %d\n", model_name.c_str(), group_id, res);
	}

	fclose(fp);	

	fp = fopen(filename3.c_str(), "w");
	if (!fp) { printf("Can't open %s.\n", filename3.c_str()); return -1; }

	int id = 0;
	for(i=0; i<ItemCount; i++)
	{
		for(j=0; j<ItemCount; j++)
			fprintf(fp, "%f\t", matrix[id++]);
		fprintf(fp, "\n");
	}

	fclose(fp);	

	string str = "Successfully output PR data files:\n" + filename1 + "\n" + filename2 + "\n" + filename3 + "\n";
	AfxMessageBox(str.c_str());


	string PRC_file;
	get_pcr(IndexDir, PRC_file);

	str = "Successfully output the average PR Curve:\n" + PRC_file + "\n"
		+ "The 1st column is Recall\n" + "The 2nd column is Precision\n";
	AfxMessageBox(str.c_str());

	return 1;

}