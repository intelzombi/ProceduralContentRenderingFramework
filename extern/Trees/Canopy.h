//Canopy Class
//Written by Hugh Smith November 2007
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

#ifndef _Canopy
#define _Canopy

#include "vector3.h"
#include <vector>
#include "aabb.h"
#include "Observer.h"
#include "SpeciesGrammar.h"
struct treeNode;
enum CanopyType {HEX_CANOPY=0x01, PATCH_CANOPY=0x10, PROCEDURAL_CANOPY=0x100, TIP_CANOPY=0x1000};

class Canopy {
public: 
    ~Canopy(){
        std::vector<V3*>::iterator it,itend;
        it = m_Canopy.begin();
        itend = m_Canopy.end();
        V3 * temp;
        for(it;it!=itend;it++){
            temp = (*it);
            delete temp;
            temp = NULL;
               
        }
        m_Canopy.clear();

    };
    Canopy(int baseWidth = 9,int baseHeight =9,float WidthStep = 1.0f, float HeightStep = 1.25f, float PerturbFactor = 0.0f)
    {
        m_width      = baseWidth ; 
        m_height     = baseHeight; 
        m_widthStep  = WidthStep ; 
        m_heightStep = HeightStep;
        m_perturbFactor = PerturbFactor;
    };
	void heading(V3 Heading);
    V3 AxisHeading; // unlike the branch heading this is the normal from the plane of the tipPointList
    B3 Basis; 
    CanopyType type; 
    std::vector<V3 *> m_Canopy;
	int   m_width     ;
	int   m_height    ;
	float m_widthStep ;
	float m_heightStep;
    float m_perturbFactor;
    int m_startVertex;
    int m_startIndex; 
    int m_vertexCount;
    int m_indexCount; 
	int m_attribute;
    aabb m_AABB;
    bool m_burning;
    observer *theOverseer;
    int m_nodeLevel; //level of grammar or present depth of tree growth.
    LevelDetail *m_pSpeciesLevelGrammar; //just the facts needed for this branch. no sense dragging the whole grammar along
    void growCanopySegment(treeNode *rootOfTree, BranchBase *pCanopyBranch,aabb TreeBoundingBox, Grammar *grammar);
    void growPatchSegment(treeNode *rootOfTree, BranchBase *pCanopyBranch,aabb TreeBoundingBox, LevelDetail *grammar, V3 startHeading);
    void growHexSegment(treeNode *rootOfTree, BranchBase *pCanopyBranch,aabb TreeBoundingBox, LevelDetail *grammar, V3 startHeading);

private:
    aabb setAABB();
};
#endif
