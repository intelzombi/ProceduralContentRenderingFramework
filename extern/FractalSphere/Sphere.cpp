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

#include "sphere.h"

#pragma warning( push )
#pragma warning( disable : 6211 )

Sphere::Sphere(int level, V3 basePosition) {
    m_nodeLevel = level;
    m_position = basePosition;
    m_pLatitudes = 0;
    m_pSpeciesLevelGrammar = 0;
}

Sphere *Sphere::CreateNextSphere(int level, V3 basePosition){
    return new Sphere(level,basePosition);
}

//startHeading is assumed to be normalized
//This is the main parsing function for the tree spherees.  This is a recursive function
//that will fill the tree node with all the sphere latitude segments in a multi level linked list.
//This is the area that is most ripe for tweaking to refine the behavior of the parsing as 
//new tokens are added to the grammar.
void Sphere::growSphere(Sphere *pSphere, treeNode *ctreeNode, Grammar *grammar, V3 startHeading)
{

    // the rules:
    // radius == Rho
    // basis y axis will be the start heading
    // == m_AxisHeading
    // rotation about AxisHeading == Theta
    // basis x axis created with arbitrary vector crossed with basis y; == arbitrary vector;
    // == m_pointOHeading
    // rotation about the m_pointOHeading == Phi
    // basis z will be basis y axis crossed with basis x axis
    // == m_AxisZ
    // Latitude lines are determined by latitude division counts PI/latitudeDivisions
    // Longitude lines are determined by longitude division counts 2PI/longitudeDivisions
    // for each latitude rim the Oheading is rotated about the AxisZ by Phi
    // currentSweepVector == rotPhi transform Oheading
    // for each longitude division on that rim the currentSweepVector is rotated about 
    // the AxisHeading by Theta
    // currentSweepVector == rotTheta transform currentSweepVector
    // vertex point == currentSweepVector * Rho
    // The poles are constrained to 1 point each == +/- AxisHeading * Rho 

    // declarations
    V3 arbitrary;
	D3DXMATRIX rotTheta, rotPhi;
    V3 currentSweepVector(0,0,0);
    V3 InitialSweepVector(0,1,0);
    float theta,phi;

    // initialize update tree linked list
    ctreeNode->tree->m_SphereCount++;
    
    // get the observer
    observer *Observe = observer::Instance();
    

    // assign a grammar level to the Sphere 
    //  ||
    m_pSpeciesLevelGrammar = &grammar->m_pLevels[m_nodeLevel];
    //  ||
    //splitType, splitCount and latitudeCount used for determining next set of spheres to be created;
    calcLatitudeDepth(m_pSpeciesLevelGrammar);
    //  ||
    //  get the longitude and latitude segment counts
    m_latitudeCount  = m_pSpeciesLevelGrammar->latitudePointCount; 
    m_longitudeCount = m_pSpeciesLevelGrammar->longitudePointCount;

    // create the latitude rims that will hold the points on the sphere
    m_pLatitudes = new Latitude[m_latitudeCount];

    // theta deltas to revolve around AxisHeading
    theta = D3DX_PI*2.0f/(float)(m_longitudeCount);
    // phi deltas to revolve around the AxisZ
    phi   = D3DX_PI/(m_latitudeCount -1);

    // get radius and calculate Center
    m_heading = startHeading.Normalize();
    m_BasisCoordinates.jAxis = m_heading;
    m_radius = m_pSpeciesLevelGrammar->radius;
    m_dropAngle = m_pSpeciesLevelGrammar->dropAngle;
    m_phiAngle = m_pSpeciesLevelGrammar->AxialBias.minAngle;
    m_thetaAngle = m_pSpeciesLevelGrammar->AxialBias.maxAngle;
    m_center = m_position + (startHeading * m_radius);
    
    currentSweepVector = m_heading; 
    currentSweepVector.Normalize();
    D3DXMatrixRotationAxis(&rotTheta, &m_heading, theta);
    arbitrary = V3(m_heading.z,-m_heading.x,-m_heading.y);
    arbitrary = CrossProduct(m_heading,arbitrary);//create true orthogonal vector
    arbitrary.Normalize();
    m_BasisCoordinates.iAxis = arbitrary;
    m_BasisCoordinates.kAxis = CrossProduct(m_heading,arbitrary);
    m_BasisCoordinates.normalize();
    D3DXMatrixRotationAxis(&rotPhi,&m_BasisCoordinates.kAxis,phi);

    //create sphere points
    for (int cLatitude=0; cLatitude < m_latitudeCount; cLatitude++)
    {
	    // Get a rotation matrix about the axis
        if (cLatitude == 0)//NorthPole
        {
		    m_pLatitudes[cLatitude].m_latitudePointCount = 1;
			m_pLatitudes[cLatitude].m_type = LatitudeType::NORTH_POLE;
		    m_pLatitudes[cLatitude].initialize();
            m_pLatitudes[cLatitude].m_latitudePointList[0]= m_center + (currentSweepVector * m_radius);
            m_pLatitudes[cLatitude].m_PointNormals[0]= currentSweepVector;
			D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotPhi);
            
        }
        else if (cLatitude == m_latitudeCount-1)//SouthPole
        {
		    m_pLatitudes[cLatitude].m_latitudePointCount = 1;
			m_pLatitudes[cLatitude].m_type = LatitudeType::SOUTH_POLE;
		    m_pLatitudes[cLatitude].initialize();
            m_pLatitudes[cLatitude].m_latitudePointList[0]= m_center + (currentSweepVector * m_radius);
            m_pLatitudes[cLatitude].m_PointNormals[0]= currentSweepVector;
       }
        else //Latitude
        {
 		    m_pLatitudes[cLatitude].m_latitudePointCount = m_longitudeCount;
			m_pLatitudes[cLatitude].m_type = LatitudeType::BASIC_LATITUDE;
    		m_pLatitudes[cLatitude].initialize();
            m_pLatitudes[cLatitude].m_pointOHeading = m_BasisCoordinates.iAxis; // n0
            
            InitialSweepVector = currentSweepVector;
            for (int cLongitude=0;cLongitude < m_longitudeCount; cLongitude++)
            {
                m_pLatitudes[cLatitude].m_latitudePointList[cLongitude] = m_center + (currentSweepVector * m_radius);
                m_pLatitudes[cLatitude].m_PointNormals[cLongitude]= currentSweepVector;
			    D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotTheta);

            }
            currentSweepVector = InitialSweepVector;
			D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotPhi);


       }
    }// end Fill latitude Rim structure

    V3 nextHeading;
    SphereBase *core = new SphereBase();
	Observe->DXRS->SphereCount++;
    core->position = m_position;
    core->center = m_center;
    core->heading = m_heading;
    core->longitudeDivisions = m_longitudeCount;
    core->latitudeDivisions = m_latitudeCount;
    core->latitudes = m_pLatitudes;
    if(m_splitCount){
        core->touchPoints = new V3[m_splitCount];
    }else{
        core->touchPoints = 0;
    }
    core->startVertex = Observe->DXRS->CurrentVIndex;
    core->startIndex  = Observe->DXRS->CurrentIndex;
    core->AABB = setAABB();

	 
	if(m_splitCount>0){
		ctreeNode->pNextNodes = new treeNode[m_splitCount];
        for(int nn=0;nn<m_splitCount;nn++)
        {
            ctreeNode->pNextNodes[nn].psphere = 0;
            ctreeNode->pNextNodes[nn].pPrevNode = 0;
            ctreeNode->pNextNodes[nn].pNextNodes = 0;
            ctreeNode->pNextNodes[nn].nextNodeSize = 0;
            ctreeNode->pNextNodes[nn].LevelID = m_nodeLevel;

        }
	}else{
		ctreeNode->pNextNodes = 0;
        ctreeNode->nextNodeSize = 0;
	}
	ctreeNode->nextNodeSize = m_splitCount;
    calculateTouchPoints(&m_BasisCoordinates, m_radius, m_splitType, m_splitCount, core->touchPoints);
	ctreeNode->psphere = core;
    // tps is TipPoints per Segment there is one extra TipPoint that is only used for the end segments 
    // so we subtract 1 from all segments. 
    // We then add back in 2 for the tipPoints that are the end segments.
    // number of segments * tipPoints used in each segment + the two tipPoints for the end segments
    Observe->DXRS->CurrentVIndex = Observe->DXRS->CurrentVIndex + ((m_latitudeCount-2) * (m_longitudeCount)) + 2;
    // account for the vertices of each triangle of each latitude pair.
    // that is the latitudescount * the indexes per face times the tipPoints used per Latitude.
    // longitudeCount * 3 * 2 + ( (longitudeCount * 6 *(latitudeCount-2)) )
    Observe->DXRS->CurrentIndex =  Observe->DXRS->CurrentIndex + (m_longitudeCount * 6) + (m_longitudeCount * 6 * (m_latitudeCount-2));


    //fillBuffers();
    core->vertexCount =  Observe->DXRS->CurrentVIndex - core->startVertex;
    core->indexCount  =  Observe->DXRS->CurrentIndex  - core->startIndex;
    //Split functionality
    for (int j=0;j<m_splitCount;j++)
    {	
		ctreeNode->pNextNodes[j].pPrevNode = ctreeNode;
		ctreeNode->pNextNodes[j].tree = ctreeNode->pNextNodes[j].pPrevNode->tree;

        switch(m_splitType){
            case splitType::FRACTAL_60:
                {
                    Sphere *NextSphere = CreateNextSphere(m_nodeLevel+1, core->touchPoints[j]);
                    NextSphere->growSphere(this,&(ctreeNode->pNextNodes[j]), grammar, (core->touchPoints[j] - m_center).Normalize() );
                    delete NextSphere;
                    break;
                }
            case splitType::FRACTAL_E:
                {
                    Sphere *NextSphere = CreateNextSphere(m_nodeLevel+1, core->touchPoints[j]);
                    NextSphere->growSphere(this,&(ctreeNode->pNextNodes[j]), grammar, (core->touchPoints[j] - m_center).Normalize() );
                    delete NextSphere;
                    break;
                }
            case splitType::FRACTAL_S:
                {
                    Sphere *NextSphere = CreateNextSphere(m_nodeLevel+1, core->touchPoints[j]);
                    NextSphere->growSphere(this,&(ctreeNode->pNextNodes[j]), grammar, (core->touchPoints[j] - m_center).Normalize() );
                    delete NextSphere;
                    break;
                }
            case splitType::FRACTAL_60_FULL:
                {
                    Sphere *NextSphere = CreateNextSphere(m_nodeLevel+1, core->touchPoints[j]);
                    NextSphere->growSphere(this,&(ctreeNode->pNextNodes[j]), grammar, (core->touchPoints[j] - m_center).Normalize() );
                    delete NextSphere;
                    break;
                }
            case splitType::FRACTAL_Y:
                {
                    Sphere *NextSphere = CreateNextSphere(m_nodeLevel+1, core->touchPoints[j]);
                    NextSphere->growSphere(this,&(ctreeNode->pNextNodes[j]), grammar, (core->touchPoints[j] - m_center).Normalize() );
                    delete NextSphere;
                    break;
                }
            case splitType::FRACTAL_YPEAT://repeated
                {
                    Sphere *NextSphere = CreateNextSphere(m_nodeLevel, core->touchPoints[j]);
                    NextSphere->growSphere(this,&(ctreeNode->pNextNodes[j]), grammar, (core->touchPoints[j] - m_center).Normalize() );
                    delete NextSphere;
                    break;
                }
            case splitType::FRACTAL_I://repeated
                {
                    Sphere *NextSphere = CreateNextSphere(m_nodeLevel, core->touchPoints[j]);
                    NextSphere->growSphere(this,&(ctreeNode->pNextNodes[j]), grammar, (core->touchPoints[j] - m_center).Normalize() );
                    delete NextSphere;
                    break;
                }
            case splitType::FRACTAL_IEND:
                {
                    Sphere *NextSphere = CreateNextSphere(m_nodeLevel+1, core->touchPoints[j]);
                    NextSphere->growSphere(this,&(ctreeNode->pNextNodes[j]), grammar, (core->touchPoints[j] - m_center).Normalize() );
                    delete NextSphere;
                    break;
                }
            case splitType::TERMINAL:
                {
                    break;
                }
            default:
                break;
        }

    }

}


void Sphere::calculateTouchPoints(B3 *basis, float radius, splitType eSplitType,int splitCount, V3 *touchPoints)
{
    //rules
    // currentSweepVector starts from North Pole
    // sweep Phi then Theta
    // sweep Phi then Theta
    //declarations

    V3 arbitrary;
	D3DXMATRIX rotTheta, rotPhi, rotThetaPrime, rotThetaDoublePrime, rotPhiPrime;
    V3 currentSweepVector(0,0,0);
    V3 InitialSweepVector(0,1,0);
    observer* ob=observer::Instance();
    float testArg;// = ob->randf(0.0, 1.0);


    float theta,phi,phiPrime;
    switch (m_splitType) {
        case FRACTAL_60:
            {
                //set rotation matrices
                // theta deltas to revolve around AxisHeading
                theta = ((float)60/RadianDegree);
                // phi deltas to revolve around the AxisZ
                phi   = (1.0);
                phiPrime = ((90-RadianDegree)/RadianDegree);
                D3DXMatrixRotationAxis(&rotTheta, &m_BasisCoordinates.jAxis, theta);
                D3DXMatrixRotationAxis(&rotThetaPrime,&m_BasisCoordinates.jAxis,theta/2);
                D3DXMatrixRotationAxis(&rotThetaDoublePrime,&m_BasisCoordinates.jAxis,theta*2);
                D3DXMatrixRotationAxis(&rotPhi,&m_BasisCoordinates.kAxis,phi);
                D3DXMatrixRotationAxis(&rotPhiPrime,&m_BasisCoordinates.kAxis,phiPrime);
                currentSweepVector = m_BasisCoordinates.jAxis; 
                currentSweepVector.Normalize();
                InitialSweepVector = currentSweepVector;
			    D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotPhiPrime);
                int currentTouchPoint=0;
                //create touch points
                for (int cLatitude=0; cLatitude < 2; cLatitude++)
                {
	                // Get a rotation matrix about the axis
                    if (cLatitude == 0)//first latitude from North Pole
                    {
                        InitialSweepVector = currentSweepVector;
			            D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotThetaPrime);
                        for (int cLongitude=0;cLongitude < 3; cLongitude++)
                        {
                            currentSweepVector.Normalize();
                            touchPoints[currentTouchPoint] = m_center + currentSweepVector * m_radius;
                            currentTouchPoint++;
			                D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotThetaDoublePrime);

                        }
                        currentSweepVector = InitialSweepVector;
                    }else{
                        InitialSweepVector = currentSweepVector;
			            D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotPhi);
                        for (int cLongitude=0;cLongitude < 6; cLongitude++)
                        {
                            currentSweepVector.Normalize();
                            touchPoints[currentTouchPoint] = m_center + currentSweepVector * m_radius;
                            currentTouchPoint++;
			                D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotTheta);

                        }
                        currentSweepVector = InitialSweepVector;                   
                    
                    }
                }
                
                break;
            }
        case FRACTAL_60_FULL:
            {
                //set rotation matrices
                // theta deltas to revolve around AxisHeading
                theta = ((float)60/RadianDegree);
                // phi deltas to revolve around the AxisZ
                phi   = (1.0);
                phiPrime = ((90-RadianDegree)/RadianDegree);
                D3DXMatrixRotationAxis(&rotTheta, &m_BasisCoordinates.jAxis, theta);
                D3DXMatrixRotationAxis(&rotThetaPrime,&m_BasisCoordinates.jAxis,theta/2);
                D3DXMatrixRotationAxis(&rotThetaDoublePrime,&m_BasisCoordinates.jAxis,theta*2);
                D3DXMatrixRotationAxis(&rotPhi,&m_BasisCoordinates.kAxis,phi);
                D3DXMatrixRotationAxis(&rotPhiPrime,&m_BasisCoordinates.kAxis,phiPrime);
                currentSweepVector = m_BasisCoordinates.jAxis; 
                currentSweepVector.Normalize();
                InitialSweepVector = currentSweepVector;
			    D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotPhiPrime);
                int currentTouchPoint=0;
                //create touch points
                for (int cLatitude=0; cLatitude < 3; cLatitude++)
                {
	                // Get a rotation matrix about the axis
                    if (cLatitude == 0)//first latitude from North Pole
                    {
                        InitialSweepVector = currentSweepVector;
			            D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotThetaPrime);
                        for (int cLongitude=0;cLongitude < 3; cLongitude++)
                        {
                            currentSweepVector.Normalize();
                            touchPoints[currentTouchPoint] = m_center + currentSweepVector * m_radius;
                            currentTouchPoint++;
			                D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotThetaDoublePrime);

                        }
                        currentSweepVector = InitialSweepVector;
                    }else if (cLatitude == 2){
                        InitialSweepVector = currentSweepVector;
			            D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotPhi);
			            D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotPhi);
			            D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotThetaPrime);
                        for (int cLongitude=0;cLongitude < 3; cLongitude++)
                        {
                            currentSweepVector.Normalize();
                            touchPoints[currentTouchPoint] = m_center + currentSweepVector * m_radius;
                            currentTouchPoint++;
			                D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotThetaDoublePrime);

                        }
                        currentSweepVector = InitialSweepVector;

                    }else{
                        InitialSweepVector = currentSweepVector;
			            D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotPhi);
                        for (int cLongitude=0;cLongitude < 6; cLongitude++)
                        {
                            currentSweepVector.Normalize();
                            touchPoints[currentTouchPoint] = m_center + currentSweepVector * m_radius;
                            currentTouchPoint++;
			                D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotTheta);

                        }
                        currentSweepVector = InitialSweepVector;                   
                    
                    }
                }
                
                break;
            }
        case FRACTAL_YPEAT:
        case FRACTAL_Y:
            {
                //set rotation matrices
                // theta deltas to revolve around AxisHeading
                theta = ((float)180/RadianDegree);
                // phi deltas to revolve around the AxisZ
                phi   = (1.0);
                D3DXMatrixRotationAxis(&rotTheta, &m_BasisCoordinates.jAxis, theta);
                D3DXMatrixRotationAxis(&rotPhi,&m_BasisCoordinates.kAxis,phi);
                currentSweepVector = m_BasisCoordinates.jAxis; 
                currentSweepVector.Normalize();
                InitialSweepVector = currentSweepVector;
			    D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotPhi);
                int currentTouchPoint=0;
                //create touch points
                InitialSweepVector = currentSweepVector;
	            //D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotTheta);
                //currentSweepVector.Normalize();
                for (int cLongitude=0;cLongitude < 2; cLongitude++)
                {
                    touchPoints[currentTouchPoint] = m_center + currentSweepVector * m_radius;
                    currentTouchPoint++;
	                D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotTheta);
                    currentSweepVector.Normalize();

                }
                currentSweepVector = InitialSweepVector;

                break;
            }
        case FRACTAL_T:
            {

                break;
            }
        case FRACTAL_E:
            {
                //set rotation matrices
                // theta deltas to revolve around AxisHeading
                theta = ((float)60/RadianDegree);
                // phi deltas to revolve around the AxisZ
                phi   = (1.0);
                phiPrime = ((90-RadianDegree)/RadianDegree);
                D3DXMatrixRotationAxis(&rotThetaPrime,&m_BasisCoordinates.jAxis,theta/2);
                D3DXMatrixRotationAxis(&rotThetaDoublePrime,&m_BasisCoordinates.jAxis,theta*2);
                D3DXMatrixRotationAxis(&rotPhiPrime,&m_BasisCoordinates.kAxis,phiPrime);
                currentSweepVector = m_BasisCoordinates.jAxis; 
                currentSweepVector.Normalize();
                InitialSweepVector = currentSweepVector;
			    D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotPhiPrime);
                int currentTouchPoint=0;
                InitialSweepVector = currentSweepVector;
                D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotThetaPrime);
                //create touch points
                for (int cLongitude=0;cLongitude < 3; cLongitude++)
                {
                    currentSweepVector.Normalize();
                    touchPoints[currentTouchPoint] = m_center + currentSweepVector * m_radius;
                    currentTouchPoint++;
                    D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotThetaDoublePrime);

                }
                currentSweepVector = InitialSweepVector;
                break;
            }
        case FRACTAL_S:
            {
                int currentTouchPoint=0;
                for(int point =0;point<m_splitCount; point++){
                    //set rotation matrices
                    // theta deltas to revolve around AxisHeading
                    testArg = ob->randf(0.0, 1.0);
                    theta = testArg *(360/RadianDegree);
                    // phi deltas to revolve around the AxisZ
                    testArg = ob->randf(0.0, 1.0);
                    phi   = testArg *(180/RadianDegree);
                    D3DXMatrixRotationAxis(&rotTheta,&m_BasisCoordinates.jAxis,theta);
                    D3DXMatrixRotationAxis(&rotPhi,&m_BasisCoordinates.kAxis,phi);
                    currentSweepVector = m_BasisCoordinates.jAxis; 
                    currentSweepVector.Normalize();
                    InitialSweepVector = currentSweepVector;
			        D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotPhi);
                    D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotTheta);
                    //create touch points
                    touchPoints[currentTouchPoint] = m_center + currentSweepVector * m_radius;
                    currentSweepVector = InitialSweepVector;
                    currentTouchPoint++;
                }
                break;
            }
        case FRACTAL_I:
        case FRACTAL_IEND:
            {
                testArg= ob->randf(0.0, 1.0);
                phi = m_dropAngle;
                theta = m_phiAngle + ( testArg *(m_thetaAngle-m_phiAngle));
                //phi = (10/RadianDegree);
                D3DXMatrixRotationAxis(&rotPhi,&m_BasisCoordinates.kAxis,phi);
                D3DXMatrixRotationAxis(&rotTheta, &m_BasisCoordinates.jAxis, theta);
                int currentTouchPoint=0;
                currentSweepVector = m_BasisCoordinates.jAxis; 
                currentSweepVector.Normalize();
			    D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotPhi);
	            D3DXVec3TransformCoord(&currentSweepVector,&currentSweepVector,&rotTheta);
                touchPoints[currentTouchPoint] = m_center + currentSweepVector * m_radius;
                currentTouchPoint++;
                break;
            }
        case TERMINAL:
            {
                break;
            }
        default:
            break;
    }

}


void Sphere::calcLatitudeDepth(LevelDetail * levelGrammar)
{
    observer* ob=observer::Instance();
    bool completed =false;
    float rangeMin = 0.0;
    float rangeMax = 1.0;
    float testArg = ob->randf(0.0, 1.0);
    m_splitChoice = 1;
    while (!completed)
    {
        //testArg is between 0&1.
        //The probabilities of the splitList array elements will add up to be 1.0;
        //1.0 represents 100%.  so if you have three levels of .50,.20,&.30 respectively
        //you have a total of 100%.  the testArg will lie somewhere in one of the three ranges
        //so say we have testArg value of .68.  that means that the second split or the one with the 
        //probability of .20 will be chosen.  that is .50+.20 is .70 and .68<=.70
        rangeMax = rangeMin + levelGrammar->splitList[m_splitChoice-1].probability;
        if((rangeMin <= testArg) && (testArg < rangeMax)){
            completed = true;
            m_splitCount = levelGrammar->splitList[m_splitChoice-1].splitCount;
            m_splitType = levelGrammar->splitList[m_splitChoice-1].type;
        }else {
            rangeMin = rangeMax;
        }
        
        m_splitChoice++;
    }

    testArg = testArg;
}
aabb Sphere::setAABB(){
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
    for(int i=0;i<m_latitudeCount;i++){
        for(int j=0;j<m_pLatitudes[i].m_latitudePointCount;j++){
            V3 *vertex = &(m_pLatitudes[i].m_latitudePointList[j]);
            bool updateAABB = false;
            if (m_AABB.xMin > vertex->x) { 
                m_AABB.xMin = vertex->x; 
                updateAABB = true;
            }
            if (m_AABB.xMax < vertex->x) { 
                m_AABB.xMax = vertex->x; 
                updateAABB = true;
            }
            if (m_AABB.yMin > vertex->y) { 
                m_AABB.yMin = vertex->y; 
                updateAABB = true;
            }
            if (m_AABB.yMax < vertex->y) { 
                m_AABB.yMax = vertex->y; 
                updateAABB = true;
            }
            if (m_AABB.zMin > vertex->z) { 
                m_AABB.zMin = vertex->z; 
                updateAABB = true;
            }
            if (m_AABB.zMax < vertex->z) { 
                m_AABB.zMax = vertex->z; 
                updateAABB = true;
            }
            if(updateAABB){
                m_AABB.min = V3(m_AABB.xMin,m_AABB.yMin,m_AABB.zMin);
                m_AABB.max = V3(m_AABB.xMax,m_AABB.yMax,m_AABB.zMax);
            }
        }
    }

    return m_AABB;

}

SphereBase::SphereBase()
{

}

SphereBase::~SphereBase()
{
    delete [] touchPoints;
    touchPoints = NULL;
    delete [] latitudes;
    latitudes = NULL;

}

#pragma warning( pop )
