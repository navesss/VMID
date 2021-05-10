/************************************************
* Implementation file for class Node			*
************************************************/

//--------------------------------------------------------
//	add by ydm 2003-11-4
//#include "stdafx.h"
//#include "./math/vector3f.h"
//#include "./3d/surfel.h"
//#include "./3d/surfelscene.h"
//#include "./3d/surfelrenderer.h"
//--------------------------------------------------------

#include "OTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/************************************************
*            Constructors                      *
************************************************/

OCNode::OCNode(NODE_TYPE nodeType, OCNode* theParent){
	/*This is the constructor for the node, doing some stuff of initialization*/
	begin = NULL;
	end = NULL;
	
	F_NEchild = NULL;
	F_NWchild = NULL;
	F_SEchild = NULL;
	F_SWchild = NULL;
	
	B_NEchild = NULL;
	B_NWchild = NULL;
	B_SEchild = NULL;
	B_SWchild = NULL;
	
	type = nodeType;
	parent = theParent;
	switch(nodeType){
	case F_NE:theParent->SetF_NEchild(this);break;
	case F_NW:theParent->SetF_NWchild(this);break;
	case F_SW:theParent->SetF_SWchild(this);break;
	case F_SE:theParent->SetF_SEchild(this);break;
		
	case B_NE:theParent->SetB_NEchild(this);break;
	case B_NW:theParent->SetB_NWchild(this);break;
	case B_SW:theParent->SetB_SWchild(this);break;
	case B_SE:theParent->SetB_SEchild(this);break;
		
	default:
		break;
	}
	
	// init ann search tree
//	m_Search = NULL;
}
OCNode::OCNode(){
	/*This is the constructor for the node, doing some stuff of initialization*/
	begin = NULL;end = NULL;
	F_NEchild = NULL;F_NWchild = NULL;F_SEchild = NULL;F_SWchild = NULL;
	B_NEchild = NULL;B_NWchild = NULL;B_SEchild = NULL;B_SWchild = NULL;
	type = F_NE;parent = NULL;

	// init ann search tree
//	m_Search = NULL;
}

/************************************************
 *           Some additional functions          *
 ************************************************/
bool OCNode::isEmpty(){
	//revised by ydm 2003-11-4
	//if (_isurfels.size()<=0) return true;
	if (m_type != BOUNDARY_CELL) return true;
	else return false;
}
bool OCNode::isLeaf(){
	/*Check to see if current node is a leaf or not, remember to check all 4*/
	if((F_NEchild == NULL) && (F_NWchild == NULL) && (F_SEchild == NULL) && (F_SWchild == NULL) &&
		(B_NEchild == NULL) && (B_NWchild == NULL) && (B_SEchild == NULL) && (B_SWchild == NULL)
		) return true;
    return false;
}

/****************************************
 *     DUE PARTS					    *
 ****************************************/

//--------------- DUE SOUTH---------------------
void OCNode::getAllSouthNeighbors(vector<OCNode*>& nbrs){
	//get all south neighb of the current node
	OCNode* tmpNode = getSouthNeighborNode();
	if(tmpNode != NULL)
		tmpNode->getNorthBoundary(nbrs);
}
OCNode* OCNode::getSouthNeighborNode(){//this is for the child to call
	//get one special node on the south side
	if(parent == NULL) return NULL;
	switch(type){
	case F_NE:{
		return (parent->GetF_SEchild());}
	case F_NW:{
		return (parent->GetF_SWchild());}
	case F_SW:{
		OCNode* tmp = parent->getSouthNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetF_NWchild());}
	case F_SE:{
		OCNode* tmp = parent->getSouthNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetF_NEchild());}
		
	case B_NE:{
		return (parent->GetB_SEchild());}
	case B_NW:{
		return (parent->GetB_SWchild());}
	case B_SW:{
		OCNode* tmp = parent->getSouthNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetB_NWchild());}
	case B_SE:{
		OCNode* tmp = parent->getSouthNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetB_NEchild());}
		
	default:
		return NULL;
	}
}
//get all the node on the north side
void OCNode::getNorthBoundary(vector<OCNode*>& nbrs){
	if(isLeaf()) nbrs.push_back(this);
	else{
		F_NWchild->getNorthBoundary(nbrs);
		F_NEchild->getNorthBoundary(nbrs);
		B_NWchild->getNorthBoundary(nbrs);
		B_NEchild->getNorthBoundary(nbrs);
	}
}

//---------------DUE NORTH---------------------
// comment is the similar for the SOUTH part above
void OCNode::getAllNorthNeighbors(vector<OCNode*>& nbrs){
	OCNode* tmpNode = getNorthNeighborNode();
	if(tmpNode != NULL)
		tmpNode->getSouthBoundary(nbrs);
}

OCNode* OCNode::getNorthNeighborNode(){//this is for the child to call
	if(parent == NULL) return NULL;
	switch(type){
		
	case F_SE:{
		return (parent->GetF_NEchild());}
	case F_SW:{
		return (parent->GetF_NWchild());}
	case F_NW:{
		OCNode* tmp = parent->getNorthNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetF_SWchild());}
	case F_NE:{
		OCNode* tmp = parent->getNorthNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetF_SEchild());}
		
		
	case B_SE:{
		return (parent->GetB_NEchild());}
	case B_SW:{
		return (parent->GetB_NWchild());}
	case B_NW:{
		OCNode* tmp = parent->getNorthNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetB_SWchild());}
	case B_NE:{
		OCNode* tmp = parent->getNorthNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetB_SEchild());}
		
	default:
		return NULL;
	}
}
//get all the node on the north side
void OCNode::getSouthBoundary(vector<OCNode*>& nbrs){
	if(isLeaf()) nbrs.push_back(this);
	else{
		F_SWchild->getSouthBoundary(nbrs);
		F_SEchild->getSouthBoundary(nbrs);
		B_SWchild->getSouthBoundary(nbrs);
		B_SEchild->getSouthBoundary(nbrs);
	}
}
//---------------DUE WEST---------------------
// comment is the similar for the SOUTH part above
void OCNode::getAllWestNeighbors(vector<OCNode*>& nbrs){
	OCNode* tmpNode = getWestNeighborNode();
	if(tmpNode != NULL)
		tmpNode->getEastBoundary(nbrs);
}
OCNode* OCNode::getWestNeighborNode(){//this is for the child to call
	if(parent == NULL) return NULL;
	switch(type){
	case F_SE:{
		return (parent->GetF_SWchild());}
	case F_SW:{
		OCNode* tmp = parent->getWestNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetF_SEchild());}
	case F_NW:{
		OCNode* tmp = parent->getWestNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetF_NEchild());}
	case F_NE:{
		return (parent->GetF_NWchild());}
		
	case B_SE:{
		return (parent->GetB_SWchild());}
	case B_SW:{
		OCNode* tmp = parent->getWestNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetB_SEchild());}
	case B_NW:{
		OCNode* tmp = parent->getWestNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetB_NEchild());}
	case B_NE:{
		return (parent->GetB_NWchild());}
		
	default:
		return NULL;
	}
}
void OCNode::getEastBoundary(vector<OCNode*>& nbrs){
	if(isLeaf()) nbrs.push_back(this);
	else{
		F_NEchild->getEastBoundary(nbrs);
		F_SEchild->getEastBoundary(nbrs);
		
		B_NEchild->getEastBoundary(nbrs);
		B_SEchild->getEastBoundary(nbrs);
	}
}
//-------- DUE EAST--------------------
// comment is the similar for the SOUTH part above
void OCNode::getAllEastNeighbors(vector<OCNode*>& nbrs){
	OCNode* tmpNode = getEastNeighborNode();
	if(tmpNode != NULL)
		tmpNode->getWestBoundary(nbrs);
}
OCNode* OCNode::getEastNeighborNode(){//this is for the child to call
	if(parent == NULL) return NULL;
	switch(type){
	case F_SE:{
		OCNode* tmp = parent->getEastNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetF_SWchild());}	     
	case F_SW:{
		return (parent->GetF_SEchild());}
	case F_NW:{
		return (parent->GetF_NEchild());}
	case F_NE:{
		OCNode* tmp = parent->getEastNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetF_NWchild());}
		
	case B_SE:{
		OCNode* tmp = parent->getEastNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetB_SWchild());}	     
	case B_SW:{
		return (parent->GetB_SEchild());}
	case B_NW:{
		return (parent->GetB_NEchild());}
	case B_NE:{
		OCNode* tmp = parent->getEastNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetB_NWchild());}
		
	default:
		return NULL;
	}
} 
void OCNode::getWestBoundary(vector<OCNode*>& nbrs){
	if(isLeaf())nbrs.push_back(this);
	else{
		F_NWchild->getWestBoundary(nbrs);
		F_SWchild->getWestBoundary(nbrs);
		
		B_NWchild->getWestBoundary(nbrs);
		B_SWchild->getWestBoundary(nbrs);
	}
}
//-------- DUE FRONT (from EAST) --------------------
// comment is the similar for the SOUTH part above
void OCNode::getAllFrontNeighbors(vector<OCNode*>& nbrs){
	OCNode* tmpNode = getFrontNeighborNode();
	if(tmpNode != NULL)
		tmpNode->getBackBoundary(nbrs);
}
OCNode* OCNode::getFrontNeighborNode(){//this is for the child to call
	if(parent == NULL) return NULL;
	switch(type){
	case F_SE:{
		OCNode* tmp = parent->getFrontNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetB_SEchild());}	     
	case F_SW:{
		OCNode* tmp = parent->getFrontNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetB_SWchild());}	     
	case F_NW:{
		OCNode* tmp = parent->getFrontNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetB_NWchild());}	     
	case F_NE:{
		OCNode* tmp = parent->getFrontNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetB_NEchild());}	     
		
	case B_SE:{
		return (parent->GetF_SEchild());}
	case B_SW:{
		return (parent->GetF_SWchild());}
	case B_NW:{
		return (parent->GetF_NWchild());}
	case B_NE:{
		return (parent->GetF_NEchild());}
	default:
		return NULL;
	}
} 
void OCNode::getBackBoundary(vector<OCNode*>& nbrs){
	if(isLeaf())nbrs.push_back(this);
	else{
		B_NWchild->getBackBoundary(nbrs);
		B_SWchild->getBackBoundary(nbrs);
		
		B_NEchild->getBackBoundary(nbrs);
		B_SEchild->getBackBoundary(nbrs);
	}
}

//-------- DUE BACK --------------------
// comment is the similar for the SOUTH part above
void OCNode::getAllBackNeighbors(vector<OCNode*>& nbrs){
	OCNode* tmpNode = getBackNeighborNode();
	if(tmpNode != NULL)
		tmpNode->getFrontBoundary(nbrs);
}
OCNode* OCNode::getBackNeighborNode(){//this is for the child to call
	if(parent == NULL) return NULL;
	switch(type){
	case B_SE:{
		OCNode* tmp = parent->getBackNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetF_SEchild());}	     
	case B_SW:{
		OCNode* tmp = parent->getBackNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetF_SWchild());}	     
	case B_NW:{
		OCNode* tmp = parent->getBackNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetF_NWchild());}	     
	case B_NE:{
		OCNode* tmp = parent->getBackNeighborNode();
		if(tmp == NULL) return NULL;
		if(tmp->isLeaf()) return tmp;
		else return (tmp->GetF_NEchild());}	     
		
	case F_SE:{
		return (parent->GetB_SEchild());}
	case F_SW:{
		return (parent->GetB_SWchild());}
	case F_NW:{
		return (parent->GetB_NWchild());}
	case F_NE:{
		return (parent->GetB_NEchild());}
	default:
		return NULL;
	}
} 

void OCNode::getFrontBoundary(vector<OCNode*>& nbrs){
	if(isLeaf())nbrs.push_back(this);
	else{
		F_NWchild->getFrontBoundary(nbrs);
		F_SWchild->getFrontBoundary(nbrs);
		
		F_NEchild->getFrontBoundary(nbrs);
		F_SEchild->getFrontBoundary(nbrs);
	}
}

/************************************************
 *             CORNER-LINE PARTS                *
 ************************************************/
//the line means inner line, these routine will 
//get the nodes , say, on the NW edge of the current node

void OCNode::getNWLine(vector<OCNode*>& line){
	if(isLeaf()) line.push_back(this);
	else{
		F_NWchild->getNWLine(line);
		B_NWchild->getNWLine(line);
	}
}
void OCNode::getNELine(vector<OCNode*>& line){
	if(isLeaf()) line.push_back(this);
	else{
		F_NEchild->getNELine(line);
		B_NEchild->getNELine(line);
	}
}
void OCNode::getSWLine(vector<OCNode*>& line){
	if(isLeaf()) line.push_back(this);
	else{
		F_SWchild->getSWLine(line);
		B_SWchild->getSWLine(line);
	}
}
void OCNode::getSELine(vector<OCNode*>& line){
	if(isLeaf()) line.push_back(this);
	else{
		F_SEchild->getSELine(line);
		B_SEchild->getSELine(line);
	}
}
void OCNode::getF_NLine(vector<OCNode*>& line){
	if(isLeaf()) line.push_back(this);
	else{
		F_NEchild->getF_NLine(line);
		F_NWchild->getF_NLine(line);
	}
}
void OCNode::getF_SLine(vector<OCNode*>& line){
	if(isLeaf()) line.push_back(this);
	else{
		F_SEchild->getF_SLine(line);
		F_SWchild->getF_SLine(line);
	}
}
void OCNode::getF_ELine(vector<OCNode*>& line){
	if(isLeaf()) line.push_back(this);
	else{
		F_NEchild->getF_ELine(line);
		F_SEchild->getF_ELine(line);
	}
}
void OCNode::getF_WLine(vector<OCNode*>& line){
	if(isLeaf()) line.push_back(this);
	else{
		F_NWchild->getF_WLine(line);
		F_SWchild->getF_WLine(line);
	}
}

void OCNode::getB_NLine(vector<OCNode*>& line){
	if(isLeaf()) line.push_back(this);
	else{
		B_NEchild->getB_NLine(line);
		B_NWchild->getB_NLine(line);
	}
}
void OCNode::getB_SLine(vector<OCNode*>& line){
	if(isLeaf()) line.push_back(this);
	else{
		B_SEchild->getB_SLine(line);
		B_SWchild->getB_SLine(line);
	}
}
void OCNode::getB_ELine(vector<OCNode*>& line){
	if(isLeaf()) line.push_back(this);
	else{
		B_NEchild->getB_ELine(line);
		B_SEchild->getB_ELine(line);
	}
}
void OCNode::getB_WLine(vector<OCNode*>& line){
	if(isLeaf()) line.push_back(this);
	else{
		B_NWchild->getB_WLine(line);
		B_SWchild->getB_WLine(line);
	}
}

void OCNode::getNWNeighbors(vector<OCNode*>& nbrs){
	//get the west and the north node (equivalent or greater)
	OCNode* WestNode = getWestNeighborNode(); if(WestNode == NULL) return;
	OCNode* NorthNode = getNorthNeighborNode();if(NorthNode == NULL) return;

	//get the north of the west
	OCNode* NorthOfWestNode = WestNode->getNorthNeighborNode(); if(NorthOfWestNode == NULL) return;
	//get the west of thre north
	OCNode* WestOfNorthNode = NorthNode->getWestNeighborNode(); if(WestOfNorthNode == NULL) return;
	
	//get the south east edges of both nodes
	vector<OCNode*> SELine1;
	NorthOfWestNode->getSELine(SELine1); if(SELine1.size() == 0) return;
	vector<OCNode*> SELine2;
	WestOfNorthNode->getSELine(SELine2); if(SELine2.size() == 0) return;
	
	for(int i = 0; i < SELine1.size(); i++){
		for(int j =0; j < SELine2.size(); j++){
			if(SELine1[i] == SELine2[j]) nbrs.push_back(SELine1[i]);
		}
	}
}
void OCNode::getNENeighbors(vector<OCNode*>& nbrs){
	//get the East and the north node (equivalent or greater)
	OCNode* EastNode = getEastNeighborNode(); if(EastNode == NULL) return;
	OCNode* NorthNode = getNorthNeighborNode();if(NorthNode == NULL) return;

	//get the north of the East
	OCNode* NorthOfEastNode = EastNode->getNorthNeighborNode(); if(NorthOfEastNode == NULL) return;
	//get the East of thre north
	OCNode* EastOfNorthNode = NorthNode->getEastNeighborNode(); if(EastOfNorthNode == NULL) return;
	
	//get the south east edges of both nodes
	vector<OCNode*> SWLine1;
	NorthOfEastNode->getSWLine(SWLine1); if(SWLine1.size() == 0) return;
	vector<OCNode*> SWLine2;
	EastOfNorthNode->getSWLine(SWLine2); if(SWLine2.size() == 0) return;
	
	for(int i = 0; i < SWLine1.size(); i++){
		for(int j =0; j < SWLine2.size(); j++){
			if(SWLine1[i] == SWLine2[j]) nbrs.push_back(SWLine1[i]);
		}
	}

}
void OCNode::getSWNeighbors(vector<OCNode*>& nbrs){
	//get the west and the South node (equivalent or greater)
	OCNode* WestNode = getWestNeighborNode(); if(WestNode == NULL) return;
	OCNode* SouthNode = getSouthNeighborNode();if(SouthNode == NULL) return;

	//get the South of the west
	OCNode* SouthOfWestNode = WestNode->getSouthNeighborNode(); if(SouthOfWestNode == NULL) return;
	//get the west of the South
	OCNode* WestOfSouthNode = SouthNode->getWestNeighborNode(); if(WestOfSouthNode == NULL) return;
	
	//get the south east edges of both nodes
	vector<OCNode*> NELine1;
	SouthOfWestNode->getNELine(NELine1); if(NELine1.size() == 0) return;
	vector<OCNode*> NELine2;
	WestOfSouthNode->getNELine(NELine2); if(NELine2.size() == 0) return;
	
	for(int i = 0; i < NELine1.size(); i++){
		for(int j =0; j < NELine2.size(); j++){
			if(NELine1[i] == NELine2[j]) nbrs.push_back(NELine1[i]);
		}
	}
}
void OCNode::getSENeighbors(vector<OCNode*>& nbrs){
	//get the East and the South node (equivalent or greater)
	OCNode* EastNode = getEastNeighborNode(); if(EastNode == NULL) return;
	OCNode* SouthNode = getSouthNeighborNode();if(SouthNode == NULL) return;

	//get the South of the East
	OCNode* SouthOfEastNode = EastNode->getSouthNeighborNode(); if(SouthOfEastNode == NULL) return;
	//get the East of thre South
	OCNode* EastOfSouthNode = SouthNode->getEastNeighborNode(); if(EastOfSouthNode == NULL) return;
	
	//get the south east edges of both nodes
	vector<OCNode*> NWLine1;
	SouthOfEastNode->getNWLine(NWLine1); if(NWLine1.size() == 0) return;
	vector<OCNode*> NWLine2;
	EastOfSouthNode->getNWLine(NWLine2); if(NWLine2.size() == 0) return;
	
	for(int i = 0; i < NWLine1.size(); i++){
		for(int j =0; j < NWLine2.size(); j++){
			if(NWLine1[i] == NWLine2[j]) nbrs.push_back(NWLine1[i]);
		}
	}
}
void OCNode::getF_NNeighbors(vector<OCNode*>& nbrs){
	//get the Front and the North node (equivalent or greater)
	OCNode* FrontNode = getFrontNeighborNode(); if(FrontNode == NULL) return;
	OCNode* NorthNode = getNorthNeighborNode();if(NorthNode == NULL) return;

	//get the Front of the North
	OCNode* FrontOfNorthNode = NorthNode->getFrontNeighborNode(); if(FrontOfNorthNode == NULL) return;
	//get the North of the Front
	OCNode* NorthOfFrontNode = FrontNode->getNorthNeighborNode(); if(NorthOfFrontNode == NULL) return;
	
	//get the south east edges of both nodes
	vector<OCNode*> B_SLine1;
	FrontOfNorthNode->getB_SLine(B_SLine1); if(B_SLine1.size() == 0) return;
	vector<OCNode*> B_SLine2;
	NorthOfFrontNode->getB_SLine(B_SLine2); if(B_SLine2.size() == 0) return;
	
	for(int i = 0; i < B_SLine1.size(); i++){
		for(int j =0; j < B_SLine2.size(); j++){
			if(B_SLine1[i] == B_SLine2[j]) nbrs.push_back(B_SLine1[i]);
		}
	}
}
void OCNode::getF_SNeighbors(vector<OCNode*>& nbrs){
	//get the Front and the South node (equivalent or greater)
	OCNode* FrontNode = getFrontNeighborNode(); if(FrontNode == NULL) return;
	OCNode* SouthNode = getSouthNeighborNode();if(SouthNode == NULL) return;

	//get the Front of the South
	OCNode* FrontOfSouthNode = SouthNode->getFrontNeighborNode(); if(FrontOfSouthNode == NULL) return;
	//get the South of the Front
	OCNode* SouthOfFrontNode = FrontNode->getSouthNeighborNode(); if(SouthOfFrontNode == NULL) return;
	
	//get the south east edges of both nodes
	vector<OCNode*> B_NLine1;
	FrontOfSouthNode->getB_NLine(B_NLine1); if(B_NLine1.size() == 0) return;
	vector<OCNode*> B_NLine2;
	SouthOfFrontNode->getB_NLine(B_NLine2); if(B_NLine2.size() == 0) return;
	
	for(int i = 0; i < B_NLine1.size(); i++){
		for(int j =0; j < B_NLine2.size(); j++){
			if(B_NLine1[i] == B_NLine2[j]) nbrs.push_back(B_NLine1[i]);
		}
	}
}
void OCNode::getF_ENeighbors(vector<OCNode*>& nbrs){
	//get the Front and the East node (equivalent or greater)
	OCNode* FrontNode = getFrontNeighborNode(); if(FrontNode == NULL) return;
	OCNode* EastNode = getEastNeighborNode();if(EastNode == NULL) return;

	//get the Front of the East
	OCNode* FrontOfEastNode = EastNode->getFrontNeighborNode(); if(FrontOfEastNode == NULL) return;
	//get the East of the Front
	OCNode* EastOfFrontNode = FrontNode->getEastNeighborNode(); if(EastOfFrontNode == NULL) return;
	
	//get the East east edges of both nodes
	vector<OCNode*> B_WLine1;
	FrontOfEastNode->getB_WLine(B_WLine1); if(B_WLine1.size() == 0) return;
	vector<OCNode*> B_WLine2;
	EastOfFrontNode->getB_WLine(B_WLine2); if(B_WLine2.size() == 0) return;
	
	for(int i = 0; i < B_WLine1.size(); i++){
		for(int j =0; j < B_WLine2.size(); j++){
			if(B_WLine1[i] == B_WLine2[j]) nbrs.push_back(B_WLine1[i]);
		}
	}

}
void OCNode::getF_WNeighbors(vector<OCNode*>& nbrs){
	//get the Front and the West node (equivalent or greater)
	OCNode* FrontNode = getFrontNeighborNode(); if(FrontNode == NULL) return;
	OCNode* WestNode = getWestNeighborNode();if(WestNode == NULL) return;

	//get the Front of the West
	OCNode* FrontOfWestNode = WestNode->getFrontNeighborNode(); if(FrontOfWestNode == NULL) return;
	//get the West of the Front
	OCNode* WestOfFrontNode = FrontNode->getWestNeighborNode(); if(WestOfFrontNode == NULL) return;
	
	//get the West West edges of both nodes
	vector<OCNode*> B_ELine1;
	FrontOfWestNode->getB_ELine(B_ELine1); if(B_ELine1.size() == 0) return;
	vector<OCNode*> B_ELine2;
	WestOfFrontNode->getB_ELine(B_ELine2); if(B_ELine2.size() == 0) return;
	
	for(int i = 0; i < B_ELine1.size(); i++){
		for(int j =0; j < B_ELine2.size(); j++){
			if(B_ELine1[i] == B_ELine2[j]) nbrs.push_back(B_ELine1[i]);
		}
	}

}
void OCNode::getB_NNeighbors(vector<OCNode*>& nbrs){
	//get the Back and the North node (equivalent or greater)
	OCNode* BackNode = getBackNeighborNode(); if(BackNode == NULL) return;
	OCNode* NorthNode = getNorthNeighborNode();if(NorthNode == NULL) return;

	//get the Back of the North
	OCNode* BackOfNorthNode = NorthNode->getBackNeighborNode(); if(BackOfNorthNode == NULL) return;
	//get the North of the Back
	OCNode* NorthOfBackNode = BackNode->getNorthNeighborNode(); if(NorthOfBackNode == NULL) return;
	
	//get the south east edges of both nodes
	vector<OCNode*> F_SLine1;
	BackOfNorthNode->getF_SLine(F_SLine1); if(F_SLine1.size() == 0) return;
	vector<OCNode*> F_SLine2;
	NorthOfBackNode->getF_SLine(F_SLine2); if(F_SLine2.size() == 0) return;
	
	for(int i = 0; i < F_SLine1.size(); i++){
		for(int j =0; j < F_SLine2.size(); j++){
			if(F_SLine1[i] == F_SLine2[j]) nbrs.push_back(F_SLine1[i]);
		}
	}
}
void OCNode::getB_SNeighbors(vector<OCNode*>& nbrs){
	//get the Back and the South node (equivalent or greater)
	OCNode* BackNode = getBackNeighborNode(); if(BackNode == NULL) return;
	OCNode* SouthNode = getSouthNeighborNode();if(SouthNode == NULL) return;

	//get the Back of the South
	OCNode* BackOfSouthNode = SouthNode->getBackNeighborNode(); if(BackOfSouthNode == NULL) return;
	//get the South of the Back
	OCNode* SouthOfBackNode = BackNode->getSouthNeighborNode(); if(SouthOfBackNode == NULL) return;
	
	//get the south east edges of both nodes
	vector<OCNode*> F_NLine1;
	BackOfSouthNode->getF_NLine(F_NLine1); if(F_NLine1.size() == 0) return;
	vector<OCNode*> F_NLine2;
	SouthOfBackNode->getF_NLine(F_NLine2); if(F_NLine2.size() == 0) return;
	
	for(int i = 0; i < F_NLine1.size(); i++){
		for(int j =0; j < F_NLine2.size(); j++){
			if(F_NLine1[i] == F_NLine2[j]) nbrs.push_back(F_NLine1[i]);
		}
	}
}
void OCNode::getB_ENeighbors(vector<OCNode*>& nbrs){
	//get the Back and the East node (equivalent or greater)
	OCNode* BackNode = getBackNeighborNode(); if(BackNode == NULL) return;
	OCNode* EastNode = getEastNeighborNode();if(EastNode == NULL) return;

	//get the Back of the East
	OCNode* BackOfEastNode = EastNode->getBackNeighborNode(); if(BackOfEastNode == NULL) return;
	//get the East of the Back
	OCNode* EastOfBackNode = BackNode->getEastNeighborNode(); if(EastOfBackNode == NULL) return;
	
	//get the East east edges of both nodes
	vector<OCNode*> F_WLine1;
	BackOfEastNode->getF_WLine(F_WLine1); if(F_WLine1.size() == 0) return;
	vector<OCNode*> F_WLine2;
	EastOfBackNode->getF_WLine(F_WLine2); if(F_WLine2.size() == 0) return;
	
	for(int i = 0; i < F_WLine1.size(); i++){
		for(int j =0; j < F_WLine2.size(); j++){
			if(F_WLine1[i] == F_WLine2[j]) nbrs.push_back(F_WLine1[i]);
		}
	}
}
void OCNode::getB_WNeighbors(vector<OCNode*>& nbrs){
	//get the Back and the West node (equivalent or greater)
	OCNode* BackNode = getBackNeighborNode(); if(BackNode == NULL) return;
	OCNode* WestNode = getWestNeighborNode();if(WestNode == NULL) return;

	//get the Back of the West
	OCNode* BackOfWestNode = WestNode->getBackNeighborNode(); if(BackOfWestNode == NULL) return;
	//get the West of the Back
	OCNode* WestOfBackNode = BackNode->getWestNeighborNode(); if(WestOfBackNode == NULL) return;
	
	//get the West east edges of both nodes
	vector<OCNode*> F_ELine1;
	BackOfWestNode->getF_ELine(F_ELine1); if(F_ELine1.size() == 0) return;
	vector<OCNode*> F_ELine2;
	WestOfBackNode->getF_ELine(F_ELine2); if(F_ELine2.size() == 0) return;
	
	for(int i = 0; i < F_ELine1.size(); i++){
		for(int j =0; j < F_ELine2.size(); j++){
			if(F_ELine1[i] == F_ELine2[j]) nbrs.push_back(F_ELine1[i]);
		}
	}

}

/************************************************
 *             TRUE CORNER PARTS                *
 ************************************************/
//this part is to get the 8 boxes each of them 
//share only one vertex with the current node

OCNode* OCNode::getF_NWCorner(){//return the node on the Front NW corner
	//get the most Front NW child of the current node - corner means the corner inside current node
	if(isLeaf()) return this;
    else return F_NWchild->getF_NWCorner();
}
OCNode* OCNode::getF_NECorner(){
	//get the most NE child of the current node
    if(isLeaf()) return this;
    else return F_NEchild->getF_NECorner();
}
OCNode* OCNode::getF_SWCorner(){
	//get the most SW child of the current node
	if(isLeaf()) return this;
	else return F_SWchild->getF_SWCorner();
}
OCNode* OCNode::getF_SECorner(){
	//get the most SE child of the current node
	if(isLeaf()) return this;
	else return F_SEchild->getF_SECorner();
}


OCNode* OCNode::getB_NWCorner(){//return the node on the Front NW corner
	//get the most Front NW child of the current node - corner means the corner inside current node
	if(isLeaf()) return this;
    else return B_NWchild->getB_NWCorner();
}
OCNode* OCNode::getB_NECorner(){
	//get the most NE child of the current node
    if(isLeaf()) return this;
    else return B_NEchild->getB_NECorner();
}
OCNode* OCNode::getB_SWCorner(){
	//get the most SW child of the current node
	if(isLeaf()) return this;
	else return B_SWchild->getB_SWCorner();
}
OCNode* OCNode::getB_SECorner(){
	//get the most SE child of the current node
	if(isLeaf()) return this;
	else return B_SEchild->getB_SECorner();
}

//there are two possibilities
//------ F_NE Corner------------
OCNode* OCNode::F_NENeighbor(){
	OCNode* NorthNode = getNorthNeighborNode(); if(NorthNode == NULL) return NULL;
	OCNode* EastNode = getEastNeighborNode(); if(EastNode == NULL) return NULL;
	OCNode* FrontNode = getFrontNeighborNode(); if(FrontNode == NULL) return NULL;
	//get the north of front, east of north, and front of east
	OCNode* NorthOfFront = FrontNode->getNorthNeighborNode(); if(NorthOfFront == NULL) return NULL;
	OCNode* EastOfNorth = NorthNode->getEastNeighborNode(); if(EastOfNorth == NULL) return NULL;
	OCNode* FrontOfEast = EastNode->getFrontNeighborNode(); if(FrontOfEast == NULL) return NULL;
	//get the corner box
	OCNode* EastOfNorthOfFront = NorthOfFront->getEastNeighborNode(); if(EastOfNorthOfFront == NULL) return NULL;
	OCNode* FrontOfEastOfNorth = EastOfNorth->getFrontNeighborNode(); if(FrontOfEastOfNorth == NULL) return NULL;
	OCNode* NorthOfFrontOfEast = FrontOfEast->getNorthNeighborNode(); if(NorthOfFrontOfEast == NULL) return NULL;
	//get the corner of the corner
	OCNode* Cor1 = EastOfNorthOfFront->getB_SWCorner();if(Cor1 == NULL) return NULL;
	OCNode* Cor2 = FrontOfEastOfNorth->getB_SWCorner();if(Cor2 == NULL) return NULL;
	OCNode* Cor3 = NorthOfFrontOfEast->getB_SWCorner();if(Cor3 == NULL) return NULL;
	if((Cor1 == Cor2) && (Cor2 == Cor3)) return Cor1;
	else return NULL;
}

//------ F_NW Corner------------
OCNode* OCNode::F_NWNeighbor(){
	OCNode* NorthNode = getNorthNeighborNode(); if(NorthNode == NULL) return NULL;
	OCNode* WestNode = getWestNeighborNode(); if(WestNode == NULL) return NULL;
	OCNode* FrontNode = getFrontNeighborNode(); if(FrontNode == NULL) return NULL;
	//get the north of front, West of north, and front of West
	OCNode* NorthOfFront = FrontNode->getNorthNeighborNode(); if(NorthOfFront == NULL) return NULL;
	OCNode* WestOfNorth = NorthNode->getWestNeighborNode(); if(WestOfNorth == NULL) return NULL;
	OCNode* FrontOfWest = WestNode->getFrontNeighborNode(); if(FrontOfWest == NULL) return NULL;
	//get the corner box
	OCNode* WestOfNorthOfFront = NorthOfFront->getWestNeighborNode(); if(WestOfNorthOfFront == NULL) return NULL;
	OCNode* FrontOfWestOfNorth = WestOfNorth->getFrontNeighborNode(); if(FrontOfWestOfNorth == NULL) return NULL;
	OCNode* NorthOfFrontOfWest = FrontOfWest->getNorthNeighborNode(); if(NorthOfFrontOfWest == NULL) return NULL;
	//get the corner of the corner
	OCNode* Cor1 = WestOfNorthOfFront->getB_SECorner();if(Cor1 == NULL) return NULL;
	OCNode* Cor2 = FrontOfWestOfNorth->getB_SECorner();if(Cor2 == NULL) return NULL;
	OCNode* Cor3 = NorthOfFrontOfWest->getB_SECorner();if(Cor3 == NULL) return NULL;
	if((Cor1 == Cor2) && (Cor2 == Cor3)) return Cor1;
	else return NULL;
}

//------ F_SW Corner------------
OCNode* OCNode::F_SWNeighbor(){
	OCNode* SouthNode = getSouthNeighborNode(); if(SouthNode == NULL) return NULL;
	OCNode* WestNode = getWestNeighborNode(); if(WestNode == NULL) return NULL;
	OCNode* FrontNode = getFrontNeighborNode(); if(FrontNode == NULL) return NULL;
	//get the South of front, West of South, and front of West
	OCNode* SouthOfFront = FrontNode->getSouthNeighborNode(); if(SouthOfFront == NULL) return NULL;
	OCNode* WestOfSouth = SouthNode->getWestNeighborNode(); if(WestOfSouth == NULL) return NULL;
	OCNode* FrontOfWest = WestNode->getFrontNeighborNode(); if(FrontOfWest == NULL) return NULL;
	//get the corner box
	OCNode* WestOfSouthOfFront = SouthOfFront->getWestNeighborNode(); if(WestOfSouthOfFront == NULL) return NULL;
	OCNode* FrontOfWestOfSouth = WestOfSouth->getFrontNeighborNode(); if(FrontOfWestOfSouth == NULL) return NULL;
	OCNode* SouthOfFrontOfWest = FrontOfWest->getSouthNeighborNode(); if(SouthOfFrontOfWest == NULL) return NULL;
	//get the corner of the corner
	OCNode* Cor1 = WestOfSouthOfFront->getB_NECorner();if(Cor1 == NULL) return NULL;
	OCNode* Cor2 = FrontOfWestOfSouth->getB_NECorner();if(Cor2 == NULL) return NULL;
	OCNode* Cor3 = SouthOfFrontOfWest->getB_NECorner();if(Cor3 == NULL) return NULL;
	if((Cor1 == Cor2) && (Cor2 == Cor3)) return Cor1;
	else return NULL;
}

//------ F_SE Corner------------
OCNode* OCNode::F_SENeighbor(){
	OCNode* SouthNode = getSouthNeighborNode(); if(SouthNode == NULL) return NULL;
	OCNode* EastNode = getEastNeighborNode(); if(EastNode == NULL) return NULL;
	OCNode* FrontNode = getFrontNeighborNode(); if(FrontNode == NULL) return NULL;
	//get the South of front, East of South, and front of East
	OCNode* SouthOfFront = FrontNode->getSouthNeighborNode(); if(SouthOfFront == NULL) return NULL;
	OCNode* EastOfSouth = SouthNode->getEastNeighborNode(); if(EastOfSouth == NULL) return NULL;
	OCNode* FrontOfEast = EastNode->getFrontNeighborNode(); if(FrontOfEast == NULL) return NULL;
	//get the corner box
	OCNode* EastOfSouthOfFront = SouthOfFront->getEastNeighborNode(); if(EastOfSouthOfFront == NULL) return NULL;
	OCNode* FrontOfEastOfSouth = EastOfSouth->getFrontNeighborNode(); if(FrontOfEastOfSouth == NULL) return NULL;
	OCNode* SouthOfFrontOfEast = FrontOfEast->getSouthNeighborNode(); if(SouthOfFrontOfEast == NULL) return NULL;
	//get the corner of the corner
	OCNode* Cor1 = EastOfSouthOfFront->getB_NWCorner();if(Cor1 == NULL) return NULL;
	OCNode* Cor2 = FrontOfEastOfSouth->getB_NWCorner();if(Cor2 == NULL) return NULL;
	OCNode* Cor3 = SouthOfFrontOfEast->getB_NWCorner();if(Cor3 == NULL) return NULL;
	if((Cor1 == Cor2) && (Cor2 == Cor3)) return Cor1;
	else return NULL;
}

/****** BACK **********/
//------ B_NE Corner------------
OCNode* OCNode::B_NENeighbor(){
	OCNode* NorthNode = getNorthNeighborNode(); if(NorthNode == NULL) return NULL;
	OCNode* EastNode = getEastNeighborNode(); if(EastNode == NULL) return NULL;
	OCNode* BackNode = getBackNeighborNode(); if(BackNode == NULL) return NULL;
	//get the north of Back, east of north, and Back of east
	OCNode* NorthOfBack = BackNode->getNorthNeighborNode(); if(NorthOfBack == NULL) return NULL;
	OCNode* EastOfNorth = NorthNode->getEastNeighborNode(); if(EastOfNorth == NULL) return NULL;
	OCNode* BackOfEast = EastNode->getBackNeighborNode(); if(BackOfEast == NULL) return NULL;
	//get the corner box
	OCNode* EastOfNorthOfBack = NorthOfBack->getEastNeighborNode(); if(EastOfNorthOfBack == NULL) return NULL;
	OCNode* BackOfEastOfNorth = EastOfNorth->getBackNeighborNode(); if(BackOfEastOfNorth == NULL) return NULL;
	OCNode* NorthOfBackOfEast = BackOfEast->getNorthNeighborNode(); if(NorthOfBackOfEast == NULL) return NULL;
	//get the corner of the corner
	OCNode* Cor1 = EastOfNorthOfBack->getF_SWCorner();if(Cor1 == NULL) return NULL;
	OCNode* Cor2 = BackOfEastOfNorth->getF_SWCorner();if(Cor2 == NULL) return NULL;
	OCNode* Cor3 = NorthOfBackOfEast->getF_SWCorner();if(Cor3 == NULL) return NULL;
	if((Cor1 == Cor2) && (Cor2 == Cor3)) return Cor1;
	else return NULL;
}

//------ B_NW Corner------------
OCNode* OCNode::B_NWNeighbor(){
	OCNode* NorthNode = getNorthNeighborNode(); if(NorthNode == NULL) return NULL;
	OCNode* WestNode = getWestNeighborNode(); if(WestNode == NULL) return NULL;
	OCNode* BackNode = getBackNeighborNode(); if(BackNode == NULL) return NULL;
	//get the north of Back, West of north, and Back of West
	OCNode* NorthOfBack = BackNode->getNorthNeighborNode(); if(NorthOfBack == NULL) return NULL;
	OCNode* WestOfNorth = NorthNode->getWestNeighborNode(); if(WestOfNorth == NULL) return NULL;
	OCNode* BackOfWest = WestNode->getBackNeighborNode(); if(BackOfWest == NULL) return NULL;
	//get the corner box
	OCNode* WestOfNorthOfBack = NorthOfBack->getWestNeighborNode(); if(WestOfNorthOfBack == NULL) return NULL;
	OCNode* BackOfWestOfNorth = WestOfNorth->getBackNeighborNode(); if(BackOfWestOfNorth == NULL) return NULL;
	OCNode* NorthOfBackOfWest = BackOfWest->getNorthNeighborNode(); if(NorthOfBackOfWest == NULL) return NULL;
	//get the corner of the corner
	OCNode* Cor1 = WestOfNorthOfBack->getF_SECorner();if(Cor1 == NULL) return NULL;
	OCNode* Cor2 = BackOfWestOfNorth->getF_SECorner();if(Cor2 == NULL) return NULL;
	OCNode* Cor3 = NorthOfBackOfWest->getF_SECorner();if(Cor3 == NULL) return NULL;
	if((Cor1 == Cor2) && (Cor2 == Cor3)) return Cor1;
	else return NULL;
}

//------ B_SW Corner------------
OCNode* OCNode::B_SWNeighbor(){
	OCNode* SouthNode = getSouthNeighborNode(); if(SouthNode == NULL) return NULL;
	OCNode* WestNode = getWestNeighborNode(); if(WestNode == NULL) return NULL;
	OCNode* BackNode = getBackNeighborNode(); if(BackNode == NULL) return NULL;
	//get the South of Back, West of South, and Back of West
	OCNode* SouthOfBack = BackNode->getSouthNeighborNode(); if(SouthOfBack == NULL) return NULL;
	OCNode* WestOfSouth = SouthNode->getWestNeighborNode(); if(WestOfSouth == NULL) return NULL;
	OCNode* BackOfWest = WestNode->getBackNeighborNode(); if(BackOfWest == NULL) return NULL;
	//get the corner box
	OCNode* WestOfSouthOfBack = SouthOfBack->getWestNeighborNode(); if(WestOfSouthOfBack == NULL) return NULL;
	OCNode* BackOfWestOfSouth = WestOfSouth->getBackNeighborNode(); if(BackOfWestOfSouth == NULL) return NULL;
	OCNode* SouthOfBackOfWest = BackOfWest->getSouthNeighborNode(); if(SouthOfBackOfWest == NULL) return NULL;
	//get the corner of the corner
	OCNode* Cor1 = WestOfSouthOfBack->getF_NECorner();if(Cor1 == NULL) return NULL;
	OCNode* Cor2 = BackOfWestOfSouth->getF_NECorner();if(Cor2 == NULL) return NULL;
	OCNode* Cor3 = SouthOfBackOfWest->getF_NECorner();if(Cor3 == NULL) return NULL;
	if((Cor1 == Cor2) && (Cor2 == Cor3)) return Cor1;
	else return NULL;
}

//------ B_SE Corner------------
OCNode* OCNode::B_SENeighbor(){
	OCNode* SouthNode = getSouthNeighborNode(); if(SouthNode == NULL) return NULL;
	OCNode* EastNode = getEastNeighborNode(); if(EastNode == NULL) return NULL;
	OCNode* BackNode = getBackNeighborNode(); if(BackNode == NULL) return NULL;
	//get the South of Back, East of South, and Back of East
	OCNode* SouthOfBack = BackNode->getSouthNeighborNode(); if(SouthOfBack == NULL) return NULL;
	OCNode* EastOfSouth = SouthNode->getEastNeighborNode(); if(EastOfSouth == NULL) return NULL;
	OCNode* BackOfEast = EastNode->getBackNeighborNode(); if(BackOfEast == NULL) return NULL;
	//get the corner box
	OCNode* EastOfSouthOfBack = SouthOfBack->getEastNeighborNode(); if(EastOfSouthOfBack == NULL) return NULL;
	OCNode* BackOfEastOfSouth = EastOfSouth->getBackNeighborNode(); if(BackOfEastOfSouth == NULL) return NULL;
	OCNode* SouthOfBackOfEast = BackOfEast->getSouthNeighborNode(); if(SouthOfBackOfEast == NULL) return NULL;
	//get the corner of the corner
	OCNode* Cor1 = EastOfSouthOfBack->getF_NWCorner();if(Cor1 == NULL) return NULL;
	OCNode* Cor2 = BackOfEastOfSouth->getF_NWCorner();if(Cor2 == NULL) return NULL;
	OCNode* Cor3 = SouthOfBackOfEast->getF_NWCorner();if(Cor3 == NULL) return NULL;
	if((Cor1 == Cor2) && (Cor2 == Cor3)) return Cor1;
	else return NULL;
}

/**************************************************
 *           THE TOP FUNCTION in Neighbor finding *
 **************************************************/
void OCNode::getNeighbors(vector<OCNode*>& nbrs1, bool non_empty){
	/*
	PURPOSES: get all the neighbor of this node;
	RECEIVES: 
	RETURNS:  the list of neighbor store in the vector nbrs (vector of pointers)	
	REMARKS:
	USING THIS FUNCTION to get all the neighbor of 
	the current node, including S, W, N , E neighbor 
	as well as NW, SE, SW,... (exterior corners)
	*/
	// get the neighbors on the four due directions
	vector<OCNode*> nbrs;
	getAllSouthNeighbors(nbrs);
	getAllNorthNeighbors(nbrs);
	getAllWestNeighbors(nbrs);
	getAllEastNeighbors(nbrs);
	getAllFrontNeighbors(nbrs);
	getAllBackNeighbors(nbrs);
	//get the nbr on the edges
	getNWNeighbors(nbrs);
	getNENeighbors(nbrs);
	getSWNeighbors(nbrs);
	getSENeighbors(nbrs);
	getF_NNeighbors(nbrs);
	getF_SNeighbors(nbrs);
	getF_ENeighbors(nbrs);
	getF_WNeighbors(nbrs);
	getB_NNeighbors(nbrs);
	getB_SNeighbors(nbrs);
	getB_ENeighbors(nbrs);
	getB_WNeighbors(nbrs);
	
	//get the neighbor on the corner
	OCNode* F_NE = F_NENeighbor();//------ NE Corner------------
	if(F_NE != NULL) nbrs.push_back(F_NE);
	OCNode* F_NW = F_NWNeighbor();
	if(F_NW != NULL) nbrs.push_back(F_NW);
	OCNode* F_SW = F_SWNeighbor();
	if(F_SW != NULL) nbrs.push_back(F_SW);
	OCNode* F_SE = F_SENeighbor();
	if(F_SE != NULL) nbrs.push_back(F_SE);
	OCNode* B_NE = B_NENeighbor();
	if(B_NE != NULL) nbrs.push_back(B_NE);
	OCNode* B_NW = B_NWNeighbor();
	if(B_NW != NULL) nbrs.push_back(B_NW);
	OCNode* B_SW = B_SWNeighbor();
	if(B_SW != NULL) nbrs.push_back(B_SW);
	OCNode* B_SE = B_SENeighbor();
	if(B_SE != NULL) nbrs.push_back(B_SE);
	//*
	//delete empty neighbors
	vector<OCNode*>::iterator it = nbrs.begin();
	while(it != nbrs.end()){
		OCNode* tmp = (OCNode* )*it;

		//------------------------------------------------------------------------------------
		//	revised by ydm 2003-11-4
		//if((tmp->ptrX != -1)||(tmp->ptrY != -1)||(tmp->ptrZ != -1)) nbrs1.push_back(tmp);
		if (!non_empty)
		{	
			if (tmp->isEmpty())
				nbrs1.push_back(tmp);
		}
		else if (!tmp->isEmpty())
			nbrs1.push_back(tmp);
		//------------------------------------------------------------------------------------
		it++;
	}
	//*/
}

//-------------------------------------------------------
//AnnSearch tree fuctions, for finding the nearest surfel
//-------------------------------------------------------
/*
void Node::InitANNSearchTree(SurfelScene * pScene)
{
	for(int i=0; i<_isurfels.size(); i++)
	{
		Surfel* s= &(pScene->GetSurfels()[_isurfels[i]]);
		_Surfels.push_back(*s);
	}
	m_Search = new C_ANNSearch(_Surfels);
}

Surfel* Node::FindNearestSurfel(Surfel s, SurfelScene * pScene)
{
	int index = m_Search->FindNearestNeighbor(s);
	Surfel * nearest = &(pScene->GetSurfels()[_isurfels[index]]);
	return nearest;
}
*/
//void Node::FindNearestSurfel2(Surfel s, vector<int>& neighbor)
//{
//	neighbor.resize(0);
//	m_Search->FindNearestNeighbor2(s, neighbor);
//
////	for(int i=0; i<iv.size(); i++)
////		neighbor.push_back(pScene->GetSurfel(i));
//}

/*
void Node::FindNearestSurfelArray(Surfel s, SurfelScene * pScene, vector<Surfel*>& neighbor_surfels, float &maxdist, int num)
{
	vector<int> neighbor_index;
	neighbor_index.resize(0);
	m_Search->FindNearestNeighborArray(s, neighbor_index, maxdist, num);

	for(int i=0; i<neighbor_index.size(); i++)	
	{
		Surfel *s = &(pScene->GetSurfels()[_isurfels[neighbor_index[i]]]);
		neighbor_surfels.push_back(s);
	}
//	for(int i=0; i<iv.size(); i++)
//		neighbor.push_back(pScene->GetSurfel(i));
}
*/