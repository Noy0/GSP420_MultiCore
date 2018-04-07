#include "GraphicsEngine.h"

GraphicsEngine::GraphicsEngine(HWND hwnd)
{
	//Setting variables
	m_hWnd = hwnd;

	//Initialize DirectX
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	RECT r;
	GetClientRect(m_hWnd, &r);

	m_D3DPP.BackBufferWidth = r.right;
	m_D3DPP.BackBufferHeight = r.bottom;
	m_D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
	m_D3DPP.BackBufferCount = 1;
	m_D3DPP.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_D3DPP.MultiSampleQuality = 0;
	m_D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_D3DPP.hDeviceWindow = m_hWnd;
	m_D3DPP.Windowed = true;
	m_D3DPP.EnableAutoDepthStencil = true;
	m_D3DPP.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_D3DPP.Flags = 0;
	m_D3DPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, 
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE, &m_D3DPP, &m_pDevice);

	//Initialize each feature components
	//Light
	m_Light.dirW = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_Light.ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	m_Light.diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_Light.spec    = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

	//Camera
	m_CameraPos = D3DXVECTOR3(0.0f, 0.0f, -50.0f);
	m_CameraTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_CameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//Projection
	m_FOV = D3DX_PI * 0.25f;
	m_NearPlane = 1.0f;
	m_FarPlane = 5000.0f;

	BuildMatrix();

	//Effects
	ID3DXBuffer* errors = 0;
	D3DXCreateEffectFromFile(m_pDevice, "PhongDirLtTex.fx", 
		0, 0, D3DXSHADER_DEBUG, 0, &m_pFX, &errors);
	if( errors )
	{
		MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);
		errors->Release();
	}
	m_hTech			   = m_pFX->GetTechniqueByName("PhongDirLtTexTech");
	m_hWVP             = m_pFX->GetParameterByName(0, "gWVP");
	m_hWorldInvTrans   = m_pFX->GetParameterByName(0, "gWorldInvTrans");
	m_hMtrl            = m_pFX->GetParameterByName(0, "gMtrl");
	m_hLight           = m_pFX->GetParameterByName(0, "gLight");
	m_hEyePos          = m_pFX->GetParameterByName(0, "gEyePosW");
	m_hWorld           = m_pFX->GetParameterByName(0, "gWorld");
	m_hTex             = m_pFX->GetParameterByName(0, "gTex");

	//Sprite
	D3DXCreateSprite(m_pDevice, &m_pSprite);
}

GraphicsEngine::~GraphicsEngine()
{
	if(m_pD3D)
		m_pD3D->Release();
	if(m_pDevice)
		m_pDevice->Release();
	if(m_pSprite)
		m_pSprite->Release();
	if(m_pFX)
		m_pFX->Release();
}

/*void GraphicsEngine::ExecuteMessage()
{
	while(!m_MessageQueue.empty())
	{
		SMessage* msg = m_MessageQueue.front();
		m_MessageQueue.pop();
		switch(msg->Message)
		{
		case MSG_DRAWTEXTURE:
			{
				IDirect3DTexture9* tex = gTextureMgr->
					GetItem(((SMessageDrawTexture*)msg)->Key);
				D3DXMATRIX m = ((SMessageDrawTexture*)msg)->Transform;
				m_pSprite->SetTransform(&m);
				m_pSprite->Draw(tex, 0, 0, 0, D3DCOLOR_XRGB(255, 255, 255));
				m_pSprite->Flush();
				break;
			}
		case MSG_DRAWSTATICMESH:
			{
				SMessageDrawStaticMesh* msgx = (SMessageDrawStaticMesh*)msg;
				DrawStaticMesh(msgx->Key, &(msgx->Transform));
				break;
			}
		case MSG_DRAWTEXT:
			{
				RECT r;
				r.top = 0;
				r.left = 0;
				r.right = 2000;
				r.bottom = 2000;
				SMessageDrawText* msgx = (SMessageDrawText*)msg;
				m_pDevice->SetTransform(D3DTS_WORLD, &(msgx->Transform));
				gFontMgr->GetItem(msgx->Key)->DrawText(m_pSprite, (LPCSTR)msgx->Text, -1,
					&r, DT_TOP | DT_LEFT, msgx->Color);
				m_pSprite->Flush();
				break;
			}
		case MSG_DRAWTEXTBATCH:
			{
				RECT r;
				r.top = 0;
				r.left = 0;
				r.right = 2000;
				r.bottom = 2000;
				SMessageDrawTextBatch* msgx = (SMessageDrawTextBatch*)msg;
				for(int i = 0; i < msgx->Key.size(); ++i)
				{
					m_pDevice->SetTransform(D3DTS_WORLD, &(msgx->Transform[i]));
					gFontMgr->GetItem(msgx->Key[i])->DrawText(m_pSprite, 
						(LPCSTR)msgx->Text[i].c_str(),  -1, &r, DT_TOP | DT_LEFT, msgx->Color[i]);
					m_pSprite->Flush();
				}
				break;
			}
		}
		delete msg;
	}
}*/

void GraphicsEngine::BuildMatrix()
{
	//Build view matrix
	D3DXMatrixLookAtLH(&m_View, &m_CameraPos, &m_CameraTarget, &m_CameraUp);
	
	//Build projection matrix
	D3DXMatrixPerspectiveFovLH(&m_Projection, m_FOV, 
		(float)m_D3DPP.BackBufferWidth / (float)m_D3DPP.BackBufferHeight,
		m_NearPlane, m_FarPlane);
}

void GraphicsEngine::Begin()
{
	m_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xfff0fff0, 1.0f, 0);
	m_pDevice->BeginScene();
	
	m_pFX->SetValue(m_hLight, &m_Light, sizeof(Light));
	m_pFX->SetValue(m_hEyePos, &m_CameraPos, sizeof(D3DXVECTOR3));
	
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void GraphicsEngine::DrawTexture(IDirect3DTexture9* texture, D3DXMATRIX *transform)
{
		m_pSprite->SetTransform(transform);
		m_pSprite->Draw(texture, 0, 0, 0, D3DCOLOR_XRGB(255, 255, 255));
		m_pSprite->Flush();
}

void GraphicsEngine::Draw_Text(ID3DXFont *font, std::string text, D3DXMATRIX *transform, D3DXCOLOR color)
{
		RECT r;
		r.top = 0;
		r.left = 0;
		r.right = 2000;
		r.bottom = 2000;\
		m_pDevice->SetTransform(D3DTS_WORLD, transform);
		font->DrawText(m_pSprite, (LPCSTR)(text.c_str()), -1,
			&r, DT_TOP | DT_LEFT, color);
		m_pSprite->Flush();
}

void GraphicsEngine::End()
{
	m_pSprite->End();
	m_pDevice->EndScene();
	m_pDevice->Present(0, 0, 0, 0);
}