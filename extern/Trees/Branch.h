//Branch Class
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


class Branch;
class BranchBase;

#ifndef _Branch
#define _Branch
#include "SpeciesGrammar.h"
#include "segment.h"
#include "observer.h"
#include "canopy.h"
#include "aabb.h"

struct treeNode;
class Canopy;
class BranchBase 
{
public:
    BranchBase();
    ~BranchBase();
    
    Segment *segments; //create a list of segments for this branch
    Canopy  *canopies;
    bool isCanopy;
    int segmentCount;
	int tipPointCount;
    V3 heading; // This is overall branch direction.  different from initial segement heading.
    V3 position; // This is the root segment center point. This is the Parent branch Tip Point.
    V3 tipPoint; // This is the last segment center point.  This is the child branch position point.
    branchType type; // might be useful for extending the Grammar
    int startVertex;
    int startIndex; 
    int vertexCount;
    int indexCount; 
	int attribute;
	int levelID;
	int levelCount;
	bool m_layerTx;
    aabb AABB;


    long life;
    bool burning;

};


class Branch {
public:
    Branch(int level, V3 basePosition);
    void growBranch(Branch *pBranch, treeNode *ctreeNode, Grammar *grammar,V3 & StartHeading);
    virtual Branch *CreateNextBranch(int level, V3 basePosition);
    void calcSegmentDepth(LevelDetail * levelGrammar);
    aabb setAABB();
    V3* calcArbitraryVector(V3 &av, V3 &crossVector, V3 &axis);

    V3 m_position; // This is the root segment center point. This is the Parent branch Tip Point.
    Segment *m_pSegments; //create a list of segments for this branch
    int m_segmentCount;
    splitType m_splitType;
    observer *theOverseer;
    int m_splitCount;
    int m_nodeLevel; //level of grammar or present depth of tree growth.
//    branchNode *nodeBranch;
    float m_dropAngle; //angle from previous branch. Could actually be upward so name could be missleading
    LevelDetail *m_pSpeciesLevelGrammar; //just the facts needed for this branch. no sense dragging the whole grammar along
    aabb m_AABB;
	bool m_abstractFx;
protected:
    Branch(){};
};
#endif

    