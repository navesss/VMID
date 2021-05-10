// ResultDisplayingDlg.cpp : implementation file
//


#include "stdafx.h"
#include "VolumeVisualizer.h"
#include "ResultDisplayingDlg.h"

#include <Windows.h>
#include <Winuser.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define WS_EX_LAYERED	0x00080000
#define LWA_ALPHA		0x00000002

HMODULE		hUserDll ;
/////////////////////////////////////////////////////////////////////////////
// ResultDisplayingDlg dialog


ResultDisplayingDlg::ResultDisplayingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ResultDisplayingDlg::IDD, pParent)
{
	for ( int i = 0; i < 15; i++ ) 
	     image[i] = NULL;

	RetrievedResults.SetSize(0);
    RetrievedResults.FreeExtra();
	RetrievedResults.RemoveAll();

	PageNum = 0;
	PickedModel = -1; //no model is picked up
	hUserDll = NULL;
	state = FALSE;
	m_AlwaysDisp = FALSE;

	region[ 0].left = 28;  region[ 0].top = 32;  region[ 0].right = 112; region[ 0].bottom = 117;
	region[ 1].left = 133; region[ 1].top = 32;  region[ 1].right = 217; region[ 1].bottom = 117;
	region[ 2].left = 238; region[ 2].top = 32;  region[ 2].right = 322; region[ 2].bottom = 117;
	region[ 3].left = 343; region[ 3].top = 32;  region[ 3].right = 427; region[ 3].bottom = 117;
	region[ 4].left = 448; region[ 4].top = 32;  region[ 4].right = 532; region[ 4].bottom = 117;
	region[ 5].left = 28;  region[ 5].top = 146; region[ 5].right = 112; region[ 5].bottom = 231;
	region[ 6].left = 133; region[ 6].top = 146; region[ 6].right = 217; region[ 6].bottom = 231;
	region[ 7].left = 238; region[ 7].top = 146; region[ 7].right = 322; region[ 7].bottom = 231;
	region[ 8].left = 343; region[ 8].top = 146; region[ 8].right = 427; region[ 8].bottom = 231;
	region[ 9].left = 448; region[ 9].top = 146; region[ 9].right = 532; region[ 9].bottom = 231;
	region[10].left = 28;  region[10].top = 260; region[10].right = 113; region[10].bottom = 348;
	region[11].left = 133; region[11].top = 260; region[11].right = 218; region[11].bottom = 348;
	region[12].left = 238; region[12].top = 260; region[12].right = 323; region[12].bottom = 348;
	region[13].left = 343; region[13].top = 260; region[13].right = 428; region[13].bottom = 348;
	region[14].left = 448; region[14].top = 260; region[14].right = 533; region[14].bottom = 348;

}

ResultDisplayingDlg::~ResultDisplayingDlg()
{
	FreeMem();
}


void ResultDisplayingDlg::FreeMem()
{
	FreeImage();

	RetrievedResults.SetSize(0);
    RetrievedResults.FreeExtra();
	RetrievedResults.RemoveAll();

	if ( hUserDll )	
		::FreeLibrary( hUserDll );
}

void ResultDisplayingDlg::FreeImage()
{
    for ( int i = 0; i < 15; i++ ) {
		if ( image[i] != NULL ) {
			delete image[i];
			image[i] = NULL;
		}
	}
}


void ResultDisplayingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ResultDisplayingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ResultDisplayingDlg, CDialog)
	//{{AFX_MSG_MAP(ResultDisplayingDlg)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE( WM_ACTIVATE, OnActivate )
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUSPAGE, OnButtonPreviouspage)
	ON_BN_CLICKED(IDC_BUTTON_NEXTPAGE, OnButtonNextpage)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ResultDisplayingDlg message handlers

BOOL ResultDisplayingDlg::OpenImageFile( CString filename, int num )
{
	CString ext(FindExtension(filename));
	ext.MakeLower();
	if (ext == "") return FALSE;

	int type = FindType(ext);

	if ( image[num] != NULL ) {
		delete image[num];
		image[num] = NULL; 
	}

	Stopwatch(0);
	image[num] = new CxImage(filename, type);
	Stopwatch(1);

	if (!image[num]->IsValid()){
		AfxMessageBox(image[num]->GetLastError());
		delete image[num];
		image[num] = NULL;
		return FALSE;
	}

	return TRUE;
}

void ResultDisplayingDlg::Stopwatch(int start0stop1)
{
	if (start0stop1==0)	QueryPerformanceCounter(&m_swStart);
	else {
		QueryPerformanceCounter(&m_swStop);
		if (m_swFreq.LowPart==0 && m_swFreq.HighPart==0) m_etime = -1;
		else {
			m_etime = (float)(m_swStop.LowPart - m_swStart.LowPart);
			if (m_etime < 0) m_etime += 2^32;
			m_etime /= (m_swFreq.LowPart+m_swFreq.HighPart * 2^32);
		}
	}
}

CString ResultDisplayingDlg::FindExtension(const CString& name)
{
	int len = name.GetLength();
	int i;
	for (i = len-1; i >= 0; i--){
		if (name[i] == '.'){
			return name.Mid(i+1);
		}
	}
	return CString("");
}



int ResultDisplayingDlg::FindType(const CString& ext)
{
	int type = 0;
	if (ext == "bmp")					type = CXIMAGE_FORMAT_BMP;
#if CXIMAGE_SUPPORT_JPG
	else if (ext=="jpg"||ext=="jpeg")	type = CXIMAGE_FORMAT_JPG;
#endif
#if CXIMAGE_SUPPORT_GIF
	else if (ext == "gif")				type = CXIMAGE_FORMAT_GIF;
#endif
#if CXIMAGE_SUPPORT_PNG
	else if (ext == "png")				type = CXIMAGE_FORMAT_PNG;
#endif
#if CXIMAGE_SUPPORT_MNG
	else if (ext=="mng"||ext=="jng")	type = CXIMAGE_FORMAT_MNG;
#endif
#if CXIMAGE_SUPPORT_ICO
	else if (ext == "ico")				type = CXIMAGE_FORMAT_ICO;
#endif
#if CXIMAGE_SUPPORT_TIF
	else if (ext=="tiff"||ext=="tif")	type = CXIMAGE_FORMAT_TIF;
#endif
#if CXIMAGE_SUPPORT_TGA
	else if (ext=="tga")				type = CXIMAGE_FORMAT_TGA;
#endif
#if CXIMAGE_SUPPORT_PCX
	else if (ext=="pcx")				type = CXIMAGE_FORMAT_PCX;
#endif
#if CXIMAGE_SUPPORT_WBMP
	else if (ext=="wbmp")				type = CXIMAGE_FORMAT_WBMP;
#endif
#if CXIMAGE_SUPPORT_WMF
	else if (ext=="wmf"||ext=="emf")	type = CXIMAGE_FORMAT_WMF;
#endif
#if CXIMAGE_SUPPORT_J2K
	else if (ext=="j2k"||ext=="jp2")	type = CXIMAGE_FORMAT_J2K;
#endif
#if CXIMAGE_SUPPORT_JBG
	else if (ext=="jbg")				type = CXIMAGE_FORMAT_JBG;
#endif
#if CXIMAGE_SUPPORT_JP2
	else if (ext=="jp2"||ext=="j2k")	type = CXIMAGE_FORMAT_JP2;
#endif
#if CXIMAGE_SUPPORT_JPC
	else if (ext=="jpc"||ext=="j2c")	type = CXIMAGE_FORMAT_JPC;
#endif
#if CXIMAGE_SUPPORT_PGX
	else if (ext=="pgx")				type = CXIMAGE_FORMAT_PGX;
#endif
#if CXIMAGE_SUPPORT_RAS
	else if (ext=="ras")				type = CXIMAGE_FORMAT_RAS;
#endif
#if CXIMAGE_SUPPORT_PNM
	else if (ext=="pnm"||ext=="pgm"||ext=="ppm") type = CXIMAGE_FORMAT_PNM;
#endif
	else type = CXIMAGE_FORMAT_UNKNOWN;

	return type;
}


void ResultDisplayingDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	BOOL ret;
	CxImage tmp;
	int nCount, loop, number;
	char str[25];

    nCount = RetrievedResults.GetSize();

	if ( nCount/15 - PageNum > 0 )
		loop = 15;
	else 
		loop = nCount%15;

	for ( int i = 0; i < loop; i++ ) {
		number = RetrievedResults[PageNum*15+i].GetLength();
		RetrievedResults[PageNum*15+i].SetAt( number-1, 'g' );
		RetrievedResults[PageNum*15+i].SetAt( number-2, 'p' );
		RetrievedResults[PageNum*15+i].SetAt( number-3, 'j' );

		ret = OpenImageFile( RetrievedResults[PageNum*15+i], i );
		if ( ret == FALSE ) return;

	    tmp.Copy(*image[i]);
	    RGBQUAD c={255,255,255,0};
	    tmp.SetTransColor(c);
	    tmp.AlphaStrip();
	    tmp.Stretch(dc.GetSafeHdc(), region[i] );

        sprintf( str, "(%d)", PageNum*15+i+1 );
		dc.TextOut( (int)(0.5f * (region[i].right+region[i].left)) - 5, region[i].bottom + 5, str ); 

		//add filename
		sprintf( str, "Group %d", RetrievedGroupID[PageNum*15+i]);
		dc.TextOut( (int)(0.5f * (region[i].right+region[i].left)) - 25, region[i].bottom - 11, str ); 
	}
}



void ResultDisplayingDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	MousePos = -1;
	for ( int i = 0; i < 15; i++ ) {
		if ( point.x > region[i].left && point.x < region[i].right && point.y > region[i].top && point.y < region[i].bottom ) {
			MousePos = i;
			break;
		}  
	}

	if ( MousePos != -1 ) 
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_HAND));

	CDialog::OnMouseMove(nFlags, point);
}



void ResultDisplayingDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
 	if ( MousePos != -1 ) {
		if ( MousePos+PageNum*15 > RetrievedResults.GetSize()-1 )
		    SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_NOITEM));
		else 
			SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_PICK));
	}

	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}



void ResultDisplayingDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if ( MousePos != -1 ) {
		if ( MousePos+PageNum*15 > RetrievedResults.GetSize()-1 ) 
			SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_NOITEM));
		else {
	        PickedModel = MousePos+PageNum*15;
		    SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_PICK));
		}
	}

	((CFrameWnd*)AfxGetMainWnd())->GetActiveView()->SendMessage( WM_USER_DISPICKEDMODEL, 100, 0 );

	CDialog::OnLButtonDblClk(nFlags, point);
}


void ResultDisplayingDlg::SlideWindow() 
{
	ShowWindow(SW_SHOW );	

	if( state == FALSE ) {
	   for(int nPercent=0; nPercent <= 5; nPercent++)
		   SetTransparent(m_hWnd, 0, 255 * nPercent/5, LWA_ALPHA);
	} 

	InvalidateImageRect();	
	state = TRUE;
}


afx_msg LRESULT ResultDisplayingDlg::OnActivate( WPARAM wParam, LPARAM lParam )
{
/*
	if ( m_AlwaysDisp == TRUE ) return 1;
	if ( lParam != 0 ) {
		if ( state == TRUE ) {
			for(int nPercent=100; nPercent >= 0; nPercent--)
		        SetTransparent(m_hWnd, 0, 255 * nPercent/100, LWA_ALPHA);
	        ShowWindow(SW_HIDE);
		    state = FALSE;
		}
	}
*/
	return 1;
}


void ResultDisplayingDlg::HideDlg() 
{
    for(int nPercent=5; nPercent >= 0; nPercent--)
	    SetTransparent(m_hWnd, 0, 255 * nPercent/5, LWA_ALPHA);
	ShowWindow(SW_HIDE);
	state = FALSE;
}


BOOL ResultDisplayingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect rect;
	int width, height;

	hUserDll = ::LoadLibrary(_T("USER32.dll"));
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, ::GetWindowLong(m_hWnd, GWL_EXSTYLE) |  WS_EX_LAYERED );
	SetTransparent(m_hWnd, 0, 0, LWA_ALPHA );

	GetClientRect(&rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
    SetWindowPos( &wndTopMost, 300, 300, width+10, height+10, SWP_SHOWWINDOW );

	return TRUE;  
}


BOOL ResultDisplayingDlg::SetTransparent(HWND hWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
{
	BOOL	bRet = TRUE;
	typedef BOOL (WINAPI* lpfnSetTransparent)(HWND hWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
	
	// Check that "USER32.dll" library has been loaded successfully...
	if ( hUserDll )
	{
		lpfnSetTransparent pFnSetTransparent  = NULL;
		pFnSetTransparent  = (lpfnSetTransparent)GetProcAddress(hUserDll, "SetLayeredWindowAttributes");
		if (pFnSetTransparent )
			bRet = pFnSetTransparent(hWnd, crKey, bAlpha, dwFlags);

		else 
			bRet = FALSE;
	}

	return bRet;
} 

void ResultDisplayingDlg::OnButtonPreviouspage() 
{
	if ( PageNum > 0 ) PageNum -= 1;	

	InvalidateImageRect();
}


void ResultDisplayingDlg::OnButtonNextpage() 
{
	if ( (PageNum+1) * 15 < RetrievedResults.GetSize() )
		PageNum += 1;
	
	InvalidateImageRect();
}


void ResultDisplayingDlg::OnButtonClose() 
{
    HideDlg();     
}


void ResultDisplayingDlg::InvalidateImageRect() 
{
	CRect rect;
	rect.left = 22; rect.top = 32; rect.right = 533; rect.bottom = 368;
	InvalidateRect(rect);
}


void ResultDisplayingDlg::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
	delete this;
}
