// ----------------------------------------
//
// プレイ用UI処理の説明[play_element_ui.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "play_element_ui.h"
#include "fade.h"
#include "mark.h"
#include "game.h"
#include "number.h"
#include "score.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------

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
LPDIRECT3DTEXTURE9 CPlay_element_ui::m_pTex[PLAY_ELEMENT_UI_MAX] = {};
D3DXVECTOR3 CPlay_element_ui::m_pos[PLAY_ELEMENT_UI_MAX] = {};
D3DXVECTOR2 CPlay_element_ui::m_size[PLAY_ELEMENT_UI_MAX] = {};

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CPlay_element_ui::CPlay_element_ui() : CScene::CScene(ACTOR_PLAYER_ELEMENT_UI, LAYER_UI)
{
	// 初期化
	for (int nCnt = 0; nCnt < PLAY_ELEMENT_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = NULL;
	}
	playerID = CManager::PLAYER_1;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CPlay_element_ui::~CPlay_element_ui()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CPlay_element_ui::Init(void)
{
	for (int nCnt = 0; nCnt < PLAY_ELEMENT_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = new CScene_TWO;
		m_aScene_Two[nCnt]->SetSize(m_size[nCnt]);
		m_aScene_Two[nCnt]->Init();
		m_aScene_Two[nCnt]->BindTexture(m_pTex[nCnt]);
	}
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CPlay_element_ui::Uninit(void)
{
	for (int nCnt = 0; nCnt < PLAY_ELEMENT_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt]->Uninit();
		delete m_aScene_Two[nCnt];
		m_aScene_Two[nCnt] = NULL;
	}
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CPlay_element_ui::Update(void)
{
	for (int nCnt = 0; nCnt < PLAY_ELEMENT_UI_MAX; nCnt++)
	{
		// 種類によって処理を変える
		m_aScene_Two[nCnt]->Update();
	}
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CPlay_element_ui::Draw(void)
{
	// スコアのUI
	m_aScene_Two[CPlay_element_ui::PLAY_ELEMENT_UI_SCORE]->Draw();
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CPlay_element_ui::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// テクスチャー情報代入
	char TexData[PLAY_ELEMENT_UI_MAX][72] =
	{
		{"data/TEXTURE/UI/Frame_Score2.png"},	// スコア
		{"data/TEXTURE/UI/Time_Comment.png"},	// タイム
	};
	// 位置情報代入
	D3DXVECTOR3 pos[PLAY_ELEMENT_UI_MAX] =
	{
		{D3DXVECTOR3(100.0f,100.0f,0.0f)},
		{D3DXVECTOR3(20.0f,20.0f,0.0f)},
	};
	// サイズ情報代入
	D3DXVECTOR2 size[PLAY_ELEMENT_UI_MAX] =
	{
		{D3DXVECTOR2(500.0f,70.0f)},
		{D3DXVECTOR2(50.0f,50.0f) },
	};

	for (int nCnt = 0; nCnt < PLAY_ELEMENT_UI_MAX; nCnt++)
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
void CPlay_element_ui::UnLoad(void)
{
	// テクスチャの開放
	for (int nCnt = 0; nCnt < PLAY_ELEMENT_UI_MAX; nCnt++)
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
CPlay_element_ui * CPlay_element_ui::Create(CManager::PLAYER playerId)
{
	// 変数宣言
	CPlay_element_ui * pPlay_element_ui;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pPlay_element_ui = new CPlay_element_ui();
	// 
	pPlay_element_ui->playerID = playerId;
	// 初期化処理
	pPlay_element_ui->Init();
	// 生成したオブジェクトを返す
	return pPlay_element_ui;
}
