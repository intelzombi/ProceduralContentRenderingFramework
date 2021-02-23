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

#ifndef _Vector3
#define _Vector3
#include <d3dx9.h>

class	V3	: public D3DXVECTOR3
{
public:

	V3();

	V3(float a, float b, float c);
	V3( const D3DXVECTOR3 &v );

	float Length( void ) const;

   	V3& Normalize( );
    V3 operator+( const V3& Vector ) const
    {
        V3 v(x + Vector.x, 
             y + Vector.y, 
             z + Vector.z );
        return v;
    }

    V3 operator-( const V3& Vector ) const
    {
        V3 v(x - Vector.x, 
             y - Vector.y, 
             z - Vector.z);
        return v;
    }

};

class	V2	: public D3DXVECTOR2
{
public:

	V2();

	V2(float a, float b);
	V2( const D3DXVECTOR2 &v );

};

class B3
{
public:
    B3(){};
    B3(V3 i,V3 j, V3 k)
    { 
        iAxis = i;
        jAxis = j;
        kAxis = k;
    };
    void normalize()
    {
        iAxis.Normalize();
        jAxis.Normalize();
        kAxis.Normalize();
    };
    void spin();
    V3 iAxis;
    V3 jAxis;
    V3 kAxis;

};


float	DotProduct ( const V3& v1, const V3& v2 );
V3	CrossProduct ( const V3& v1, const V3& v2 );

#endif
