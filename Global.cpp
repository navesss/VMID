#include "stdafx.h"
#include "global.h"

void ShowProgress(BOOL show, int total)
{
	AfxGetMainWnd()->SendMessage(WM_G_SHOWPROGRESS, show, total);
}

void SetProgressPos(int pos)
{
	AfxGetMainWnd()->SendMessage(WM_G_SETPROGRESSPOS, pos);
}

void SetProgressText(LPCSTR text)
{
	AfxGetMainWnd()->SendMessage(WM_G_SETPROGRESSTEXT, (WPARAM)text);
}

void SetStatusText(LPCSTR text, int pane)
{
	AfxGetMainWnd()->SendMessage(WM_G_SETSTATUSTEXTS, (WPARAM)text, pane);
}

void SetStatusText(UINT textid, int pane)
{
	AfxGetMainWnd()->SendMessage(WM_G_SETSTATUSTEXTU, textid, pane);
}