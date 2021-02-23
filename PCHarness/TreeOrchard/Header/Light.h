#pragma once
#include <d3dx10.h>
//#include <dxerr.h>

namespace light {
	enum LightType {Directional, Point};

typedef struct _Light {
	public:
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Direction;	
		
	D3DXCOLOR	Color;

	LightType	Type;


	void initLight();
	void setDirection(float x, float y, float z);
	void setPosition(float x, float y, float z);
    void setColor(float r, float g, float b, float a);
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);
	void strafe(float d);
	void walk(float d);
	void rise(float d);
} Light;
}