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

#include "FSTree.h"
#include "Sphere.h"

bool FSTree::counting = false;

FSTree::FSTree(std::string grammarName, V3 thePosition, long theSeed)
{

    m_grammar = createGrammar(grammarName);
    m_position = thePosition;
    m_seed     = theSeed;
    theOverseer = observer::Instance();
    m_boundingBox.xMin = FLT_MAX;
    m_boundingBox.yMin = FLT_MAX;
    m_boundingBox.zMin = FLT_MAX;
    m_boundingBox.xMax = -FLT_MAX;
    m_boundingBox.yMax = -FLT_MAX;
    m_boundingBox.zMax = -FLT_MAX;
    m_boundingBox.max = V3(m_boundingBox.xMax,
                         m_boundingBox.yMax,
                         m_boundingBox.zMax);
    m_boundingBox.min = V3(m_boundingBox.xMin,
                         m_boundingBox.yMin,
                         m_boundingBox.zMin);
	theOverseer->DXRS->AABB = m_boundingBox;
	m_StartSphereIndexBuffer = theOverseer->DXRS->CurrentIndex;
	m_StartSphereVertexBuffer = theOverseer->DXRS->CurrentVIndex;
	m_nodeTree = new treeNode();
	m_nodeTree->pPrevNode = m_nodeTree;
	m_nodeTree->nextNodeSize = 0;
    m_nodeTree->LevelID = 0;
    m_SphereCount=0;

}
FSTree::~FSTree(){
    delete Trunk;
    Trunk = 0;
    delete m_grammar;
    m_grammar = 0;
	deleteNode (m_nodeTree);
    delete m_nodeTree;
    m_nodeTree = 0;
}

Sphere* FSTree::createTrunk(int level, V3 basePosition){
    return new Sphere(level,basePosition);
}

void FSTree::deleteNode(treeNode *ctn){
	if(ctn->nextNodeSize == 0){
        if(ctn->psphere != 0){
		    delete ctn->psphere;
            ctn->psphere = 0;
        }
	}else{
		for (int i = 0; i<ctn->nextNodeSize;i++){
			deleteNode(&(ctn->pNextNodes[i]));
		}
        delete ctn->psphere;
        ctn->psphere = 0;
        delete [](ctn->pNextNodes);
        ctn->pNextNodes = 0;
	}
}
//linked list traversal function used to count the number of vertices used
void FSTree::countVertices(treeNode *ctn)
{	
	if(!counting){
		counting = true;
		m_FaceCount = 0;
	}
	if(ctn->nextNodeSize == 0){
    // brackets for if statement left in for easier debugging		
	}else{
		for (int i = 0; i<ctn->nextNodeSize;i++){
			countVertices(&(ctn->pNextNodes[i]));
		}
	}
	m_FaceCount += (ctn->psphere->longitudeDivisions * (ctn->psphere->longitudeDivisions-2))+ 2;

}
//linked list traversal function
void FSTree::countSpheres(treeNode *ctn)
{	
	if(!counting){
		counting = true;
		m_SphereCount = 0;
	}
	m_SphereCount ++;
	if(ctn->nextNodeSize == 0){
    // brackets for if statement left in for easier debugging		
	}else{
		for (int i = 0; i<ctn->nextNodeSize;i++){
			countSpheres(&(ctn->pNextNodes[i]));
		}
	}
}

//linked list traversal function
void FSTree::fillSpheres(treeNode *ctn)
{	
    fillBuffers((ctn->psphere));
	if(ctn->nextNodeSize == 0){
    // brackets for if statement left in for easier debugging			
	}else{
		for (int i = 0; i<ctn->nextNodeSize;i++){
			fillSpheres(&(ctn->pNextNodes[i]));
		}
	}
}
//linked list traversal function
void FSTree::SphereToList(treeNode *ctn, std::vector<SphereBase*> *treeNodeList)
{
    treeNodeList->push_back(ctn->psphere);
	if(ctn->nextNodeSize == 0){
    // brackets for if statement left in for easier debugging				
	}else{
		for (int i = 0; i<ctn->nextNodeSize;i++){
            SphereToList(&(ctn->pNextNodes[i]),treeNodeList);
		}
	}
}

//linked list traversal function
// Extends the Bounding box for the tree to include all points
// contained in the canopy latitudes
void FSTree::BoundingBox(treeNode *ctn)
{	
    bounds((ctn->psphere));
	if(ctn->nextNodeSize == 0){
		
	}else{
		for (int i = 0; i<ctn->nextNodeSize;i++){
			BoundingBox(&(ctn->pNextNodes[i]));
		}
	}
}
void FSTree::bounds(SphereBase *sphere)
{
    theOverseer = observer::Instance();
	int tps = sphere->latitudes[0].m_latitudePointCount;;  
    for(int i = 0;i<sphere->latitudeDivisions;i++){
        for(int k=0; k<tps;k++){
           theOverseer->BoundingBox(sphere->latitudes[i].m_latitudePointList[k]);
           m_boundingBox = theOverseer->DXRS->AABB;
        }
    }
}

//linked list traversal function
void FSTree::getSphere(treeNode *ctn, int sphere)
{
	theOverseer = observer::Instance();
	
	//what is my current Sphere
	// except for very first sphere (trunk) spherecount should be correct coming into function.
	if(!counting){
		counting = true;
		theOverseer->m_sphereFound = false;
		m_SphereCount = 0;
		
	}

	if(m_SphereCount == sphere){
		theOverseer->m_SoughtAfterSphere = ctn->psphere;
		theOverseer->m_sphereFound = true;
		return ;
	}
	if(ctn->nextNodeSize == 0){
		
	}else{
		for (int i = 0; i<ctn->nextNodeSize;i++){
			m_SphereCount++;
			if(!theOverseer->m_sphereFound){
				getSphere(&(ctn->pNextNodes[i]), sphere);
			}
		}
	}

}
Grammar * FSTree::createGrammar(std::string grammarName)
{
    const char * pstr; 
    std::string defaultFileName("stg1.tdf");
    if ( strcmp( grammarName.c_str(), "" ) == 0 )
    {
        pstr = defaultFileName.c_str();
    }else
    {
        pstr = grammarName.c_str();
    }
	Grammar *newGrammar = new Grammar(grammarName);
    return reinterpret_cast<Grammar*>(newGrammar);
    return reinterpret_cast<Grammar*>(0);

}


// creates the FSTree nodes linked list of spherees and canopies
// you grow spherees in the same call to growTree
// you need to traverse the grammar once for the spherees and once for the canopies
// therefore you need to reset the observer back to the initialization between the 
void FSTree::growTree(bool justcount)
{
    sprout();
    Trunk = createTrunk(0, m_position);
    //m_nodeTree = new treeNode();
    m_nodeTree->pPrevNode =0;
    m_nodeTree->tree = this;
 
    V3 startHeading = m_grammar->m_pLevels[0].heading.Heading.Normalize();
    Trunk->growSphere(Trunk, m_nodeTree, m_grammar, startHeading);
	theOverseer = observer::Instance();
    
	m_EndSphereIndexBuffer = theOverseer->DXRS->CurrentIndex;
	m_EndSphereVertexBuffer = theOverseer->DXRS->CurrentVIndex;
    BoundingBox(m_nodeTree);
    theOverseer->DXRS->CurrentIndex=0;
    theOverseer->DXRS->CurrentVIndex=0;
    

    
}
void FSTree::sprout()
{
    m_levelCount = m_grammar->m_levelCount;
    observer *ob = observer::Instance();
    ob->seed(m_seed);
}
void FSTree::pruneTree(V3 *pVertexBuffer)
{
    //stub for future work
}


// iterate through the sphere latitude segments of the tree and 
// fill the index and vertex buffers associated with them.
// 
void FSTree::fillBuffers(SphereBase *sphere)
{
    // rules
    // think in ordinal values.  0,1,...,n-1 indexing for arrays
    // first Latitude and last Latitude have only 1 point.  the Poles  [0]=position of only point
    // n latitudes means n-2 full rims (non Poles)
    // if we consider the second rim + the North pole as a cap and
    // if we consider the second to the last rim + the South pole as a cap
    // we then have two caps to fill as a P01,P12,P23,P34,...,P(n-1)1 index list P == Pole Position
    // we have n - 3 triangle lists to fill
    // initial cap using latitudes 0,1
    // lists starting with latitudes 1,2 ending with n-3,n-2
    // lists indices would fill by (lat1[0],lat2[0],lat1[1])(lat1[1],lat1[0],lat2[1])....
    // end cap using latitudes n-2,n-1
    // for comments below assume a 12 latDiv X 12 longDiv sphere.
    // for now Texture wrap goes 0-1 for beginning to mid point and then 1-0 for last half.  
    // this basically mirrors the texture around.  for solid colors it is fine.  for other textures you will have
    // a mirror on the back half of the sphere.  This was to avoid an additional duplicate vertex with the same position but 
    // a different tex coord.  This can be changes if desired by adding an additional point in each rim
    theOverseer = observer::Instance();
    DWORD idx = sphere->startVertex; //local vertex indices idx and p
    DWORD p;    
    //V3 normal;
    
    V3 TexCoord;
    float texUCoord;
    float texVCoord;
    const int latDivs  = sphere->latitudeDivisions;
    const int longDivs = sphere->longitudeDivisions;
    V3  center   = sphere->center;
    VertexType Vtype = VertexType::VPNT;

    // Add Vertices
    //North Pole
    texUCoord = 0.5f;
    texVCoord = 0.0f;
    TexCoord.x = texUCoord;
    TexCoord.y = texVCoord;
    TexCoord.z = 0.0f;
    theOverseer->addVertex(Vtype, 
                           sphere->latitudes[0].m_latitudePointList[0],
                           sphere->latitudes[0].m_PointNormals[0].Normalize(), 
                           TexCoord);

    //do everything from the [2] to the the [n-1] rims.
    for(int latDiv=1;latDiv<latDivs-1;latDiv++){ 
        for(int longDiv=0;longDiv<longDivs;longDiv++){
            texVCoord = (float)latDiv/latDivs;
            if( longDiv <= (float)longDivs/2 ){
			    texUCoord = (float)longDiv / ((float)longDivs/2);
		    }else{
			    texUCoord= (float)(longDivs -longDiv)/((float)longDivs/2);
		    }
            
            TexCoord.x = texUCoord;
            TexCoord.y = texVCoord;

            theOverseer->addVertex(Vtype, 
                                    sphere->latitudes[latDiv].m_latitudePointList[longDiv],
                                    sphere->latitudes[latDiv].m_PointNormals[longDiv].Normalize(), 
                                    TexCoord);
        }
    }

    //South Pole
    texUCoord = 0.5f;
    texVCoord = 1.0f;
    TexCoord.x = texUCoord;
    TexCoord.y = texVCoord;
    theOverseer->addVertex(Vtype, 
                           sphere->latitudes[latDivs-1].m_latitudePointList[0],
                           sphere->latitudes[latDivs-1].m_PointNormals[0].Normalize(), 
                           TexCoord);


    //Add Indices
    //initial North Pole cap has 1 + 12 indices with 12 x 3 additions to the index buffer.
    if(theOverseer->DXRS->ReverseWindingOrder){
	    for(int h=0; h<longDivs; h++){
		    theOverseer->addIndexes(idx,idx+1 +(h+1)%(longDivs),idx+h+1);
	    }
    }else{

	    for(int h=0; h<longDivs; h++){
		    theOverseer->addIndexes(idx,idx+h+1,idx+1 +(h+1)%(longDivs));
	    }
    }

    //Add [1] to [n-1] Indices
    p = idx+1;
    idx = idx+longDivs+1;
    for(int latDiv=1;latDiv<latDivs-2;latDiv++){ 
        if(theOverseer->DXRS->ReverseWindingOrder){
            for(int lgDv=0; lgDv<longDivs; lgDv++){
	            theOverseer->addIndexes(p+lgDv, idx+(lgDv+1)%(longDivs), idx +  lgDv   %(longDivs)    );//(p  ,idx+1,idx   );
	            theOverseer->addIndexes(p+lgDv, p  +(lgDv+1)%(longDivs), idx + (lgDv+1)%(longDivs));//(p  ,p+1  ,idx+1 ); 
            }
         }else{
            for(int lgDv=0; lgDv<longDivs; lgDv++){
	            theOverseer->addIndexes(p+lgDv, idx +  lgDv   %(longDivs), idx+(lgDv+1)%(longDivs) );//(p  ,idx  ,idx+1);
	            theOverseer->addIndexes(p+lgDv, idx + (lgDv+1)%(longDivs), p  +(lgDv+1)%(longDivs) );    //(p  ,idx+1,p+1  );
            }
        }
        p   = p   + longDivs;
        idx = idx + longDivs;

    }
    

    // Final South Pole cap Indices
    if(theOverseer->DXRS->ReverseWindingOrder){
	    for(int h=0; h<longDivs; h++){
		    theOverseer->addIndexes(idx,p+h,p+(h+1)%(longDivs));
	    }
    }else{
        
	    for(int h=0; h<longDivs; h++){
		    theOverseer->addIndexes(idx,p+(h+1)%(longDivs),p+h);
	    }
    }
    
    
}
void FSTree::KeepVariableAlive(void * a)
{
    m_KAV = a;
}

