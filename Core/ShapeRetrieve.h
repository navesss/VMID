 // ShapeRetrieve.h
#pragma once

#include <vector>
#include <string>


using namespace std;

class CShapeDistribution;
class CShapeRetrieve;
class CVolumeVisualizerDoc;


//////////////////////////////////////////////////////////////
// CShapeDistribution

struct CS_RetrievedModel
{
	string				m_dir_name;		// the file name of retrieved model
	string				m_file_name;		// the file name of retrieved model
	string				m_file_name2;		// the file name of retrieved model
	string				m_group_name;
	double				m_score;	// the similarity score between query and retrieved model
	int					m_group_id;

	CS_RetrievedModel(){m_group_id = -1;};

	void set_file_name();
	void set_file_name2();
	void set_group_name();

	void set_group_id(int id);
	void add_group_id(int &id);
};

//////////////////////////////////////////////////////////////
// CShapeDistribution

class CShapeDistribution
{
public:
	vector<long>	m_Distribution;

public:
	CShapeDistribution();
	CShapeDistribution(CShapeDistribution *dist);
	~CShapeDistribution();

};

double CompareDistributionHistogram( vector<long> &dis1, vector<long> &dis2, int n );

//////////////////////////////////////////////////////////////
// class CShapeRetrieve

class CShapeRetrieve
{
public:
	CVolumeVisualizerDoc	*m_doc;

	vector<CS_RetrievedModel>	m_RetrievedModels;

public: 
	CShapeRetrieve();
	CShapeRetrieve(CVolumeVisualizerDoc *doc);
	~CShapeRetrieve();

	void Init();

	void SetDocment(CVolumeVisualizerDoc *doc);
	CVolumeVisualizerDoc* GetDocment();

	void DoRetrieve(int method);

	void get_PRCurves_data(string IndexDir);
	int output_PRCurves_data(vector<CS_RetrievedModel>	&m_RetrievedModels, float* matrix, string IndexDir);
};


