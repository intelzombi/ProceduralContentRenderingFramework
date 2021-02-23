#include "Light.h"


namespace light {

void Light::initLight()
{
	Position  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Direction = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	Color     = D3DXCOLOR  (1.0f, 1.0f, 1.0f, 1.0f);
	Type      = LightType::Directional; 

}

void Light::setDirection(float x, float y, float z)
{
	Direction = D3DXVECTOR3(x,y,z);
}

void Light::setPosition(float x, float y, float z)
{
	Position = D3DXVECTOR3(x,y,z);
}

void Light::setColor(float r, float g, float b, float a)
{
	Color = D3DXCOLOR(r,g,b,a);
}

void Light::strafe(float d)
{
	Position.x += d*Position.x;
}

void Light::walk(float d)
{
	Position.z += d*Position.z;
}

void Light::rise(float d)
{
	Position.y += d*Position.y;
}


void Light::rotateX(float angle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationX(&R, angle);

	D3DXVec3TransformNormal(&Direction, &Direction, &R);
}

void Light::rotateY(float angle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationY(&R, angle);

	D3DXVec3TransformNormal(&Direction, &Direction, &R);
}

void Light::rotateZ(float angle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationZ(&R, angle);

	D3DXVec3TransformNormal(&Direction, &Direction, &R);
}

}