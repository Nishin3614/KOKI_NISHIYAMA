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
#include "selection_ui.h"
#include "mark.h"
#include "fade.h"

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
LPDIRECT3DTEXTURE9 CSelection_ui::m_pTex[SELECTION_UI_MAX] = {};
D3DXVECTOR3 CSelection_ui::m_pos[SELECTION_UI_MAX] = {};
D3DXVECTOR2 CSelection_ui::m_size[SELECTION_UI_MAX] = {};

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CSelection_ui::CSelection_ui() : CScene::CScene(ACTOR_SELECTION_UI, LAYER_UI)
{
	// 初期化
	for (int nCnt = 0; nCnt < SELECTION_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = NULL;
	}
	m_fMult = 1.0f;
	m_fAdd = 0.001f;
	m_nCntCol = 0;
	m_mark = NULL;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CSelection_ui::~CSelection_ui()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CSelection_ui::Init(void)
{
	for (int nCnt = 0; nCnt < SELECTION_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = new CScene_TWO;
		m_aScene_Two[nCnt]->SetSize(m_size[nCnt]);
		m_aScene_Two[nCnt]->Init();
		m_aScene_Two[nCnt]->SetPosition(m_pos[nCnt]);
		m_aScene_Two[nCnt]->BindTexture(m_pTex[nCnt]);
		// 背景
		if (nCnt == SELECTION_UI_BG)
		{
			m_aScene_Two[nCnt]->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f));
		}
		// 選択画面の一人の場合
		if (nCnt == SELECTION_UI_SINGLE)
		{
			// 目印
			m_mark = CMark::Create(
				m_aScene_Two[nCnt]->GetPosition(),
				m_aScene_Two[nCnt]->GetSize(),
				CScene::ACTOR_MARK);
			m_nSelect = SELECTION_UI_SINGLE;
		}
	}
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CSelection_ui::Uninit(void)
{
	for (int nCnt = 0; nCnt < SELECTION_UI_MAX; nCnt++)
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
void CSelection_ui::Update(void)
{
	for (int nCnt = 0; nCnt < SELECTION_UI_MAX; nCnt++)
	{
		if (m_aScene_Two[nCnt] != NULL)
		{
			// 種類によって処理を変える
			m_aScene_Two[nCnt]->Update();
		}
	}
	D3DXVECTOR2 size = m_size[m_nSelect];	// サイズの取得

	// 下を押したら
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_DOWN))
	{
		// 選択アップ
		m_nSelect += 1;

		// 上限超えたら
		if (m_nSelect > SELECTION_UI_DISCRIPTION)
		{
			m_nSelect = SELECTION_UI_SINGLE;
		}
		// サイズの設定
		m_aScene_Two[m_nSelect]->SetSize(size);
		m_fMult = 1.0f;
		m_mark->SetPosGoal(m_aScene_Two[m_nSelect]->GetPosition(), m_aScene_Two[m_nSelect]->GetSize());
	}
	// 上を押したら
	else if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_UP))
	{
		// 選択アップ
		m_nSelect -= 1;
		// 下限超えたら
		if (m_nSelect < SELECTION_UI_SINGLE)
		{
			m_nSelect = SELECTION_UI_DISCRIPTION;
		}
		// サイズの設定
		m_aScene_Two[m_nSelect]->SetSize(size);
		m_fMult = 1.0f;
		m_mark->SetPosGoal(m_aScene_Two[m_nSelect]->GetPosition(), m_aScene_Two[m_nSelect]->GetSize());
	}
	// エンター押したら
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_RETURN))
	{
		// 画面遷移の状態が遷移していない状態だったら
		if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
		{

			// エンター音
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_ENTER);
			// 一人でゲーム
			if (m_nSelect == SELECTION_UI_SINGLE)
			{
				CManager::GetFade()->SetFade(CManager::MODE_GAME);
			}
			// チュートリアル
			if (m_nSelect == SELECTION_UI_DISCRIPTION)
			{
				CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
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
	size *= m_fMult;

	// サイズの設定
	m_aScene_Two[m_nSelect]->SetSize(size);

}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CSelection_ui::Draw(void)
{
	for (int nCnt = 0; nCnt < SELECTION_UI_MAX; nCnt++)
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
HRESULT CSelection_ui::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// テクスチャー情報代入
	char TexData[SELECTION_UI_MAX][72] =
	{
		"png",
		"data/TEXTURE/UI/Single.png",
		"data/TEXTURE/UI/Description.png" 
	};
	// 位置情報代入
	D3DXVECTOR3 pos[SELECTION_UI_MAX] =
	{
		D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f),
		D3DXVECTOR3(SCREEN_WIDTH / 2,200.0f,0.0f),
		D3DXVECTOR3(SCREEN_WIDTH / 2,500.0f,0.0f)
	};
	// サイズ情報代入
	D3DXVECTOR2 size[SELECTION_UI_MAX] =
	{
		D3DXVECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT),
		D3DXVECTOR2(700.0f,300.0f),
		D3DXVECTOR2(700.0f,300.0f)
	};

	for (int nCnt = 0; nCnt < SELECTION_UI_MAX; nCnt++)
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
void CSelection_ui::UnLoad(void)
{
	// テクスチャの開放
	for (int nCnt = 0; nCnt < SELECTION_UI_MAX; nCnt++)
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
CSelection_ui * CSelection_ui::Create(void)
{
	// 変数宣言
	CSelection_ui * pSelection_ui;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pSelection_ui = new CSelection_ui();
	// 初期化処理
	pSelection_ui->Init();
	// 生成したオブジェクトを返す
	return pSelection_ui;
}
