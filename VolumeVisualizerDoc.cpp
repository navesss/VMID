// VolumeVisualizerDoc.cpp : implementation of the CVolumeVisualizerDoc class
//

#include "stdafx.h"
#include "VolumeVisualizer.h"

#include "VolumeVisualizerDoc.h"

#include "FileManager.h"
#include "volume.h"
#include "ProteinSignature.h"
#include "GLVisualizer.h"
#include "_global.h"
#include "_common.h"

#include "octreeDlg.h"
#include "global.h"
#include "SetValueDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//static char file_open[] = "MRC files(*.mrc)|*.mrc|SOF files(*.sof)|*.sof|All files(*.*)|*.*||";
static char file_open[] = "Volume files(*.mrc;*.sof)|*.mrc;*.sof|Mesh files(*.obj)|*.obj|All files(*.*)|*.*||";
static char file_save[] = "MRC files(*.mrc)|*.mrc|VRML files(*.wrl)|*.wrl|";


// CVolumeVisualizerDoc

IMPLEMENT_DYNCREATE(CVolumeVisualizerDoc, CDocument)

BEGIN_MESSAGE_MAP(CVolumeVisualizerDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CVolumeVisualizerDoc::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CVolumeVisualizerDoc::OnFileSave)
	ON_COMMAND(ID_OCTREE_SETOCTREE32798, &CVolumeVisualizerDoc::OnOctreeSetoctree32798)
	ON_COMMAND(ID_INNERDISTANCE_SETCOMPUTEVALUE, &CVolumeVisualizerDoc::OnInnerdistanceSetcomputevalue)
END_MESSAGE_MAP()


// CVolumeVisualizerDoc construction/destruction

CVolumeVisualizerDoc::CVolumeVisualizerDoc()
{
	m_vol = NULL;
	m_mesh = NULL;
	m_signature = NULL;
	m_visualizer = NULL;

	m_drawText = true;
	m_drawSamplePoints = false;
	m_drawIDvisibility = false;
	m_drawIDvisibility2 = false;
	m_drawMode = DRAW_VOLUME_BORDER_POINTS;

	m_drawOctree=false;
	m_threshold = 0.0f;
	m_group_id = -1;

	GetCurrentDirectory(512, m_ShapeDir);
}

CVolumeVisualizerDoc::~CVolumeVisualizerDoc()
{
	Free();
}

BOOL CVolumeVisualizerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	Free();

	return TRUE;
}

void CVolumeVisualizerDoc::Free()
{
	if(m_vol != NULL)
	{
		delete m_vol;
		m_vol = NULL;
	}

	if(m_mesh != NULL)
	{
		delete m_mesh;
		m_mesh = NULL;
	}

	if(m_signature != NULL)
	{
		delete m_signature;
		m_signature = NULL;
	}

	if(m_visualizer != NULL)
	{
		delete m_visualizer;
		m_visualizer = NULL;
	}
}


// CVolumeVisualizerDoc serialization

void CVolumeVisualizerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CVolumeVisualizerDoc diagnostics

#ifdef _DEBUG
void CVolumeVisualizerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVolumeVisualizerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CVolumeVisualizerDoc commands

void CVolumeVisualizerDoc::OnFileOpen()
{
	//Read point date from specified file
	CFileDialog dia(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, file_open, NULL);
	if(dia.DoModal() != IDOK){
		return;
	}

	CString name = dia.GetPathName();

	FileOpen(name);

	UpdateAllViews(NULL);
}

void CVolumeVisualizerDoc::OnFileSave() 
{
	if(m_vol == NULL)
	{
		AfxMessageBox("No the first Volume model for saving!");
		return;
	}

	CFileDialog dia(FALSE, "*.mrc", "1.mrc", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		file_save, NULL);

	if(dia.DoModal() != IDOK){
		return;
	}

	CString name = dia.GetPathName();
	// Extension
	CString ext = name;
	int pos = ext.ReverseFind('.');
	pos = ext.GetLength() - pos;
	ext = ext.Right(pos);
	ext.MakeLower();

	// Path "c:\path\file.wrl" -> c:\path
	CString path = name;
	path = path.Left(path.ReverseFind('\\'));
	SetCurrentDirectory(path);

	BeginWaitCursor();

	if(ext == ".wrl")
	{
		FileManager fm;

		fm.write_VRML(m_visualizer, name);
	}
	else if(ext == ".mrc")
	{
		FileManager fm;

		fm.write_MRC(m_visualizer->m_signature->m_vol, name);
	}
	else
		return;

	EndWaitCursor();

	UpdateAllViews(NULL);
}

void CVolumeVisualizerDoc::FileOpen(CString name)
{
	BeginWaitCursor();

	m_filename = get_file_name(name.GetBuffer());
	m_group_id = -1;

	// Extension
	CString ext = name;
	int pos = ext.ReverseFind('.');
	pos = ext.GetLength() - pos;
	ext = ext.Right(pos);
	ext.MakeLower();

	// Path "c:\path\file.mrc" -> c:\path
	CString path = name;
	path = path.Left(path.ReverseFind('\\'));
	SetCurrentDirectory(path);

	if(ext == ".mrc" || ext == ".sof")
	{
		FileManager fm;

		Free();

		ShowProgress(TRUE, 10);	

		SetProgressText("Reading file...");
		SetProgressPos(0);

		if(ext == ".mrc")
			m_vol = fm.read_MRC(name);
		else if(ext == ".sof")
			m_vol = fm.read_SOF(name);

		SetProgressText("Reading finished");
		SetProgressPos(2);

		float tmax = m_vol->getMax();;
		float tmin = m_vol->getMin();
	
		//m_threshold = 0.5; // 1AON.mrc
		//m_threshold = 0.436; // 1AON.mrc
		//m_threshold = 0.1; // 1AON.mrc
		

		m_threshold = 0.5*(tmax+tmin);
		
		m_vol->threshold2( m_threshold, 0, 1 ) ;

		if(m_vol)
		{
			m_signature = new ProteinSignature(m_vol);

			SetProgressText("Building Object...");
			SetProgressPos(4);

			m_visualizer = new GLVisualizer(m_signature);
		
			SetProgressText("Building Finished.");
			SetProgressPos(8);

			

			m_depth=5;
			m_signature->BuildOctree(m_depth);

			
			SetProgressText("Build Octree...");
			SetProgressPos(10);

			SetProgressText("Building Finished.");
			ShowProgress(FALSE, 0);

			m_drawMode = DRAW_VOLUME_BORDER_POINTS;
		}
		else
		{
			AfxMessageBox("No model file found!");
			return;
		}

	}
	else if(ext == ".obj")
	{
		FileManager fm;

		Free();

		m_mesh = fm.read_OBJ(name);

		if(m_mesh)
		{
			m_signature = new ProteinSignature(m_vol);

			m_visualizer = new GLVisualizer(m_signature);

			m_drawMode = DRAW_VOLUME_BORDER_POINTS;
		}
		else
		{
			AfxMessageBox("No model file found!");
			return;
		}

		
	}
		return;

	EndWaitCursor();

	UpdateAllViews(NULL);
}


void CVolumeVisualizerDoc::OnOctreeSetoctree32798()
{
	CoctreeDlg dlg;

	if(m_visualizer==NULL)
		return;
	if(dlg.DoModal()==IDOK)
	{
		/*if(m_depth != dlg.m_depth)*/
		/*{*/
			m_depth=dlg.m_depth;
			m_signature->BuildOctree(m_depth);
			m_drawOctree=!m_drawOctree;
			//AfxMessageBox(_T("八叉树建立完成。"));
		/*}*/
	}

	UpdateAllViews(NULL);
}

void CVolumeVisualizerDoc::OnInnerdistanceSetcomputevalue()
{
	// TODO: 在此添加命令处理程序代码
	CSetValueDlg dlg;

	if(m_visualizer==NULL)
		return;
	if(dlg.DoModal()==IDOK)
	{

		m_signature->m_accuracy=dlg.m_accuracy;
	}

	UpdateAllViews(NULL);
}
