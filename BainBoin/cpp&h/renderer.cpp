// ------------------------------------------
//
// レンダリング処理の説明[renderer.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------
#include "renderer.h"
#include "scene.h"
#include "scene_load.h"
#include "fade.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "ranking.h"
#include "debugproc.h"
#include "camera.h"
#include "light.h"

// ------------------------------------------
//
// マクロ関数
//
// ------------------------------------------

// ------------------------------------------
//
// プロトタイプ宣言
//
// ------------------------------------------

// ------------------------------------------
//
// 静的変数宣言
//
// ------------------------------------------
#ifdef _DEBUG
CDebugproc * CRenderer::m_debugproc = NULL;
#endif

// ------------------------------------------
// コンストラクタ
// ------------------------------------------
CRenderer::CRenderer()
{
	m_pD3D = NULL;
	m_pDevice = NULL;
}

// ------------------------------------------
// デストラクタ
// ------------------------------------------
CRenderer::~CRenderer()
{
}

// ------------------------------------------
// 初期化処理
// ------------------------------------------
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	// 変数宣言
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;							// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;							// バックバッファの形式
	d3dpp.BackBufferCount = 1;										// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;							// デプスバッファ(Ｚバッファ)とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;										// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート(現在の速度に合わせる)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル(VSyncを待って描画)
	// d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// クライアント領域を直ちに更新する

	// Direct3Dデバイスの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,	// ディスプレイアダプタ
		D3DDEVTYPE_HAL,									// デバイスタイプ
		hWnd,											// フォーカスするウインドウへのハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING,			// デバイス作成制御の組み合わせ
		&d3dpp,											// デバイスのプレゼンテーションパラメータ
		&m_pDevice)))									// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダラーステートの設定
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// テクスチャステージステートパラメータの設定
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

#ifdef _DEBUG
	m_debugproc = new CDebugproc;
	// デバッグ表示初期化
	m_debugproc->Init(m_pDevice);
#endif
	// シーンの読み込み
	CScene_load::LoadAll();
	// カメラの生成
	m_pCamera = CCamera::Create();
	// ライトの生成
	m_pLight = CLight::Create();
	return S_OK;
}

// ------------------------------------------
// 終了処理
// ------------------------------------------
void CRenderer::Uninit(void)
{
	// 読み込んだものを破棄する処理
	CScene_load::UnLoadAll();
	// シーン親子のリリース処理
	CScene::ReleaseAll();
	// カメラの生成
	m_pCamera->Uninit();
	// ライトの生成
	m_pLight->Uninit();

	// デバイスの破棄
	if (m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	// D3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
#ifdef _DEBUG
	// デバッグ表示
	if (m_debugproc != NULL)
	{
		m_debugproc->Uninit();
		delete m_debugproc;
		m_debugproc = NULL;
	}

#endif // _DEBUG
}

// ------------------------------------------
// 更新処理
// ------------------------------------------
void CRenderer::Update(void)
{
	// カメラの生成
	m_pCamera->Update();
	// ライトの生成
	m_pLight->Update();

	// ポーズ時以外
	if (CGame::GetState() != CGame::STATE_PAUSE)
	{
		// シーン
		CScene::UpdateAll();
	}
#ifdef _DEBUG
	// FPS表示
	DrawFPS();

#endif // _DEBUG

}

// -----------------------
// 描画処理
// --------------------------
void CRenderer::Draw(void)
{
	// バックバッファとZバッファのクリア
	m_pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画開始
	if (SUCCEEDED(m_pDevice->BeginScene()))
	{
		// カメラの生成
		m_pCamera->Set();
		// シーン
		CScene::DrawAll();
		// 前のモード終了
		switch (CManager::GetMode())
		{
			// タイトル
		case CManager::MODE_TITLE:
			CManager::GetTitle()->Draw();
			break;
			// ゲーム
		case CManager::MODE_GAME:
			CManager::GetGame()->Draw();
			break;
			// ゲーム
		case CManager::MODE_TUTORIAL:
			CManager::GetTutorial()->Draw();
			break;
			// ランキング
		case CManager::MODE_RANKING:
			CManager::GetRanking()->Draw();
			break;
		default:
			break;
		}

		CManager::GetFade()->Draw();

#ifdef _DEBUG
		// デバッグ表示の描画
		CDebugproc::Draw();
#endif
		// Direct3Dによる描画終了
		m_pDevice->EndScene();
	}

	// バックバッファとフロートバッファの入れ替え
	m_pDevice->Present(NULL, NULL, NULL, NULL);
}

// ------------------------------------------
// デバイス取得処理
// ------------------------------------------
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pDevice;
}

// ------------------------------------------
// カメラ取得処理
// ------------------------------------------
CCamera * CRenderer::GetCamera(void)
{
	return m_pCamera;
}

// ------------------------------------------
// ライト取得処理
// ------------------------------------------
CLight * CRenderer::GetLight(void)
{
	return m_pLight;
}

#if _DEBUG
// ------------------------------------------
// FPS描画処理
// ------------------------------------------
void CRenderer::DrawFPS(void)
{
	int nCntFPS = GetFPS();

	CDebugproc::Print("FPS:%d\n",nCntFPS);
}
#endif // _DEBUG