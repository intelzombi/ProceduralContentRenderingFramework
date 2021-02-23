#pragma once
#include <d3dx10.h>
//#include <dxerr.h>

namespace camera {
typedef struct _Camera {
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Right;
	D3DXVECTOR3 Up;
	D3DXVECTOR3 Look;

	D3DXMATRIX View;
	D3DXMATRIX Proj;
} Camera;

void initCamera();
void setLens(float fovY, float aspect, float zn, float zf);
void rebuildView();
void pitch(float angle);
void rotateY(float angle);
void strafe(float d);
void walk(float d);
void rise(float d);

}

