// ----------------------------------------
//
// 爆発処理の説明[explosion.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "explosion.h"

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
LPDIRECT3DTEXTURE9 CExplosion::m_pTex[CBullet::TYPE_MAX] = {};

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CExplosion::CExplosion() : CScene_TWO::CScene_TWO(ACTOR_EXPLOSION,LAYER_PARTICLE)
{
	/* 変数の初期化 */
	// 位置
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	// タイプ
	m_type = CBullet::TYPE_BEE;
	// 体力
	m_nLife = 100;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CExplosion::~CExplosion()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CExplosion::Init(void)
{
	// サイズ設定
	CScene_TWO::SetSize(D3DXVECTOR2(50.0f, 50.0f));
	// 初期化
	CScene_TWO::Init();	
	// 現在の位置設定
	CScene_TWO::SetPosition(m_pos);
	// 色設定
	CScene_TWO::SetCol(m_col);
	// テクスチャー設定
	CScene_TWO::BindTexture(m_pTex[m_type]);
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CExplosion::Uninit(void)
{
	CScene_TWO::Uninit();
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CExplosion::Update(void)
{
	// 体力減少
	m_nLife--;
	// 透明度変更
	m_col.a -= 0.01f;
	// 現在の位置設定
	CScene_TWO::SetPosition(m_pos);
	// 更新
	CScene_TWO::Update();
	// 色の設定
	CScene_TWO::SetCol(m_col);
	// 体力が0以下
	if (m_nLife <= 0)
	{
		CScene::Release();
	}
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CExplosion::Draw(void)
{
	// 描画処理
	CScene_TWO::Draw();
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CExplosion::Load(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =					// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	char cTex[CBullet::TYPE_MAX][128] =			// テクスチャー設定
	{
		"data/TEXTURE/Effect/Explosion2.png",
	};

	// テクスチャー設定
	for (int nCnt = 0; nCnt < CBullet::TYPE_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice, cTex[nCnt], &m_pTex[nCnt]);
	}

	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CExplosion::UnLoad(void)
{
	for (int nCnt = 0; nCnt < CBullet::TYPE_MAX; nCnt++)
	{
		// テクスチャの開放
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
CExplosion * CExplosion::Create(D3DXVECTOR3 pos, CBullet::TYPE type)
{
	// 変数宣言
	CExplosion * pExplosion;		// 爆発2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pExplosion = new CExplosion();
	// 現在の位置
	pExplosion->m_pos = pos;
	// タイプ
	pExplosion->m_type = type;
	// 色
	switch (pExplosion->m_type)
	{
		// ハチ
	case CBullet::TYPE_BEE:
		pExplosion->m_col = D3DXCOLOR(1.0f, 0.7f, 0.2f, 1.0f);
		break;
		// 猫
	case CBullet::TYPE_CAT:
		pExplosion->m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;
		// 魚
	case CBullet::TYPE_FISH:
		pExplosion->m_col = D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f);
		break;
		// 鷹
	case CBullet::TYPE_HAWK:
		pExplosion->m_col = D3DXCOLOR(0.4f, 0.2f, 0.2f, 1.0f);
		break;
		// 鷹
	case CBullet::TYPE_SNAKE:
		pExplosion->m_col = D3DXCOLOR(0.1f, 1.0f, 0.1f, 1.0f);
		break;
	default:
		break;
	}
	// 初期化処理
	pExplosion->Init();
	// 生成したオブジェクトを返す
	return pExplosion;
}