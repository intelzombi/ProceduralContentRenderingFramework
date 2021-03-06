//Latitude Class
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

#include "LatitudeRing.h"


void Latitude::initialize()
{
	m_latitudePointList = new V3[m_latitudePointCount];
	m_PointNormals = new V3[m_latitudePointCount];
}

Latitude::~Latitude()
{
    delete []m_latitudePointList;
    m_latitudePointList=0;
    delete []m_PointNormals;
    m_PointNormals=0;
}

