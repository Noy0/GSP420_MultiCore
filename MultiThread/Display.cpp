#include "Display.h"
//==============================
//Engine Constructor:
//Take a handle to the window
//then initialize DirectX
//===============================
GraphicsEngine::GraphicsEngine(	HWND Hwnd )
{
	mHwnd = Hwnd;
	mDevice = NULL;
	mEffect = NULL;
	wireFrame = false;
	mCameraPos = D3DXVECTOR3(0.0f, 0.0f, -5.0f);
	mCameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	mCameraTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_FOV = D3DX_PI * .25f;
	m_NearPlane = 1.0f;
	m_FarPlane = 5000.0f;

	R = 180;
	G = 180;
	B = 250;
	InitDirectX();
}
//===================================
//Initialize DirectX:
//Setups all the necessary 
//parameters to have DirectX 
//===================================
void GraphicsEngine::InitDirectX()
{
	IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	
	if(d3d9 == NULL)
	{
		::MessageBox(mHwnd, _T("Direct 3D Create 9"), _T("ERROR"),  MB_ICONWARNING | MB_OK);
	}
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	
	int vp = 0;
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else 
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	if(caps.VertexShaderVersion < D3DVS_VERSION(2, 0) || caps.PixelShaderVersion < D3DPS_VERSION(2,0))
	{
		::MessageBox(mHwnd, _T("Your Graphic card does not support vertex and pixelshaders version 2.0"), _T("Error"), MB_OK);
	}
	RECT Rct;
	GetClientRect( mHwnd, &Rct);
	width = Rct.right;
	height = Rct.bottom;

	
	d3dpp.BackBufferWidth            = width;
	d3dpp.BackBufferHeight           = height;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = mHwnd;
	d3dpp.Windowed                   = windowed;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	if(FAILED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mHwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE, &d3dpp, &mDevice)))
	{
		MessageBox(mHwnd, _T("Failed to create IDirect3DDevice9"),_T("ERROR"),  MB_ICONWARNING | MB_OK);
	}

	d3d9->Release();

	mLight.dirW = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	mLight.ambient = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
	mLight.diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	mLight.spec = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);

	D3DXCreateSprite(mDevice, &mSprite);
	m_white.Ambient = m_white.Specular = m_white.Diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_white.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_white.Power = 1.0f;

	D3DXCreateTextureFromFile(mDevice, "whitetex.dds", &m_WhiteTex);
	BuildFX();
	CreateMatrix();
}
//====================================
//Pre-Render
//function that contains the
// Begin function and clear to
// clear the BB then be ready to draw
//======================================
void GraphicsEngine::PreRender()
{
		//mDevice->SetTransform(D3DTS_PROJECTION, &proj);
		//mDevice->SetTransform(D3DTS_VIEW, &view);

		//if(wireFrame)
		//	mDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		//else
		//	mDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	mDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		mDevice->BeginScene();
		mEffect->SetValue(m_hLight, &mLight, sizeof(Light));
		mEffect->SetValue(m_hEyePos, &mCameraPos, sizeof(D3DXVECTOR3));
		mSprite->Begin(D3DXSPRITE_ALPHABLEND);
}
//===============================
//Post Render
//Has the End and Present function
//call this function to finish the
//rendering
//===============================
void GraphicsEngine::PostRender()
{
	mSprite->End();
	mDevice->EndScene();
	mDevice->Present(0, 0, 0, 0);
}

GraphicsEngine::~GraphicsEngine()
{
	CleanUp();
}

//=============================
//Clean UP:
//Deletes all the pointers 
//that are created when the 
//engine is
//=============================
void GraphicsEngine::CleanUp()
{
	if(mSprite != NULL)
	mSprite->Release();
	if(mEffect != NULL)
	mEffect->Release();
	if(mDevice != NULL)
	mDevice->Release();
}
//==================================
//Render State
//Sets up the necessary parameters
//for being able to render alpha
//textures
//==================================
void GraphicsEngine::RenderState()
{
	mDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	mDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	mDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	mDevice->SetRenderState(D3DRS_ALPHAREF, 10);
	mDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	
	mDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	mDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	mDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	mDevice->SetRenderState(D3DRS_LIGHTING, true);
	mDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));  

}

//====================================
//Render Texture
// takes in a IDirect3DTexture9 pointer
// a posistion, center and translation
//====================================
void GraphicsEngine::RenderTexture(IDirect3DTexture9* Tex, D3DXMATRIX Combine)
{
 mSprite->SetTransform(&Combine);
 mSprite->Draw(Tex, 0, 0, 0, D3DCOLOR_XRGB(255,255,255));
 mSprite->Flush();
}
//======================================
//Render Font
//Send it a font pointer, the text
//color, and position
//======================================
void GraphicsEngine::RenderFont(ID3DXFont *mFont, std::string Text, D3DXCOLOR Color, D3DXMATRIX Trans)
{
	RECT R = {0,0, 1000, 1000};
	mSprite->SetTransform(&Trans);
	mFont->DrawTextA(mSprite, Text.c_str(), -1, &R, DT_TOP|DT_LEFT, Color);
	mSprite->Flush();
}
void GraphicsEngine::ViewMatrix()
{
			//Create Transformation Matrices
			D3DXMatrixIdentity(&world);
			D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0.0f, 5.0f, 5.0f), &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
			D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 4.0f, (float)1200 / (float)760, 0.1f, 1000.0f);

			//Set transformation matrices
			mDevice->SetTransform(D3DTS_WORLD, &world);
			mDevice->SetTransform(D3DTS_VIEW, &view);
			mDevice->SetTransform(D3DTS_PROJECTION, &proj);
}
void GraphicsEngine::RenderMesh(ID3DXMesh* mMesh, vector<IDirect3DTexture9*> m_textures, vector<D3DMATERIAL9> m_materials, D3DXMATRIX Trans )
{
	D3DXMATRIX WVP(Trans * view * proj);
	mEffect->SetMatrix(m_hWVP, &WVP);
	D3DXMATRIX worldInvTrans;
	D3DXMatrixInverse(&worldInvTrans, 0, &Trans);
	D3DXMatrixTranspose(&worldInvTrans, &worldInvTrans);
	mEffect->SetMatrix(m_hWorldInvTrans, &worldInvTrans);
	mEffect->SetMatrix(m_hWorld, &Trans);

	mEffect->SetTechnique(m_hTech);
	UINT numPasses = 0;
	mEffect->Begin(&numPasses, 0);
	mEffect->BeginPass(0);

	int numMaterials = (int)m_materials.size();

	for(int i=0;i<numMaterials;i++)
	{	
		//Set material
		mEffect->SetValue(m_hMtrl, &(m_materials[i]), sizeof(D3DMATERIAL9));
		if(m_textures[i] != NULL)
			mEffect->SetTexture(m_hTex, (m_textures[i]));
		else 
			mEffect->SetTexture(m_hTex, (m_WhiteTex));
		mEffect->CommitChanges();

		//Render mesh
		mMesh->DrawSubset(i);
	}

	
	mEffect->EndPass();
	mEffect->End();
}

void GraphicsEngine::RenderMeshTex(ID3DXMesh* Mesh, IDirect3DTexture9* m_textures, vector<D3DMATERIAL9> m_materials, D3DXMATRIX Trans)
{
	D3DXMATRIX WVP(Trans * view * proj);
	mEffect->SetMatrix(m_hWVP, &WVP);
	D3DXMATRIX worldInvTrans;
	D3DXMatrixInverse(&worldInvTrans, 0, &Trans);
	D3DXMatrixTranspose(&worldInvTrans, &worldInvTrans);
	mEffect->SetMatrix(m_hWorldInvTrans, &worldInvTrans);
	mEffect->SetMatrix(m_hWorld, &Trans);

	mEffect->SetTechnique(m_hTech);
	UINT numPasses = 0;
	mEffect->Begin(&numPasses, 0);
	mEffect->BeginPass(0);

	int numMaterials = (int)m_materials.size();

	//Set Texture
	mDevice->SetTexture(0, m_textures);

	for(int i=0;i<numMaterials;i++)
	{	
		//Set material
		mEffect->SetValue(m_hMtrl, &(m_materials[i]), sizeof(D3DMATERIAL9));
		mEffect->SetTexture(m_hTex, m_textures);
		mEffect->CommitChanges();
		//Render mesh
		Mesh->DrawSubset(i);
	}

	
	mEffect->EndPass();
	mEffect->End();
}

void GraphicsEngine::BuildFX()
{	
	ID3DXBuffer* errors = 0;
	D3DXCreateEffectFromFile(mDevice, "PhongDirLtTex.fx", 
		0, 0, D3DXSHADER_DEBUG, 0, &mEffect, &errors);
	if( errors )
		MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);

	// Obtain handles.
	m_hTech            = mEffect->GetTechniqueByName("PhongDirLtTexTech");
	m_hWVP             = mEffect->GetParameterByName(0, "gWVP");
	m_hWorldInvTrans   = mEffect->GetParameterByName(0, "gWorldInvTrans");
	m_hMtrl            = mEffect->GetParameterByName(0, "gMtrl");
	m_hLight           = mEffect->GetParameterByName(0, "gLight");
	m_hEyePos          = mEffect->GetParameterByName(0, "gEyePosW");
	m_hWorld           = mEffect->GetParameterByName(0, "gWorld");
	m_hTex             = mEffect->GetParameterByName(0, "gTex");


}

void GraphicsEngine::ChangeBGColor(int Red, int Green, int Blue)
{
	R = Red;
	G = Green;
	B = Blue;
}
void GraphicsEngine::CreateMatrix()
{
        D3DXMatrixLookAtLH(&view, &mCameraPos, &mCameraTarget, &mCameraUp);
        D3DXMatrixPerspectiveFovLH(&proj, m_FOV, (float)width / (float)height, m_NearPlane, m_FarPlane);
}
void GraphicsEngine::ChangePos(D3DXVECTOR3 Pos)
{
	mCameraPos = Pos;
	CreateMatrix();
}
void GraphicsEngine::ChangeLook(D3DXVECTOR3 Look)
{
	mCameraTarget = Look;
	CreateMatrix();
}
void GraphicsEngine::ChangeUP(D3DXVECTOR3 UP)
{
	mCameraUp = UP;
	CreateMatrix();
}
void GraphicsEngine::RotationCamera(float Angle)
{
	float Radius = 5.0f;
	float CameraHeight = 5.0f;

	D3DXMatrixLookAtLH(&view, &D3DXVECTOR3((cos(Angle)* Radius), CameraHeight, sin(sin(Angle)*Radius)), &mCameraUp, &mCameraTarget);

	CreateMatrix();
}