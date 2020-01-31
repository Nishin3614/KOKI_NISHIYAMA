// ----------------------------------------
//
// バッググラウンドドーム処理の説明[bgdome.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "bgdome.h"
#include "meshdome.h"
#include "game.h"
#include "scene.h"
#include "debugproc.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define BGDOME_ROTMOVE (0.1f)				// 角度の移動量
#define BGDOME_SIZE (100.0f)				// サイズ

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
CMeshdome			*CBgdome::m_aMeshdome[TYPE_MAX] = {};	// メッシュドーム
bool				CBgdome::m_bUse = false;				// 使用状態
int					CBgdome::m_CntFrame = 0;				// カウントフレーム
int					CBgdome::m_nMaxFrame = 0;				// 最大フレーム

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CBgdome::CBgdome() : CScene(ACTOR_BGDOME,LAYER_3DOBJECT)
{
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CBgdome::~CBgdome()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CBgdome::Init(void)
{
	/* 変数の初期化 */
	for (int nCntBgdome = 0; nCntBgdome < TYPE_MAX; nCntBgdome++)
	{
		m_aMeshdome[nCntBgdome] = CMeshdome::Create(
			D3DXVECTOR3(500.0f, 500.0f, 500.0f),
			10,
			10,
			CMeshdome::TYPE_NORMAL
		);
	}
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CBgdome::Uninit(void)
{
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CBgdome::Update(void)
{
	if (m_bUse)
	{
		// フレームカウントアップ
		m_CntFrame++;
		if (m_CntFrame >= m_nMaxFrame)
		{
			// 使用状態
			SetUse(false);
		}
	}
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CBgdome::Draw(void)
{
}

#ifdef _DEBUG
// ----------------------------------------
// デバッグ表示処理
// ----------------------------------------
void CBgdome::Debug(void)
{
}
#endif // _DEBUG
// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CBgdome::Load(void)
{
	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CBgdome::UnLoad(void)
{
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CBgdome * CBgdome::Create(void)
{
	// 変数宣言
	CBgdome * pBgdome;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pBgdome = new CBgdome();
	// 初期化処理
	pBgdome->Init();
	// 生成したオブジェクトを返す
	return pBgdome;
}

// ----------------------------------------
// 使用状態処理
// ----------------------------------------
void CBgdome::SetUse(bool bUse)
{
	for (int nCntBgdome = 0; nCntBgdome < TYPE_MAX; nCntBgdome++)
	{
		m_aMeshdome[nCntBgdome]->SetUse(bUse);
	}
	m_bUse = bUse;
}

// ----------------------------------------
// 位置設定処理
// ----------------------------------------
void CBgdome::SetPos(D3DXVECTOR3 const & pos)
{
	for (int nCntBgdome = 0; nCntBgdome < TYPE_MAX; nCntBgdome++)
	{
		m_aMeshdome[nCntBgdome]->SetPos(pos);
	}
}

// ----------------------------------------
// 回転設定処理
// ----------------------------------------
void CBgdome::SetRot(D3DXVECTOR3 const & rot)
{
	for (int nCntBgdome = 0; nCntBgdome < TYPE_MAX; nCntBgdome++)
	{
		m_aMeshdome[nCntBgdome]->SetRot(rot);
	}
}

// ----------------------------------------
// 設定処理
// ----------------------------------------
void CBgdome::Set(
	D3DXVECTOR3 const & pos, 
	D3DXVECTOR3 const & rot, 
	int const & nFrame
)
{
	for (int nCntBgdome = 0; nCntBgdome < TYPE_MAX; nCntBgdome++)
	{
		m_aMeshdome[nCntBgdome]->SetPos(pos);
		m_aMeshdome[nCntBgdome]->SetRot(rot);
		m_aMeshdome[nCntBgdome]->SetUse(true);
	}
	m_CntFrame = 0;
	m_bUse = true;
	m_nMaxFrame = nFrame;
}
