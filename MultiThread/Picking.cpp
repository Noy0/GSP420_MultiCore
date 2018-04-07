#include "Picking.h"
#include <cmath>



//using namespace std;
int Picking::picking(POINT mousePos, D3DXMATRIX proj, D3DXMATRIX view, vector<int> id, vector<D3DXVECTOR3> pos, vector<D3DXVECTOR3> halfscale, float screenW, float screenH)
{	
	int temp;
	int pickedID = 0;
	PickingCheck tempStruct;
	//D3DXVECTOR3 tempHalfScale;
	//	tempHalfScale.x = 5;
	//	tempHalfScale.y = 5;
	//	tempHalfScale.z = 5;




	D3DXVECTOR3 originW;
		originW.x = 0;
		originW.y = 0;
		originW.z = 0;

	D3DXVECTOR3 dirW;
		dirW.x = 0;
		dirW.y = 0;
		dirW.z = 0;

	getWorldPickingRay(mousePos, &originW, &dirW,proj,view,screenW,screenH);
	
	for(int i = 1;i < id.size(); i++)
	{
		temp = isPicked(pos[i], halfscale[i], &originW, &dirW);
		if(temp == 1)
		{
			tempStruct.objectPos = pos[i];
			tempStruct.objectID = id[i];
			mCheck.push_back(tempStruct);
			
		}

		
	}

	pickedID = distanceCheck(&originW);
	mCheck.clear();
	return pickedID;
	
}
int Picking::isPicked(D3DXVECTOR3 objectPos
		, D3DXVECTOR3 halfscale,D3DXVECTOR3* originW, D3DXVECTOR3* dirW)
{
		D3DXVECTOR3 aabbMinPt = D3DXVECTOR3(objectPos.x - halfscale.x,  objectPos.y - halfscale.y,  objectPos.z - halfscale.z);
		D3DXVECTOR3 aabbMaxPt = D3DXVECTOR3(objectPos.x + halfscale.x,  objectPos.y + halfscale.y,  objectPos.z + halfscale.z);
		D3DXMATRIX R, T;
		D3DXVECTOR3 objectRotation = D3DXVECTOR3(0,0,0);
		float objectTheta = 0;
		D3DXMatrixRotationAxis(&R, &objectRotation, objectTheta);
		D3DXMatrixTranslation(&T, objectPos.x, objectPos.y, objectPos.z);

		D3DXMATRIX toWorld = R*T;

		// Did we pick it?
		if(D3DXBoxBoundProbe(&aabbMinPt, &aabbMaxPt, originW, dirW))
		{
			return 1;
		}
		else 
			return 0;


}

void Picking::getWorldPickingRay(POINT mousePos, D3DXVECTOR3* originW, D3DXVECTOR3* dirW, D3DXMATRIX proj, D3DXMATRIX view,
								 float screenW, float screenH)
{
	float w = screenW;
	float h = screenH;

	float x = (2.0f*mousePos.x/w -1.0f) / proj(0,0);
	float y = (-2.0f*mousePos.y/h +1.0f) / proj(1,1);

	D3DXVECTOR3 origin(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 dir(x,y,1.0f);

	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView,0,&view);

	D3DXVec3TransformCoord(originW, &origin, &invView);
	D3DXVec3TransformNormal(dirW, &dir, &invView);
	D3DXVec3Normalize(dirW, dirW);
}

int Picking::distanceCheck(D3DXVECTOR3* originW)
{
	int closestID = 0;
	double closestDistance = 0;
	double currentDistance;
	double dx;
	double dy;
	double dz;
	if(mCheck.size() > 0)
	{
		for(int i = 0; i < mCheck.size(); i++)
		{
			dx = mCheck[i].objectPos.x - originW->x;
			dy = mCheck[i].objectPos.y - originW->y;
			dz = mCheck[i].objectPos.z - originW->z;
			
			currentDistance = sqrt(abs(dx)*abs(dx) + abs(dy)*abs(dy) + abs(dz)*abs(dz));
			if(abs(currentDistance) < closestDistance || closestDistance == 0)
			{
				closestID = mCheck[i].objectID;
			}

		}
	}	

	return closestID;
}