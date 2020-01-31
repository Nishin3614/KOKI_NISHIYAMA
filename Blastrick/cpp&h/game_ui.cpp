// ----------------------------------------
//
// ポーズ用ui処理の説明[selection_ui.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "game_ui.h"
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
LPDIRECT3DTEXTURE9 CGame_ui::m_pTex[GAME_UI_MAX] = {};
D3DXVECTOR3 CGame_ui::m_pos[GAME_UI_MAX] = {};
D3DXVECTOR2 CGame_ui::m_size[GAME_UI_MAX] = {};

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CGame_ui::CGame_ui() : CScene::CScene(ACTRO_GAME_UI, LAYER_UI)
{
	// 初期化
	for (int nCnt = 0; nCnt < GAME_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = NULL;
	}
	m_pNumber = NULL;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CGame_ui::~CGame_ui()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CGame_ui::Init(void)
{
	for (int nCnt = 0; nCnt < GAME_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = new CScene_TWO;
		m_aScene_Two[nCnt]->SetSize(m_size[nCnt]);
		m_aScene_Two[nCnt]->Init();
		m_aScene_Two[nCnt]->SetPosition(m_pos[nCnt]);
		m_aScene_Two[nCnt]->BindTexture(m_pTex[nCnt]);
		// スタート
		if (nCnt == GAME_UI_START)
		{
			m_pNumber = CNumber::Create();
			m_pNumber->SetTex(CNumber::TEX_SCORE);
			m_pNumber->SetPos(
				m_aScene_Two[nCnt]->GetPosition() + D3DXVECTOR3(0.0f,200.0f,0.0f),
				200.0f);
			m_pNumber->SetNum(1);
			m_pNumber->SetCol(D3DXCOLOR(0.0f, 1.0f, 0.0f,1.0f));
		}
	}
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CGame_ui::Uninit(void)
{
	for (int nCnt = 0; nCnt < GAME_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt]->Uninit();
		delete m_aScene_Two[nCnt];
		m_aScene_Two[nCnt] = NULL;
	}
	// 番号
	if (m_pNumber != NULL)
	{
		m_pNumber->Uninit();
		delete m_pNumber;
		m_pNumber = NULL;
	}
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CGame_ui::Update(void)
{
	for (int nCnt = 0; nCnt < GAME_UI_MAX; nCnt++)
	{
		// 種類によって処理を変える
		m_aScene_Two[nCnt]->Update();
	}
	// 番号
	if (m_pNumber != NULL)
	{
		m_pNumber->Update();
	}
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CGame_ui::Draw(void)
{
	CGame::STATE state = CManager::GetGame()->GetState();
	// 選択スタート時
	if (state == CGame::STATE_START)
	{
		m_aScene_Two[CGame_ui::GAME_UI_START]->Draw();
		// 番号
		if (m_pNumber != NULL)
		{
			m_pNumber->Draw();
		}
	}
	// 選択クリア時
	else if (state == CGame::STATE_CLEAR)
	{
		m_aScene_Two[CGame_ui::GAME_UI_STAGECLEAR]->Draw();
	}
	// 選択オーバー時oi
	else if (state == CGame::STATE_GAMEOVER)
	{
		m_aScene_Two[CGame_ui::GAME_UI_GAMEOVER]->Draw();
	}
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// デバッグ表示
//-------------------------------------------------------------------------------------------------------------
void CGame_ui::Debug(void)
{
}
#endif // _DEBUG

// ----------------------------------------
// 番号取得処理
// ----------------------------------------
CNumber * CGame_ui::GetNumber(void)
{
	return m_pNumber;
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CGame_ui::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// テクスチャー情報代入
	char TexData[GAME_UI_MAX][72] =
	{
		{"data/TEXTURE/UI/stage.png"},
		{"data/TEXTURE/UI/stageclear.png"},
		{"data/TEXTURE/UI/gameover.png"},
	};
	// 位置情報代入
	D3DXVECTOR3 pos[GAME_UI_MAX] =
	{
		{D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f)},
		{D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f)},
		{D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f)},
	};
	// サイズ情報代入
	D3DXVECTOR2 size[GAME_UI_MAX] =
	{
		{D3DXVECTOR2(SCREEN_WIDTH * 0.25f,SCREEN_HEIGHT * 0.25f)},
		{D3DXVECTOR2(SCREEN_WIDTH * 0.25f,SCREEN_HEIGHT * 0.25f)},
		{D3DXVECTOR2(SCREEN_WIDTH * 0.25f,SCREEN_HEIGHT * 0.25f)},
	};

	for (int nCnt = 0; nCnt < GAME_UI_MAX; nCnt++)
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
void CGame_ui::UnLoad(void)
{
	// テクスチャの開放
	for (int nCnt = 0; nCnt < GAME_UI_MAX; nCnt++)
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
CGame_ui * CGame_ui::Create(void)
{
	// 変数宣言
	CGame_ui * pGame_ui;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pGame_ui = new CGame_ui();
	// 初期化処理
	pGame_ui->Init();
	// 生成したオブジェクトを返す
	return pGame_ui;
}
