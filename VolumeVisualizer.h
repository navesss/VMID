// VolumeVisualizer.h : main header file for the VolumeVisualizer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CVolumeVisualizerApp:
// See VolumeVisualizer.cpp for the implementation of this class
//

class CVolumeVisualizerApp : public CWinApp
{
public:
	CVolumeVisualizerApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CVolumeVisualizerApp theApp;