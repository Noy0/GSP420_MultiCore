#include "AI_CORE.h"
#include "AI_Message.h"
#include "SteeringManager.h"


AI_Core::AI_Core()
{
	m_pSteering = new CMovement_Algorithms();
	mDDSteeringData = 0;
	DDSteeringObstacle = 0;
	PositionData = 0;
	m_AI.LoadAISet();	
	Desire = 0;
	CurrentDesire = 0;
}

AI_Core::~AI_Core()
{
}

void AI_Core::Update(float dt)
{
	CopyMessage();
	AIMessage();

	vector<SteeringData> tempSD;
	if(mDDSteeringData)
	{
		vector<DDSteeringData> Steering;
		mDDSteeringData->Copy(Steering);

		for(int i = 0; i < Steering.size(); ++i)
		{
			SteeringData temp;
			temp.ID = Steering[i].ID;
			temp.m_dSpeed = Steering[i].m_dSpeed;
			temp.m_iEvadeID = Steering[i].m_iEvadeID;
			temp.m_iPursuitID = Steering[i].m_iPursuitID;
			temp.SHeading = Steering[i].SHeading;
			temp.SPosition = Steering[i].SPosition;
			temp.SVelocity = Steering[i].SVelocity;
			temp.m_iEntityType = gSCMgr->GetItem(Steering[i].SteeringType)->m_iEntityType;
			temp.m_iSteeringFlags = gSCMgr->GetItem(Steering[i].SteeringType)->m_iSteeringFlags;
			temp.m_iPursuingType = gSCMgr->GetItem(Steering[i].SteeringType)->m_iPursuingType;
			temp.m_iMinDetectionBoxLength = gSCMgr->GetItem(Steering[i].SteeringType)->m_iMinDetectionBoxLength;
			temp.m_dMaxSpeed = gSCMgr->GetItem(Steering[i].SteeringType)->m_dMaxSpeed;
			temp.m_dRadius = gSCMgr->GetItem(Steering[i].SteeringType)->m_dRadius;
			temp.m_dScale = gSCMgr->GetItem(Steering[i].SteeringType)->m_dScale;
			temp.m_dMaxForce = gSCMgr->GetItem(Steering[i].SteeringType)->m_dMaxForce;

			tempSD.push_back(temp);
			
		}
		
		if(DDSteeringObstacle)
		{
			vector<SteeringObstacles> Obst;
			DDSteeringObstacle->Copy(Obst);

			m_vFinalSteering = m_pSteering->Update(tempSD, Obst);
			for(int i = 0; i < m_vFinalSteering.size(); ++i)
			{
				gCoreMgr->HandleMessage(new SMessageSetForce(&m_vFinalSteering[i].first, D3DXVECTOR3(m_vFinalSteering[i].second.x * 20,
					0, m_vFinalSteering[i].second.y * 20)));
			}
		}
		
	}
	

}

void AI_Core::AIMessage()
{
	queue<SMessage*> tempQueue;
		while(!m_QueueBuffer.empty())
		{
			SMessage* msg = m_QueueBuffer.front();
			switch(msg->Message)
			{
////////////////////////////////////////////////FuzzyLogic/////////////////////////////////////////////////////
				case MSG_ADDRULE:
					{
						SAddRule* Msgx = (SAddRule*)msg;
						m_Module.AddRule(Msgx->Ant,Msgx->Con);
						delete msg;
						break;
					}

				case MSG_FUZZY:
					{
						SFuzzify* Msgx = (SFuzzify*)msg;
						m_Module.Fuzzify(Msgx->FLV,Msgx->FuzzVal);
						delete msg;
						break;
					}

				case MSG_DEFUZZY:
					{
						SDefuzzify* Msgx = (SDefuzzify*)msg;
						m_Module.DeFuzzify(Msgx->DLV);
						delete msg;
						break;
					}
								
				
				case MSG_AITURN:
					{
						SMessageAITurn* Msgx = (SMessageAITurn*)msg;
						AI_Turn(Msgx->CameraPos, Msgx->AI_Target);
						delete msg;
						break;
					}
////////////////////////////////////////////////Movement/////////////////////////////////////////////////////
				case MSG_REGISTERSTEERING:
					{
						SMessageRegisterSteering* Msgx = (SMessageRegisterSteering*)msg;
						mDDSteeringData = Msgx->Steering;
						DDSteeringObstacle = Msgx->Obst;
						delete msg;
						break;
					}
				case MSG_AIREGISTERPOSITION:
					{
						SMessageRegisterAIPosition* Msgx = (SMessageRegisterAIPosition*)msg;
						PositionData = Msgx->EntityPos;
						delete msg;
						break;
					}
///////////////////////////////////////////////PathFinding///////////////////////////////////////////////////
				case MSG_CREATEGRAPH:
					{
						SCreateGraph* Msgx = (SCreateGraph*)msg;
						m_pPathPlanner.CreateGraph(Msgx->iCellsUp, Msgx->iCellsAcross);
						delete msg;
						break;
					}
				case MSG_CHANGESOURCE:
					{
						SChangeSource* Msgx = (SChangeSource*)msg;
						m_pPathPlanner.ChangeSource(Msgx->iCell);
						delete msg;
						break;
					}
				case MSG_CHANGETARGET:
					{
						SChangeTarget* Msgx = (SChangeTarget*)msg;
						m_pPathPlanner.ChangeTarget(Msgx->iCell);
						delete msg;
						break;
					}
				case MSG_CREATEPATHASTAR:
					{
						SCreatePathAStar* Msgx = (SCreatePathAStar*)msg;
						m_pPathPlanner.CreatePathAStar();
						delete msg;
						break;
					}
				case MSG_LOAD:
					{
						SLoad* Msgx = (SLoad*)msg;
						m_pPathPlanner.Load(Msgx->cFileName);
						delete msg;
						break;
					}
				case MSG_SAVE:
					{
						SSave* Msgx = (SSave*)msg;
						m_pPathPlanner.Save(Msgx->cFileName);
						delete msg;
						break;
					}
				case MSG_POINTTOINDEX:
					{
						SPointToIndex* Msgx = (SPointToIndex*)msg;
						m_pPathPlanner.PointToIndex(Msgx->pp, Msgx->iNodeIndex);
						delete msg;
						break;
					}
				case MSG_GETPATH:
					{
						SGetPath* Msgx = (SGetPath*)msg;
						m_pPathPlanner.GetPath();
						delete msg;
						break;
					}
				

			}

			m_QueueBuffer.pop();
		}
		m_QueueBuffer = tempQueue;
}

void AI_Core::AI_Turn(D3DXVECTOR3 Camera,D3DXVECTOR3 Target)
{	

	AI_Camera = Camera;

	vector<DDGameData> EntityPos;
	PositionData->Copy(EntityPos);

	for(unsigned int i = 0; i < EntityPos.size(); i++)
	{
			InFOV = m_AI.IsInFOV(AI_Camera,Target,EntityPos[i].Position);
			
			if(InFOV)
			{
				
				AI_ID.push_back(EntityPos[i].ID);

			}
	}
	
	for(unsigned int i = 0; i < EntityPos.size(); i++)
	{
		if(EntityPos[i].Type == 1)
		{
		CurrentMarbleDist = m_AI.GetDistance(EntityPos[i].Position);
		}
			for(unsigned int j = 1; j < EntityPos.size(); j++)
			{
					if(EntityPos[j].Type == 1)
					{
						BallBlock = m_AI.OnLine(AI_Camera, EntityPos[i].Position,EntityPos[j].Position);
					}
					else if(EntityPos[j].Type == 3)
					{
						ObjBlock = m_AI.OnLine(AI_Camera, EntityPos[i].Position,EntityPos[j].Position);
					}

					
						Desire = m_AI.GetDesirability(CurrentMarbleDist,BallBlock,ObjBlock);

						if(Desire > CurrentDesire)
						{
								CurrentDesire = Desire;
								TopDesireEntity = EntityPos[i].ID;
								TopDesire.x = EntityPos[TopDesireEntity].Position.x;
								TopDesire.z = EntityPos[TopDesireEntity].Position.y;
								TopDesire.y = 0.0f;
						}
					
					
			}
	
	}


	
					
	gCoreMgr->HandleMessage(new SMessageCameraLook(TopDesire));


	static int temp = 100;
	gCoreMgr->HandleMessage( new SMessageAddEntity( &temp, EP_PLAYERMARBLE, AI_Camera ) );
	gCoreMgr->HandleMessage( new SMessageSetVelocity( &temp, (D3DXVECTOR3(0,0,0) - AI_Camera) * 1 ) );

	return;
	//gCoreMgr->HandleMessage(new SMessageCameraLook(D3DXVECTOR3(0,0,0)));
}

void AI_Core::LoadGraph()
{

	m_pPathPlanner.CreateGraph(19,19);

	PathPlanner::NavGraph pGraph = *m_pPathPlanner.GetGraph();

	m_pPathPlanner.TrimGraphForCircle();

}

void AI_Core::SetPath()
{
	vector<DDGameData> ObjPos;
	PositionData->Copy(ObjPos);

	for(unsigned int i = 0; i < ObjPos.size(); i++)
	{
		if(ObjPos[i].Type == 3)
		{
				m_TargetNode = rand() % m_pPathPlanner.m_pGraph->NumNodes();
				

				for (int i=0; i<m_pPathPlanner.m_pGraph->NumNodes(); ++i)
				{
					NavGraphNode<void*> ngn = m_pPathPlanner.m_pGraph->GetNode(i);
					if(ngn.Pos().x == ObjPos[i].Position.x && ngn.Pos().y == ObjPos[i].Position.z)
					{
						
						m_CurNode = ngn.Index();
						ObjInd = i;
						//v_Nodes.push_back(m_CurNode);
						
					}
				}

		}

	}

			m_pPathPlanner.ChangeSource(m_CurNode);

			m_pPathPlanner.ChangeTarget(m_TargetNode);
			m_pPathPlanner.CreatePathAStar();
			m_CurPath = m_pPathPlanner.GetPath();

			
			
			//for(int it = m_CurPath.begin(); it < m_CurPath.end(); it++)
			//{
			//	NavGraphNode<void*> ngn = m_pPathPlanner.m_pGraph->GetNode(it);
			//	
			//	ObjPos[ObjInd].Position;
			//	SVector3D temp;
			//	D3DXVECTOR3 temp1;
			//	temp1.x = ngn.Pos().x;
			//	temp1.z = ngn.Pos().y;
			//	temp = m_pSteering->AriveTemp(ObjPos[ObjInd].Position,temp1);
			//	ObjPos[ObjInd].Position.x = temp.x;
			//	ObjPos[ObjInd].Position.z = temp.y;
			//}
}
