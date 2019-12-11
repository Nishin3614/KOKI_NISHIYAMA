// ----------------------------------------
//
// ポーズ用ui処理の説明[tutorial_ui.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "gauge.h"
#include "fade.h"
#include "input.h"
#include "player.h"
#include "game.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define GAUGE_X				(50 / 2)		//ゲージの横幅
#define GAUGE_Y				(120)			//ゲージの縦幅

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
LPDIRECT3DTEXTURE9	CGauge::m_pTex[GAUGE_MAX] = {};
D3DXVECTOR3			CGauge::m_pos[GAUGE_MAX] = {};
D3DXVECTOR2			CGauge::m_size[GAUGE_MAX] = {};

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CGauge::CGauge() : CScene::CScene(ACTRO_GAME_UI, LAYER_UI)
{
	// 初期化
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = NULL;
	}

	m_nCharge = 0; //ゲージのチャージ度
	m_nMaxGauge = 0;  //ゲージのチャージ量の初期値
	m_fTex = 0.0f;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CGauge::~CGauge()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CGauge::Init(void)
{
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = new CScene_TWO;
		m_aScene_Two[nCnt]->SetSize(m_size[nCnt]);
		m_aScene_Two[nCnt]->Init();
		m_aScene_Two[nCnt]->SetPosition(m_pos[nCnt]);
		m_aScene_Two[GAUGE_GAUGE]->SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
		m_aScene_Two[nCnt]->BindTexture(m_pTex[nCnt]);
	}
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CGauge::Uninit(void)
{
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
	{
		if (m_aScene_Two[nCnt] != NULL)
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
void CGauge::Update(void)
{
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
	{
		if (m_aScene_Two[nCnt] != NULL)
		{
			// 種類によって処理を変える
			Various(GAUGE_GAUGE);
			// 種類によって処理を変える
			//m_aScene_Two[nCnt]->Update();
		}
	}
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CGauge::Draw(void)
{
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
	{
		if (m_aScene_Two[nCnt] != NULL)
		{
			// 種類によって処理を変える
			m_aScene_Two[nCnt]->Draw();
		}
	}

}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CGauge::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// テクスチャー情報代入
	char TexData[GAUGE_MAX][72] =
	{
		"data/TEXTURE/UI/Gauge.png",
		"data/TEXTURE/UI/frame.png",
	};
	// 位置情報代入
	D3DXVECTOR3 pos[GAUGE_MAX] =
	{
		D3DXVECTOR3(1200,620,0),
		D3DXVECTOR3(1200,500,0),
	};
	// サイズ情報代入
	D3DXVECTOR2 size[GAUGE_MAX] =
	{
		D3DXVECTOR2(GAUGE_X / 2,120),
		D3DXVECTOR2(GAUGE_X * 2 + 10,250),
	};
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
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
void CGauge::UnLoad(void)
{
	// テクスチャの開放
	for (int nCnt = 0; nCnt < GAUGE_MAX; nCnt++)
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
CGauge * CGauge::Create(void)
{
	// 変数宣言
	CGauge * pGauge;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pGauge = new CGauge();
	// 初期化処理
	pGauge->Init();
	// 生成したオブジェクトを返す
	return pGauge;
}

// ----------------------------------------
// ゲージの値の取得
// ----------------------------------------
int CGauge::GetGaugeValue(void)
{
	return m_nCharge;
}

// ----------------------------------------
// 値の再設定(初期化)
// ----------------------------------------
void CGauge::ResetJump(void)
{
	m_nCharge = 0;
	m_nMaxGauge = 0;
	D3DXVECTOR2 size(GAUGE_X, (float)m_nMaxGauge);
	m_aScene_Two[GAUGE_GAUGE]->SetGaugeSize(size);
	m_fTex = 0.0f;
}
// ----------------------------------------
// 種類別の処理
// ----------------------------------------
void CGauge::Various(int nCnt)
{
	// 情報取得
	CPlayer *pPlayer = CGame::GetPlayer();

	if (nCnt == GAUGE_GAUGE)
	{
		for (int nCntlayer = 0; nCntlayer < CScene::GetMaxLayer(CScene::LAYER_3DOBJECT); nCntlayer++)
		{
			if (pPlayer = (CPlayer *)CScene::GetActor(CScene::ACTOR_PLAYER, CScene::LAYER_3DOBJECT, nCntlayer)) break;
		}
		if (pPlayer->GetbJump() == false)
		{
			if (CManager::GetKeyboard()->GetKeyboardPress(DIK_SPACE))
			{//spaceを押しているときにゲージの値を加算
				float fHeight = 50 * ((float)m_nCharge) / 25;

				if (m_nCharge < GAGE_VALUE_MAX)
				{
					m_nCharge++;
				}

				if (m_nMaxGauge < fHeight)
				{
					if (m_nMaxGauge >= 0)
					{
						m_nMaxGauge += 2;

						D3DXVECTOR2 size(GAUGE_X, (float)m_nMaxGauge);
						m_aScene_Two[GAUGE_GAUGE]->SetGaugeSize(size);

						m_fTex -= 0.0083f;

						m_aScene_Two[GAUGE_GAUGE]->SetTex(D3DXVECTOR2(1.0f, m_fTex), D3DXVECTOR2(0.0f, 0.0f));
					}
				}
			}

			if (CManager::GetJoy()->GetPress(0, CJoypad::KEY_A))
			{
				float fHeight = 50 * ((float)m_nCharge) / 25;

				if (m_nCharge < GAGE_VALUE_MAX)
				{
					m_nCharge++;
				}

				if (m_nMaxGauge < fHeight)
				{
					if (m_nMaxGauge >= 0)
					{
						m_nMaxGauge += 2;

						D3DXVECTOR2 size(GAUGE_X, (float)m_nMaxGauge);
						m_aScene_Two[GAUGE_GAUGE]->SetGaugeSize(size);

						m_fTex -= 0.0083f;

						m_aScene_Two[GAUGE_GAUGE]->SetTex(D3DXVECTOR2(1.0f, m_fTex), D3DXVECTOR2(0.0f, 0.0f));
					}
				}
			}
		}
	}
}
