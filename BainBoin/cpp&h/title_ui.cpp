// ----------------------------------------
//
// ポーズ用ui処理の説明[title_ui.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "title_ui.h"
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
LPDIRECT3DTEXTURE9 CTitle_ui::m_pTex[TITLE_UI_MAX] = {};
D3DXVECTOR3 CTitle_ui::m_pos[TITLE_UI_MAX] = {};
D3DXVECTOR2 CTitle_ui::m_size[TITLE_UI_MAX] = {};

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CTitle_ui::CTitle_ui() : CScene::CScene(ACTOR_TITLE_UI, LAYER_UI)
{
	// 初期化
	for (int nCnt = 0; nCnt < TITLE_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = NULL;
	}
	m_fMult = 1.0f;
	m_fAdd = 0.001f;
	m_nCntCol = 0;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CTitle_ui::~CTitle_ui()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CTitle_ui::Init(void)
{
	for (int nCnt = 0; nCnt < TITLE_UI_MAX; nCnt++)
	{
		m_aScene_Two[nCnt] = new CScene_TWO;
		m_aScene_Two[nCnt]->SetSize(m_size[nCnt]);
		m_aScene_Two[nCnt]->Init();
		m_aScene_Two[nCnt]->SetPosition(m_pos[nCnt]);
		m_aScene_Two[nCnt]->BindTexture(m_pTex[nCnt]);
	}
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CTitle_ui::Uninit(void)
{
	for (int nCnt = 0; nCnt < TITLE_UI_MAX; nCnt++)
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
void CTitle_ui::Update(void)
{
	for (int nCnt = 0; nCnt < TITLE_UI_MAX; nCnt++)
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
void CTitle_ui::Draw(void)
{
	for (int nCnt = 0; nCnt < TITLE_UI_MAX; nCnt++)
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
HRESULT CTitle_ui::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// テクスチャー情報代入
	char TexData[TITLE_UI_MAX][72] =
	{
		"data/TEXTURE/UI/TitleName.png",
		"data/TEXTURE/UI/ClickBottun.png"
	};
	// 位置情報代入
	D3DXVECTOR3 pos[TITLE_UI_MAX] =
	{
		D3DXVECTOR3(640.0f,150,0.0f),
		D3DXVECTOR3(540.0f,650.0f,0.0f)
	};
	// サイズ情報代入
	D3DXVECTOR2 size[TITLE_UI_MAX] =
	{
		D3DXVECTOR2(700.0f,200.0f),
		D3DXVECTOR2(500.0f,60.0f)
	};

	for (int nCnt = 0; nCnt < TITLE_UI_MAX; nCnt++)
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
void CTitle_ui::UnLoad(void)
{
	// テクスチャの開放
	for (int nCnt = 0; nCnt < TITLE_UI_MAX; nCnt++)
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
CTitle_ui * CTitle_ui::Create(void)
{
	// 変数宣言
	CTitle_ui * pTitle_ui;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pTitle_ui = new CTitle_ui();
	// 初期化処理
	pTitle_ui->Init();
	// 生成したオブジェクトを返す
	return pTitle_ui;
}

// ----------------------------------------
// 種類別の処理
// ----------------------------------------
void CTitle_ui::Various(int nCnt)
{
	// タイトルの名前の場合
	if (nCnt == TITLE_UI_NAME)
	{
		D3DXVECTOR2 size = m_size[nCnt];	// サイズの取得
													// 加算か減算
		if (m_fMult >= 1.1f || m_fMult <= 0.9f)
		{
			m_fAdd *= -1;
		}
		// サイズの変化
		m_fMult += m_fAdd;
		size *= m_fMult;

		// サイズの設定
		m_aScene_Two[nCnt]->SetSize(size);
	}
	// クリック
	else if (nCnt == TITLE_UI_CLICK)
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
