// ----------------------------------------
//
// 集中線処理の説明[forceline.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "forceline.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define FORCELINE_TEX "data/TEXTURE/forceline2.jpg"

// ----------------------------------------
//
// グローバル変数
//
// ----------------------------------------

// ----------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------
LPDIRECT3DTEXTURE9 CForceline::m_pTex = NULL;
bool CForceline::m_bUse = false;

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CForceline::CForceline() : CScene_TWO::CScene_TWO(ACTOR_FORCELINE,LAYER_UI)
{
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CForceline::~CForceline()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CForceline::Init(void)
{	
	// サイズ設定
	CScene_TWO::SetSize(D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
	// 初期化
	CScene_TWO::Init();
	// 位置設定
	CScene_TWO::SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	CScene_TWO::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
	// 頂点テクスチャー設定
	CScene_TWO::SetTex(
		D3DXVECTOR2(0.0f,0.0f),
		D3DXVECTOR2(1.0f,1.0f));
	// テクスチャー設定
	CScene_TWO::BindTexture(m_pTex);
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CForceline::Uninit(void)
{
	CScene_TWO::Uninit();
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CForceline::Update(void)
{
	if (m_bUse)
	{
		CScene_TWO::Update();
	}
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CForceline::Draw(void)
{
	if (m_bUse)
	{
		//CScene_TWO::Draw();
	}
}

// ----------------------------------------
// 使用状態処理
// ----------------------------------------
void CForceline::Use(bool bUse)
{
	m_bUse = bUse;
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CForceline::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice =		
		CManager::GetRenderer()->GetDevice();
	// テクスチャー設定
	D3DXCreateTextureFromFile(pDevice, FORCELINE_TEX, &m_pTex);
	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CForceline::UnLoad(void)
{
	// テクスチャの開放
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CForceline * CForceline::Create(void)
{
	// 変数宣言
	CForceline * pForceline;		// 集中線2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pForceline = new CForceline();
	// 初期化処理
	pForceline->Init();
	// 生成したオブジェクトを返す
	return pForceline;
}