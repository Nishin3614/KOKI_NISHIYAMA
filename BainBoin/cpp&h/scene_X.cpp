// ----------------------------------------
//
// Xシーン処理の説明[scene_X.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "scene_X.h"
#include "manager.h"
#include "debugproc.h"
#include "scene_three.h"
#include "game.h"
#include "floor.h"
#include "block.h"	

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define OBJECT_COLLISIONSIZE (120.0)	// 当たり判定の大きさ

// ----------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CScene_X::CScene_X()
{
	m_model_info.pMesh = NULL;									// メッシュ情報へのポインタ
	m_model_info.pBuffMat = NULL;									// マテリアル情報へのポインタ
	m_model_info.nNumMat = NULL;									// マテリアルの数
	m_model_info.pTexture = NULL;
	m_model_info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_model_info.posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 初期位置
	m_model_info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 現在回転量
	m_model_info.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// キー間の回転量
	m_model_info.nParent = 0;								// 親モデルのインデックス
}

// ----------------------------------------
// イニシャライザコンストラクタ
// ----------------------------------------
CScene_X::CScene_X(ACTOR actor, LAYER layer) : CScene::CScene(actor,layer)
{
	m_model_info.pMesh = NULL;									// メッシュ情報へのポインタ
	m_model_info.pBuffMat = NULL;									// マテリアル情報へのポインタ
	m_model_info.nNumMat = NULL;									// マテリアルの数
	m_model_info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_model_info.posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 初期位置
	m_model_info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 現在回転量
	m_model_info.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// キー間の回転量
	m_model_info.nParent = 0;								// 親モデルのインデックス
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CScene_X::~CScene_X()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CScene_X::Init(void)
{
	m_model_info.pMesh = NULL;									// メッシュ情報へのポインタ
	m_model_info.pBuffMat = NULL;									// マテリアル情報へのポインタ
	m_model_info.nNumMat = NULL;									// マテリアルの数
	m_model_info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_model_info.posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 初期位置
	m_model_info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 現在回転量
	m_model_info.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// キー間の回転量
	m_model_info.nParent = 0;								// 親モデルのインデックス
	m_model_info.size = D3DXVECTOR3(100.0f, 100.0f, 100.0f);
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CScene_X::Uninit(void)
{
	// メッシュの開放
	if (m_model_info.pMesh != NULL)
	{
		m_model_info.pMesh->Release();
		m_model_info.pMesh = NULL;
	}

	// マテリアルの開放
	if (m_model_info.pBuffMat != NULL)
	{
		m_model_info.pBuffMat->Release();
		m_model_info.pBuffMat = NULL;
	}

	// テクスチャーの開放
	if (m_model_info.pTexture != NULL)
	{
		m_model_info.pTexture->Release();
		m_model_info.pTexture = NULL;
	}
	// テクスチャーの開放
	for (int nCntTex = 0; nCntTex < 30; nCntTex++)
	{
		if (pTexture[nCntTex] != NULL)
		{
			pTexture[nCntTex]->Release();
			pTexture[nCntTex] = NULL;
		}
	}
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CScene_X::Update(void)
{

}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CScene_X::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX			mtxRot, mtxTrans;		// 計算用マトリックス
	D3DXMATERIAL		*pMat;					// 現在のマテリアル保存
	D3DMATERIAL9		matDef;					// マテリアルデータのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	// スケールを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_model_info.rot.y,
		m_model_info.rot.x,
		m_model_info.rot.z);

	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld, &mtxRot);


	// 位置を反映 //
	// 平行移動行列作成(オフセット)
	D3DXMatrixTranslation(&mtxTrans,	// 総合の入れ物
		m_model_info.pos.x,
		m_model_info.pos.y,
		m_model_info.pos.z);

	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxTrans);					// 3

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_model_info.pBuffMat->GetBufferPointer();
	
		pDevice->SetTexture(0, m_model_info.pTexture);

	// カウントマテリアル
	for (int nCntMat = 0; nCntMat < (int)m_model_info.nNumMat; nCntMat++, pMat++)
	{
		// テクスチャー設定
		if (pTexture[nCntMat] != NULL)
		{
			//pDevice->SetTexture(0, pTexture[nCntMat]);
		}
		// マテリアルの設定
		pDevice->SetMaterial(&pMat->MatD3D);

		// 描画
		m_model_info.pMesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CScene_X * CScene_X::Create()
{
	// 変数宣言
	CScene_X * pScene_Two;		// シーン3Dクラス

	// メモリの生成(初め->基本クラス,後->派生クラス)
	pScene_Two = new CScene_X();

	// 初期化処理
	pScene_Two->Init();
	
	// 生成したオブジェクトを返す
	return pScene_Two;
}

// ----------------------------------------
// 位置設定処理
// ----------------------------------------
void CScene_X::SetPos(D3DXVECTOR3 pos)
{
	m_model_info.pos = pos;
}

// ----------------------------------------
// 回転設定処理
// ----------------------------------------
void CScene_X::SetRot(D3DXVECTOR3 size)
{
	m_model_info.size = size;

	/*
	// 当たり判定生成
	CCollision *pCollision;
	pCollision = CCollision::Create();
	pCollision->SetPos(&m_model_info.pos);
	pCollision->SetSize(D3DXVECTOR3(OBJECT_COLLISIONSIZE, OBJECT_COLLISIONSIZE, OBJECT_COLLISIONSIZE));
	pCollision->SetMove(NULL);
	pCollision->SetType(CCollision::TYPE_WAIT);
	*/
}

// ----------------------------------------
// 位置取得処理
// ----------------------------------------
D3DXVECTOR3 CScene_X::GetPos(void)
{
	return m_model_info.pos;
}

// ----------------------------------------
// 回転取得処理
// ----------------------------------------
D3DXVECTOR3 CScene_X::GetRot(void)
{
	return m_model_info.rot;
}

// ----------------------------------------
// テクスチャー設定処理
// ----------------------------------------
void CScene_X::BindTexture(LPDIRECT3DTEXTURE9 p_Tex)
{
	m_model_info.pTexture = p_Tex;
}

// ----------------------------------------
// モデル設定
// ----------------------------------------
void CScene_X::BindModel(MODEL_INFO const &model_info)
{
	m_model_info = model_info;

	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX			mtxRot, mtxTrans;		// 計算用マトリックス
	D3DXMATERIAL		*pMat;					// 現在のマテリアル保存

	m_model_info = model_info;

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_model_info.pBuffMat->GetBufferPointer();
	// カウントマテリアル
	for (int nCntMat = 0; nCntMat < (int)m_model_info.nNumMat; nCntMat++, pMat++)
	{
		if (pMat->pTextureFilename != "")
		{
			D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &pTexture[nCntMat]);
		}
	}
}
