// ----------------------------------------
//
// ポーズ用ui処理の説明[pause_ui.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "pause_ui.h"
#include "pause.h"
#include "topscore.h"
#include "fade.h"
#include "mark.h"
#include "game.h"

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
LPDIRECT3DTEXTURE9 CPause_ui::m_pTex[PAUSE_UI_MAX] = {};
D3DXVECTOR3 CPause_ui::m_pos[PAUSE_UI_MAX] = {};
D3DXVECTOR2 CPause_ui::m_size[PAUSE_UI_MAX] = {};
int CPause_ui::m_nSelect = 2;

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CPause_ui::CPause_ui() : CScene::CScene(ACTOR_PAUSE_UI, LAYER_UI)
{
	// 初期化
	for (int nCnt = 0; nCnt < PAUSE_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = NULL;
	}
	m_mark = NULL;
	m_nCntCol = 0;
	m_fMult = 1.0f;
	m_fAdd = 0;
	m_nCntCol = 0;
	m_nSelect = 2;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CPause_ui::~CPause_ui()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CPause_ui::Init(void)
{
	for (int nCnt = 0; nCnt < PAUSE_UI_MAX; nCnt++)
	{
		if (m_pTex[nCnt] != NULL)
		{
			m_aScene_Two[nCnt] = new CScene_TWO;
			m_aScene_Two[nCnt]->SetSize(m_size[nCnt]);
			m_aScene_Two[nCnt]->Init();
			if (nCnt == 0)
			{
				m_aScene_Two[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			}
			m_aScene_Two[nCnt]->SetPosition(m_pos[nCnt]);
			m_aScene_Two[nCnt]->BindTexture(m_pTex[nCnt]);
			// マーク設定
			if (m_pTex[nCnt] == m_pTex[PAUSE_UI_GAME])
			{
				// 目印
				m_mark = CMark::Create(
					m_aScene_Two[nCnt]->GetPosition(),
					m_aScene_Two[nCnt]->GetSize(),
					ACTOR_PAUSE_UI);
				// 目標の位置・サイズの設定処理
				m_mark->SetPosGoal(m_aScene_Two[nCnt]->GetPosition(), m_aScene_Two[nCnt]->GetSize());
			}

		}
	}
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CPause_ui::Uninit(void)
{
	for (int nCnt = 0; nCnt < PAUSE_UI_MAX; nCnt++)
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
void CPause_ui::Update(void)
{
	D3DXVECTOR2 size = m_size[m_nSelect];	// サイズの取得

	for (int nCnt = 0; nCnt < PAUSE_UI_MAX; nCnt++)
	{
		// ヌルチェック
		if (m_pTex[nCnt] != NULL)
		{
			// 種類によって処理を変える
			m_aScene_Two[nCnt]->Update();
		}
	}

	// 下を押したら
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_D) ||
		CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RIGHT))
	{
		// 選択アップ
		m_nSelect++;

		// 上限超えたら
		if (m_nSelect > PAUSE_UI_RESTART)
		{
			m_nSelect = PAUSE_UI_GAME;
		}
		// サイズの設定
		m_aScene_Two[m_nSelect]->SetSize(size);
		// 倍数
		m_fMult = 1.0f;
		// 目標の位置・サイズの設定処理
		m_mark->SetPosGoal(m_aScene_Two[m_nSelect]->GetPosition(), m_aScene_Two[m_nSelect]->GetSize());
	}
	// 上を押したら
	else if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_A) ||
		CManager::GetKeyboard()->GetKeyboardTrigger(DIK_LEFT))
	{
		// 選択アップ
		m_nSelect--;
		// 下限超えたら
		if (m_nSelect < PAUSE_UI_GAME)
		{
			m_nSelect = PAUSE_UI_RESTART;
		}
		// サイズの設定
		m_aScene_Two[m_nSelect]->SetSize(size);
		// 倍数
		m_fMult = 1.0f;
		// 目標の位置・サイズの設定処理
		m_mark->SetPosGoal(m_aScene_Two[m_nSelect]->GetPosition(), m_aScene_Two[m_nSelect]->GetSize());
	}
	// エンター押したら
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN))
	{
		// 選択状態の設定
		CManager::GetGame()->SetState(CGame::STATE_NORMAL);
		// エンター音
		CManager::GetSound()->PlaySound(CSound::LABEL_SE_ENTER);

		// ゲームに戻る
		if (m_nSelect == PAUSE_UI_GAME)
		{
			CManager::GetGame()->SetState(CGame::STATE_NORMAL);
			CPause::SetPause(false);
		}
		// タイトルへ戻る
		if (m_nSelect == PAUSE_UI_TITLE)
		{
			// 画面遷移の状態が遷移していない状態だったら
			if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
			{
				CManager::GetFade()->SetFade(CManager::MODE_TITLE);
			}
		}
		// もう一度プレイ
		if (m_nSelect == PAUSE_UI_RESTART)
		{
			// 画面遷移の状態が遷移していない状態だったら
			if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
			{
				CManager::GetFade()->SetFade(CManager::MODE_GAME);
			}
		}
	}
	// 加算か減算
	if (m_fMult >= 1.1f || m_fMult <= 0.9f)
	{
		m_fAdd *= -1;
	}
	// サイズの変化
	m_fMult += m_fAdd;

	// サイズ倍数
	size *= m_fMult;

	// サイズの設定
	m_aScene_Two[m_nSelect]->SetSize(size);
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CPause_ui::Draw(void)
{
	for (int nCnt = 0; nCnt < PAUSE_UI_MAX; nCnt++)
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
HRESULT CPause_ui::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// テクスチャー情報代入
	char TexData[PAUSE_UI_MAX][72] =
	{
		{""},
		{"data/TEXTURE/PAUSE/00_pousecomment.png"},
		{"data/TEXTURE/PAUSE/01_GoGame.png"},
		{"data/TEXTURE/PAUSE/02_GoTitle.png"},
		{"data/TEXTURE/PAUSE/03_ReGame.png"},
	};
	// 位置情報代入
	D3DXVECTOR3 pos[PAUSE_UI_MAX] =
	{
		{D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f)},
		{D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 200.0f,0.0f)},
		{D3DXVECTOR3(SCREEN_WIDTH * 0.25f, 500.0f,0.0f)},
		{D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 500.0f,0.0f)},
		{D3DXVECTOR3(SCREEN_WIDTH * 0.75f, 500.0f,0.0f)},
	};
	// サイズ情報代入
	D3DXVECTOR2 size[PAUSE_UI_MAX] =
	{
		{ D3DXVECTOR2(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2) },
		{ D3DXVECTOR2(500.0f,500.0f) },
		{ D3DXVECTOR2(300.0f,300.0f) },
		{D3DXVECTOR2(300.0f,300.0f)},
		{D3DXVECTOR2(300.0f,300.0f)},
	};

	for (int nCnt = 0; nCnt < PAUSE_UI_MAX; nCnt++)
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
void CPause_ui::UnLoad(void)
{
	// テクスチャの開放
	for (int nCnt = 0; nCnt < PAUSE_UI_MAX; nCnt++)
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
CPause_ui * CPause_ui::Create(void)
{
	// 変数宣言
	CPause_ui * pPause_ui;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pPause_ui = new CPause_ui();
	// 初期化処理
	pPause_ui->Init();
	// 生成したオブジェクトを返す
	return pPause_ui;
}
