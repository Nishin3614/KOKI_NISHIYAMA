//=============================================================================
//
// モデル処理 [player.cpp]
// Author : KOKI NISHIYAMA
//
//=============================================================================
#include "model.h"

//=============================================================================
// マクロ定義
//=============================================================================

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CModel::CModel()
{
	m_model_info.pMesh = NULL;
	m_model_info.pBuffMat = NULL;
	m_model_info.pTexture = NULL;
	m_model_info.nNumMat = 0;
	m_model_info.vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_model_info.vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_model_info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_model_info.posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_model_info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_model_info.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_model_info.nParent = 0;
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nFrame = 0;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CModel::~CModel()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CModel::Init(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();

	// 初期化
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CModel::Uninit(void)
{
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CModel::Update(void)
{
	// モデル事のモーション動作
	m_model_info.pos += (m_model_info.posDest - m_model_info.pos) / (float)m_nFrame;
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CModel::Draw(
	D3DXMATRIX & mtx
)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX			mtxRot, mtxTrans ,mtxScal;	// 計算用マトリックス
	D3DXMATERIAL		*pMat;						// 現在のマテリアル保存



	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	m_Falts = m_mtxWorld;

	// スケールを反映
	D3DXMatrixScaling(&mtxScal,
		m_model_info.scal.x,
		m_model_info.scal.y,
		m_model_info.scal.z);

	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld, &mtxScal);

	// スケールを反映
	D3DXMatrixScaling(&mtxScal,
		1.0f,
		1.0f,
		1.0f);

	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
	D3DXMatrixMultiply(&m_Falts,
		&m_Falts, &mtxScal);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_model_info.rot.y,
		m_model_info.rot.x,
		m_model_info.rot.z);

	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld, &mtxRot);

	D3DXMatrixMultiply(&m_Falts,
		&m_Falts, &mtxRot);

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

	D3DXMatrixMultiply(&m_Falts,
		&m_Falts, &mtxTrans);

		// 親情報を持っているとき
	if (m_Parent != NULL)
	{
		// 親と子の行列の積(1:ワールド行列 = 2:ワールド行列 * 3:体[親]行列)
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld,
			&m_Parent->m_Falts);
	}
	// すべての親の場合
	else
	{
		// 親と子の行列の積(1:ワールド行列 = 2:ワールド行列 * 3:体[親]行列)
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld,
			&mtx);

		D3DXMatrixMultiply(&m_Falts,
			&m_Falts,
			&mtx);
	}
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&m_matDef);

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_model_info.pBuffMat->GetBufferPointer();
	// テクスチャー設定
	pDevice->SetTexture(0, NULL);

	// カウントマテリアル
	for (int nCntMat = 0; nCntMat < (int)m_model_info.nNumMat; nCntMat++, pMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat->MatD3D);
		// 描画
		m_model_info.pMesh->DrawSubset(nCntMat);
	}
	// マテリアルをデフォルトに戻す
	//pDevice->SetMaterial(&matDef);
	pDevice->SetMaterial(&m_matDef);

	//頂点フォーマットの設定
	//pDevice->SetFVF(FVF_VERTEX_3D);
}

// ----------------------------------------
// 生成処理
// ----------------------------------------
CModel * CModel::Create()
{
	// 変数宣言
	CModel * pModel;	// モデル
	// メモリ確保
	pModel = new CModel();
	// 初期化
	pModel->Init();
	// 情報を返す
	return pModel;
}

// ----------------------------------------
// 親パーツの設定処理
// ----------------------------------------
void CModel::SetParent(CModel * pModel)
{
	m_Parent = pModel;
}

// ----------------------------------------
// モデルの情報渡し処理
// ----------------------------------------
void CModel::BindModel(
	MODEL_INFO &model_info,
	MODEL_OFFSET &offset	// モデルのオフセット
)
{
	// モデル情報の取得
	m_model_info = model_info;
	// モデルのオフセット
	m_model_info.pos = offset.pos;
	m_model_info.rot = offset.rot;
	m_model_info.posDest = offset.pos;
	m_model_info.rotDest = offset.rot;
	m_model_info.nParent = offset.nParent;
}

// ----------------------------------------
// モデルの位置設定処理
// ----------------------------------------
void CModel::SetPos(D3DXVECTOR3 &pos)
{
	m_model_info.pos = pos;
}

// ----------------------------------------
// モーションの設定処理
// ----------------------------------------
void CModel::SetMotion(
	KEY &key, 
	int nMaxFrame
)
{
	m_model_info.posDest = key.pos;
	m_model_info.rotDest = key.rot;
	m_nFrame = nMaxFrame;
}

// ----------------------------------------
// モデル情報の取得
// ----------------------------------------
MODEL_INFO *CModel::GetModelInfo(void)
{
	return &m_model_info;
}

// ----------------------------------------
// モデル情報の設定
// ---------------------------------------
void CModel::SetModelInfo(CONST MODEL_INFO &ModelInfo)
{
	m_model_info = ModelInfo;
}
