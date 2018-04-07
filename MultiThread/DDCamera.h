#ifndef _DDCAMERA_H
#define _DDCAMERA_H

#include <d3dx9.h>
#include "DynamicData.h"

struct DDCameraData
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Target;
	D3DXVECTOR3 Up;
	float Width;
	float Height;
	D3DXMATRIX Projection;
	D3DXMATRIX View;
	DDCameraData() : Position(0, 0, 0), Target(1, 0, 0), Up(0, 1, 0), Width(800), Height(600)
	{
		D3DXMatrixLookAtLH(&View, &Position, &Target, &Up);
        D3DXMatrixPerspectiveFovLH(&Projection, 1.0f, 1.0f, 1.0f, 1000.0f);
	}
};

class DD_Camera : public DynamicData<DDCameraData>
{
public:
	DD_Camera() : DynamicData("TimwuRule_Camera"){}
};

#endif