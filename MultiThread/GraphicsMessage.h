#ifndef _GRAPHICSMESSAGE_H
#define _GRAPHICSMESSAGE_H

#include "Message.h"
#include <d3dx9.h>
#include "EScene.h"
#include "TextureList.h"
#include "StaticMeshList.h"
#include "FontList.h"

enum EGraphicsMessage
{
	MSG_NULL,
	MSG_REGISTERSCENELIST,
	MSG_ADDTEXTURE,
	MSG_ADDSTATICMESH,
	MSG_ADDTEXT,
	MSG_CHANGETEXTURE,
	MSG_CHANGEMATRIX,
	MSG_CHANGEUP,
	MSG_CHANGELOOK,
	MSG_CHANGEPOS,
	MSG_ROTATECAMERA
};

struct SMessageRegisterSceneList : public SMessage
{
	SMessageRegisterSceneList(int* idOut, int type, void* item) : 
		SMessage(CORE_GRAPHICS, MSG_REGISTERSCENELIST),
		IDOut(idOut), Type(type), Item(item)
	{}
	int* IDOut;
	int Type;
	void* Item;
};

struct SMessageAddTexture : public SMessage
{
	SMessageAddTexture(int* id, int texture, D3DXVECTOR3 position) : SMessage(CORE_GRAPHICS, MSG_ADDTEXTURE)
	{
		ID = id;
		Texture = texture;
		Position = position;
	}

	~SMessageAddTexture(){}

	int* ID;
	int Texture;
	D3DXVECTOR3 Position;
};

struct SMessageAddStaticMesh : public SMessage
{
	SMessageAddStaticMesh(int *id, int mesh, D3DXMATRIX transform) : SMessage(CORE_GRAPHICS, MSG_ADDSTATICMESH)
	{
		ID = id;
		Mesh = mesh;
		Transform = transform;
	}
	int* ID;
	int Mesh;
	D3DXMATRIX Transform;
};

struct SMessageSetMatrix : public SMessage
{
	SMessageSetMatrix(int *id, D3DXMATRIX matrix) : SMessage(CORE_GRAPHICS, MSG_CHANGEMATRIX)
	{
		ID = id;
		Matrix = matrix;
	}

	~SMessageSetMatrix(){}
	int* ID;
	D3DXMATRIX Matrix;
};

struct SMessageRotateCamera : public SMessage
{
	SMessageRotateCamera(float angle) : SMessage(CORE_GRAPHICS, MSG_ROTATECAMERA)
	{
		Angle = angle;
	}
	float Angle;
};

struct SMessageCameraUp : public SMessage
{
	SMessageCameraUp(D3DXVECTOR3 Up) : SMessage(CORE_GRAPHICS, MSG_CHANGEUP)
	{
		CameraUp = Up;
	}
	D3DXVECTOR3 CameraUp;
};

struct SMessageCameraLook : public SMessage
{
	SMessageCameraLook(D3DXVECTOR3 Look) : SMessage(CORE_GRAPHICS, MSG_CHANGELOOK)
	{
		CameraLook = Look;
	}
	D3DXVECTOR3 CameraLook;
};

struct SMessageCameraPos : public SMessage
{
	SMessageCameraPos(D3DXVECTOR3 Pos) : SMessage(CORE_GRAPHICS, MSG_CHANGEPOS)
	{
		CameraPos = Pos;
	}
	D3DXVECTOR3 CameraPos;
};

#endif