#include "ResourceLoader.h"
#include "TextureManager.h"
#include "TextureList.h"
#include "StaticMeshManager.h"
#include "StaticMeshList.h"
#include "FontManager.h"
#include "FontList.h"
#include "UIMessage.h"
#include "EntityPropertyManager.h"
#include "EntityPropertyList.h"
#include "SteeringManager.h"
#include "SteeringList.h"
#include "CoreManager.h"
#include "UIList.h"

// Modified by TP (to enable PhysicsMats, and to add new shapes for new physics models)
// & integrated changes from AI team

void ResourceLoader::Load()
{
	if(gTextureMgr)
	{
		//Load all textures
		gTextureMgr->AddItemFromFile(TEXTURE_WHITE, "whitetex.dds");
		gTextureMgr->AddItemFromFile(TEXTURE_BUTTON_RED, "Button_Red.bmp");
		gTextureMgr->AddItemFromFile(TEXTURE_BUTTON_RED2, "Button_Red2.bmp");
		gTextureMgr->AddItemFromFile(TEXTURE_BUTTON_RED3, "Button_Red3.bmp");
		gTextureMgr->AddItemFromFile(TEXTURE_RECTANGLE, "Rectangle.bmp");

		gTextureMgr->AddItemFromFile(TEXTURE_RIGHT_NOTCLICKED, "Resources/Images/Arrows2.bmp");
		gTextureMgr->AddItemFromFile(TEXTURE_RIGHT_CLICKED, "Resources/Images/Arrows2D.bmp");
		gTextureMgr->AddItemFromFile(TEXTURE_RIGHT_OVER, "Resources/Images/Arrows2P.bmp");

		gTextureMgr->AddItemFromFile(TEXTURE_LEFT_NOTCLICKED, "Resources/Images/Arrow1.bmp");
		gTextureMgr->AddItemFromFile(TEXTURE_LEFT_CLICKED, "Resources/Images/ArrowD1.bmp");
		gTextureMgr->AddItemFromFile(TEXTURE_LEFT_OVER, "Resources/Images/ArrowP1.bmp");

		gTextureMgr->AddItemFromFile(TEXTURE_SPLAYER, "Resources/Images/SinglePlayer1.bmp");
		gTextureMgr->AddItemFromFile(TEXTURE_SPLAYER_CLICKED, "Resources/Images/SinglePlayer2.bmp");
		//gTextureMgr->AddItemFromFile(TEXTURE_SPLAYER_OVER, "SPlayerover.bmp");
		gTextureMgr->AddItemFromFile(TEXTURE_HOST, "Resources/Images/HostB.bmp");
		gTextureMgr->AddItemFromFile(TEXTURE_HOSTOVER, "Resources/Images/HostBD.bmp");
		gTextureMgr->AddItemFromFile(TEXTURE_JOIN, "Resources/Images/JoinB.bmp");
		gTextureMgr->AddItemFromFile(TEXTURE_JOINOVER, "Resources/Images/JoinBD.bmp");
		gTextureMgr->AddItemFromFile(TEXTURE_BACKGROUND1, "Resources/Images/Bg1.bmp");

		gTextureMgr->AddItemFromFile(TEXTURE_POWERBARFRONT, "Resources/Images/PowerBarAlpha.bmp");
		gTextureMgr->AddItemFromFile(TEXTURE_POWERBARBACK, "Resources/Images/PowerBar.bmp");
		gTextureMgr->AddItemFromFile(TEXTURE_POWERBAR, "Resources/Images/BGimage4PB.bmp");
	}
	if(gStaticMeshMgr)
	{
		//Load all static meshes
		gStaticMeshMgr->AddItemFromFile(STATICMESH_SKULL, "Resources/Static Mesh/skullocc.x");
		gStaticMeshMgr->AddItemFromFile(STATICMESH_SOLDIER, "Resources/Static Mesh/soldier.x");
		gStaticMeshMgr->AddItemFromFile(STATICMESH_SPHERE, "Resources/Static Mesh/Sphere.x");
		gStaticMeshMgr->AddItemFromFile(STATICMESH_SPHERE1, "Resources/Static Mesh/Sphere1.x");
		gStaticMeshMgr->AddItemFromFile(STATICMESH_SPHERE2, "Resources/Static Mesh/Sphere2.x");
		gStaticMeshMgr->AddItemFromFile(STATICMESH_SPHERE3, "Resources/Static Mesh/Sphere3.x");
		gStaticMeshMgr->AddItemFromFile(STATICMESH_CUBE, "Resources/Static Mesh/Cube.x");
		gStaticMeshMgr->AddItemFromFile(STATICMESH_SMALLMARBLE, "Resources/Static Mesh/Marble1.x");
		gStaticMeshMgr->AddItemFromFile(STATICMESH_MEDIUMMARBLE, "Resources/Static Mesh/Marble2.x");
		gStaticMeshMgr->AddItemFromFile(STATICMESH_LARGEMARBLE, "Resources/Static Mesh/Marble3.x");
		gStaticMeshMgr->AddItemFromFile(STATICMESH_CYLINDER, "Resources/Static Mesh/Cylinder.x");
		gStaticMeshMgr->AddItemFromFile(STATICMESH_PLANE, "Resources/Static Mesh/Plane.x");
		gStaticMeshMgr->AddItemFromFile(STATICMESH_CAPSULE, "Resources/Static Mesh/Capsule.x");
	}
	if(gFontMgr)
	{
		//Load all fonts
		D3DXFONT_DESC fontDesc;
		fontDesc.Height          = 20;
		fontDesc.Width           = 10;
		fontDesc.Weight          = FW_BOLD;
		fontDesc.MipLevels       = 0;
		fontDesc.Italic          = false;
		fontDesc.CharSet         = DEFAULT_CHARSET;
		fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
		fontDesc.Quality         = DEFAULT_QUALITY;
		fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
		strcpy(fontDesc.FaceName, ("Times New Roman"));
		gFontMgr->AddItem(FONT_CHAT, fontDesc);
	}

	//SUIInfo uiinfo;
	//uiinfo.ID = UI_SINGLE;
	//uiinfo.Position = D3DXVECTOR3(400, 100, 0);
	//uiinfo.Size.x = 100;
	//uiinfo.Size.y = 100;
	//uiinfo.type = UI_BUTTON;
	//uiinfo.TextureNormal = TEXTURE_BUTTON_RED;
	//uiinfo.TextureMouseOver = TEXTURE_BUTTON_RED2;
	//uiinfo.TexturePressed = TEXTURE_BUTTON_RED3;

	//gCoreMgr->HandleMessage(new SMessageAddUI(uiinfo));
	//gCoreMgr->HandleMessage(new SMessageActivateUI(UI_STARTBUTTON));

	//uiinfo.ID = 6;
	//uiinfo.Position = D3DXVECTOR3(100, 150, 0);
	//uiinfo.Size.x = 256;
	//uiinfo.Size.y = 64;
	//uiinfo.type = UI_TEXTBOX;
	//uiinfo.TextureNormal = TEXTURE_RECTANGLE;
	//uiinfo.TextureMouseOver = TEXTURE_RECTANGLE;
	//uiinfo.TexturePressed = 0;

	//gCoreMgr->HandleMessage(new SMessageAddUI(uiinfo));
	//gCoreMgr->HandleMessage(new SMessageActivateUI(6));

	if(gEPMgr)
	{
		//Load Entity Properties
		EntityPropertyData epd;
		SpherePMat* demoMat = new SpherePMat();
		demoMat->friction = 0.9;
		demoMat->restitution = 0.5;
		demoMat->radius = 0.5;
		demoMat->scalar = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		demoMat->mass = 10;
		epd.mat = demoMat;
		epd.PhysicsType = PHYSICS_DYNAMIC;
		epd.GraphicsScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		epd.RenderType = SCENE_STATICMESH;
		epd.GResourceID = STATICMESH_SPHERE1;
		epd.ScriptName = "";
		gEPMgr->AddItem(EP_DEMO, new EntityPropertyData(epd));
		//small marble
		SpherePMat* smallMarbleMat = new SpherePMat();
		smallMarbleMat->friction = 0.9;
		smallMarbleMat->restitution = 0.5;
		smallMarbleMat->radius = 0.5;
		smallMarbleMat->scalar = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		smallMarbleMat->mass = 10;
		epd.mat = smallMarbleMat;
		epd.PhysicsType = PHYSICS_DYNAMIC;
		epd.GraphicsScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		epd.RenderType = SCENE_STATICMESH;
		epd.GResourceID = STATICMESH_SPHERE1;
		epd.ScriptName = "";
		gEPMgr->AddItem(EP_SMALLMARBLE, new EntityPropertyData(epd));
		//Medium Marble
		SpherePMat* mediumMarbleMat = new SpherePMat();
		mediumMarbleMat->friction = 0.9;
		mediumMarbleMat->restitution = 0.5;
		mediumMarbleMat->radius = 0.75;
		mediumMarbleMat->scalar = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		mediumMarbleMat->mass = 20;
		epd.mat = mediumMarbleMat;
		epd.PhysicsType = PHYSICS_DYNAMIC;
		epd.GraphicsScale = D3DXVECTOR3(1.5f, 1.5f, 1.5f);
		epd.RenderType = SCENE_STATICMESH;
		epd.GResourceID = STATICMESH_SPHERE2;
		epd.ScriptName = "";
		gEPMgr->AddItem(EP_MEDIUMMARBLE, new EntityPropertyData(epd));
		//Large marble
		SpherePMat* largeMarbleMat = new SpherePMat();
		largeMarbleMat->friction = 0.9;
		largeMarbleMat->restitution = 0.5;
		largeMarbleMat->radius = 1.0;
		largeMarbleMat->scalar = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		largeMarbleMat->mass = 40;
		epd.mat = largeMarbleMat;
		epd.PhysicsType = PHYSICS_DYNAMIC;
		epd.GraphicsScale = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
		epd.RenderType = SCENE_STATICMESH;
		epd.GResourceID = STATICMESH_SPHERE3;
		epd.ScriptName = "";
		gEPMgr->AddItem(EP_LARGEMARBLE, new EntityPropertyData(epd));
		//platform
		CylinderPMat* platformMat = new CylinderPMat();
		platformMat->friction = 4.0f;
		platformMat->restitution = 0;
		platformMat->radius = 1.0;
		platformMat->length = 1.0;
		platformMat->scalar = D3DXVECTOR3(20.0f, 1.0f, 20.0f);
		platformMat->mass = 0;
		epd.mat = platformMat;
		epd.PhysicsType = PHYSICS_KINEMATIC;
		epd.GraphicsScale = D3DXVECTOR3(40.0f, 2.0f, 40.0f);
		epd.RenderType = SCENE_STATICMESH;
		epd.GResourceID = STATICMESH_CYLINDER;
		epd.ScriptName = "";
		gEPMgr->AddItem(EP_PLATFORM, new EntityPropertyData(epd));
		//box
		BoxPMat* boxMat = new BoxPMat();
		boxMat->friction = 4.0f;
		boxMat->restitution = 0;
		boxMat->scalar = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		boxMat->mass = 0;
		epd.mat = boxMat;
		epd.PhysicsType = PHYSICS_KINEMATIC;
		epd.GraphicsScale = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
		epd.RenderType = SCENE_STATICMESH;
		epd.GResourceID = STATICMESH_CUBE;
		epd.ScriptName = "";
		gEPMgr->AddItem(EP_BOX, new EntityPropertyData(epd));
		//Player Marble		
		SpherePMat* playerMarbleMat = new SpherePMat();
		playerMarbleMat->friction = 0.9;
		playerMarbleMat->restitution = 0.5;
		playerMarbleMat->radius = 1.0;
		playerMarbleMat->scalar = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		playerMarbleMat->mass = 80;
		epd.mat = playerMarbleMat;
		epd.PhysicsType = PHYSICS_DYNAMIC;
		epd.GraphicsScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		epd.RenderType = SCENE_STATICMESH;
		epd.GResourceID = STATICMESH_SPHERE;
		epd.ScriptName = "";
		gEPMgr->AddItem(EP_PLAYERMARBLE, new EntityPropertyData(epd));
		//Demo Plane	
		PlanePMat* demoPlaneMat = new PlanePMat();
		demoPlaneMat->friction = 0.9;
		demoPlaneMat->restitution = 0.5;
		demoPlaneMat->pNormal = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.0f);
		demoPlaneMat->scalar = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		demoPlaneMat->mass = 80;
		epd.mat = demoPlaneMat;
		epd.PhysicsType = PHYSICS_KINEMATIC;
		epd.GraphicsScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		epd.RenderType = SCENE_STATICMESH;
		epd.GResourceID = STATICMESH_PLANE;
		epd.ScriptName = "";
		gEPMgr->AddItem(EP_DEMOPLANE, new EntityPropertyData(epd));
		//Demo Capsule	
		CapsulePMat* demoCapsuleMat = new CapsulePMat();
		demoCapsuleMat->friction = 0.9;
		demoCapsuleMat->restitution = 0.5;
		demoCapsuleMat->radius = 1.0;
		demoCapsuleMat->length = 2.0;
		demoCapsuleMat->scalar = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		demoCapsuleMat->mass = 80;
		epd.mat = demoCapsuleMat;
		epd.PhysicsType = PHYSICS_DYNAMIC;
		epd.GraphicsScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		epd.RenderType = SCENE_STATICMESH;
		epd.GResourceID = STATICMESH_CAPSULE;
		epd.ScriptName = "";
		gEPMgr->AddItem(EP_DEMOCAPSULE, new EntityPropertyData(epd));
	}

	if(gSCMgr)
	{
		//wander
		SteeringConstants sc;
		sc.m_dMaxForce = 15;
		sc.m_dMaxSpeed = 15;
		sc.m_dRadius = 10;
		sc.m_dScale = 5;
		sc.m_iEntityType = 1;
		sc.m_iMinDetectionBoxLength = 40;
		sc.m_iSteeringFlags = 1;
		gSCMgr->AddItem(ST_WANDER, new SteeringConstants(sc));

		//arrive
		sc.m_dMaxForce = 15;
		sc.m_dMaxSpeed = 15;
		sc.m_dRadius = 10;
		sc.m_dScale = 5;
		sc.m_iEntityType = 2;
		sc.m_iMinDetectionBoxLength = 40;
		sc.m_iSteeringFlags = 2;
		gSCMgr->AddItem(ST_ARRIVE, new SteeringConstants(sc));

		//hide
		sc.m_dMaxForce = 15;
		sc.m_dMaxSpeed = 15;
		sc.m_dRadius = 10;
		sc.m_dScale = 5;
		sc.m_iEntityType = 3;
		sc.m_iMinDetectionBoxLength = 40;
		sc.m_iSteeringFlags = 3;
		gSCMgr->AddItem(ST_HIDE, new SteeringConstants(sc));

		//seek
		sc.m_dMaxForce = 15;
		sc.m_dMaxSpeed = 15;
		sc.m_dRadius = 10;
		sc.m_dScale = 5;
		sc.m_iEntityType = 4;
		sc.m_iMinDetectionBoxLength = 40;
		sc.m_iSteeringFlags = 4;
		gSCMgr->AddItem(ST_SEEK, new SteeringConstants(sc));

		//flee
		sc.m_dMaxForce = 15;
		sc.m_dMaxSpeed = 2;
		sc.m_dRadius = 5;
		sc.m_dScale = 5;
		sc.m_iEntityType = 5;
		sc.m_iMinDetectionBoxLength = 40;
		sc.m_iSteeringFlags = 5;
		gSCMgr->AddItem(ST_FLEE, new SteeringConstants(sc));
	}
}