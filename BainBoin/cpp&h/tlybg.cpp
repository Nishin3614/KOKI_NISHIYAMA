// ----------------------------------------
//
// シーン処理の説明[tlybg.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "tlybg.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define TLYBG "data/hackdoll3.png"

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
LPDIRECT3DTEXTURE9 CTlyBg::m_pTex[CManager::MODE_MAX][MAX_TLYBG] = {};
CManager::MODE CTlyBg::m_mode = CManager::MODE_TITLE;
D3DXVECTOR3 CTlyBg::m_pos[CManager::MODE_MAX][MAX_TLYBG] = {};
D3DXVECTOR2 CTlyBg::m_size[CManager::MODE_MAX][MAX_TLYBG] = {};

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CTlyBg::CTlyBg() : CScene::CScene(ACTOR_BG, LAYER_UI)
{
	// 初期化
	for (int nCnt = 0; nCnt < MAX_TLYBG; nCnt++)
	{
		m_aScene_Two[nCnt] = NULL;
	}
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CTlyBg::~CTlyBg()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CTlyBg::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_TLYBG; nCnt++)
	{
		if (m_pTex[m_mode][nCnt] != NULL)
		{
			m_aScene_Two[nCnt] = new CScene_TWO;
			m_aScene_Two[nCnt]->SetSize(m_size[m_mode][nCnt]);
			m_aScene_Two[nCnt]->Init();
			m_aScene_Two[nCnt]->SetPosition(m_pos[m_mode][nCnt]);
			m_aScene_Two[nCnt]->BindTexture(m_pTex[m_mode][nCnt]);
		}
	}
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CTlyBg::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_TLYBG; nCnt++)
	{
		if (m_pTex[m_mode][nCnt] != NULL)
		{
			m_aScene_Two[nCnt]->Uninit();
			delete m_aScene_Two[nCnt];
			m_aScene_Two[nCnt] = NULL;
		}
	}
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CTlyBg::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_TLYBG; nCnt++)
	{
		if (m_pTex[m_mode][nCnt] != NULL)
		{
			m_aScene_Two[nCnt]->Update();
		}
	}
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CTlyBg::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_TLYBG; nCnt++)
	{
		if (m_pTex[m_mode][nCnt] != NULL)
		{
			m_aScene_Two[nCnt]->Draw();
		}
	}
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CTlyBg::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// テクスチャー情報代入
	char TexData[CManager::MODE_MAX][MAX_TLYBG][36] =
	{
		{ "data/TEXTURE/BG/RankBG.png" },		// ゲーム用の背景
		{ "data/TEXTURE/BG/TitleBG.png" },		// チュートリアル用の背景
		{ "data/TEXTURE/BG/TitleBG.png" },		// タイトル用の背景
		{ "data/TEXTURE/BG/RankBG.png" },		// ランキングの背景
	};
	// 位置情報代入
	D3DXVECTOR3 pos[CManager::MODE_MAX][MAX_TLYBG] =
	{
		{ D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f) },
		{ D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f) },
		{ D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f) },
		{ D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f) },
	};
	// サイズ情報代入
	D3DXVECTOR2 size[CManager::MODE_MAX][MAX_TLYBG] =
	{
		{ D3DXVECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT) },
		{ D3DXVECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT) },
		{ D3DXVECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT) },
		{ D3DXVECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT) },
	};

	// 読み込み
	for (int nCntMode = 0; nCntMode < CManager::MODE_MAX; nCntMode++)
	{
		for (int nCnt = 0; nCnt < MAX_TLYBG; nCnt++)
		{
			// テクスチャーがあったら
			if (strcmp(TexData[nCntMode][nCnt], "") != 0)
			{
				D3DXCreateTextureFromFile(pDevice, TexData[nCntMode][nCnt], &m_pTex[nCntMode][nCnt]);
				/* 位置・サイズのロード */
				m_pos[nCntMode][nCnt] = pos[nCntMode][nCnt];
				m_size[nCntMode][nCnt] = size[nCntMode][nCnt];
			}
		}
	}

	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CTlyBg::UnLoad(void)
{
	// テクスチャの開放
	for (int nCntMode = 0; nCntMode < CManager::MODE_MAX; nCntMode++)
	{
		for (int nCnt = 0; nCnt < MAX_TLYBG; nCnt++)
		{
			if (m_pTex[nCntMode][nCnt] != NULL)
			{
				m_pTex[nCntMode][nCnt]->Release();
				m_pTex[nCntMode][nCnt] = NULL;
			}
		}
	}
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CTlyBg * CTlyBg::Create(CManager::MODE mode)
{
	// 変数宣言
	CTlyBg * pTlyBg;
	// モード情報設定
	m_mode = mode;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pTlyBg = new CTlyBg();
	// 初期化処理
	pTlyBg->Init();
	// 生成したオブジェクトを返す
	return pTlyBg;
}