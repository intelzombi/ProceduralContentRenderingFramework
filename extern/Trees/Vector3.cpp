//Vector Class
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

#include "Vector3.h"



V3::V3(){};

V3::V3(float a, float b, float c)
{
	x = a;
	y = b;
	z = c;
}

V3::V3( const D3DXVECTOR3 &v )
{
	x = v.x;
	y = v.y;
	z = v.z;
}


float V3::Length( void ) const
{
    float magnitude = ( x * x ) + 
			          ( y * y ) +
			          ( z * z );
	return sqrtf ( magnitude);
}

V3& V3::Normalize( )
{
	float len = Length();
	if ( len > 0.0f ) 
	{
		len = 1.0f / len;
		x *= len;
		y *= len;
		z *= len;
	}
	else
	{
	}
	return *this;
}


V2::V2(){};

V2::V2(float a, float b)
{
	x = a;
	y = b;
}

V2::V2( const D3DXVECTOR2 &v )
{
	x = v.x;
	y = v.y;
}

float	DotProduct ( const V3& v1, const V3& v2 ) 
{
	
	
	return ( v1.x * v2.x ) + ( v1.y * v2.y ) + ( v1.z * v2.z );
}


V3	CrossProduct ( const V3& v1, const V3& v2 ) 
{	
	return V3 (( v1.y * v2.z ) - ( v1.z * v2.y ),
				   ( v1.z * v2.x ) - ( v1.x * v2.z ),
				   ( v1.x * v2.y ) - ( v1.y * v2.x ));
}
void B3::spin()
{
}