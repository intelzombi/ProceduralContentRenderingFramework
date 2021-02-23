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

#include "branch.h"

#pragma warning( push )
#pragma warning( disable : 6211 )

Branch::Branch(int level, V3 basePosition) {
    m_nodeLevel = level;
    m_position = basePosition;
    m_pSegments = 0;
    m_pSpeciesLevelGrammar = 0;
	m_abstractFx = 0;
}

Branch *Branch::CreateNextBranch(int level, V3 basePosition){
    return new Branch(level,basePosition);
}

//startHeading is assumed to be normalized
//This is the main parsing function for the tree branches.  This is a recursive function
//that will fill the tree node with all the branch segments in a multi level linked list.
//This is the area that is most ripe for tweaking to refine the behavior of the parsing as 
//new tokens are added to the grammar.
void Branch::growBranch(Branch *pBranch, treeNode *ctreeNode, Grammar *grammar, V3 &StartHeading)
{
    V3 startHeading = StartHeading;
    ctreeNode->tree->m_BranchCount++;
    observer *Observe = observer::Instance();
    m_pSpeciesLevelGrammar = &grammar->m_pLevels[m_nodeLevel];
	m_abstractFx = (bool) grammar->m_abstractEffect;
    calcSegmentDepth(m_pSpeciesLevelGrammar);
    m_segmentCount = m_pSpeciesLevelGrammar->segmentCount;
	int tps = m_pSpeciesLevelGrammar->tipPointCount;//tps is TipPoints per Segment
    // !!!! segments is a structure that created here but owned by the branchBase structure.
    // the destruction is handled in the destructor of the branchBase Class.
    m_pSegments = new Segment[m_segmentCount]; // remember segment is just a one ring of points and a heading vector.
    float tipsize;
	float taper;	
	taper = m_pSpeciesLevelGrammar->taper;
    if (ctreeNode->pPrevNode == 0){
        tipsize = m_pSpeciesLevelGrammar->diameter;// converting from Diameter to Radius... Must change semantics sometime
         //= m_pSegments->m_tip;
    }else{
        tipsize = ctreeNode->pPrevNode->pbranch->segments[ctreeNode->pPrevNode->pbranch->segmentCount-1].m_tip;
        if(m_pSpeciesLevelGrammar->diameter <= ctreeNode->pPrevNode->pbranch->segments[ctreeNode->pPrevNode->pbranch->segmentCount-1].m_tip){
        
            //tipsize = ctreeNode->pPrevNode->pbranch->segments[ctreeNode->pPrevNode->pbranch->segmentCount-1].m_tip;
            // alter taper to accommodate steeper drop in diameter
            // taper * diameter --> desired end diameter
            // taper * prevTip ->> effective end diameter
            // 1 - (desired diameter/prevTip) 
            taper = 1.0f - ((taper * m_pSpeciesLevelGrammar->diameter)/tipsize);
        }
    }


    float segmentTaper = taper/(float)(m_segmentCount-1);
    for (int i=0;i<m_segmentCount;i++)
    {
		m_pSegments[i].m_tipPointCount = tps;
		m_pSegments[i].initialize(tps);
        if(i==0){
            m_pSegments[i].m_base = tipsize;//m_pSpeciesLevelGrammar->diameter;
			m_pSegments[i].m_tip = tipsize;
        }else{
            m_pSegments[i].m_base = m_pSegments[i-1].m_tip;
			m_pSegments[i].m_tip = m_pSegments[i].m_base * (1.0f-segmentTaper);
        }
		if (i==0) {
			m_pSegments[i].m_type = SegmentType::INITIAL_SEGMENT;
		}else if (i==(m_segmentCount-1)){
			m_pSegments[i].m_type = SegmentType::TIP_SEGMENT;
		}else{
			m_pSegments[i].m_type = SegmentType::BASIC_SEGMENT;
		}
	    // Get a rotation matrix about the axis
        V3 arbitrary;
	    D3DXMATRIX rot;
	    D3DXMATRIX rotTwist;
        V3 axis;
        V3 currentSweep;
        V3 crossVector;
		V3 offsetVector;
        float twistAngle;
        float dropAngle;
        // there are three main types of segment roles conceived of right now.
        // there is the initial segment.  This is the first segment in the branch.
        // The previous heading must get it's heading from the start heading passed in.
        // all other segments get their start heading from the previous segment.
        // the basic and tip segments use a random number generation to calculate axial angles
        // drop angles and wobbles.  The tip segment is unique in that it will be used to give the next 
        // branch heading.  The tip segment is where you have all the info to go ahead and store everything 
        // a branch object and add it to the tree.  It is also the point where you call the next 
        // growbranch iteration. 
        switch (m_pSegments[i].m_type)
        {
        case SegmentType::INITIAL_SEGMENT:
			offsetVector = V3(0.0f,1.0f,0.0f);
            m_pSegments[i].m_tipPointList[0] = m_position;// + offsetVector;// + offsetVector; //c0
            // need to calculate later, right now just go straight
            // ROTik = Ax1


            m_pSegments[i].m_AxisHeading = startHeading; // * ROTik => ax0
            axis = startHeading;
	        D3DXMatrixRotationAxis(&rot, &axis, D3DX_PI*2.0f/(float)(tps-1));
            axis.Normalize();
            //arbitrary = V3(axis.z,-axis.x,-axis.y);

            //arbitrary = CrossProduct(axis,arbitrary);//create true orthogonal vector
            //arbitrary.Normalize();
			if(m_pSpeciesLevelGrammar->brnchType == branchType::TRUNK){
				arbitrary = V3(axis.z,-axis.x,-axis.y);
			}else{
				arbitrary = ctreeNode->pPrevNode->pbranch->segments[ctreeNode->pPrevNode->pbranch->segmentCount-1].m_pointOHeading;
			}
            m_pSegments[i].m_pointOHeading =  *(calcArbitraryVector(arbitrary, crossVector, axis)); //arbitrary; // n0

			if(m_pSpeciesLevelGrammar->brnchType == branchType::TRUNK){
                currentSweep = (m_pSegments[i].m_pointOHeading * m_pSegments[i].m_base);
				for(int h=1;h<tps;h++){//tps change  int h=1;h<=6;h++
					m_pSegments[i].m_tipPointList[h] = m_pSegments[i].m_tipPointList[0] + currentSweep;//m_pSpeciesLevelGrammar->diameter); //p0
					D3DXVec3TransformCoord(&currentSweep,&currentSweep,&rot);
				}
			}else{
				for(int h=1;h<tps;h++){//tps change  int h=1;h<=6;h++
                    if(ctreeNode->pPrevNode !=0){
    					m_pSegments[i].m_tipPointList[h] = ctreeNode->pPrevNode->pbranch->segments[ctreeNode->pPrevNode->pbranch->segmentCount-1].m_tipPointList[h];// + offsetVector ;
                    }
				}
				m_pSegments[i].m_tipPointList[0] = m_position; //c0
			}
            break;
        case SegmentType::BASIC_SEGMENT:
            // need to calculate later, right now just go straight
            // ROTik = Ax1
            axis = m_pSegments[i-1].m_AxisHeading; // * ROTik => Ax1
            axis.Normalize();
            

            //arbitrary = V3(axis.z,-axis.x,-axis.y);
            
            //arbitrary = CrossProduct(axis,arbitrary);
            //arbitrary.Normalize();
			arbitrary = m_pSegments[i-1].m_pointOHeading;
            calcArbitraryVector(arbitrary, crossVector, axis);//arbitrary; 

            //
            //twistAngle = Observe->randf(m_pSpeciesLevelGrammar->AxialBias.minAngle,m_pSpeciesLevelGrammar->AxialBias.maxAngle);
            twistAngle = Observe->randf(0,D3DX_PI*10.0f);
            //twistAngle += Observe->randf(0,D3DX_PI*2.0f);
            dropAngle  = Observe->randf(m_pSpeciesLevelGrammar->heading.biasRange.minAngle,m_pSpeciesLevelGrammar->heading.biasRange.maxAngle);
            D3DXMatrixRotationAxis(&rot,&crossVector,dropAngle);
            D3DXMatrixRotationAxis(&rotTwist,&axis,twistAngle);
            D3DXVec3TransformCoord(&axis,&axis,&rot); // rotate by Theta
            D3DXVec3TransformCoord(&axis,&axis,&rotTwist); // rotate by phi
            //
            m_pSegments[i].m_AxisHeading = axis;
            m_pSegments[i].m_pointOHeading = * (calcArbitraryVector(arbitrary, crossVector, axis));

 	        D3DXMatrixRotationAxis(&rot, &axis, D3DX_PI*2.0f/(float)(tps-1));
            m_pSegments[i].m_tipPointList[0] = m_pSegments[i-1].m_tipPointList[0] + (m_pSegments[i].m_AxisHeading * m_pSpeciesLevelGrammar->length);
            currentSweep = (m_pSegments[i].m_pointOHeading * m_pSegments[i].m_base);
			for(int h=1;h<tps;h++){//tps change  int h=1;h<=6;h++
				m_pSegments[i].m_tipPointList[h] = m_pSegments[i].m_tipPointList[0] + currentSweep;
				D3DXVec3TransformCoord(&currentSweep,&currentSweep,&rot);
			}

            break;
        case SegmentType::TIP_SEGMENT:
            // need to calculate later, right now just go straight
            // ROTik = Ax1
            axis = m_pSegments[i-1].m_AxisHeading; // * ROTik => Ax1
            axis.Normalize();
            //arbitrary = V3(axis.z,-axis.x,-axis.y);

            //arbitrary = CrossProduct(axis,arbitrary);
            //arbitrary.Normalize();
            //m_pSegments[i].m_pointOHeading
			arbitrary = m_pSegments[i-1].m_pointOHeading;
            calcArbitraryVector(arbitrary, crossVector, axis); 
            //
            dropAngle  = Observe->randf(m_pSpeciesLevelGrammar->heading.biasRange.minAngle,m_pSpeciesLevelGrammar->heading.biasRange.maxAngle);
            twistAngle = Observe->randf(0,D3DX_PI*10.0f);
            //twistAngle = Observe->randf(m_pSpeciesLevelGrammar->heading.biasRange.minAngle,m_pSpeciesLevelGrammar->heading.biasRange.maxAngle);
            
			D3DXMatrixRotationAxis(&rot,&crossVector,dropAngle);
            D3DXMatrixRotationAxis(&rotTwist,&axis,twistAngle);
            D3DXVec3TransformCoord(&axis,&axis,&rot); // rotate by Theta
            D3DXVec3TransformCoord(&axis,&axis,&rotTwist); // rotate by phi
            //
            m_pSegments[i].m_AxisHeading = axis;
            //m_pSegments[i].m_pointOHeading = arbitrary;
            m_pSegments[i].m_pointOHeading = * (calcArbitraryVector(arbitrary, crossVector, axis));


	        D3DXMatrixRotationAxis(&rot, &axis, D3DX_PI*2.0f/(float)(tps-1));
            m_pSegments[i].m_tipPointList[0] = m_pSegments[i-1].m_tipPointList[0] + (m_pSegments[i].m_AxisHeading * m_pSpeciesLevelGrammar->length);//c1
            currentSweep = (m_pSegments[i].m_pointOHeading * m_pSegments[i].m_base);
			for(int h=1;h<tps;h++){//tps change  int h=1;h<=6;h++
				m_pSegments[i].m_tipPointList[h] = m_pSegments[i].m_tipPointList[0] + currentSweep;
				D3DXVec3TransformCoord(&currentSweep,&currentSweep,&rot);
			}
            V3 twist = m_pSegments[i].m_pointOHeading;
            V3 nextHeading;
            BranchBase *core = new BranchBase();
            core->canopies = 0;
            core->isCanopy = false;
			Observe->DXRS->BranchCount++;
            core->segmentCount = m_segmentCount;
            core->segments = m_pSegments;
            core->position = m_position;
            core->heading = startHeading;
            core->tipPoint = m_pSegments[i].m_tipPointList[0];
            core->life = 100;
            core->burning = false;
            core->startVertex = Observe->DXRS->CurrentVIndex;
            core->startIndex  = Observe->DXRS->CurrentIndex;
            core->AABB = setAABB();
			core->levelID = m_nodeLevel;
			core->levelCount = 6;//grammar->m_levelCount-1;
			core->m_layerTx = (bool) grammar->m_layerTexture;

			 
			if(m_splitCount>0){
				ctreeNode->pNextNodes = new treeNode[m_splitCount];
			}else{
				ctreeNode->pNextNodes = 0;
			}
			ctreeNode->nextNodeSize = m_splitCount;
            
			ctreeNode->pbranch = core;
            // tps is TipPoints per Segment there is one extra TipPoint that is only used for the end segments 
            // so we subtract 1 from all segments. 
            // We then add back in 2 for the tipPoints that are the end segments.
            // number of segments * tipPoints used in each segment + the two tipPoints for the end segments
            Observe->DXRS->CurrentVIndex = Observe->DXRS->CurrentVIndex + (m_segmentCount * (tps-1)) + 2; //tps change
            // account for the vertices of each triangle of each segment pair.
            // that is the segmentcount * the indexes per face times the tipPoints used per segment.
            Observe->DXRS->CurrentIndex =  Observe->DXRS->CurrentIndex + (m_segmentCount)*6*(tps-1);

            //fillBuffers();
            core->vertexCount =  Observe->DXRS->CurrentVIndex - core->startVertex;
            core->indexCount  =  Observe->DXRS->CurrentIndex  - core->startIndex;
            //Split functionality
			currentSweep = axis.Normalize();
            for (int j=0;j<m_splitCount;j++)
            {	
				ctreeNode->pNextNodes[j].pPrevNode = ctreeNode;
				ctreeNode->pNextNodes[j].tree = ctreeNode->pNextNodes[j].pPrevNode->tree;
                if(j==0 && (m_splitType == OPPOSED || m_splitType == splitType::REPEAT_OPPOSED || 
                    m_splitType == splitType::SQUARE || m_splitType == splitType::REPEAT_SQUARE ||
                    m_splitType == splitType::CANOPY || m_splitType == splitType::REPEAT_CANOPY)){
                        switch(m_splitType){
                            case splitType::REPEAT_CANOPY:
                                {
                                    core->isCanopy = true;
                                    ctreeNode->tree->m_CanopyCountTest++;
                                }
                            case splitType::REPEAT_OPPOSED:// intentional fall through, type not completely defined
                            case splitType::REPEAT_SQUARE:
                                {
                                    Branch *NextBranch = CreateNextBranch(m_nodeLevel, m_pSegments[i].m_tipPointList[0]);
                                    NextBranch->growBranch(this,&(ctreeNode->pNextNodes[j]), grammar, axis.Normalize());
                                    delete NextBranch;
                                    break;
                                }
                            case splitType::CANOPY:
                                {
                                    core->isCanopy = true;
                                    ctreeNode->tree->m_CanopyCountTest++;
                                }
                            case splitType::OPPOSED:// intentional fall through, type not completely defined
                            case splitType::SQUARE:
                                {
                                    Branch *NextBranch = CreateNextBranch(m_nodeLevel+1, m_pSegments[i].m_tipPointList[0]);
                                    NextBranch->growBranch(this,&(ctreeNode->pNextNodes[j]), grammar, axis.Normalize());
                                    delete NextBranch;
                                    break;
                                }
                            default:
                                break;
                        }

                }else{
                    if(j==0 && m_splitType == ORDINARY_CANOPY){
                        core->isCanopy = true;
                        ctreeNode->tree->m_CanopyCountTest++;
                    }
					axis=currentSweep;
                    // Phi  Rot about AX1  
                    // Theta Drop angle 
                    dropAngle = Observe->randf(m_pSpeciesLevelGrammar->dropAngle.minAngle,m_pSpeciesLevelGrammar->dropAngle.maxAngle);
                    twistAngle = Observe->randf(m_pSpeciesLevelGrammar->AxialBias.minAngle,m_pSpeciesLevelGrammar->AxialBias.maxAngle);
                    //twistAngle += Observe->randf(m_pSpeciesLevelGrammar->AxialBias.minAngle,m_pSpeciesLevelGrammar->AxialBias.maxAngle);
                    D3DXMatrixRotationAxis(&rot,&twist,dropAngle);
                    D3DXMatrixRotationAxis(&rotTwist,&axis,twistAngle);
                    D3DXVec3TransformCoord(&axis,&axis,&rot); // rotate by Theta
                    D3DXVec3TransformCoord(&nextHeading,&axis,&rotTwist); // rotate by phi
                    Branch *NextBranch = CreateNextBranch(m_nodeLevel+1, m_pSegments[i].m_tipPointList[0]);
                    NextBranch->growBranch(this,&(ctreeNode->pNextNodes[j]), grammar, nextHeading.Normalize());
                    delete NextBranch;
                }
            }
            if(m_splitCount == 0){
                core->isCanopy = true;
                ctreeNode->tree->m_CanopyCountTest++;
            }

            break;
        }

    }

}




void Branch::calcSegmentDepth(LevelDetail * levelGrammar)
{
    observer* ob=observer::Instance();
    bool completed =false;
    float rangeMin = 0.0;
    float rangeMax = 1.0;
    float testArg = ob->randf(0.0, 1.0);
    int splitChoice = 1;
    while (!completed)
    {
        //testArg is between 0&1.
        //The probabilities of the splitList array elements will add up to be 1.0;
        //1.0 represents 100%.  so if you have three levels of .50,.20,&.30 respectively
        //you have a total of 100%.  the testArg will lie somewhere in one of the three ranges
        //so say we have testArg value of .68.  that means that the second split or the one with the 
        //probability of .20 will be chosen.  that is .50+.20 is .70 and .68<=.70
        rangeMax = rangeMin + levelGrammar->splitList[splitChoice-1].probability;
        if((rangeMin <= testArg) && (testArg < rangeMax)){
            completed = true;
            m_splitCount = levelGrammar->splitList[splitChoice-1].splitCount;
            m_splitType = levelGrammar->splitList[splitChoice-1].type;
        }else {
            rangeMin = rangeMax;
        }
        
        splitChoice++;
    }

    testArg = testArg;
}
aabb Branch::setAABB(){
    m_AABB.xMin = FLT_MAX;
    m_AABB.yMin = FLT_MAX;
    m_AABB.zMin = FLT_MAX;
    m_AABB.xMax = -FLT_MAX;
    m_AABB.yMax = -FLT_MAX;
    m_AABB.zMax = -FLT_MAX;
    m_AABB.max = V3(m_AABB.xMax,
                  m_AABB.yMax,
                  m_AABB.zMax);
    m_AABB.min = V3(m_AABB.xMin,
                  m_AABB.yMin,
                  m_AABB.zMin);
    for(int i=0;i<m_segmentCount;i++){
        for(int j=0;j<m_pSegments[i].m_tipPointCount;j++){
            V3 vertex = m_pSegments[i].m_tipPointList[j];
            if (m_AABB.xMin > vertex.x) m_AABB.xMin = vertex.x;
            if (m_AABB.xMax < vertex.x) m_AABB.xMax = vertex.x;
            if (m_AABB.yMin > vertex.y) m_AABB.yMin = vertex.y;
            if (m_AABB.yMax < vertex.y) m_AABB.yMax = vertex.y;
            if (m_AABB.zMin > vertex.z) m_AABB.zMin = vertex.z;
            if (m_AABB.zMax < vertex.z) m_AABB.zMax = vertex.z;
            m_AABB.min = V3(m_AABB.xMin,m_AABB.yMin,m_AABB.zMin);
            m_AABB.max = V3(m_AABB.xMax,m_AABB.yMax,m_AABB.zMax);
        }
    }

    return m_AABB;

}

V3* Branch::calcArbitraryVector(V3 &av, V3 &cv, V3 &axis)
{
    //V3 arbitrary;
    axis.Normalize();
    
    av = CrossProduct(av,axis);
	if(!m_abstractFx){
		av = CrossProduct(axis,av);
	}
    av.Normalize();

    cv = CrossProduct(av,axis);

    cv.Normalize();

    return &av;
    
};


BranchBase::BranchBase()
{
    canopies = 0;
    segments = 0;
	m_layerTx = 0;

}

BranchBase::~BranchBase()
{
    if(isCanopy){
        if(canopies){
            delete canopies;
            canopies = 0;
        }
    }
    if(segments){
        delete [] segments;
        segments = 0;
    }
}

#pragma warning( pop )
