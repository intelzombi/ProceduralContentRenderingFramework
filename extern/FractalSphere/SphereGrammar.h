//SphereGrammar Class
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

#ifndef _SphereGrammar
#define _SphereGrammar
#include "Vector3.h"
#include <string>

#pragma warning( push )
#pragma warning( disable : 4635 6011 )
#include "TinyXml.h"
#pragma warning( pop )

#define RadianDegree (float)57.29578

enum treeType {FRACTAL, ELIPTICAL, CLUSTER, LINEAR};
enum sphereType {TRUNK, CHILD,  ELIPSE};
//Ordinary:  no specified split; random in twist and follows normal drop angle pattern
//Opposed: First split continues on in the heading of the last segment of the previous sphere
//basically continues in the general direction(straight) but it progresses to the next grammar level
//Repeat_Opposed:  Same as opposed except it repeats the grammar level
//Spear: no use at this point
//Square: no use at this point
//Repeat_Square: no use at this point
//Uniform: no use at this point
enum splitType {ORDINARY, REPEAT_ORDINARY, FRACTAL_60_FULL, FRACTAL_60, FRACTAL_E, FRACTAL_I, FRACTAL_IEND, FRACTAL_Y, FRACTAL_YPEAT, FRACTAL_T, FRACTAL_S,TERMINAL};

struct split{
    int splitCount;
    float probability;
    splitType type;
};

struct range{
    float minAngle;
    float maxAngle;
    float biasAngle;
};

struct plumbBias{
    V3 biasDirection;
    range biasRange;
};

struct HeadingBias{
    V3 Heading;
    range biasRange;
};



struct LevelDetail {

    int LevelID;
    split *splitList;
    HeadingBias heading;
    float dropAngle;
    range AxialBias;
    float radius; 
    int maxLatitudeCount;
	int latitudePointCount;
	int longitudePointCount;
    sphereType sphrType;
    int splitCount;
};


class Grammar {
public:
    LevelDetail *m_pLevels;
    int m_levelCount;
    std::string m_species;
	std::string m_filename;
    treeType m_type;
    Grammar(){ 
    };
	Grammar(std::string fn){ 
		m_filename = fn;
		loadLevel(m_filename);
	};
    ~Grammar(){
        for (int i=0;i<m_levelCount;i++){
            delete [](split*)(m_pLevels[i].splitList);
        }
        delete [](LevelDetail*)m_pLevels;
    };
	void loadLevel(std::string filename);
    LevelDetail *m_pCurrentLevel;
};
#endif
