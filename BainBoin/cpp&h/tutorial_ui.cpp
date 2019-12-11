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
#include "tutorial_ui.h"
#include "fade.h"
#include "input.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define TUTORIAL_UI_ANIM (2)
#define TUTORIAL_UI_SPLIT (1.0f / TUTORIAL_UI_ANIM)

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
LPDIRECT3DTEXTURE9	CTutorial_ui::m_pTex[TUTORIAL_UI_MAX] = {};
D3DXVECTOR3			CTutorial_ui::m_pos[TUTORIAL_UI_MAX] = {};
D3DXVECTOR2			CTutorial_ui::m_size[TUTORIAL_UI_MAX] = {};

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CTutorial_ui::CTutorial_ui() : CScene::CScene(ACTOR_TUTORIAL_UI, LAYER_UI)
{
	// 初期化
	for (int nCnt = 0; nCnt < TUTORIAL_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = NULL;
	}
	m_fMult = 1.0f;
	m_fAdd = 0.001f;
	m_nCntCol = 0;
	m_nSelect = 0;
	m_fAnim = 0;
	m_TutorialState = TUTORIAL_STATE_FIRST;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CTutorial_ui::~CTutorial_ui()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CTutorial_ui::Init(void)
{
	for (int nCnt = 0; nCnt < TUTORIAL_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = new CScene_TWO;
		m_aScene_Two[nCnt]->SetSize(m_size[nCnt]);
		m_aScene_Two[nCnt]->Init();
		m_aScene_Two[nCnt]->SetPosition(m_pos[nCnt]);
		m_aScene_Two[TUTORIAL_UI_MANUAL]->SetTex(D3DXVECTOR2(m_nSelect * TUTORIAL_UI_SPLIT, 0.0f), D3DXVECTOR2(m_nSelect * TUTORIAL_UI_SPLIT + TUTORIAL_UI_SPLIT, 1.0f));
		m_aScene_Two[nCnt]->BindTexture(m_pTex[nCnt]);
	}
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CTutorial_ui::Uninit(void)
{
	for (int nCnt = 0; nCnt < TUTORIAL_UI_MAX; nCnt++)
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
void CTutorial_ui::Update(void)
{
	for (int nCnt = 0; nCnt < TUTORIAL_UI_MAX; nCnt++)
	{
		if (m_aScene_Two[nCnt] != NULL)
		{
			// 種類によって処理を変える
			Various(nCnt);
			// 種類によって処理を変える
			m_aScene_Two[nCnt]->Update();
		}
	}
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CTutorial_ui::Draw(void)
{
	for (int nCnt = 0; nCnt < TUTORIAL_UI_MAX; nCnt++)
	{
		if (m_aScene_Two[nCnt] != NULL)
		{
			m_aScene_Two[TUTORIAL_UI_MANUAL]->Draw();

			if (m_TutorialState == TUTORIAL_STATE_FIRST)
			{
				m_aScene_Two[TUTORIAL_UI_CLICK_R]->Draw();
			}

			if (m_TutorialState == TUTORIAL_STATE_SECOND)
			{
				m_aScene_Two[TUTORIAL_UI_CLICK_L]->Draw();
				m_aScene_Two[TUTORIAL_UI_START]->Draw();
			}

			// 種類によって処理を変える
			//m_aScene_Two[nCnt]->Draw();
		}
	}
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CTutorial_ui::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// テクスチャー情報代入
	char TexData[TUTORIAL_UI_MAX][72] =
	{
		"data/TEXTURE/TUTORIAL/Tutorial_Manual.png",
		"data/TEXTURE/TUTORIAL/ClickBottunR.png",
		"data/TEXTURE/TUTORIAL/ClickBottunL.png",
		"data/TEXTURE/TUTORIAL/ClickBottunS.png"
	};
	// 位置情報代入
	D3DXVECTOR3 pos[TUTORIAL_UI_MAX] =
	{
		D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f),
		D3DXVECTOR3(1200.0f,650,0.0f),
		D3DXVECTOR3(50.0f,650,0.0f),
		D3DXVECTOR3(1200.0f,650,0.0f)
	};
	// サイズ情報代入
	D3DXVECTOR2 size[TUTORIAL_UI_MAX] =
	{
		D3DXVECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT),
		D3DXVECTOR2(100.0f,100.0f),
		D3DXVECTOR2(100.0f,100.0f),
		D3DXVECTOR2(150.0f,150.0f)
	};

	for (int nCnt = 0; nCnt < TUTORIAL_UI_MAX; nCnt++)
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
void CTutorial_ui::UnLoad(void)
{
	// テクスチャの開放
	for (int nCnt = 0; nCnt < TUTORIAL_UI_MAX; nCnt++)
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
CTutorial_ui * CTutorial_ui::Create(void)
{
	// 変数宣言
	CTutorial_ui * pTutorial_ui;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pTutorial_ui = new CTutorial_ui();
	// 初期化処理
	pTutorial_ui->Init();
	// 生成したオブジェクトを返す
	return pTutorial_ui;
}

// ----------------------------------------
// 種類別の処理
// ----------------------------------------
void CTutorial_ui::Various(int nCnt)
{
	// 説明書の場合
	if (nCnt == TUTORIAL_UI_MANUAL)
	{
		//キーボード
		if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RIGHT))
		{
			m_nSelect++;
			m_TutorialState = TUTORIAL_STATE_SECOND;
			// エンター音
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_1);

			// 選択画面へ
			if (m_nSelect >= 2)
			{
				// 画面遷移の状態が遷移していない状態だったら
				if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
				{
					CManager::GetFade()->SetFade(CManager::MODE_GAME);
				}
			}
			// 次のページへ
			else
			{
				m_aScene_Two[TUTORIAL_UI_MANUAL]->SetTex(
					D3DXVECTOR2(m_nSelect * TUTORIAL_UI_SPLIT, 0.0f),
					D3DXVECTOR2(m_nSelect * TUTORIAL_UI_SPLIT + TUTORIAL_UI_SPLIT, 1.0f));
			}
		}

		else if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_LEFT))
		{
			m_nSelect--;
			m_TutorialState = TUTORIAL_STATE_FIRST;
			// エンター音
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_1);

			// 選択画面へ
			if (m_nSelect < 0)
			{
				m_nSelect = 0;
			}
			// 前のページへ
			else
			{
				m_aScene_Two[TUTORIAL_UI_MANUAL]->SetTex(
					D3DXVECTOR2(m_nSelect * TUTORIAL_UI_SPLIT, 0.0f),
					D3DXVECTOR2(m_nSelect * TUTORIAL_UI_SPLIT + TUTORIAL_UI_SPLIT, 1.0f));
			}
		}

		//ジョイスティック 十字キー
		if (CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_RIGHT))
		{
			m_nSelect++;
			m_TutorialState = TUTORIAL_STATE_SECOND;
			// エンター音
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_1);

			// 選択画面へ
			if (m_nSelect >= 2)
			{
				// 画面遷移の状態が遷移していない状態だったら
				if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
				{
					CManager::GetFade()->SetFade(CManager::MODE_GAME);
				}
			}
			// 次のページへ
			else
			{
				m_aScene_Two[TUTORIAL_UI_MANUAL]->SetTex(
					D3DXVECTOR2(m_nSelect * TUTORIAL_UI_SPLIT, 0.0f),
					D3DXVECTOR2(m_nSelect * TUTORIAL_UI_SPLIT + TUTORIAL_UI_SPLIT, 1.0f));
			}
		}
		else if (CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_LEFT))
		{
			m_nSelect--;
			m_TutorialState = TUTORIAL_STATE_FIRST;
			// エンター音
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_1);

			// 選択画面へ
			if (m_nSelect < 0)
			{
				m_nSelect = 0;
			}
			// 前のページへ
			else
			{
				m_aScene_Two[TUTORIAL_UI_MANUAL]->SetTex(
					D3DXVECTOR2(m_nSelect * TUTORIAL_UI_SPLIT, 0.0f),
					D3DXVECTOR2(m_nSelect * TUTORIAL_UI_SPLIT + TUTORIAL_UI_SPLIT, 1.0f));
			}
		}

		//説明書が二枚目だったら
		if (m_nSelect == 1)
		{
			if (CManager::GetJoy()->GetTrigger(0, CJoypad::KEY_START))
			{
				CManager::GetFade()->SetFade(CManager::MODE_GAME);
			}
		}

	}

	// クリック
	else if (nCnt == TUTORIAL_UI_CLICK_R)
	{
		// カウントカラーアップ
		m_nCntCol++;
		// 表示状態へ
		if (m_nCntCol % 40 == 0)
		{
			m_aScene_Two[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		// 非表示状態へ
		else if (m_nCntCol % 20 == 0)
		{
			m_aScene_Two[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}
	// クリック
	else if (nCnt == TUTORIAL_UI_CLICK_L)
	{
		// カウントカラーアップ
		m_nCntCol++;
		// 表示状態へ
		if (m_nCntCol % 40 == 0)
		{
			m_aScene_Two[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		// 非表示状態へ
		else if (m_nCntCol % 20 == 0)
		{
			m_aScene_Two[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}
	// クリック
	else if (nCnt == TUTORIAL_UI_START)
	{
		// カウントカラーアップ
		m_nCntCol++;
		// 表示状態へ
		if (m_nCntCol % 40 == 0)
		{
			m_aScene_Two[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		// 非表示状態へ
		else if (m_nCntCol % 20 == 0)
		{
			m_aScene_Two[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}
}
