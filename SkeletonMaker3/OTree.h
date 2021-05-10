#ifndef OTREE_H
#define OTREE_H
//----------------------------------------------//
// add by ydm 2003-11-4
#include "vector3f.h"
//#include "./3d/surfel.h"
//#include "./ANNLIB/ANNSearch.h"
//----------------------------------------------//
#include <vector>

using namespace std;

enum NODE_TYPE{F_NE, F_NW, F_SW, F_SE, B_NE, B_NW, B_SW, B_SE};

//add by ydm 2003-11-4
enum NODE_TYPE2{UNKOWN_CELL, BOUNDARY_CELL, INTERIOR_CELL, EXTERNAL_CELL};

struct PointElement{
	//this structure is used in special vector datastructure....
	PointElement(int a, int b, int c):prev(a),item(b),next(c){}
	int prev;
	int item;                      
	int next;
};

class SurfelScene; 

class OCNode{
	//the node of the octtree, see octtree at the end of this file
	//each node corresponding to a box when breaking planes
public:
	NODE_TYPE type;
	
	//these two point is for the boundary of the corresponding box
	vector3f* begin; //F_SW corner of the box
	vector3f* end; //B_NE corner of the box
	
	OCNode* parent;
	OCNode* F_NEchild; //front side
	OCNode* F_NWchild;
	OCNode* F_SEchild;
	OCNode* F_SWchild;
	
	OCNode* B_NEchild; //back side
	OCNode* B_NWchild;
	OCNode* B_SEchild;
	OCNode* B_SWchild;

	//------------------------------------------------------------------------------------
	// add by ydm 2003-11-4 
public:
	vector<int>		_isurfels;	//记录该结点包含的surfels的序号数组
//	vector<Surfel>	_Surfels;	//记录该结点包含的surfels的信息
	int				m_depth;	//记录该结点的深度
	NODE_TYPE2		m_type;
	vector3f		m_n;		//average nomral over all surfels in the boundary cell
	float			m_d1, m_d2; //the min value and max value of (n*x), respectively
	int				m_s1, m_s2; //record the index of d1 and d2, respectively
	vector<int>		_interInNodes;		//记录与该结点(B实体)相交的A实体的内部空结点序号
	vector<int>		_interOutNodes;		//记录与该结点(B实体)相交的A实体的外部空结点序号
	vector<int>		_interBoundNodes;	//记录与该结点(B实体)相交的A实体的Boundary结点序号

public:
	 void			SetDepth(int depth)						{m_depth = depth;};
	 int			GetDepth()								{return m_depth;};
	 void			SetType2(NODE_TYPE2 type)				{m_type = type;};
	 NODE_TYPE2		GetType2()								{return m_type;};
	 void			SetSurfelsArray(vector<int> isurfels)	{_isurfels = isurfels;};
	 vector<int>&	GetSurfelsArray()						{return _isurfels;};
	 void			AddSurfelsIndex(int i)					{_isurfels.push_back(i);};
	 void			SetAverageNomal(vector3f n)				{m_n = n;};
	 vector3f		GetAverageNomal()						{return m_n;};

	 void			SetD1D2(float d1, float d2)				{m_d1 = d1; m_d2 = d2;};
	 void			GetD1D2(float&d1, float&d2)				{d1 = m_d1; d2 = m_d2;};
	 void			SetS1S2(int s1, int s2)					{m_s1 = s1; m_s2 = s2;};
	 void			GetS1S2(int&s1, int&s2)					{s1 = m_s1; s2 = m_s2;};

	 vector<int>&	GetInterInNodesArray()					{return _interInNodes;};
	 void			AddInterInNodes(int node)				{_interInNodes.push_back(node);};
	 vector<int>&	GetInterOutNodesArray()					{return _interOutNodes;};
	 void			AddInterOutNodes(int node)				{_interOutNodes.push_back(node);};
	 vector<int>&	GetInterBoundNodesArray()				{return _interBoundNodes;};
	 void			AddInterBoundNodes(int node)			{_interBoundNodes.push_back(node);};
	//------------------------------------------------------------------------------------
	

public:
	OCNode(NODE_TYPE nodeType, OCNode* theParent);
	OCNode();
	~OCNode(){
		if(F_NWchild != NULL) delete F_NWchild;
		if(F_NEchild != NULL) delete F_NEchild;
		if(F_SEchild != NULL) delete F_SEchild;
		if(F_SWchild != NULL) delete F_SWchild;
		
		if(B_NWchild != NULL) delete B_NWchild;
		if(B_NEchild != NULL) delete B_NEchild;
		if(B_SEchild != NULL) delete B_SEchild;
		if(B_SWchild != NULL) delete B_SWchild;
		
		if(begin != NULL) delete  begin;
		if(end != NULL) delete end;
//		if(m_Search != NULL) delete m_Search;
	}
	
	void  SetBegin(vector3f* b){begin = b;}
	void  SetParent(OCNode* n){parent = n;}
	void  SetEnd(vector3f* e){end = e;}
	
	void  SetF_NEchild(OCNode* n){F_NEchild = n;}
	void  SetF_NWchild(OCNode* n){F_NWchild = n;}
	void  SetF_SEchild(OCNode* n){F_SEchild = n;}
	void  SetF_SWchild(OCNode* n){F_SWchild = n;}
	
	void  SetB_NEchild(OCNode* n){B_NEchild = n;}
	void  SetB_NWchild(OCNode* n){B_NWchild = n;}
	void  SetB_SEchild(OCNode* n){B_SEchild = n;}
	void  SetB_SWchild(OCNode* n){B_SWchild = n;}
	
	
	vector3f* GetBegin(){return(begin);}
	vector3f* GetEnd(){return(end);}
	
	OCNode*    GetF_NEchild(){return(F_NEchild);}
	OCNode*    GetF_NWchild(){return(F_NWchild);}
	OCNode*    GetF_SEchild(){return(F_SEchild);}
	OCNode*    GetF_SWchild(){return(F_SWchild);}
	
	OCNode*    GetB_NEchild(){return(B_NEchild);}
	OCNode*    GetB_NWchild(){return(B_NWchild);}
	OCNode*    GetB_SEchild(){return(B_SEchild);}
	OCNode*    GetB_SWchild(){return(B_SWchild);}

	bool isLeaf();		// is this a leaf node
	bool isEmpty();		//contains any point or not
	/*
	TO GET NEIGHBORS:
	call the following function, getNeighbors, pass your 
	vector of Node* to it (Pass by reference). After that your vector
	is filled with Node*
	*/

	void getNeighbors(vector<OCNode*>& nbrs, bool non_empty=false);
	/****************************************
	 *             DUE SECTION	            *
	 ****************************************/
	//---------------DUE SOUTH---------------------
	void getAllSouthNeighbors(vector<OCNode*>& nbrs);
	OCNode* getSouthNeighborNode();
	//get all the node on the north side
	void getNorthBoundary(vector<OCNode*>& nbrs);
	//---------------DUE NORTH---------------------
	// comment is the similar for the SOUTH part above
	void getAllNorthNeighbors(vector<OCNode*>& nbrs);
	OCNode* getNorthNeighborNode();
	//get all the node on the north side
	void getSouthBoundary(vector<OCNode*>& nbrs);
	//---------------DUE WEST---------------------
	// comment is the similar for the SOUTH part above
	void getAllWestNeighbors(vector<OCNode*>& nbrs);
	OCNode* getWestNeighborNode();
	void getEastBoundary(vector<OCNode*>& nbrs);
	//--------DUE EAST--------------------
	// comment is the similar for the SOUTH part above
	void getWestBoundary(vector<OCNode*>& nbrs);
	void getAllEastNeighbors(vector<OCNode*>& nbrs);
	OCNode* getEastNeighborNode();
	//---------------DUE FRONT---------------------
	// comment is the similar for the SOUTH part above
	void getAllFrontNeighbors(vector<OCNode*>& nbrs);
	OCNode* getFrontNeighborNode();
	void getBackBoundary(vector<OCNode*>& nbrs);
	//---------------DUE BACK---------------------
	// comment is the similar for the SOUTH part above
	void getAllBackNeighbors(vector<OCNode*>& nbrs);
	OCNode* getBackNeighborNode();
	void getFrontBoundary(vector<OCNode*>& nbrs);
	/************************************************
	 *             CORNER-LINE PARTS                *
	 ************************************************/
	//supporting routine
	void getNWLine(vector<OCNode*>& line);
	void getNELine(vector<OCNode*>& line);
	void getSWLine(vector<OCNode*>& line);
	void getSELine(vector<OCNode*>& line);
	void getF_NLine(vector<OCNode*>& line);
	void getF_SLine(vector<OCNode*>& line);
	void getF_ELine(vector<OCNode*>& line);
	void getF_WLine(vector<OCNode*>& line);
	void getB_NLine(vector<OCNode*>& line);
	void getB_SLine(vector<OCNode*>& line);
	void getB_ELine(vector<OCNode*>& line);
	void getB_WLine(vector<OCNode*>& line);
	
	//the neighbors on the edge
	void getNWNeighbors(vector<OCNode*>& nbrs);
	void getNENeighbors(vector<OCNode*>& nbrs);
	void getSWNeighbors(vector<OCNode*>& nbrs);
	void getSENeighbors(vector<OCNode*>& nbrs);
	void getF_NNeighbors(vector<OCNode*>& nbrs);
	void getF_SNeighbors(vector<OCNode*>& nbrs);
	void getF_ENeighbors(vector<OCNode*>& nbrs);
	void getF_WNeighbors(vector<OCNode*>& nbrs);
	void getB_NNeighbors(vector<OCNode*>& nbrs);
	void getB_SNeighbors(vector<OCNode*>& nbrs);
	void getB_ENeighbors(vector<OCNode*>& nbrs);
	void getB_WNeighbors(vector<OCNode*>& nbrs);
	/***********************************************
	*             TRUE CORNER PARTS                *
	************************************************/
	//supprorting routine
	OCNode* getF_NWCorner();
	OCNode* getF_NECorner();
	OCNode* getF_SWCorner();
	OCNode* getF_SECorner();

	OCNode* getB_NWCorner();
	OCNode* getB_NECorner();
	OCNode* getB_SWCorner();
	OCNode* getB_SECorner();

	//here come the box on the vetex
	OCNode* F_NENeighbor();//------ NE Corner------------
	OCNode* F_NWNeighbor();
	OCNode* F_SWNeighbor();
	OCNode* F_SENeighbor();

	OCNode* B_NENeighbor();//------ NE Corner------------
	OCNode* B_NWNeighbor();
	OCNode* B_SWNeighbor();
	OCNode* B_SENeighbor();

//private:
public:
	//------------------------------------------------------------------
	//use ANNkd-tree to improve the speed of finding the nearest surfel
	//add by ydm 2003-11-27
//	C_ANNSearch* m_Search;
		
public:
//	void	InitANNSearchTree(SurfelScene * pScene);
//	Surfel* FindNearestSurfel(Surfel s, SurfelScene * pScene);
	//void FindNearestSurfel2(Surfel s, vector<int>& neighbor);
//	void FindNearestSurfelArray(Surfel s, SurfelScene * pScene, vector<Surfel*>& neighbor_surfels, float &maxdist, int num = 13);
	//------------------------------------------------------------------
};

#endif




























