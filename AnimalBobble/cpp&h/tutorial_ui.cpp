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
#define TUTORIAL_UI_ANIM (4)
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
LPDIRECT3DTEXTURE9 CTutorial_ui::m_pTex = NULL;

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CTutorial_ui::CTutorial_ui() : CScene::CScene(ACTOR_TUTORIAL_UI, LAYER_UI)
{
	// 初期化
	m_aScene_Two = NULL;
	m_nSelect = 0;
	m_fAnim = 0;
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
	m_aScene_Two = new CScene_TWO;
	m_aScene_Two->SetSize(D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
	m_aScene_Two->Init();
	m_aScene_Two->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	m_aScene_Two->SetTex(D3DXVECTOR2(m_nSelect * TUTORIAL_UI_SPLIT, 0.0f), D3DXVECTOR2(m_nSelect * TUTORIAL_UI_SPLIT + TUTORIAL_UI_SPLIT, 1.0f));
	m_aScene_Two->BindTexture(m_pTex);
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CTutorial_ui::Uninit(void)
{
	if (m_aScene_Two != NULL)
	{
		m_aScene_Two->Uninit();
		delete m_aScene_Two;
		m_aScene_Two = NULL;
	}
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CTutorial_ui::Update(void)
{
	if (m_aScene_Two != NULL)
	{
		if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN))
		{
			m_nSelect++;
			// エンター音
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_ENTER);

			// 選択画面へ
			if (m_nSelect >= 4)
			{
				// 画面遷移の状態が遷移していない状態だったら
				if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
				{
					CManager::GetFade()->SetFade(CManager::MODE_SELECTION);
				}
			}
			// 次のページへ
			else
			{
				m_aScene_Two->SetTex(
					D3DXVECTOR2(m_nSelect * TUTORIAL_UI_SPLIT, 0.0f),
					D3DXVECTOR2(m_nSelect * TUTORIAL_UI_SPLIT + TUTORIAL_UI_SPLIT, 1.0f));
			}
		}
		// 種類によって処理を変える
		m_aScene_Two->Update();
	}
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CTutorial_ui::Draw(void)
{
	if (m_aScene_Two != NULL)
	{
		// 種類によって処理を変える
		m_aScene_Two->Draw();
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
	char TexData[72] =
	{
		"data/TEXTURE/Tutorial/discriptions_UI.png",
	};

		// テクスチャーがあったら
	if (strcmp(TexData, "") != 0)
	{
		/* テクスチャーのロード */
		D3DXCreateTextureFromFile(pDevice, TexData, &m_pTex);
	}
	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CTutorial_ui::UnLoad(void)
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