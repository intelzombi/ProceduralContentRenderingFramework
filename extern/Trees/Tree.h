//Tree Class
//Written by Hugh Smith April 2007
// Part of Tree Grammar Structure for creating procedural trees

// Copyright © 2008 Intel Corporation
// All Rights Reserved
// 
// The sample source code contained or described herein and all documents
// related to the sample source code ("Material") are owned by Intel Corporation
// or its suppliers or licensors. Title to the Material remains with Intel Corporation
// or its suppliers and licensors. The Material may contain trade secrets and
// proprietary and confidential information of Intel Corporation and its suppliers
// and licensors, and is protected by worldwide copyright and trade secret laws and
// treaty provisions. The sample source code is provided AS IS, with no warranties
// of any kind, express or implied. Except as expressly permitted by the Software
// license, neither Intel Corporation nor its suppliers assumes any responsibility
// or liability for any errors or inaccuracies that may appear herein.

class Tree;
#ifndef _Tree
#define _Tree
#include "Branch.h"
#include "Observer.h"
#include "Canopy.h"
#include "aabb.h"
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include "GrammarDef.h"


class Tree;

struct treeNode {
    BranchBase *pbranch;
    treeNode *pPrevNode;
    treeNode *pNextNodes;
    int nextNodeSize;
    Tree *tree;
};

class Tree {
public:
	Tree(std::string grammarName, V3 position, long seed);
	~Tree();
    virtual Branch* createTrunk(int level, V3 basePosition);
	void deleteNode(treeNode *ctn);
    void BoundingBox(treeNode *ctn, bool Canopy=false);
	void countVertices(treeNode *ctn);
	void countBranches(treeNode *ctn);
	void getBranch(treeNode *ctn, int branch);
    virtual void growTree(bool justCount=false);
    virtual void growCanopy(treeNode *ctn);
    //virtual void growCanopySegment(treeNode *rootOfTree, branchBase *pCanopyBranch);
    // pruneTree will be a function to split up the tree or remove branches.  
    // It is strictly a placeholder for future development
    void pruneTree(V3 *pVertexBuffer);
    void sprout();
    void fillBuffers(Canopy *cnpy);
    void fillBuffers(BranchBase *branch);
    void fillBranches(treeNode *ctn);
    void fillCanopies(treeNode *ctn);
    void BranchToList(treeNode *ctn, std::vector<BranchBase*> *treeNodeList);
    void CanopyToList(treeNode *ctn, std::vector<Canopy*> *treeNodeCanopyList);

    observer *theOverseer;
    Branch *Trunk;
    aabb m_boundingBox;
    aabb *m_pBoundingboxes;
    Grammar * m_pgrammar;
    int m_levelCount;
    V3 m_position;
    long m_seed;
	long m_StartBranchIndexBuffer;
	long m_EndBranchIndexBuffer;
	long m_StartBranchVertexBuffer;
	long m_EndBranchVertexBuffer;
	long m_StartCanopyIndexBuffer;
	long m_EndCanopyIndexBuffer;
	long m_StartCanopyVertexBuffer;
	long m_EndCanopyVertexBuffer;
    long m_StartAttribBuffer;
    long m_EndAttribBuffer;
    treeNode *m_nodeTree; // node tree of branch base points.  doesn't include segment info.
    bool m_OnFire;
    int m_Life;
	int m_FaceCount;
	int m_BranchCount;
    int m_CanopyCount;
    int m_CanopyCountTest;
	static bool counting;
    // this is a function used strictly for Release mode debugging. 
    // you can pass a variable into this function and the compiler won't optimize it out.  
    virtual void KeepVariableAlive(void * a);

protected:
    void *                                  m_KAV;

    void bounds(BranchBase *branch);
	Grammar * createGrammar(std::string grammarName);
    void bounds(Canopy *Canopy);
    Tree(){};

};
#endif
