//Sphere Class
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


class Sphere;
class SphereBase;

#ifndef _Sphere
#define _Sphere
#include "SphereGrammar.h"
#include "LatitudeRing.h"
#include "observer.h"
#include "aabb.h"

struct treeNode;
class SphereBase 
{
public:
    SphereBase();
    ~SphereBase();
    
    Latitude *latitudes; //create a list of latitudes for this sphere
    V3 *touchPoints;
    float radius;
    V3 position; // This is the root or middle latitude center point. (Sphere Center)
    V3 center;
    V3 heading; // This is overall sphere direction.  Normal to Parent surface attachpoint.
    int longitudeDivisions;
    int latitudeDivisions;
    int startVertex;
    int startIndex; 
    int vertexCount;
    int indexCount; 
    aabb AABB;
};


class Sphere {
public:
    Sphere(int level, V3 basePosition);
    void growSphere(Sphere *pSphere, treeNode *ctreeNode, Grammar *grammar,V3 startHeading);
    virtual Sphere *CreateNextSphere(int level, V3 basePosition);
    void calcLatitudeDepth(LevelDetail * levelGrammar);
    void calculateTouchPoints(B3 *basis,float radius, splitType eSplitType,int splitCount, V3 touchPoints[]);
    aabb setAABB();

    V3 m_position; // This is the root Latitude center point. This is the Parent sphere surface attach Point.
    V3 m_center; // This is the root Latitude center point. This is the Parent sphere surface attach Point.
    V3 m_heading; // This is the root Latitude center point. This is the Parent sphere surface attach Point.
    B3 m_BasisCoordinates;
    float m_radius;
    Latitude *m_pLatitudes; //create a list of Latitudes for this sphere
    int m_splitChoice;
    int m_latitudeCount;
    int m_longitudeCount;
    splitType m_splitType;
    observer *theOverseer;
    int m_splitCount;
    int m_nodeLevel; //level of grammar or present depth of tree growth.
//    sphereNode *nodeSphere;
    float m_dropAngle; 
    float m_thetaAngle; 
    float m_phiAngle; 
    LevelDetail *m_pSpeciesLevelGrammar; //just the facts needed for this sphere. no sense dragging the whole grammar along
    aabb m_AABB;
protected:
    Sphere(){};
};
#endif

    