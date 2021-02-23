#include "Camera.h"
//extern Camera              g_Camera;

extern camera::Camera g_Camera;
namespace camera {
void initCamera()
{
	g_Camera.Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_Camera.Right    = D3DXVECTOR3(-1.0f, 0.0f, 0.0f); // -Z axis projecting from camera
	g_Camera.Right    = D3DXVECTOR3(1.0f, 0.0f, 0.0f);// +Z axis projecting from camera
	g_Camera.Up       = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//g_Camera.Look     = D3DXVECTOR3(0.0f, 0.0f, -1.0f); // -Z axis projecting from camera
	g_Camera.Look     = D3DXVECTOR3(0.0f, 0.0f, 1.0f); // +Z axis projecting from camera

	D3DXMatrixIdentity(&g_Camera.View);
	D3DXMatrixIdentity(&g_Camera.Proj);
    
    //return g_Camera;
}

void setLens(float fovY, float aspect, float zn, float zf)
{
	D3DXMatrixPerspectiveFovLH(&g_Camera.Proj, fovY, aspect, zn, zf);
}

void strafe(float d)
{
	g_Camera.Position += d*g_Camera.Right;
}

void walk(float d)
{
	g_Camera.Position += d*g_Camera.Look;
}

void rise(float d)
{
	g_Camera.Position += d*g_Camera.Up;
}

void pitch(float angle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &g_Camera.Right, angle);

	D3DXVec3TransformNormal(&g_Camera.Up, &g_Camera.Up, &R);
	D3DXVec3TransformNormal(&g_Camera.Look, &g_Camera.Look, &R);
}

void rotateY(float angle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationY(&R, angle);

	D3DXVec3TransformNormal(&g_Camera.Right, &g_Camera.Right, &R);
	D3DXVec3TransformNormal(&g_Camera.Up, &g_Camera.Up, &R);
	D3DXVec3TransformNormal(&g_Camera.Look, &g_Camera.Look, &R);
}

void rebuildView()
{
	// Keep camera's axes orthogonal to each other and of unit length.
	D3DXVec3Normalize(&g_Camera.Look, &g_Camera.Look);

	D3DXVec3Cross(&g_Camera.Up, &g_Camera.Look, &g_Camera.Right);
	D3DXVec3Normalize(&g_Camera.Up, &g_Camera.Up);

	D3DXVec3Cross(&g_Camera.Right, &g_Camera.Up, &g_Camera.Look);
	D3DXVec3Normalize(&g_Camera.Right, &g_Camera.Right);

	// Fill in the view matrix entries.
	float x = -D3DXVec3Dot(&g_Camera.Position, &g_Camera.Right);
	float y = -D3DXVec3Dot(&g_Camera.Position, &g_Camera.Up);
	float z = -D3DXVec3Dot(&g_Camera.Position, &g_Camera.Look);

	g_Camera.View(0,0) = g_Camera.Right.x; 
	g_Camera.View(1,0) = g_Camera.Right.y; 
	g_Camera.View(2,0) = g_Camera.Right.z; 
	g_Camera.View(3,0) = x;   

	g_Camera.View(0,1) = g_Camera.Up.x;
	g_Camera.View(1,1) = g_Camera.Up.y;
	g_Camera.View(2,1) = g_Camera.Up.z;
	g_Camera.View(3,1) = y;  

	g_Camera.View(0,2) = g_Camera.Look.x; 
	g_Camera.View(1,2) = g_Camera.Look.y; 
	g_Camera.View(2,2) = g_Camera.Look.z; 
	g_Camera.View(3,2) = z;   

	g_Camera.View(0,3) = 0.0f;
	g_Camera.View(1,3) = 0.0f;
	g_Camera.View(2,3) = 0.0f;
	g_Camera.View(3,3) = 1.0f;
}

//Camera& GetCamera()
//{
//	
//	return g_Camera;
//}

}