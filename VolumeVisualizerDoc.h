// VolumeVisualizerDoc.h : interface of the CVolumeVisualizerDoc class
//


#pragma once


#include <string>


using namespace std;



class VolumeReader;
class Volume;
class ProteinSignature;
class GLVisualizer;
class CMesh;

class CVolumeVisualizerDoc : public CDocument
{
protected: // create from serialization only
	CVolumeVisualizerDoc();
	DECLARE_DYNCREATE(CVolumeVisualizerDoc)

// Attributes
public:
	Volume	*m_vol;
	CMesh	*m_mesh;
	ProteinSignature *m_signature;
	GLVisualizer * m_visualizer;

	bool	m_drawText;
	int		m_drawMode;
	bool	m_drawSamplePoints;
	bool	m_drawIDvisibility;
	bool	m_drawIDvisibility2;

	bool	m_drawOctree;
	float	m_threshold;

	string	m_filename;
	int		m_group_id;

	char	m_ShapeDir[512];

	int m_depth;
// Operations
public:

	void Free();
// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CVolumeVisualizerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();

	void FileOpen(CString name);
public:
	afx_msg void OnOctreeSetoctree32798();
public:
	afx_msg void OnInnerdistanceSetcomputevalue();
};


