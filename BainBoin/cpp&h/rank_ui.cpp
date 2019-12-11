// ----------------------------------------
//
// ランク用ui処理の説明[rank_ui.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "rank_ui.h"
#include "topscore.h"
#include "fade.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define RANK_UI "data/hackdoll3.png"

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
LPDIRECT3DTEXTURE9 CRank_ui::m_pTex[MAX_RANK_UI] = {};
D3DXVECTOR3 CRank_ui::m_pos[MAX_RANK_UI] = {};
D3DXVECTOR2 CRank_ui::m_size[MAX_RANK_UI] = {};
int CRank_ui::m_nSelect = 0;

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CRank_ui::CRank_ui() : CScene::CScene(ACTOR_RANK_UI, LAYER_UI)
{
	// 初期化
	for (int nCnt = 0; nCnt < MAX_RANK_UI; nCnt++)
	{
		m_aScene_Two[nCnt] = NULL;
	}
	m_nCntCol = 0;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CRank_ui::~CRank_ui()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CRank_ui::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_RANK_UI; nCnt++)
	{
		if (m_pTex[nCnt] != NULL)
		{
			m_aScene_Two[nCnt] = new CScene_TWO;
			m_aScene_Two[nCnt]->SetSize(m_size[nCnt]);
			m_aScene_Two[nCnt]->Init();
			m_aScene_Two[nCnt]->SetPosition(m_pos[nCnt]);
			m_aScene_Two[nCnt]->BindTexture(m_pTex[nCnt]);
			if (nCnt > 1)
			{
				// スコア
				CTopscore * pTopscore = CTopscore::Create(
					m_aScene_Two[nCnt]->GetPosition() +
					D3DXVECTOR3(300.0f, 10.0f, 0.0f));
				pTopscore->SetTopscore(nCnt - 2);
			}
		}
	}
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CRank_ui::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_RANK_UI; nCnt++)
	{
		if (m_pTex[nCnt] != NULL)
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
void CRank_ui::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_RANK_UI; nCnt++)
	{
		// ヌルチェック
		if (m_pTex[nCnt] != NULL)
		{
			// 種類によって処理を変える
			m_aScene_Two[nCnt]->Update();
		}
	}
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CRank_ui::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_RANK_UI; nCnt++)
	{
		if (m_pTex[nCnt] != NULL)
		{
			m_aScene_Two[nCnt]->Draw();
		}
	}
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CRank_ui::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// テクスチャー情報代入
	char TexData[MAX_RANK_UI][72] =
	{
		{"data/TEXTURE/RANKING/ranking_name.png"},
		{"data/TEXTURE/RANKING/ranking_frame.png"},
		{"data/TEXTURE/RANKING/1st.png"},
		{"data/TEXTURE/RANKING/2nd.png"},
		{"data/TEXTURE/RANKING/3rd.png"},
		{"data/TEXTURE/RANKING/4th.png"},
		{"data/TEXTURE/RANKING/5th.png"},
	};
	// 位置情報代入
	D3DXVECTOR3 pos[MAX_RANK_UI] =
	{
		{ D3DXVECTOR3(SCREEN_WIDTH * 0.5f,80.0f,0.0f) },
		{ D3DXVECTOR3(SCREEN_WIDTH * 0.5f,400.0f,0.0f) },
		{ D3DXVECTOR3(300.0f,200.0f,0.0f)},
		{ D3DXVECTOR3(300.0f,300,0.0f)},
		{ D3DXVECTOR3(300.0f,400.0f,0.0f)},
		{ D3DXVECTOR3(300.0f,500.0f,0.0f)},
		{ D3DXVECTOR3(300.0f,600.0f,0.0f)},
	};
	// サイズ情報代入
	D3DXVECTOR2 size[MAX_RANK_UI] =
	{
		{ D3DXVECTOR2(500.0f,150.0f) },
		{ D3DXVECTOR2(700.0f,500.0f) },
		{ D3DXVECTOR2(150.0f,100.0f) },
		{ D3DXVECTOR2(150.0f,100.0f) },
		{ D3DXVECTOR2(150.0f,100.0f) },
		{ D3DXVECTOR2(150.0f,100.0f)},
		{ D3DXVECTOR2(150.0f,100.0f)},
	};

	for (int nCnt = 0; nCnt < MAX_RANK_UI; nCnt++)
	{
		// テクスチャーがあったら
		if (strcmp(TexData[nCnt], "") != 0)
		{
			/* テクスチャーのロード */
			D3DXCreateTextureFromFile(pDevice, TexData[nCnt], &m_pTex[nCnt]);
			/* 位置・サイズのロード */
			m_pos[nCnt] = pos[nCnt];
			m_size[nCnt] = size[nCnt];
		}
	}
	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CRank_ui::UnLoad(void)
{
	// テクスチャの開放
	for (int nCnt = 0; nCnt < MAX_RANK_UI; nCnt++)
	{
		if (m_pTex[nCnt] != NULL)
		{
			m_pTex[nCnt]->Release();
			m_pTex[nCnt] = NULL;
		}
	}
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CRank_ui * CRank_ui::Create(void)
{
	// 変数宣言
	CRank_ui * pRank_ui;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pRank_ui = new CRank_ui();
	// 初期化処理
	pRank_ui->Init();
	// 生成したオブジェクトを返す
	return pRank_ui;
}
