//********************************************
// Object3d.h
//********************************************
// class CObject3d, the base 3d object
//********************************************
// pierre.alliez@cnet.francetelecom.fr
// Created : 17/12/97
// Modified : 19/12/97
//********************************************

#ifndef _OBJECT3D_
#define _OBJECT3D_

class CObject3d
{
private :
	long	m_id;

public :
	CObject3d() {init();}
	virtual ~CObject3d() {}

	void init(){m_id = -1;}

	// id
	long GetID() const;
	void SetID(long id);
	void AddID(long &id);
};

#endif // _OBJECT3D_
