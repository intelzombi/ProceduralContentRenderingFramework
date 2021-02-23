//FSTree Class
//Written by Hugh Smith April 2007
// Part of FSTree Grammar Structure for creating procedural trees

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

class FSTree;
#ifndef _FSTree
#define _FSTree
#include "Sphere.h"
#include "Observer.h"
#include "aabb.h"
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include "GrammarDef.h"


class FSTree;

struct treeNode {
    SphereBase *psphere;
    treeNode *pPrevNode;
    treeNode *pNextNodes;
    int LevelID;
    int nextNodeSize;
    FSTree *tree;
};

class FSTree {
public:
	FSTree(std::string grammarName, V3 position, long seed);
	~FSTree();
    virtual Sphere* createTrunk(int level, V3 basePosition);
	void deleteNode(treeNode *ctn);
    void BoundingBox(treeNode *ctn);
	void countVertices(treeNode *ctn);
	void countSpheres(treeNode *ctn);
	void getSphere(treeNode *ctn, int sphere);
    virtual void growTree(bool justCount=false);
    void pruneTree(V3 *pVertexBuffer);
    void sprout();
    void fillBuffers(SphereBase *sphere);
    void fillSpheres(treeNode *ctn);
    void fillCanopies(treeNode *ctn);
    void SphereToList(treeNode *ctn, std::vector<SphereBase*> *treeNodeList);

    observer *theOverseer;
    Sphere *Trunk;
    aabb m_boundingBox;
    aabb *m_pBoundingboxes;
    Grammar *m_grammar;
    int m_levelCount;
    V3 m_position;
    int m_seed;
	long m_StartSphereIndexBuffer;
	long m_EndSphereIndexBuffer;
	long m_StartSphereVertexBuffer;
	long m_EndSphereVertexBuffer;
    long m_StartAttribBuffer;
    long m_EndAttribBuffer;
    treeNode *m_nodeTree; // node tree of sphere base points.  doesn't include segment info.
    bool m_OnFire;
    int m_Life;
	int m_FaceCount;
	int m_SphereCount;
	static bool counting;
    // this is a function used strictly for Release mode debugging. 
    // you can pass a variable into this function and the compiler won't optimize it out.  
    virtual void KeepVariableAlive(void * a);

protected:
    void *                                  m_KAV;

    void bounds(SphereBase *sphere);
	Grammar * createGrammar(std::string grammarName);
    FSTree(){};

};
#endif
