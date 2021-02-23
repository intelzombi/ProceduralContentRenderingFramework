//Segment Class
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

#ifndef _Segment
#define _Segment
//#define TipPointCount 11
#include "vector3.h"
enum SegmentType {INITIAL_SEGMENT=0x01, BASIC_SEGMENT=0x10, ONLY_SEGMENT=0x100, TIP_SEGMENT=0x1000};

class Segment {
public: 
    Segment(){};
    ~Segment();
    Segment(float baseWidth, float tipWidth);
	void initialize(int tpc);
    V3 *m_tipPointList;//1 center point c0 and 6 rim points p0-p0''''', tps change
    V3 m_AxisHeading; // unlike the branch heading this is the normal from the plane of the tipPointList
    V3 m_pointOHeading; // the Normal vector between the center c0 and the rim point p0.
    SegmentType m_type; 
    float m_base; // radius: information comes from previous segment
    float m_tip;  // radius
	int m_tipPointCount;
private:
};
#endif
