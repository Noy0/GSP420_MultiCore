#include "AIplayer.h"


Player_AI::Player_AI()
{
	FOV = 20.0;
	origin = SVector3D(0.0f,0.0f);
	LoadAISet();
}

void Player_AI::LoadAISet()
{
  Variables& MarbleDistFromCenter = g_Module.CreateFLV("MarbleDistFromCenter");
  
  ProxySet& Target_Close = MarbleDistFromCenter.AddLeftShoulderSet("Target_Close", 0, 15, 30);
  ProxySet& Target_Medium = MarbleDistFromCenter.AddTriangularSet("Target_Medium", 30, 50, 70);
  ProxySet& Target_Far = MarbleDistFromCenter.AddRightShoulderSet("Target_Far", 70, 85, 100);

  Variables& IsBlockedByObject = g_Module.CreateFLV("IsBlockedByObject");
  
  ProxySet& Target_NotBlockedByObj = IsBlockedByObject.AddSingletonSet("Target_NotBlockedByObj", 0, 0, 0);
  ProxySet& Target_BlockedByObj = IsBlockedByObject.AddSingletonSet("Target_BlockedByObj", 1,1,1);

  Variables& IsBlockedByBall = g_Module.CreateFLV("IsBlockedByBall");
  
  ProxySet& Target_NotBlockedByBall = IsBlockedByBall.AddSingletonSet("Target_NotBlockedByBall", 0, 0, 0);
  ProxySet& Target_BlockedByBall = IsBlockedByBall.AddSingletonSet("Target_BlockedByBall", 1,1,1);
  

  Variables& AIDesirability = g_Module.CreateFLV("AIDesirability");
  
  ProxySet& VeryDesirable = AIDesirability.AddRightShoulderSet("VeryDesirable", 50, 75, 100);
  ProxySet& Desirable = AIDesirability.AddTriangularSet("Desirable", 25, 50, 75);
  ProxySet& Undesirable = AIDesirability.AddLeftShoulderSet("Undesirable", 0, 25, 50);


  g_Module.AddRule(Target_Close,Undesirable);
  g_Module.AddRule(Target_Medium,Desirable);
  g_Module.AddRule(Target_Far,VeryDesirable);

  g_Module.AddRule(Target_NotBlockedByObj,VeryDesirable);
  g_Module.AddRule(Target_BlockedByObj,Undesirable);
  
  g_Module.AddRule(Target_NotBlockedByBall,VeryDesirable);
  g_Module.AddRule(Target_BlockedByBall,Desirable);

}

int Player_AI::GetDesirability(int MarbleDistFromCenter, bool Ball, bool Obj)
{
	static int m_dLastScore;
	  
    g_Module.Fuzzify("MarbleDistFromCenter", MarbleDistFromCenter);
    g_Module.Fuzzify("IsBlockedByObject", Obj);
	g_Module.Fuzzify("IsBlockedByBall", Ball);

    m_dLastScore = g_Module.DeFuzzify("AIDesirability");
  

  return m_dLastScore;
 
}

bool Player_AI::IsInFOV(D3DXVECTOR3 CamPos,D3DXVECTOR3 Facing,D3DXVECTOR3 MarblePos)
{
	AI_CamPos.x = CamPos.x;
	AI_CamPos.y = CamPos.z;
	AI_Facing.x = Facing.x;
	AI_Facing.y = Facing.z;
	AI_MarblePos.x = MarblePos.x;
	AI_MarblePos.x = MarblePos.z;

	InFOV = isSecondInFOVOfFirst(AI_CamPos,AI_Facing,AI_MarblePos,FOV);

	return InFOV;
}

double Player_AI::GetDistance(D3DXVECTOR3 target)
{
	double ySeparation = target.y - origin.y;
	 double xSeparation = target.x - origin.x;

	return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}

bool Player_AI::OnLine(D3DXVECTOR3 Camera,D3DXVECTOR3 target, D3DXVECTOR3 checked)
{
		slope  = ((target.y - Camera.z)/(target.x - Camera.x));
		double checkSlope1 = ((checked.z - Camera.z)/(checked.x - Camera.x));
		double checkSlope2 = ((checked.z - target.z)/(checked.x - target.x));

		if(checkSlope1 == checkSlope2)
			return true;


		return false;
	
}