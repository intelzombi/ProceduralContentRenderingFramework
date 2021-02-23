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

#include "observer.h"
observer* observer::_instance =0;
RenderStructure *observer::DXRS =0; 
observer* observer::Instance() {
    if(_instance == 0) {
        _instance = new observer;
    }
    return _instance;
}

void observer::deleteInstance(){
    delete DXRS;
    delete _instance;
}

void observer::BoundingBox(V3 vertex){
    bool updateAABB = false;
    if (DXRS->AABB.xMin > vertex.x) { 
        DXRS->AABB.xMin = vertex.x;
        updateAABB = true;
    }
    if (DXRS->AABB.xMax < vertex.x) { 
        DXRS->AABB.xMax = vertex.x;
        updateAABB = true;
    }
    if (DXRS->AABB.yMin > vertex.y) { 
        DXRS->AABB.yMin = vertex.y;
        updateAABB = true;
    }
    if (DXRS->AABB.yMax < vertex.y) { 
        DXRS->AABB.yMax = vertex.y;
        updateAABB = true;
    }
    if (DXRS->AABB.zMin > vertex.z) { 
        DXRS->AABB.zMin = vertex.z;
        updateAABB = true;
    }
    if (DXRS->AABB.zMax < vertex.z) { 
        DXRS->AABB.zMax = vertex.z;
        updateAABB = true;
    }
    if(updateAABB){
        DXRS->AABB.min = V3(DXRS->AABB.xMin,DXRS->AABB.yMin,DXRS->AABB.zMin);
        DXRS->AABB.max = V3(DXRS->AABB.xMax,DXRS->AABB.yMax,DXRS->AABB.zMax);
    }
}
int observer::addVertex(VertexType type, V3 vertex, V3 normal = V3(0.0,1.0,0.0), V3 theTexture = V3(0.5, 0.5, 0.0))
{
    BoundingBox(vertex);
    if (type == VertexType::VPNT && DXRS->vpnt) {
        DXRS->vpnt[DXRS->CurrentVIndex++] = VertexPNT(vertex.x, vertex.y, vertex.z,normal.x, normal.y, normal.z, theTexture.x, theTexture.y);
    }else if (type == VertexType::VP && DXRS->vpos) {
        DXRS->vpos[DXRS->CurrentVIndex++] = VertexPos(vertex.x, vertex.y, vertex.z); 
    }else{
        DXRS->CurrentVIndex++;
    }
   return 1;
}
void observer::addIndexes(DWORD a, DWORD b, DWORD c)
{
    if (DXRS->ptrIBData) {
        DXRS->ptrIBData[DXRS->CurrentIndex++]=a;
        DXRS->ptrIBData[DXRS->CurrentIndex++]=b;
        DXRS->ptrIBData[DXRS->CurrentIndex++]=c;
    } else {
        DXRS->CurrentIndex++;
        DXRS->CurrentIndex++;
        DXRS->CurrentIndex++;
    }
}
 
 

float observer::randf(float f = 1.0f){
    return f*(float)rand()/429496796.0f;
}
float observer::randf(float min, float max){
    return min + (max-min)*(float)rand()/(RAND_MAX + 1) ;

}
int observer::randi(unsigned int max=101){
    return rand() % max;
}
int observer::randi(int min, int max){
    return (int) randf((float)min, float(max));
}
void observer::seed(unsigned int theSeed){
    srand(theSeed);   
}
