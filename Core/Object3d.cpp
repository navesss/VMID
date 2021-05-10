//********************************************
// Object3d.cpp
//********************************************
// class CObject3d, the base 3d object
//********************************************
// pierre.alliez@cnet.francetelecom.fr
// Created : 17/12/97
// Modified : 15/01/98
//********************************************

#include "stdafx.h"

#include "Object3d.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//********************************************
// GetType
//********************************************
// id
long CObject3d::GetID() const
{
	return m_id;
}

void CObject3d::SetID(long id)
{
	m_id = id;
}

void CObject3d::AddID(long &id)
{
	if(m_id == -1)
		m_id = id ++;
}

// ** EOF **



