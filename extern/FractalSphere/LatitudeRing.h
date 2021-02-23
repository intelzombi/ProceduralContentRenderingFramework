//Latitude Class
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

#ifndef _Latitude
#define _Latitude
#include "vector3.h"
enum LatitudeType {NORTH_POLE=0x01, BASIC_LATITUDE=0x10, EQUATOR=0x100, SOUTH_POLE=0x1000};

class Latitude {
public: 
    Latitude(){};
    ~Latitude();
	void initialize();
    V3 *m_latitudePointList;//1 center point c0 and latitudePointCount rim points.
    V3 *m_PointNormals;
    V3 m_AxisHeading; // this is the normal from the plane of the latitudePointList
    V3 m_pointOHeading; // the Normal vector between the center c0 and the rim point p0.
    V3 m_AxisZ; // cross of axisHeading and OHeading.
    LatitudeType m_type; 
    //float m_sliceAngle; // radius: information comes from previous Latitude
    float m_radius;  // radius
	int m_latitudePointCount;
private:
};
#endif
