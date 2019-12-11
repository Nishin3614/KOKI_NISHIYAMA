// ----------------------------------------
//
// ブロック処理の説明[block.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "block.h"
#include "model_info.h"

// ----------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------
LPDIRECT3DTEXTURE9 CBlock::m_pTexture[TYPE_MAX] = {};		// テクスチャへのポインタ
CLoad	*CBlock::m_pload = NULL;							// ロード
CModel_info	*CBlock::m_pmodel_info = NULL;					// モデル情報
int		CBlock::m_nAll = 0;									// 番号

// ----------------------------------------
// イニシャライザコンストラクタ
// ----------------------------------------
CBlock::CBlock() : CScene_X::CScene_X(ACTOR_BLOCK,LAYER_3DOBJECT)
{
	m_Type = TYPE_1;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CBlock::~CBlock()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CBlock::Init(void)
{
	// Xオブジェクト生成
	CScene_X::Init();
	// モデル情報設定
	CScene_X::BindModel(m_pmodel_info->GetModel_info());
	// テクスチャー設定
	CScene_X::BindTexture(m_pTexture[m_Type]);
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CBlock::Uninit(void)
{
	CScene_X::Uninit();
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CBlock::Update(void)
{
	CScene_X::Update();
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CBlock::Draw(void)
{
	CScene_X::Draw();
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CBlock * CBlock::Create(void)
{
	// 変数宣言
	CBlock * pBlock;		// シーン3Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pBlock = new CBlock();
	// 初期化処理
	pBlock->Init();
	// 生成したオブジェクトを返す
	return pBlock;
}

// ----------------------------------------
// 読み込み作成処理
// ----------------------------------------
void CBlock::LoadCreate(void)
{
	// 変数宣言
	CBlock * pBlock;	// ブロック
	// ブロック
	for (int nCntBlock = 0; nCntBlock < m_nAll; nCntBlock++)
	{
		pBlock = CBlock::Create();							// 生成
		pBlock->SetPos(m_pload->GetInfo(nCntBlock).pos);	// 位置
		pBlock->SetRot(m_pload->GetInfo(nCntBlock).rot);	// 回転
	}
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CBlock::Load(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =					// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	char cTex[TYPE_MAX][128] =					// テクスチャー名
	{
		"data/TEXTURE/block.png",
	};
	char cModelfile[TYPE_MAX][128] =			// モデルファイル名
	{
		"data/MODEL/object/block.x",
	};
	// テクスチャー設定
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice, cTex[nCnt], &m_pTexture[nCnt]);
	}
	// ロード生成
	m_pload = new CLoad;
	// ロード読み込み
	m_nAll = m_pload->Load("data/LOAD/OBJECT/materials.txt");
	// モデル情報生成
	m_pmodel_info = new CModel_info;
	// モデル読み込み
	m_pmodel_info->Load(pDevice, cModelfile[0]);
	return S_OK;
}

// ----------------------------------------
// unload処理
// ----------------------------------------
void CBlock::UnLoad(void)
{
	// テクスチャー設定
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
	// ロードの破棄
	delete m_pload;
	m_pload = NULL;
	// モデルの破棄
	m_pmodel_info->Unload();
	delete m_pmodel_info;
	m_pmodel_info = NULL;
}

// ----------------------------------------
// 位置の設定処理
// ----------------------------------------
void CBlock::SetPos(D3DXVECTOR3 pos)
{
	CScene_X::SetPos(pos);
}

// ----------------------------------------
// 回転の設定処理
// ----------------------------------------
void CBlock::SetRot(D3DXVECTOR3 rot)
{
	CScene_X::SetRot(rot);
}

// ----------------------------------------
// 位置の取得処理
// ----------------------------------------
D3DXVECTOR3 CBlock::GetPos(void)
{
	return CScene_X::GetPos();
}