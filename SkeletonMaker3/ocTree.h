#ifndef OCTREE_H
#define OCTREE_H
//----------------------------------------------//
// add by ydm 2003-11-4
#include "vector3f.h"
#include "OTree.h"
//#include "./3d/surfel.h"
//#include "./ANNLIB/ANNSearch.h"
//----------------------------------------------//
#include <vector>

using namespace std;

class OctTree{
	OCNode* octRoot;
public:
	OctTree(){
		octRoot = new OCNode();
		octRoot->SetParent(NULL);
	}
	~OctTree(){
		if(octRoot != NULL) delete octRoot;
	}
	void SetCubeArea(vector3f* begin, vector3f* end){
		//This function sets the boundaries of the quad tree
		octRoot->SetBegin(begin);
		octRoot->SetEnd(end);
	}

	OCNode* GetRoot(){ return(octRoot);}
};

#endif