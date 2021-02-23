//Observer Class
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


// When dealing with linked lists or tree structures it is often cumbersome to deal
// with the data used in the recursive functions.  trying to keep track of where you
// are can get confusing and difficult.  The choices for the programmar are to either pass a lot 
// of data in the function creating a lot of stack bloat or create a lot of global variables.
// I'm not personally fond of either solution so the observer class was created as a singleton
// container that will basically manage those variables for you.  A managed global container
// (not to be confused with the Observer Pattern).  It oversees the variables needed and provides
// a way for the application using the library to pass in the data without having extern in or out a bunch
// of variables.  You can call the instance at any point and know you have the variables in there present state.
//
class observer;

#ifndef OBSERVER_H
#define OBSERVER_H

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include "Vertex.h"
#include "Vector3.h"
#include "Sphere.h"
#include "FSTree.h"
#include "aabb.h"
#include "stdlib.h"
#include <vector>


struct RenderStructure {
    VertexType type;    
    VertexPos *vpos;  //vertex buffer pointer
    VertexPNT *vpnt;  //vertex buffer pointer
    DWORD *ptrIBData;  //index buffer pointer
    DWORD CurrentIndex;
    DWORD CurrentVIndex; 
	WORD SphereCount;
    bool ReverseWindingOrder;
    aabb AABB;
};


class observer {
public:
    static observer * Instance();
    static void deleteInstance();
    static RenderStructure *DXRS;
    void BoundingBox(V3 vertex);

    int addVertex(VertexType type, V3 vertex, V3 normal, V3 theTexture);
    void addIndexes(DWORD a, DWORD b, DWORD c);
    float randf(float f);//defaults to number between 0(inclusive)-1(exclusive) 
    float randf(float min, float max);
    int randi(unsigned int i);
    int randi(int min, int max);
    void seed(unsigned int theSeed);
    FSTree *m_tree;
	SphereBase *m_SoughtAfterSphere;
	bool m_sphereFound;
    
protected:
    observer(){};
private:
    static observer* _instance;
    
};
#endif //OBSERVER_H