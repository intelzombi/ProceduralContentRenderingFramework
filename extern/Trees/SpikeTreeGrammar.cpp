//SpikeTreeGrammar Class
//Written by Hugh Smith April 2007
// Part of Tree Grammar Structure for creating procedural trees

// Copyright � 2008 Intel Corporation
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

#include "SpikeTreeGrammar.h"
#include "math.h"
/*
Probability	Split 	Split Type	DropAngle	Axial Bias	Segment Length	Segment Diameter	Wobble	        Taper
-----------------	----------  ---------   ----------  --------------  ----------------    -------------   -------
Level        0								   
5	         3        general	45-80	    30-330	        10	            2	            0-20 	        0.25
5	         2        Square		  			            	                            (bias up)
25	         3        Opposed					            	
0	         1        						
0	         1        						
10	         3        Opposed						
55	         2        					            	
Probability	Split 		        DropAngle	Axial Bias	Segment Length	Segment Diameter	Trajectory	    Taper
-----------------	----------  ---------   ----------  --------------  ----------------    -------------   -------
Level 1						                30-330	               	
5	         2         Square	 35-50	  	                7	            1.5	            0-35	        0.2
5	         2         Square	 35-50		                               	                		
25	         3         Opposed	 40-45		                               	                		
60	         2	       Square	 35-50		                               	                		
5	         3	       Opposed	 40-45		                               	                		
Probability	Split 		        DropAngle	Axial Bias	Segment Length	Segment Diameter	Trajectory	    Taper
-----------------	----------  ---------   ----------  --------------  ----------------    -------------   -------
Level 2			                			30-330 		
0	         1	       spear	 35[b]-50		            6               1.25	        0-20	        0.33
0	         1							
0	         1							
0	         1				                               	                		
100	         2	       Square	 35-50		                               	                		
Probability	Split 		        DropAngle	Axial Bias	Segment Length	Segment Diameter	Trajectory	    Taper
-----------------	----------  ---------   ----------  --------------  ----------------    -------------   -------
Level 3			                			30-330		
0	         1				                                4               1.25	        0-10	        0.5
100	         2	       General	35 -40		                               	                	    	
Probability	Split 		        DropAngle	Axial Bias	Segment Length	Segment Diameter	Trajectory	    Taper
-----------------	----------  ---------   ----------  --------------  ----------------    -------------   -------
Level 4			                			30-330		
0	         1	       spear	0		                    3               1	            20	            0
0	         1							
0	         1							
0	         1							
0	         1							
0	         1							
0	         1							
0	         1							
0	         1							
100	         1	       Spear						
Probability	Split 		        DropAngle	Axial Bias	Segment Length	Segment Diameter	Trajectory	    Taper
-----------------	----------  ---------   ----------  --------------  ----------------    -------------   -------
Level 5						                30-330		

*/

SpikeTree::SpikeTree()
{
    m_type = SPIKE;
    m_pLevels = new LevelDetail[8];
    m_pCanopyDefinition = new CanopyDefinition();
    m_levelCount = 8;
    //level 0
	m_pLevels[0].tipPointCount = 13;
    m_pLevels[0].AxialBias.maxAngle = 330.0f / RadianDegree; //convert to radians
    m_pLevels[0].AxialBias.minAngle = 30.0f / RadianDegree;
    m_pLevels[0].length = 10.0f;
    m_pLevels[0].diameter = 4.0f;//3.0;
    m_pLevels[0].taper = 0.1f;//0.25;
    m_pLevels[0].LevelID = 0;
    //levels[0].dropAngle.minAngle = 40.0f / RadianDegree;
    //levels[0].dropAngle.maxAngle = 45.0f / RadianDegree;
    m_pLevels[0].dropAngle.minAngle = 10.0f / RadianDegree;
    m_pLevels[0].dropAngle.maxAngle = 25.0f / RadianDegree;
    m_pLevels[0].dropAngle.biasAngle = 50.0f / RadianDegree;
    m_pLevels[0].heading.Heading = V3(0.0,1.0,0.0);
    m_pLevels[0].heading.biasRange.minAngle = -5.0f/ RadianDegree;
    m_pLevels[0].heading.biasRange.maxAngle = 5.0f / RadianDegree;
    m_pLevels[0].splitList = new split[4];
    m_pLevels[0].splitList[0].probability = 0.25f;
    m_pLevels[0].splitList[0].splitCount = 5;
    m_pLevels[0].splitList[0].type = REPEAT_OPPOSED;//OPPOSED
    m_pLevels[0].splitList[1].probability = 0.05f;
    m_pLevels[0].splitList[1].splitCount = 2;
    m_pLevels[0].splitList[1].type = ORDINARY;
    m_pLevels[0].splitList[2].probability = 0.15f;
    m_pLevels[0].splitList[2].splitCount = 3;
    m_pLevels[0].splitList[2].type = REPEAT_OPPOSED;//OPPOSED

    m_pLevels[0].splitList[3].probability = 0.55f;
    m_pLevels[0].splitList[3].splitCount = 2;
    m_pLevels[0].splitList[3].type = ORDINARY;
    m_pLevels[0].brnchType = branchType::TRUNK;
    m_pLevels[0].cnpyType = canopyType::NULL_CANOPY;
//    levels[0].splitList[4].splitCount = ;
    
    //level 1
	m_pLevels[1].tipPointCount = 11;
    m_pLevels[1].AxialBias.maxAngle = 330.0f / RadianDegree; //convert to radians
    m_pLevels[1].AxialBias.minAngle = 30.0f / RadianDegree;
    m_pLevels[1].length = 7.0f;
    m_pLevels[1].diameter = 2.0f;
    m_pLevels[1].taper = 0.20f;
    m_pLevels[1].LevelID = 1;
    m_pLevels[1].dropAngle.minAngle = 35.0f / RadianDegree;
    m_pLevels[1].dropAngle.maxAngle = 50.0f / RadianDegree;
    m_pLevels[1].dropAngle.biasAngle = 40.0f / RadianDegree;
    //levels[1].heading.biasRange.minAngle = -8.0f/ RadianDegree;
    //levels[1].heading.biasRange.maxAngle = 8.0f / RadianDegree;
    m_pLevels[1].heading.biasRange.minAngle = -20.0f/ RadianDegree;
    m_pLevels[1].heading.biasRange.maxAngle = 20.0f / RadianDegree;
    m_pLevels[1].splitList = new split[5];
    m_pLevels[1].splitList[0].probability = 0.05f;
    m_pLevels[1].splitList[0].splitCount = 3;
    m_pLevels[1].splitList[0].type = REPEAT_OPPOSED;
    m_pLevels[1].splitList[1].probability = 0.05f;
    m_pLevels[1].splitList[1].splitCount = 3;
    m_pLevels[1].splitList[1].type = REPEAT_OPPOSED;
    m_pLevels[1].splitList[2].probability = 0.25f;
    m_pLevels[1].splitList[2].splitCount = 3;
    m_pLevels[1].splitList[2].type = REPEAT_OPPOSED;
    m_pLevels[1].splitList[3].probability = 0.45f;
    m_pLevels[1].splitList[3].splitCount = 2;
    m_pLevels[1].splitList[3].type = ORDINARY;
    m_pLevels[1].splitList[4].probability =  0.20f;
    m_pLevels[1].splitList[4].splitCount = 2;
    m_pLevels[1].splitList[4].type = ORDINARY;

    m_pLevels[1].brnchType = branchType::BRANCH;
    m_pLevels[1].cnpyType = canopyType::NULL_CANOPY;
    
    //level 2
	m_pLevels[2].tipPointCount = 9;
    m_pLevels[2].AxialBias.maxAngle = 330.0f / RadianDegree; //convert to radians
    m_pLevels[2].AxialBias.minAngle = 30.0f / RadianDegree;
    m_pLevels[2].length = 6.0f;
    m_pLevels[2].diameter = 1.0f;
    m_pLevels[2].taper = 0.33f;
    m_pLevels[2].LevelID = 2;
    m_pLevels[2].dropAngle.minAngle = 35.0f / RadianDegree;
    m_pLevels[2].dropAngle.maxAngle = 50.0f / RadianDegree;
    m_pLevels[2].dropAngle.biasAngle = 40.0f / RadianDegree;
    //levels[2].heading.biasRange.minAngle = -8.0f/ RadianDegree;
    //levels[2].heading.biasRange.maxAngle = 8.0f / RadianDegree;
    m_pLevels[2].heading.biasRange.minAngle = -20.0f/ RadianDegree;
    m_pLevels[2].heading.biasRange.maxAngle = 20.0f / RadianDegree;
    m_pLevels[2].splitList = new split[3];
    m_pLevels[2].splitList[0].probability = 0.0f;
    m_pLevels[2].splitList[0].splitCount = 1;
    m_pLevels[2].splitList[0].type = ORDINARY;
    m_pLevels[2].splitList[1].probability = 0.0f;
    m_pLevels[2].splitList[1].splitCount = 1;
    m_pLevels[2].splitList[1].type = ORDINARY;
    m_pLevels[2].splitList[2].probability = 1.0f;
    m_pLevels[2].splitList[2].splitCount = 2;
    m_pLevels[2].splitList[2].type = ORDINARY;
    m_pLevels[2].brnchType = branchType::BRANCH;
    m_pLevels[2].cnpyType = canopyType::NULL_CANOPY;

    //level 3
	m_pLevels[3].tipPointCount = 7;
    m_pLevels[3].AxialBias.maxAngle = 330.0f / RadianDegree; //convert to radians
    m_pLevels[3].AxialBias.minAngle = 30.0f / RadianDegree;
    m_pLevels[3].length = 4.0f;
    m_pLevels[3].diameter = 1.0f;
    m_pLevels[3].taper = 0.50f;
    m_pLevels[3].LevelID = 3;
    m_pLevels[3].dropAngle.minAngle = 55.0f / RadianDegree;
    m_pLevels[3].dropAngle.maxAngle = 70.0f / RadianDegree;
    m_pLevels[3].dropAngle.biasAngle = 60.0f / RadianDegree;
    //levels[3].heading.biasRange.minAngle = -8.0f/ RadianDegree;
    //levels[3].heading.biasRange.maxAngle = 8.0f / RadianDegree;
    m_pLevels[3].heading.biasRange.minAngle = -20.0f/ RadianDegree;
    m_pLevels[3].heading.biasRange.maxAngle = 20.0f / RadianDegree;
    m_pLevels[3].splitList = new split[2];
    m_pLevels[3].splitList[0].probability = 0.10f;
    m_pLevels[3].splitList[0].splitCount = 1;
    m_pLevels[3].splitList[0].type = REPEAT_CANOPY;
    m_pLevels[3].splitList[1].probability = 0.9f;
    m_pLevels[3].splitList[1].splitCount = 2;
    m_pLevels[3].splitList[1].type = ORDINARY;
    m_pLevels[3].brnchType = branchType::BRANCH;
    m_pLevels[3].cnpyType = canopyType::NULL_CANOPY;

    //level 4
	m_pLevels[4].tipPointCount = 7;
    m_pLevels[4].AxialBias.maxAngle = 330.0f / RadianDegree; //convert to radians
    m_pLevels[4].AxialBias.minAngle = 30.0f / RadianDegree;
    m_pLevels[4].length = 3.0f;
    m_pLevels[4].diameter = 0.5f;
    m_pLevels[4].taper = 0.0f;
    m_pLevels[4].LevelID = 4;
    m_pLevels[4].dropAngle.minAngle = 70.0f / RadianDegree;
    m_pLevels[4].dropAngle.maxAngle = 90.0f / RadianDegree;
    m_pLevels[4].dropAngle.biasAngle = 75.0f / RadianDegree;
    //levels[4].heading.biasRange.minAngle = -20.0f/ RadianDegree;
    //levels[4].heading.biasRange.maxAngle = 20.0f / RadianDegree;
    m_pLevels[4].heading.biasRange.minAngle = -40.0f/ RadianDegree;
    m_pLevels[4].heading.biasRange.maxAngle = 40.0f / RadianDegree;
    m_pLevels[4].splitList = new split[2];
    m_pLevels[4].splitList[0].probability = 0.0f;
    m_pLevels[4].splitList[0].splitCount = 0; // should be 1 when we add leaf node
    m_pLevels[4].splitList[0].type = ORDINARY;
    m_pLevels[4].splitList[1].probability = 1.0f;
    m_pLevels[4].splitList[1].splitCount = 0; // should be 1 when we add leaf node
    m_pLevels[4].splitList[1].type = ORDINARY;
    m_pLevels[4].brnchType = branchType::STEM;
    m_pLevels[4].cnpyType = canopyType::NULL_CANOPY;


    //level 5 leaf (later implement)
    m_pLevels[5].cnpyType = canopyType::HEX;
    m_pLevels[5].brnchType = branchType::NULL_BRANCH;
    m_pCanopyDefinition->perturbFactor = 2.0f;
    m_pLevels[5].splitList = new split[5];
    m_pLevels[5].splitList[0].probability = 0.20f;
    m_pLevels[5].splitList[0].splitCount = 3;
    m_pLevels[5].splitList[0].type=REPEAT_ORDINARY;

    m_pLevels[5].splitList[1].probability = 0.20f;
    m_pLevels[5].splitList[1].splitCount = 4;
    m_pLevels[5].splitList[1].type=REPEAT_ORDINARY;

    m_pLevels[5].splitList[2].probability = 0.10f;
    m_pLevels[5].splitList[2].splitCount = 5;
    m_pLevels[5].splitList[2].type=ORDINARY;

    m_pLevels[5].splitList[3].probability = 0.20f;
    m_pLevels[5].splitList[3].splitCount = 6;
    m_pLevels[5].splitList[3].type=ORDINARY;

    m_pLevels[5].splitList[4].probability = 0.30f;
    m_pLevels[5].splitList[4].splitCount = 0;
    m_pLevels[5].splitList[4].type = ORDINARY;

    m_pLevels[6].cnpyType = canopyType::HEX;
    m_pLevels[6].brnchType = branchType::NULL_BRANCH;
    m_pCanopyDefinition->perturbFactor = 2.0f;
    m_pLevels[6].splitList = new split[5];
    m_pLevels[6].splitList[0].probability = 0.20f;
    m_pLevels[6].splitList[0].splitCount = 3;
    m_pLevels[6].splitList[0].type=REPEAT_ORDINARY;

    m_pLevels[6].splitList[1].probability = 0.20f;
    m_pLevels[6].splitList[1].splitCount = 4;
    m_pLevels[6].splitList[1].type=REPEAT_ORDINARY;

    m_pLevels[6].splitList[2].probability = 0.10f;
    m_pLevels[6].splitList[2].splitCount = 5;
    m_pLevels[6].splitList[2].type=ORDINARY;

    m_pLevels[6].splitList[3].probability = 0.20f;
    m_pLevels[6].splitList[3].splitCount = 6;
    m_pLevels[6].splitList[3].type=ORDINARY;

    m_pLevels[6].splitList[4].probability = 0.30f;
    m_pLevels[6].splitList[4].splitCount = 0;
    m_pLevels[6].splitList[4].type = ORDINARY;

    m_pLevels[7].cnpyType = canopyType::HEX;
    m_pLevels[7].brnchType = branchType::NULL_BRANCH;
    m_pCanopyDefinition->perturbFactor = 2.0f;
    m_pLevels[7].splitList = new split[3];
    m_pLevels[7].splitList[0].probability = 0.10f;
    m_pLevels[7].splitList[0].splitCount = 3;
    m_pLevels[7].splitList[0].type=REPEAT_ORDINARY;

    m_pLevels[7].splitList[1].probability = 0.10f;
    m_pLevels[7].splitList[1].splitCount = 4;
    m_pLevels[7].splitList[1].type= REPEAT_ORDINARY;

    m_pLevels[7].splitList[2].probability = 0.80f;
    m_pLevels[7].splitList[2].splitCount = 0;
    m_pLevels[7].splitList[2].type = ORDINARY;
  
};         

SpikeTree::~SpikeTree()
{
    for(int i=0;i<m_levelCount;i++)
    {
        delete [] (m_pLevels[i].splitList);
    }
    delete [] m_pLevels;
    delete     m_pCanopyDefinition;

}